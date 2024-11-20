#include "DHTesp.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

const int DHT_PIN = 15;  // ขา Sensor

DHTesp dhtSensor;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

const char* serverUrl = "http://104.214.177.30:3001/updateTemperature"; // HTTP ของตัว Express Server

void setup() {
  Serial.begin(115200);
  delay(10);

  // เชื่อมต่อ Wi-Fi
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // เริ่มต้นเซ็นเซอร์ DHT
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);  
}

void loop() {
  // อ่านอุณหภูมิจาก Sensor
  float temperature = dhtSensor.getTemperature();

  // เช็คข้อมูลจาก Sensor ก่อนส่ง
  if (!isnan(temperature)) {
    HTTPClient http;
    http.begin(serverUrl); // HTTP POST request ไปยัง Server
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"temperature\": " + String(temperature) + "}";
    int httpResponseCode = http.POST(jsonPayload);  // ส่งข้อมูล JSON

    if (httpResponseCode > 0) {
      Serial.println("Temperature sent successfully!");
    } else {
      Serial.println("Error sending temperature");
    }
    http.end();
  } else {
    Serial.println("Error: Temperature reading failed");
  }
  
  delay(1000);
}