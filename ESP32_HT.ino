#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include "DHT.h"

// --- DHT22 ---
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- Wi-Fi ---
const char* ssid = "RLMK";       
const char* password = "rlmk1234";

// --- API Flask ---
const char* serverName = "http://10.61.55.3:5000/api/donnees"; // IP locale du PC avec Flask
const char* apiKey = "cleApi_12345";

// --- WebServer local ---
WebServer server(80);

// --- Fonctions ---
void sendData(String type, float valeur, String unite) {
  // JSON sans timestamp
  String jsonData = "{";
  jsonData += "\"Type_Donnee\":\"" + type + "\"," ;
  jsonData += "\"Unite_Donnee\":\"" + unite + "\"," ;
  jsonData += "\"Valeur_Donnee\":" + String(valeur, 2) + "," ;
  jsonData += "\"Id_ESP32\":1" ;
  jsonData += "}";

  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", apiKey);

  int httpResponseCode = http.POST(jsonData);
  if (httpResponseCode > 0) {
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("Erreur POST: " + String(httpResponseCode));
  }
  http.end();
}

// --- Handlers Web ---
void handleStatus() {
  String json = "{\"ok\":true,\"ip\":\"" + WiFi.localIP().toString() + "\"}";
  server.send(200, "application/json", json);
}

void handleRoot() {
  server.send(200, "text/plain", "ESP32 connecté au réseau local. /status pour JSON.");
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connexion Wi-Fi avec IP fixe
  IPAddress local_IP(10,61,55,169);
  IPAddress gateway(10,61,55,1);
  IPAddress subnet(255,255,255,0);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Erreur configuration IP fixe !");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connexion au Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  Serial.println("Wi-Fi connecté !");
  Serial.print("Adresse IP: "); Serial.println(WiFi.localIP());

  // Serveur web local
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.begin();
  Serial.println("Serveur web démarré");
}

// --- Loop ---
void loop() {
  server.handleClient();

  if (WiFi.status() == WL_CONNECTED) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) {
      Serial.print("Humidité: "); Serial.print(h); Serial.print("%\t");
      Serial.print("Température: "); Serial.println(t);

      sendData("Temperature", t, "C");
      sendData("Humidite", h, "%");
    } else {
      Serial.println("Erreur lecture DHT !");
    }
  } else {
    Serial.println("Erreur Wi-Fi");
  }

  delay(10000); // toutes les 10 secondes
}
