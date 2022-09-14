#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

//Define Firebase Keys
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

//Define WIFI SSID and Password
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define DHTPIN 4 //Pin Connected to DHT Sensor
#define DHTTYPE DHT11 //DHT 11 Temp Sensor

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;
int Vresistor = A0; 
int Vrdata = 0; 

#define DHTPIN 4 //Pin Connected to DHT Sensor

#define DHTTYPE DHT11 //DHT 11 Temp Sensor

DHT_Unified dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);

  //Initialize WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //Initialize temp sensor
  dht.begin();

  //Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

//Is WIFI connected?
bool isConnected = false;

void loop() {
  sensors_event_t event;

  //WiFi Status
  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    Serial.println("Connected");
    isConnected = true;
  }

  if (WiFi.status() != WL_CONNECTED) {    
    Serial.println(".");
    delay(1000);
    isConnected = false;
  }

  //Get temperature event
  dht.temperature().getEvent(&event);
  float temp = event.temperature;

  //Get humidity event
  dht.humidity().getEvent(&event);
  float humid = event.relative_humidity;

  //Check if value is nan else print temp and humidity values
  if ( isnan(temp) || isnan(humid) ) {
    Serial.println(F("Failed to read"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(temp);
    Serial.println(F("°C"));

    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.println(F("%"));
  }

  //Set corresponding JSON values
  json.set("/temperature", temp);
  json.set("/humidity", humid);

  //Update database node
  Firebase.updateNode(firebaseData,"/sensor",json);

  //Loop every 2 seconds
  delay(2000);
}