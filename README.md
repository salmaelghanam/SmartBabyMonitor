# 🍼 Smart Baby Monitor

An IoT-based baby monitoring system that combines real-time sensor data, AI prediction, and cloud connectivity to keep parents informed about their baby's wellbeing.

---

## ✨ Features

- 🌡️ **Body Temperature Monitoring** — Detects fever using a DS18B20 sensor and triggers an alert if temperature exceeds 37.5°C
- 🔊 **Cry Detection** — Detects baby crying via a sound sensor and sends a real-time Blynk push notification
- 💧 **Room Humidity & Temperature** — Monitors the environment using a DHT11 sensor
- 📳 **Motion Detection** — Detects baby movement using an MPU6050 accelerometer
- 🤖 **AI Baby State Prediction** — A Flask server runs a machine learning model to predict the baby's state (e.g., discomfort) based on sensor readings
- 📲 **Blynk Cloud Dashboard** — All sensor data is streamed live to a Blynk IoT dashboard viewable from any device
- 🔔 **Push Notifications** — Instant alerts sent to the parent's phone for crying, high temperature, or movement events

---

## 🏗️ System Architecture

```
┌──────────────────────────────────────────────┐
│              ESP32 Microcontroller           │
│                                              │
│  DS18B20 → Body Temp      MPU6050 → Motion  │
│  DHT11   → Room Temp/Hum  Sound  → Cry Det. │
│                                              │
│  Every 3s: reads all sensors                 │
│  → Sends data to Blynk Cloud (V1–V5)         │
│  → On cry detected: HTTP GET → Flask Server  │
└──────────────┬────────────────────┬──────────┘
               │                    │
               ▼                    ▼
     ┌─────────────────┐   ┌──────────────────┐
     │   Blynk Cloud   │   │  Flask AI Server │
     │  Dashboard &    │   │  /trigger route  │
     │  Notifications  │   │  → ML Prediction │
     └─────────────────┘   └──────────────────┘
```

---

## 🔌 Hardware Components

| Component | Purpose | Pin |
|-----------|---------|-----|
| ESP32 | Main microcontroller | — |
| DS18B20 | Body temperature sensor | GPIO 4 |
| DHT11 | Room temperature & humidity | GPIO 15 |
| Sound Sensor (analog) | Cry / noise detection | GPIO 36 |
| MPU6050 | Accelerometer (motion detection) | I2C |
| Buzzer | Local audio alert | GPIO 18 |

---

## 📊 Blynk Virtual Pins

| Virtual Pin | Sensor Data |
|-------------|-------------|
| V1 | Body Temperature (°C) |
| V2 | Room Temperature (°C) |
| V3 | Humidity (%) |
| V4 | Sound Level |
| V5 | Total Acceleration |

---

## 🤖 AI Prediction (Flask Server)

A machine learning model (`baby_model.pkl`) runs on a local server. When crying is detected, the ESP32 sends an HTTP GET request to `/trigger`, which returns a JSON prediction of the baby's state.

**Example response:**
```json
{
  "prediction": "discomfort"
}
```

The `/predict` endpoint also accepts POST requests with sensor values:
```json
{
  "temp": 37.2,
  "cry": 1,
  "move": 0
}
```

---

## 📂 Project Structure

```
SmartBabyMonitor/
│
├── hardware.ino          # Arduino/ESP32 firmware
├── app.py                # Flask AI prediction server
├── ai.ipynb              # ML model training notebook
└── donateacry_corpus/    # Dataset used for training cry detection
```

---

## 🚀 Getting Started

### 1. Hardware Setup

- Wire up components according to the pin table above
- Flash `hardware.ino` to your ESP32 using Arduino IDE

### 2. Configure Credentials

Open `hardware.ino` and fill in:
```cpp
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
const char* httpTriggerURL = "http://YOUR_SERVER_IP:5000/trigger";
```

### 3. Set Up Blynk

1. Create a new template in [Blynk Cloud](https://blynk.cloud)
2. Add datastream widgets for V1–V5
3. Set up event notifications for `high_body_temp`, `baby_crying`, and `baby_moved`

### 4. Run the AI Server

```bash
pip install flask joblib scikit-learn
python app.py
```

The server will run on `http://0.0.0.0:5000`

---

## 📦 Dependencies

### Arduino Libraries
- `BlynkSimpleEsp32`
- `DHT sensor library`
- `DallasTemperature` + `OneWire`
- `Adafruit MPU6050` + `Adafruit Unified Sensor`

### Python
- `flask`
- `joblib`
- `scikit-learn`

---

## 👩‍💻 Author

**Salma Elghanam**  
Computer Science & AI Student  
[GitHub Profile](https://github.com/salmaelghanam)

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
