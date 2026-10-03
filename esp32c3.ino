
#include <WiFi.h>
#include <WebServer.h>

// ========================================
// WiFi
// ========================================
const char* WIFI_SSID = "Shinde";
const char* WIFI_PASSWORD = "aaru@1234";

// ========================================
// LED
// ========================================
#define LED_PIN 8

// ========================================
// Web Server
// ========================================
WebServer server(80);

// ========================================
// CORS
// ========================================
void addCORS()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "*");
}

// ========================================
// LED ON
// ========================================
void handleLedOn()
{
    digitalWrite(LED_PIN, HIGH);

    addCORS();
    server.send(200, "text/plain", "LED ON");
}

// ========================================
// LED OFF
// ========================================
void handleLedOff()
{
    digitalWrite(LED_PIN, LOW);

    addCORS();
    server.send(200, "text/plain", "LED OFF");
}

// ========================================
// LED STATUS
// ========================================
void handleStatus()
{
    addCORS();

    if (digitalRead(LED_PIN))
        server.send(200, "text/plain", "ON");
    else
        server.send(200, "text/plain", "OFF");
}

// ========================================
// OPTIONS request
// ========================================
void handleOptions()
{
    addCORS();
    server.send(204);
}

// ========================================
// SETUP
// ========================================
void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Connect WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");

    Serial.print("ESP32 IP address: ");
    Serial.println(WiFi.localIP());

    // Routes
    server.on("/led/on", HTTP_GET, handleLedOn);
    server.on("/led/off", HTTP_GET, handleLedOff);
    server.on("/led/status", HTTP_GET, handleStatus);

    server.on("/led/on", HTTP_OPTIONS, handleOptions);
    server.on("/led/off", HTTP_OPTIONS, handleOptions);
    server.on("/led/status", HTTP_OPTIONS, handleOptions);

    server.begin();

    Serial.println("HTTP server started");
}

// ========================================
// LOOP
// ========================================
void loop()
{
    server.handleClient();
}
