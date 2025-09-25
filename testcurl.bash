Invoke-WebRequest -Method POST "http://10.61.55.3:5000/api/donnees" `
  -Headers @{ "Content-Type" = "application/json"; "x-api-key" = "cleApi_12345" } `
  -Body '{
    "Timestamp": "2025-09-24 12:00:00",
    "Type_Donnee": "Temperature",
    "Unite_Donnee": "C",
    "Valeur_Donnee": 25.7,
    "Id_ESP32": 1
  }'
