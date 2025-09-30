# IoT Safety Monitoring System

A comprehensive ESP32-based environmental monitoring and safety system with real-time Firebase integration and an advanced web dashboard for detecting fire, gas/smoke hazards, and monitoring temperature and humidity.

![System Status](https://img.shields.io/badge/Status-Production-success)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![License](https://img.shields.io/badge/License-MIT-green)

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [Web Dashboard](#web-dashboard)
- [Firebase Database Structure](#firebase-database-structure)
- [API Reference](#api-reference)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## 🎯 Overview

This IoT Safety Monitoring System is a complete solution for environmental monitoring and hazard detection. It combines ESP32 microcontroller capabilities with Firebase real-time database and a sophisticated web dashboard to provide:

- **Real-time hazard detection** for fire and gas/smoke
- **Environmental monitoring** of temperature and humidity
- **Automated alerting** with visual and audible warnings
- **Historical data logging** and trend analysis
- **Remote monitoring** via web dashboard
- **System health tracking** and diagnostics

The system is designed for residential, commercial, or industrial applications where continuous environmental monitoring and rapid hazard detection are critical.

## ✨ Features

### Hardware Features
- **Multi-sensor Integration**
  - DHT11 temperature and humidity sensor
  - Flame detection sensor (digital and analog)
  - MQ-series gas/smoke sensor (digital and analog)
  - Active buzzer for audible alerts
  - LED indicator for visual alerts

- **Robust Connectivity**
  - WiFi connectivity with auto-reconnection
  - Firebase real-time database integration
  - NTP time synchronization
  - Connection health monitoring

### Software Features
- **Advanced Data Processing**
  - Moving average calculation for temperature and humidity
  - Sensor reading validation and error handling
  - Historical data buffering
  - Alert cooldown to prevent false alarms

- **Comprehensive Logging**
  - Real-time sensor data logging
  - System status tracking
  - Alert history with timestamps
  - Diagnostic information (uptime, memory, WiFi signal)

### Dashboard Features
- **Real-time Monitoring**
  - Live sensor readings with 3-second update rate
  - Dynamic status indicators
  - Color-coded alert cards
  - Responsive design for all devices

- **Data Visualization**
  - Interactive Chart.js graphs
  - Temperature and humidity trends
  - Analog sensor reading plots
  - Historical data analysis (15-point rolling window)

- **System Diagnostics**
  - Connection status monitoring
  - WiFi signal strength indicator
  - System uptime tracking
  - Read success/failure statistics
  - Memory usage monitoring

- **Alert Management**
  - Recent alerts display (last 10)
  - Alert categorization (fire/gas)
  - Timestamp tracking
  - Alert history persistence

## 🏗️ System Architecture

```
┌─────────────────┐
│   ESP32 Board   │
└────────┬────────┘
         │
    ┌────┴────┐
    │ Sensors │
    ├─────────┤
    │  DHT11  │ ──► Temperature & Humidity
    │  Flame  │ ──► Fire Detection
    │   MQ    │ ──► Gas/Smoke Detection
    └────┬────┘
         │
    ┌────┴────┐
    │ Outputs │
    ├─────────┤
    │ Buzzer  │ ──► Audible Alert
    │   LED   │ ──► Visual Alert
    └────┬────┘
         │
    ┌────┴────────┐
    │ WiFi Module │
    └──────┬──────┘
           │
    ┌──────┴───────────┐
    │ Firebase RTDB    │
    │  - Sensors/      │
    │  - SystemStatus/ │
    │  - SystemInfo/   │
    │  - Alerts/       │
    │  - SensorLogs/   │
    └──────┬───────────┘
           │
    ┌──────┴──────┐
    │ Web Dashboard│
    │  - Real-time │
    │  - Charts    │
    │  - History   │
    └──────────────┘
```

## 🔧 Hardware Requirements

### Core Components
| Component | Specification | Quantity |
|-----------|--------------|----------|
| ESP32 Development Board | ESP32-WROOM-32 or similar | 1 |
| DHT11 Sensor | Temperature & Humidity | 1 |
| Flame Sensor Module | Infrared flame detector | 1 |
| MQ-2/MQ-5 Gas Sensor | Gas/smoke detector | 1 |
| Buzzer | Active buzzer (3.3V/5V) | 1 |
| LED | Any color (recommend red) | 1 |
| Resistor | 220Ω-330Ω (for LED) | 1 |
| Breadboard | Standard size | 1 |
| Jumper Wires | Male-to-male, male-to-female | 20+ |
| USB Cable | Micro USB (for ESP32) | 1 |
| Power Supply | 5V, 2A minimum | 1 |

### Pin Connections
```
ESP32 Pin → Component
─────────────────────
GPIO 27   → DHT11 Data
GPIO 25   → Flame Sensor Digital Out
GPIO 34   → Flame Sensor Analog Out
GPIO 26   → Gas Sensor Digital Out
GPIO 35   → Gas Sensor Analog Out
GPIO 32   → Buzzer Positive
GPIO 33   → LED Anode (+ resistor)
GND       → All component grounds
3.3V/5V   → Sensor power (check specs)
```

## 💻 Software Requirements

### Development Environment
- **Arduino IDE** 1.8.19 or higher (or Arduino IDE 2.x)
- **ESP32 Board Support** via Board Manager
- **USB Drivers** (CP210x or CH340 depending on board)

### Required Libraries
Install via Arduino Library Manager:

```cpp
// Core Libraries
- DHT sensor library by Adafruit (v1.4.4+)
- Adafruit Unified Sensor (v1.1.9+)
- WiFi (built-in with ESP32)

// Firebase Libraries
- Firebase ESP32 Client by Mobizt (v4.3.5+)

// System Libraries (built-in)
- time.h
```

### Web Dashboard Requirements
- **Modern Web Browser**
  - Chrome 90+
  - Firefox 88+
  - Safari 14+
  - Edge 90+
- **Internet Connection** (for Firebase and CDN resources)

### Firebase Requirements
- **Firebase Project** with Realtime Database
- **Authentication** enabled (Email/Password)
- **Database Rules** configured for read/write access

## 📦 Installation

### 1. Hardware Setup

1. **Connect the DHT11 Sensor:**
   - VCC → 3.3V or 5V
   - GND → GND
   - DATA → GPIO 27

2. **Connect the Flame Sensor:**
   - VCC → 5V
   - GND → GND
   - DO → GPIO 25
   - AO → GPIO 34

3. **Connect the Gas Sensor:**
   - VCC → 5V
   - GND → GND
   - DO → GPIO 26
   - AO → GPIO 35

4. **Connect Alert Outputs:**
   - Buzzer: Positive → GPIO 32, Negative → GND
   - LED: Anode → GPIO 33 → 220Ω resistor → Cathode → GND

### 2. Software Setup

#### Arduino IDE Configuration

1. **Install ESP32 Board Support:**
   ```
   File → Preferences → Additional Board Manager URLs:
   https://dl.espressif.com/dl/package_esp32_index.json
   ```

2. **Install ESP32 Boards:**
   ```
   Tools → Board → Boards Manager → Search "ESP32" → Install
   ```

3. **Install Required Libraries:**
   ```
   Sketch → Include Library → Manage Libraries
   - Search and install: DHT sensor library
   - Search and install: Adafruit Unified Sensor
   - Search and install: Firebase ESP32 Client
   ```

4. **Select Board:**
   ```
   Tools → Board → ESP32 Arduino → ESP32 Dev Module
   ```

5. **Configure Upload Settings:**
   ```
   Tools → Upload Speed: 115200
   Tools → Flash Frequency: 80MHz
   Tools → Partition Scheme: Default 4MB
   ```

#### Firebase Setup

1. **Create Firebase Project:**
   - Go to [Firebase Console](https://console.firebase.google.com/)
   - Click "Add Project"
   - Enter project name (e.g., "iot-safety-system")
   - Disable Google Analytics (optional)
   - Click "Create Project"

2. **Enable Realtime Database:**
   - In Firebase Console, go to "Build" → "Realtime Database"
   - Click "Create Database"
   - Select location (choose closest to you)
   - Start in "Test mode" (for development)
   - Click "Enable"

3. **Get Database URL:**
   - Copy the database URL (e.g., `https://your-project.firebaseio.com`)

4. **Get API Key:**
   - Go to Project Settings (gear icon)
   - Under "General" tab, find "Web API Key"
   - Copy this key

5. **Enable Authentication:**
   - Go to "Build" → "Authentication"
   - Click "Get Started"
   - Select "Email/Password"
   - Enable it and save

6. **Create User Account:**
   - In Authentication, click "Add User"
   - Enter email and password
   - Click "Add User"

7. **Configure Database Rules** (for production):
   ```json
   {
     "rules": {
       ".read": "auth != null",
       ".write": "auth != null"
     }
   }
   ```

### 3. Code Configuration

1. **Open the Arduino sketch** (ESP32 code)

2. **Update WiFi credentials:**
   ```cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASSWORD "YourWiFiPassword"
   ```

3. **Update Firebase configuration:**
   ```cpp
   #define API_KEY "Your-Firebase-API-Key"
   #define DATABASE_URL "https://your-project.firebaseio.com"
   #define USER_EMAIL "your-email@example.com"
   #define USER_PASSWORD "your-password"
   ```

4. **Upload Code:**
   - Connect ESP32 via USB
   - Select correct COM port: `Tools → Port`
   - Click Upload button
   - Wait for "Done uploading" message

5. **Monitor Serial Output:**
   - Open Serial Monitor: `Tools → Serial Monitor`
   - Set baud rate to `115200`
   - Observe connection and sensor initialization

### 4. Web Dashboard Setup

1. **Open the HTML file** in a text editor

2. **Update Firebase configuration:**
   ```javascript
   const firebaseConfig = {
       apiKey: "Your-Firebase-API-Key",
       databaseURL: "https://your-project.firebaseio.com",
       authDomain: "your-project.firebaseapp.com"
   };
   ```

3. **Update authentication:**
   ```javascript
   auth.signInWithEmailAndPassword("your-email@example.com", "your-password")
   ```

4. **Deploy Dashboard:**
   - **Option A:** Open HTML file directly in browser (for testing)
   - **Option B:** Host on Firebase Hosting (recommended)
   - **Option C:** Host on any web server (Apache, Nginx, etc.)

#### Firebase Hosting Deployment (Optional)

```bash
# Install Firebase CLI
npm install -g firebase-tools

# Login to Firebase
firebase login

# Initialize project
firebase init hosting

# Deploy
firebase deploy --only hosting
```

## ⚙️ Configuration

### Sensor Calibration

#### DHT11 Sensor
- No calibration needed
- Operating range: 0-50°C, 20-90% RH
- Accuracy: ±2°C, ±5% RH

#### Flame Sensor
- Adjust sensitivity potentiometer for desired detection range
- Digital threshold: Adjust until LED turns ON when flame detected
- Typical detection range: 80cm - 100cm

#### Gas Sensor (MQ-series)
- **Preheat time:** 24-48 hours for stable readings
- Adjust sensitivity potentiometer based on target gas
- Digital threshold: Set based on your safety requirements

### Alert Configuration

```cpp
// Alert cooldown (prevent rapid repeated alerts)
const long alertCooldown = 2000;  // 2 seconds

// Buzzer tones
tone(buzzerPin, 2500, 500);  // Fire: 2500Hz, 500ms
tone(buzzerPin, 1500, 300);  // Gas: 1500Hz, 300ms
```

### Data Update Intervals

```cpp
const long interval = 3000;           // Sensor reading: 3 seconds
const long statusInterval = 1000;     // System status: 1 second
const long wifiCheckInterval = 30000; // WiFi check: 30 seconds
```

### History Buffer Size

```cpp
const int historySize = 5;      // ESP32: 5 readings for averaging
const maxPoints = 15;           // Dashboard: 15 points on charts
```

## 🚀 Usage

### Starting the System

1. **Power on ESP32**
2. **Wait for initialization** (check Serial Monitor):
   ```
   ESP32 Advanced IoT Safety System
   Version 2.0 Enhanced
   
   → [1/5] Initializing Hardware... OK
   → [2/5] Connecting to WiFi... Connected!
   → [3/5] Configuring Firebase... OK
   → [4/5] Authenticating... Authenticated
   → [5/5] Syncing Time... Synchronized
   
   System Ready - Monitoring Active
   ```

3. **Open Web Dashboard**
4. **Verify connection status** (both badges should show "Connected")

### Normal Operation

The system will:
- Read sensors every 3 seconds
- Display readings on Serial Monitor
- Upload data to Firebase in real-time
- Update web dashboard automatically
- Trigger alerts when hazards detected
- Log all data with timestamps

### Alert Response

When hazard detected:

**Fire Alert:**
- 🔥 Red LED flashes
- 🔊 Buzzer sounds at 2500Hz
- 📱 Dashboard shows "FIRE DETECTED!"
- 📝 Alert logged to Firebase
- 🕒 Timestamped entry created

**Gas Alert:**
- 💨 Red LED flashes
- 🔊 Buzzer sounds at 1500Hz
- 📱 Dashboard shows "GAS DETECTED!"
- 📝 Alert logged to Firebase
- 🕒 Timestamped entry created

### Interpreting Dashboard Data

#### Status Badges
- **Green "Online"**: System functioning normally
- **Red "Offline"**: Connection lost or error
- **Yellow "Warning"**: Degraded performance

#### Sensor Cards
- **Climate Control**: Current and average temp/humidity
- **Flame Sensor**: Digital state and analog intensity
- **Gas Sensor**: Digital state and analog concentration

#### Charts
- **Temperature & Humidity Trends**: Last 15 readings
- **Sensor Analog Readings**: Raw ADC values (0-4095)

#### System Statistics
- **Uptime**: Time since last restart
- **Successful/Failed Reads**: Data quality metrics
- **WiFi Disconnects**: Network stability
- **Firebase Errors**: Cloud connectivity issues

## 🌐 Web Dashboard

<img width="1893" height="1075" alt="Image" src="https://github.com/user-attachments/assets/19af74a9-41ec-456e-8284-8d61ea230d2f" />
<img width="1894" height="289" alt="Image" src="https://github.com/user-attachments/assets/0a20f2da-ea04-4779-9f68-78670c826313" />
<img width="1893" height="763" alt="Image" src="https://github.com/user-attachments/assets/5026b32c-fa2e-419b-9d55-c4576ed01b7d" />
<img width="1887" height="575" alt="Image" src="https://github.com/user-attachments/assets/af5d5838-b9ce-4e1b-bec2-28bf995f629f" />
<img width="1889" height="884" alt="Image" src="https://github.com/user-attachments/assets/3c4fa347-4b96-4e5a-91ff-ad04a0c9d605" />


### Features

#### Real-time Monitoring
- Live sensor data updates every 3 seconds
- No page refresh required
- Automatic reconnection on connection loss

#### Interactive Charts
- Zoom and pan capabilities
- Hover for exact values
- Auto-scaling axes
- Smooth animations

#### Responsive Design
- Desktop: Multi-column layout
- Tablet: Adaptive grid
- Mobile: Single-column stack

#### Visual Alerts
- Color-coded danger indicators
- Animated status dots
- Gradient backgrounds
- Glassmorphism design

### Browser Compatibility

| Browser | Version | Support |
|---------|---------|---------|
| Chrome | 90+ | ✅ Full |
| Firefox | 88+ | ✅ Full |
| Safari | 14+ | ✅ Full |
| Edge | 90+ | ✅ Full |
| Opera | 76+ | ✅ Full |

### Performance Optimization

The dashboard includes:
- Efficient data updates (no full page reloads)
- Chart animation throttling
- Lazy loading of historical data
- Optimized CSS animations
- Minimal DOM manipulation

## 🗄️ Firebase Database Structure

```
iot-project/
├── Sensors/
│   ├── timestamp: "2025-09-30 14:23:45"
│   ├── temperature: 25.5
│   ├── humidity: 60.2
│   ├── temperature_avg: 25.3
│   ├── humidity_avg: 59.8
│   ├── flame/
│   │   ├── digital: 0
│   │   └── analog: 1234
│   ├── gas/
│   │   ├── digital: 0
│   │   └── analog: 567
│   ├── danger: 0
│   ├── fire_detected: 0
│   ├── gas_detected: 0
│   ├── sensor_status: "OK"
│   ├── uptime: "0d 02:15:32"
│   └── wifi_signal: -45
│
├── SystemStatus/
│   ├── online: true
│   ├── uptime: "0d 02:15:32"
│   ├── wifi_signal: -45
│   ├── wifi_connected: true
│   ├── firebase_connected: true
│   ├── successful_reads: 450
│   ├── failed_reads: 2
│   ├── firebase_errors: 0
│   ├── wifi_disconnects: 1
│   ├── free_heap: 245632
│   └── last_update: "2025-09-30 14:23:45"
│
├── SystemInfo/
│   ├── device_id: "AA:BB:CC:DD:EE:FF"
│   ├── firmware_version: "2.0"
│   └── boot_time: "2025-09-30 12:08:13"
│
├── Alerts/
│   ├── 1727700225000/
│   │   ├── timestamp: "2025-09-30 14:23:45"
│   │   ├── type: "FIRE"
│   │   ├── severity: "HIGH"
│   │   └── message: "Fire detected!"
│   └── 1727700230000/
│       ├── timestamp: "2025-09-30 14:23:50"
│       ├── type: "GAS"
│       ├── severity: "HIGH"
│       └── message: "Gas-Smoke detected!"
│
└── SensorLogs/
    ├── 1727700225000/
    │   └── [complete sensor data snapshot]
    └── 1727700228000/
        └── [complete sensor data snapshot]
```

### Data Types

- **Sensors/**: Current real-time readings (updates every 3s)
- **SystemStatus/**: Device health metrics (updates every 1s)
- **SystemInfo/**: Static device information (set once)
- **Alerts/**: Time-stamped danger events (created on detection)
- **SensorLogs/**: Historical snapshots (created every 3s)

## 📚 API Reference

### Reading Sensor Data

```javascript
// Get current sensor readings
database.ref('Sensors').once('value')
  .then((snapshot) => {
    const data = snapshot.val();
    console.log(data.temperature);
    console.log(data.humidity);
  });

// Listen for real-time updates
database.ref('Sensors').on('value', (snapshot) => {
  const data = snapshot.val();
  updateUI(data);
});
```

### Reading System Status

```javascript
// Get system health
database.ref('SystemStatus').once('value')
  .then((snapshot) => {
    const status = snapshot.val();
    console.log(status.uptime);
    console.log(status.wifi_signal);
  });
```

### Reading Alert History

```javascript
// Get last 10 alerts
database.ref('Alerts').limitToLast(10).once('value')
  .then((snapshot) => {
    snapshot.forEach((child) => {
      const alert = child.val();
      console.log(alert.message, alert.timestamp);
    });
  });
```

### Reading Historical Logs

```javascript
// Get logs from last hour
const oneHourAgo = Date.now() - (3600 * 1000);
database.ref('SensorLogs')
  .orderByKey()
  .startAt(String(oneHourAgo))
  .once('value')
  .then((snapshot) => {
    // Process historical data
  });
```

## 🔍 Troubleshooting

### ESP32 Won't Connect to WiFi

**Symptoms:** WiFi connection fails, "Failed!" message

**Solutions:**
1. Verify SSID and password are correct
2. Check WiFi network is 2.4GHz (ESP32 doesn't support 5GHz)
3. Move ESP32 closer to router
4. Restart router
5. Check Serial Monitor for specific error codes

### Firebase Authentication Fails

**Symptoms:** "Authentication Failed" message

**Solutions:**
1. Verify API key is correct
2. Check email/password credentials
3. Ensure Email/Password auth is enabled in Firebase Console
4. Verify database URL format
5. Check Firebase project is active

### DHT Sensor Returns NaN

**Symptoms:** Temperature/humidity show "ERROR"

**Solutions:**
1. Check sensor connections (especially DATA pin)
2. Verify power supply (3.3V or 5V per sensor specs)
3. Wait 2 seconds after power-on for sensor initialization
4. Replace sensor if defective
5. Add 10kΩ pull-up resistor to DATA line if needed

### False Fire/Gas Alarms

**Symptoms:** Alerts trigger without actual hazard

**Solutions:**
1. Adjust sensor sensitivity potentiometers
2. Increase `alertCooldown` value
3. Check for environmental interference (sunlight for flame, cooking for gas)
4. Allow gas sensor 24-48 hour preheat period
5. Calibrate sensors in clean environment

### Web Dashboard Shows "Offline"

**Symptoms:** Dashboard can't connect to Firebase

**Solutions:**
1. Check internet connection
2. Verify Firebase credentials in HTML
3. Check browser console for errors (F12)
4. Clear browser cache
5. Verify Firebase Realtime Database is active
6. Check database rules allow read/write

### Chart Data Not Updating

**Symptoms:** Graphs show flat lines or no data

**Solutions:**
1. Verify ESP32 is uploading data (check Serial Monitor)
2. Check Firebase database for new entries
3. Refresh browser page
4. Check browser console for JavaScript errors
5. Verify Chart.js CDN is loading

### High Memory Usage / ESP32 Crashes

**Symptoms:** ESP32 restarts randomly, "free_heap" decreasing

**Solutions:**
1. Reduce `historySize` constant
2. Increase data upload intervals
3. Reduce number of Firebase writes
4. Check for memory leaks in custom code
5. Use ESP.getFreeHeap() to monitor memory

### Inconsistent Sensor Readings

**Symptoms:** Wild fluctuations in data

**Solutions:**
1. Add capacitors to sensor power lines (0.1µF ceramic)
2. Use shielded cables for longer connections
3. Increase averaging buffer size
4. Add delays between sensor reads
5. Check power supply stability

## 🤝 Contributing

Contributions are welcome! 

### Reporting Bugs

1. Check existing issues first
2. Use the bug report template
3. Include:
   - ESP32 board model
   - Sensor models
   - Complete error messages
   - Serial Monitor output
   - Steps to reproduce

### Suggesting Enhancements

1. Check existing feature requests
2. Describe the feature clearly
3. Explain use cases
4. Provide mockups if applicable

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style Guidelines

- Use descriptive variable names
- Add comments for complex logic
- Follow existing indentation
- Test thoroughly before submitting

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 Acknowledgments

### Libraries & Frameworks
- [Adafruit DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [Firebase ESP32 Client](https://github.com/mobizt/Firebase-ESP32)
- [Chart.js](https://www.chartjs.org/)
- [Firebase JavaScript SDK](https://firebase.google.com/docs/web/setup)

### Resources
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Firebase Documentation](https://firebase.google.com/docs)
- [Arduino Reference](https://www.arduino.cc/reference/en/)

### Inspiration
- IoT safety systems and smart home automation projects
- Environmental monitoring best practices
- Firebase real-time data visualization examples

## 📞 Contact & Support

- **Issues:** [GitHub Issues](https://github.com/obaidah3/iot-safety-system/issues)
- **Discussions:** [GitHub Discussions](https://github.com/obaidah3/iot-safety-system/discussions)
- **Email:** Abdulrahman.e.eissa@gmail.com

## 🗺️ Roadmap

### Version 2.1 (Planned)
- [ ] SMS/Email notifications
- [ ] Mobile app (React Native)
- [ ] Multiple device support
- [ ] Machine learning anomaly detection

### Version 3.0 (Future)
- [ ] Voice assistant integration (Alexa/Google)
- [ ] Advanced analytics dashboard
- [ ] Predictive maintenance
- [ ] Multi-zone monitoring

---

**Made with ❤️ for IoT enthusiasts and safety-conscious developers**
