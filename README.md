📡 Projet ESP32 – Capteur Température & Humidité avec API Flask

Ce projet utilise une carte ESP32 équipée d’un capteur DHT22 afin de mesurer la température et l’humidité, puis d’envoyer les données vers une API Flask.
L’ESP32 héberge également un petit serveur web local pour fournir des informations de statut.

⚙️ Fonctionnalités

Lecture de la température (°C) et de l’humidité (%) via un capteur DHT22

Connexion au réseau Wi-Fi avec adresse IP fixe

Envoi périodique des données au serveur Flask via requêtes HTTP POST (JSON)

Ajout d’une clé API (x-api-key) pour sécuriser les requêtes

Serveur web intégré avec deux routes :

/ → message de bienvenue

/status → JSON indiquant l’état et l’adresse IP locale

🛠️ Matériel requis

Carte ESP32

Capteur DHT22 (broche de données sur GPIO 4)

Réseau Wi-Fi accessible

Un serveur Flask fonctionnel pour la réception des données

📂 Dépendances Arduino

Dans l’IDE Arduino, installer les bibliothèques suivantes :

WiFi.h
 (native ESP32)

HTTPClient.h

WebServer.h

DHT sensor library
 (Adafruit)

🔑 Paramètres à modifier

Avant de téléverser le code, personnalisez les variables suivantes dans le fichier .ino :

// Wi-Fi
const char* ssid = "VotreSSID";
const char* password = "VotreMotDePasse";

// API Flask
const char* serverName = "http://IP_FLASK:5000/api/donnees";
const char* apiKey = "VotreCleAPI";

// Adresse IP fixe de l’ESP32
IPAddress local_IP(10,61,55,169);
IPAddress gateway(10,61,55,1);
IPAddress subnet(255,255,255,0);

🚀 Utilisation

Brancher l’ESP32 et téléverser le code via l’IDE Arduino.

Le module se connecte au Wi-Fi avec l’IP configurée.

Chaque 10 secondes, l’ESP32 envoie :

La température (°C)

L’humidité (%)
vers le serveur Flask en JSON.

Exemple de JSON envoyé :
{
  "Type_Donnee": "Temperature",
  "Unite_Donnee": "C",
  "Valeur_Donnee": 23.50,
  "Id_ESP32": 1
}

Routes disponibles sur le serveur local ESP32 :

http://<IP_ESP32>/ → message texte

http://<IP_ESP32>/status → état en JSON

📝 Exemple de réponse Flask

Si la requête est valide, le serveur Flask renverra une confirmation du stockage des données :

{
  "success": true,
  "message": "Données reçues"
}

📌 Notes

Le délai entre chaque envoi est réglé à 10 secondes (delay(10000);).

En cas d’échec de lecture du capteur ou de perte Wi-Fi, un message d’erreur s’affiche dans le Moniteur Série.

Chaque ESP32 peut être identifié par un Id_ESP32 unique.
