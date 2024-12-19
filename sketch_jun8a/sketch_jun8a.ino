#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <fstream>

const char* ssid = "Redmi";
const char* password = "rozowy245";
DHTesp dht;
ESP8266WebServer server(80);
const int diodka = 2;
const float alarm = 70.0;

void handleRoot();
//
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie...");
  }
  Serial.println("mission complete ez 8)... połączyło");
  Serial.print("Adresik IP : ");
  Serial.println(WiFi.localIP());  
  server.on("/", handleRoot);
  server.begin();
  dht.setup(D2, DHTesp::DHT11);
  pinMode(diodka, OUTPUT);
}

void loop() {
  server.handleClient();

  delay(dht.getMinimumSamplingPeriod());

  float wilgot = dht.getHumidity();
  float ciepelko = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t Wilogotność = ");
  Serial.print(wilgot, 1);
  Serial.print(" %");
  Serial.print("\t Temperatura = \t ");
  Serial.print(ciepelko, 1);
  Serial.print(" C");
  Serial.println();


  if (wilgot < alarm) {
    digitalWrite(diodka, HIGH);
  } else {
    digitalWrite(diodka, LOW);
  }
}

void handleRoot() {
  float wilgot = dht.getHumidity();
  float ciepelko = dht.getTemperature();


   String webpage = " <html><head><title>Odczyty DHT11</title><meta http-equiv=\"refresh\" content=\"2\"> ";
   webpage += "    <h1>Stacja pogodowa</h1> ";
   webpage += "    <h2> Tomasz Kotyla</h2>  ";
   webpage += "    <p>Temperatura: " + String(ciepelko, 1) + " C</p> ";
   webpage += "    <p>Wilgotnosc: " + String(wilgot, 1) + " %</p> ";
   webpage += "</body> ";
   webpage += "</html> ";

  server.send(200, "text/html", webpage);
}

