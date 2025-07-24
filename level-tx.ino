#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define STASSID "wifi_ssid"
#define STAPSK "wifi_passkey"
#define TRIG_PIN 0
#define ECHO_PIN 2
#define SERVER_IP "192.168.1.92"
#define SERVER_PORT "3005"
#define SENSOR_CODE "water_level_001"

long duration;
float distance;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.print("Connecting to ");
  Serial.println(STASSID);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("[WiFi] Connected");
  Serial.print("[WiFi] IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("[Sensor] Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  postSensorReading(distance);
  delay(500);
}

String postSensorReading(float reading) {
  String response = "{}";
  if (reading < 1) {
    return response;
  }
  if ((millis() - lastTime) <= timerDelay) {
    return response;
  }
  if ((WiFi.status() != WL_CONNECTED)) {
    return response;
  }
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] Begin...\n");
  http.begin(client, "http://" SERVER_IP ":" SERVER_PORT "/broadcast/");
  http.addHeader("Content-Type", "application/json");
  Serial.print("[HTTP] POST...\n");
  String payload = "{\"sensorCode\":\"" SENSOR_CODE "\",\"reading\":" + String(reading) + "}";
  int httpCode = http.POST(payload);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      response = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(response);
      Serial.println(">>");
    }
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  Serial.println("[HTTP] Wait " + String(timerDelay/1000) + "s before the next reading");
  lastTime = millis();
  return response;
}
