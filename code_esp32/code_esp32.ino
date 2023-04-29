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

volatile bool gps_status=true;
const int interruptPin = 15;
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  lcd.init();                      // initialize the LCD
  lcd.backlight();
  gpsSerial.begin(9600);
  pinMode(interruptPin, INPUT);
  attachInterrupt(interruptPin, interruptHandler, FALLING);
  connectToWifi();
  connectToFirebase();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ready!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(8,1);
  lcd.print("GPS: ");
  lcd.setCursor(13,1);
  lcd.print("ON");
}

void loop() {
  
  while (gpsSerial.available() > 0) {
    if(gps_status){
    lcd.setCursor(8,1);
    lcd.print("GPS: ");
    lcd.setCursor(13,1);
    lcd.print("ON");
    }
    else{
    lcd.setCursor(8,1);
    lcd.print("GPS: ");
    lcd.setCursor(13,1);
    lcd.print("OFF");
    }
    if (gps.encode(gpsSerial.read())) {
      float latitude, longitude;
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();

        if(gps_status){
          sendPositionToFirebase(latitude, longitude);
        }
      }
    }
  }
  
  
  
}

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting ");
  lcd.setCursor(0, 1);
  lcd.print("to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Wi-Fi connected");
}

void connectToFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting ");
  lcd.setCursor(0, 1);
  lcd.print("to Firebase...");
  while (!Firebase.ready()) {
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Firebase connected");
}

void sendPositionToFirebase(float latitude, float longitude) {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!Firebase.ready()) {
    connectToFirebase();
  }

  Firebase.setFloat(firebaseData, "/Buses/Bus1/latitude", latitude);
  Firebase.setFloat(firebaseData, "/Buses/Bus1/longitude", longitude);
  lcd.setCursor(0,1);
  lcd.print("Latitude: ");
  lcd.print(latitude, 6); // display up to 6 decimal places
  lcd.setCursor(0,2);
  lcd.print("Longitude: ");
  lcd.print(longitude, 6); // display up to 6 decimal places
}

//-------------interrupt------------------------------

void interruptHandler() {
  button_time = millis();
  if (button_time - last_button_time > 250) {
    gps_status = !gps_status;
    last_button_time = button_time;
}
}
