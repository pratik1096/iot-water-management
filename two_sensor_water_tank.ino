

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

const char* ssid = "ssid";
const char* password = "password";
String payload;
const char* host = "0.0.0.0:0000/myfile";
const char* serverName = "0.0.0.0:0000/myfile";
const char* cloudserverName = "0.0.0.0:0000/myfile";
String apiKeyValue = "***********";
String productcode = "***********";
String uniquekey = "*************";
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

void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  pinMode (relay, OUTPUT);
  Serial.begin(115200); // Starts the serial communication

  pinMode(relay, OUTPUT);
  Serial.println("Reading From the Sensor ...");

  delay(2000);
  delay(10);
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
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&tank=" + percent1 + "&tank1=" + percent2 + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
void cloudserver() {

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(cloudserverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&tank=" + percent1 + "&tank1=" + percent2 
                              + "&date=" + date + "&hh=" + hh + "&mm=" + mm + "" ;
        Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
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
