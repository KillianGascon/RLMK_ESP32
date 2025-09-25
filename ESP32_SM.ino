#include <WiFi.h>
#include <HTTPClient.h>

// --- Capteur Soil Moisture ---
#define SOIL_PIN 34  // pin analogique du capteur

// --- Wi-Fi ---
const char* ssid = "RLMK";
const char* password = "rlmk1234";

// --- API Flask ---
const char* serverName = "http://10.61.55.3:5000/api/donnees"; // IP de ton PC
const char* apiKey = "cleApi_12345";

// --- Fonctions ---
void sendSoilMoisture(float value) {
  String jsonData = "{";
  jsonData += "\"Type_Donnee\":\"SoilMoisture\",";
  jsonData += "\"Unite_Donnee\":\"%\",";
  jsonData += "\"Valeur_Donnee\":" + String(value, 2) + ",";
  jsonData += "\"Id_ESP32\":2";
  jsonData += "}";

  Serial.println(">>> Envoi JSON : " + jsonData);

  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("x-api-key", apiKey);

  int httpResponseCode = http.POST(jsonData);
  if (httpResponseCode > 0) {
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + http.getString());
  } else {
    Serial.println("Erreur POST: " + String(httpResponseCode));
  }
  http.end();
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  pinMode(SOIL_PIN, INPUT);

  // Connexion Wi-Fi avec IP fixe
  IPAddress local_IP(10,61,55,170);  // IP fixe
  IPAddress gateway(10,61,55,1);
  IPAddress subnet(255,255,255,0);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Erreur configuration IP fixe !");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connexion au Wi-Fi");
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 40) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  Serial.println();

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Wi-Fi connecté !");
    Serial.print("Adresse IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("Échec connexion Wi-Fi !");
  }
}

// --- Loop ---
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int sensorValue = analogRead(SOIL_PIN);
    // Convertir la valeur analogique en pourcentage
    float soilPercent = map(sensorValue, 0, 4095, 0, 100);

    Serial.print("Soil Moisture: ");
    Serial.print(soilPercent);
    Serial.println("%");

    sendSoilMoisture(soilPercent);
  } else {
    Serial.println("Erreur Wi-Fi");
  }

  delay(10000); // envoi toutes les 10 secondes
}
