#include "DHTesp.h"
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

const int DHT_PIN = 15;  // ขา Sensor

DHTesp dhtSensor;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

const char* serverUrlTemp = "http://104.214.176.253/:3001/updateTemperature"; // HTTP ของตัว Express Server
const char* serverUrlHum = "http://104.214.176.253/:3001/updateHumidity";

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
  float humidity = dhtSensor.getHumidity();

  // เช็คข้อมูลจาก Sensor ก่อนส่ง
  if (!isnan(temperature)) {
    HTTPClient http;
    http.begin(serverUrlTemp); // HTTP POST request ไปยัง Server
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
  
  if (!isnan(humidity)) {
    HTTPClient http;
    http.begin(serverUrlHum); // HTTP POST request ไปยัง Server
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"humidity\": " + String(humidity) + "}";
    int httpResponseCode = http.POST(jsonPayload);  // ส่งข้อมูล JSON
  delay(1000);