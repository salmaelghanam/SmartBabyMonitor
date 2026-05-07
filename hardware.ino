#define BLYNK_TEMPLATE_ID "TMPL2KNvE6r89"
#define BLYNK_TEMPLATE_NAME "baby monitor sys"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

#include <WiFi.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

#define DHTPIN 15
#define DHTTYPE DHT11
#define DS18B20_PIN 4
#define SOUND_SENSOR_PIN 36
#define BUZZER_PIN 18

const int soundThreshold = 120;
const char* httpTriggerURL = "http://YOUR_SERVER_IP:5000/trigger";

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;
BlynkTimer timer;

int getAverageSoundLevel(int pin) {
  long sum = 0;
  for (int i = 0; i < 100; i++) {
    sum += analogRead(pin);
    delay(1);
  }
  return sum / 100;
}

void readSensors() {
  ds18b20.requestTemperatures();
  float bodyTemp = ds18b20.getTempCByIndex(0);
  Blynk.virtualWrite(V1, bodyTemp);
  if (bodyTemp > 37.5) {
    Blynk.logEvent("high_body_temp", "High Body Temp Detected!");
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }
  float roomTemp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Blynk.virtualWrite(V2, roomTemp);
  Blynk.virtualWrite(V3, humidity);
  int soundLevel = getAverageSoundLevel(SOUND_SENSOR_PIN);
  Blynk.virtualWrite(V4, soundLevel);
  if (soundLevel > soundThreshold) {
    tone(BUZZER_PIN, 2000);
    Blynk.logEvent("baby_crying", "Crying Detected!");
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(httpTriggerURL);
      int response = http.GET();
      http.end();
    }
  }
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float totalAccel = abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z);
  Blynk.virtualWrite(V5, totalAccel);
  if (totalAccel > 15.0) {
    Blynk.logEvent("baby_moved", "Baby Moved!");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_OUTPUT);
  dht.begin();
  ds18b20.begin();
  if (!mpu.begin()) {
    Serial.println("MPU6050 not found!");
    while (1) delay(10);
  }
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(3000L, readSensors);
}

void loop() {
  Blynk.run();
  timer.run();
}
