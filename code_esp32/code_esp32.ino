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

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  connectToWifi();
  connectToFirebase();
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      float latitude, longitude;
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
        sendPositionToFirebase(latitude, longitude);
      }
    }
  }
}

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
}

void connectToFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.print("Connecting to Firebase...");
  while (!Firebase.ready()) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Firebase connected");
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
  Serial.println("------Data Sent-----------");
  Serial.print("Latitude: ");
  Serial.print(latitude);
  Serial.print(" Longitude: ");
  Serial.println(longitude);


}
