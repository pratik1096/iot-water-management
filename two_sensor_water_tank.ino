

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "AUKCSC_SMARTOFFICE";
const char* password = "PASSWORD2020";
String payload;
const char* host = "http://192.168.1.1/gettimewlm.php";
const char* serverName = "http://192.168.1.1/WLM.php";
const char* cloudserverName = "http://52.237.118.211:8000/iotdevicedata/Water_level";
String apiKeyValue = "tPmAT5Ab3j7F9";
String productcode = "AukproDevice11032";
String uniquekey = "!AiOtAtAuKpRoWl01";
String modelname = "Water_level";
int p1;
String date;
int p2;
String currenttime;
int p3;
String hh;
int p4;
String mm;
WiFiClient espClient;
PubSubClient client(espClient);

String status;


const int trigPin1 = D3;
const int echoPin1 = D2;
const int trigPin2 = D6;
const int echoPin2 = D7;
int relay = D5;

long duration1;
int distance1 = 0;
int safetyDistance1;
int longdistance1 ;
long duration2;
int distance2 = 0;
int safetyDistance2;
int longdistance2 ;
long now = millis();
long lastMeasure = 0;
float percent2 = 0;
float percent1 = 0;


// Don't change the function below. This functions connects your ESP8266 to your router
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

#include <Wire.h>


void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode (relay, OUTPUT);
  Serial.begin(115200); // Starts the serial communication

  pinMode(relay, OUTPUT);
  //  pinMode(sensor_pin, INPUT);
  Serial.println("Reading From the Sensor ...");

  delay(2000);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void gettime() {
  HTTPClient http;
  http.begin(host);
  int httpCode = http.GET();
  if (httpCode > 0) {
    payload = http.getString();
    // Serial.println(payload);
    p1 = payload.indexOf('&');
    date = payload.substring(0, p1);
    p2 = payload.indexOf('&', p1 + 1 );
    currenttime = payload.substring(p1 + 1, p2 + 1);
    Serial.println(date);
    currenttime.remove(8);
    //  Serial.println(fuel);
    p3 = currenttime.indexOf(':');
    hh = currenttime.substring(0, p3);
    Serial.println(hh);
    p4 = currenttime.indexOf(':', p3 + 1 );
    mm = currenttime.substring(p3 + 1, p4 + 1);
    mm.remove(2);
    Serial.println(mm);
  }
  http.end();
  delay(1000);
}


void localserver() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&tank=" + percent1 + "&tank1=" + percent2 + "";

    //Motion_status  Moisture_level  Light_Intensity Water_level LAT_LONG  RFID

    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds


}
void cloudserver() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(cloudserverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&tank=" + percent1 + "&tank1=" + percent2 
                              + "&date=" + date + "&hh=" + hh + "&mm=" + mm + "" ;
          
    //Motion_status  Moisture_level  Light_Intensity Water_level LAT_LONG  RFID

    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds

}


void loop() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  percent1 = 100 - ((distance1 * 100) / 120);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  percent2 = 100 - ((distance2 * 100) / 120);
  gettime();
  localserver();
  cloudserver();

  delay(2000);

}
