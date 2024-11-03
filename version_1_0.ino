#include <WiFi.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>
#include <Update.h>

// Configurazione WiFi
const char* ssid = "Sunrise_5Ghz";
const char* password = "classylotus261$";

// Configurazione del server
const char* versionURL = "https://raw.githubusercontent.com/joker84nero/test_OTA_ESP32/refs/heads/main/version.txt";
const char* firmwareURL = "https://github.com/joker84nero/test_OTA_ESP32/raw/refs/heads/main/version_1_0.ino.bin";

// Versione attuale
String currentVersion = "2.0";

// Inizializza il display
TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);

    // Inizializza e configura la retroilluminazione del display
    tft.init();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_WHITE, TFT_GREEN);
    tft.setTextSize(2);
    tft.drawString("Versione 2.0", 10, 10);

    // Connessione WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connesso al WiFi");

    // Controllo aggiornamento firmware
    checkForUpdates();
}

void checkForUpdates() {
    HTTPClient http;
    http.begin(versionURL);
    int httpCode = http.GET();
    if (httpCode == 200) {
        String newVersion = http.getString();
        if (newVersion != currentVersion) {
            downloadAndUpdateFirmware();
        }
    }
    http.end();
}

void downloadAndUpdateFirmware() {
    HTTPClient http;
    http.begin(firmwareURL);
    int httpCode = http.GET();
    if (httpCode == 200) {
        size_t written = Update.writeStream(http.getStream());
        if (written > 0 && Update.end()) {
            ESP.restart();
        }
    }
    http.end();
}

void loop() {
    // Vuoto per ora
}
