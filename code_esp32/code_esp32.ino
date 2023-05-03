#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define RX_PIN 16
#define TX_PIN 17

#define WIFI_SSID "azerty"
#define WIFI_PASSWORD "123456789E"

#define FIREBASE_HOST "https://my-bus-final-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyAGkegz-UP_A015MVRaTy_bdzkWApNM7IY"

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
TinyGPSPlus gps;
FirebaseData firebaseData;


const int interruptPin = 15;
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

bool gps_data_status= false;
volatile bool gps_status=true;


LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  lcd.init();                      // initialize the LCD
  lcd.backlight();
  Display_info();
  gpsSerial.begin(9600);
  pinMode(interruptPin, INPUT);
  attachInterrupt(interruptPin, interruptHandler, FALLING);
  connectToWifi();
  connectToFirebase();
  delay(1000);
}

void loop() {
  
  while (gpsSerial.available() > 0) {
    
    if (gps.encode(gpsSerial.read())) {
      float latitude, longitude;
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();

        if(gps_status){
          sendPositionToFirebase(latitude, longitude);
        }else{
          gps_data_status=false;
        }
      }
    }
    Update_Display_info(gps_data_status,gps_status);
  }
  
  
  
}

//function that connects the esp32 dev board to Wi-Fi

void connectToWifi() {
  lcd.setCursor(5, 0);
  lcd.print("F");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  lcd.setCursor(5, 0);
  lcd.print("T");
}

//function that connects to firebase realtime database

void connectToFirebase() {
  lcd.setCursor(9, 1);
  lcd.print("F");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  while (!Firebase.ready()) {
    delay(1000);
  }
  lcd.setCursor(9, 1);
  lcd.print("T");
}

//Function that sends latitude and longitude coordinates to firebase realtime database

void sendPositionToFirebase(float latitude, float longitude) {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!Firebase.ready()) {
    connectToFirebase();
  }
  if(Firebase.setFloat(firebaseData, "/Buses/Bus1/latitude", latitude) && Firebase.setFloat(firebaseData, "/Buses/Bus1/longitude", longitude)){
    gps_data_status = true;
  }
  else{
    gps_data_status = false;
  }
  
}

//-------------interrupt------------------------------

void interruptHandler() {
  button_time = millis();
  if (button_time - last_button_time > 250) {
    gps_status = !gps_status;
    last_button_time = button_time;
}
}


void Update_Display_info(bool gps_data_status ,bool gps_status){
  lcd.setCursor(15, 1);
  if(gps_status){
    lcd.print("T");
  } else{
    lcd.print("F");
  }
  lcd.setCursor(15, 0);
  if(gps_data_status){
    lcd.print("T");
  } else{
    lcd.print("F");
  }
}

void Display_info(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi:F    Data:-");
  lcd.setCursor(0, 1);
  lcd.print("Firebase:F GPS:T");
}
