#include "DHT.h"
#include <WiFi.h>
#include <FirebaseESP32.h>

// WiFi Configuration
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// Firebase Configuration
#define API_KEY  "API_KEY"
#define DATABASE_URL "DATABASE_URL"
#define USER_EMAIL "Example@gmail.com"
#define USER_PASSWORD "Password"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Pin Configuration
const int flameDO = 25;   
const int flameAO = 34;   
const int mqDO = 26;      
const int mqAO = 35;      
const int buzzerPin = 32;  // Buzzer 
const int ledPin = 33;     // LED
#define DHTPIN 27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Time Configuration
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 2 * 3600;
const int   daylightOffset_sec = 0;

// Timing Variables
unsigned long previousMillis = 0;
unsigned long previousStatusUpdate = 0;
unsigned long wifiReconnectMillis = 0;
const long interval = 3000;  // قراءة البيانات 
const long statusInterval = 1000;  // تحديث الحالة  
const long wifiCheckInterval = 30000;  // فحص WiFi 

// System Status Variables
bool setupComplete = false;
bool firebaseConnected = false;
int failedReadings = 0;
int successfulReadings = 0;
int firebaseErrors = 0;
int wifiDisconnects = 0;
unsigned long systemUptime = 0;
unsigned long lastSuccessfulRead = 0;

// Sensor History for averaging
const int historySize = 5;
float tempHistory[historySize] = {0};
float humHistory[historySize] = {0};
int historyIndex = 0;

// Alert Configuration
bool previousFireState = false;
bool previousGasState = false;
unsigned long lastAlertTime = 0;
const long alertCooldown = 2000;  // منع تكرار الإنذارن

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("ESP32 Advanced IoT Safety System\n");
  Serial.println("Version 2.0 Enhanced\n  ");
  
  // Sensors and Output Initialization
  Serial.print("→ [1/5] Initializing Hardware... ");
  dht.begin();
  pinMode(flameDO, INPUT);
  pinMode(mqDO, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  Serial.println(" OK");
  
  // WiFi Connection
  Serial.print("→ [2/5] Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int wifiTimeout = 0;
  while (WiFi.status() != WL_CONNECTED && wifiTimeout < 40) {
    delay(500);
    Serial.print(".");
    wifiTimeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected!");
    Serial.printf("  SSID: %s\n", WIFI_SSID);
    Serial.printf("  IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("  Signal: %d dBm\n", WiFi.RSSI());
    Serial.printf("  MAC: %s\n", WiFi.macAddress().c_str());
  } else {
    Serial.println(" Failed!");
    Serial.println(" System will continue with limited functionality");
  }
  
  // Firebase Configuration
  Serial.print("→ [3/5] Configuring Firebase... ");
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "tiny");
  Serial.println("OK");
  
  // Firebase Authentication
  Serial.print("→ [4/5] Authenticating... ");
  int fbTimeout = 0;
  while (!Firebase.ready() && fbTimeout < 30) {
    delay(1000);
    Serial.print(".");
    fbTimeout++;
  }
  
  if (Firebase.ready()) {
    Serial.println("Authenticated");
    firebaseConnected = true;
    
    // إرسال معلومات النظام الأولية
    FirebaseJson systemInfo;
    systemInfo.set("device_id", WiFi.macAddress());
    systemInfo.set("firmware_version", "2.0");
    systemInfo.set("boot_time", getTimeStamp());
    Firebase.setJSON(fbdo, "/SystemInfo", systemInfo);
  } else {
    Serial.println(" Failed!");
    Serial.println(" Data will be logged locally only");
  }
  
  // Time Synchronization
  Serial.print("→ [5/5] Syncing Time... ");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(1000);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(" Synchronized");
    Serial.printf("   • Current Time: %s\n", getTimeStamp().c_str());
  } else {
    Serial.println(" Failed");
  }
  
  Serial.println("System Ready - Monitoring Active  \n");
  
  setupComplete = true;
  systemUptime = millis();
  
  // Test Alert
  testAlert();
}

void testAlert() {
  Serial.println("→ Testing Alert System...");
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 2000, 200);
  delay(300);
  digitalWrite(ledPin, LOW);
  Serial.println(" Alert System OK\n");
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    firebaseConnected = false;
    Serial.printf(" Token Error: %s\n", info.error.message.c_str());
  } else if (info.status == token_status_ready) {
    firebaseConnected = true;
  }
}

String getTimeStamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Time-Error";
  }
  char buffer[30];
  strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buffer);
}

String getUptime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  
  char buffer[50];
  sprintf(buffer, "%lud %02lu:%02lu:%02lu", days, hours, minutes, seconds);
  return String(buffer);
}

float getAverageTemp() {
  float sum = 0;
  for (int i = 0; i < historySize; i++) {
    sum += tempHistory[i];
  }
  return sum / historySize;
}

float getAverageHum() {
  float sum = 0;
  for (int i = 0; i < historySize; i++) {
    sum += humHistory[i];
  }
  return sum / historySize;
}

void triggerAlert(bool fire, bool gas) {
  unsigned long currentTime = millis();
  
  // منع تكرار الإنذار 
  if (currentTime - lastAlertTime < alertCooldown) {
    return;
  }
  
  if (fire || gas) {
    digitalWrite(ledPin, HIGH);
    if (fire) {
      tone(buzzerPin, 2500, 500);  // صوت  للحريق
    } else {
      tone(buzzerPin, 1500, 300);  // صوت  للغاز
    }
    lastAlertTime = currentTime;
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiDisconnects++;
    Serial.println("\n WiFi Disconnected! Attempting reconnection...");
    WiFi.reconnect();
    
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
      delay(500);
      Serial.print(".");
      retries++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n WiFi Reconnected!");
    } else {
      Serial.println("\n Reconnection Failed");
    }
  }
}

void updateSystemStatus() {
  if (!Firebase.ready()) return;
  
  FirebaseJson status;
  status.set("online", true);
  status.set("uptime", getUptime());
  status.set("wifi_signal", WiFi.RSSI());
  status.set("wifi_connected", WiFi.status() == WL_CONNECTED);
  status.set("firebase_connected", firebaseConnected);
  status.set("successful_reads", successfulReadings);
  status.set("failed_reads", failedReadings);
  status.set("firebase_errors", firebaseErrors);
  status.set("wifi_disconnects", wifiDisconnects);
  status.set("free_heap", ESP.getFreeHeap());
  status.set("last_update", getTimeStamp());
  
  Firebase.setJSON(fbdo, "/SystemStatus", status);
}

void loop() {
  if (!setupComplete) return;
  
  unsigned long currentMillis = millis();
  
  // فحص اتصال WiFi بشكل دوري
  if (currentMillis - wifiReconnectMillis >= wifiCheckInterval) {
    wifiReconnectMillis = currentMillis;
    checkWiFiConnection();
  }
  
  // تحديث حالة النظام
  if (currentMillis - previousStatusUpdate >= statusInterval) {
    previousStatusUpdate = currentMillis;
    updateSystemStatus();
  }
  
  // قراءة البيانات من الحساسات
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // قراءة الحساسات
    int flameDigital = digitalRead(flameDO);
    int flameAnalog  = analogRead(flameAO);
    int mqDigital = digitalRead(mqDO);
    int mqAnalog  = analogRead(mqAO);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // التحقق من صحة القراءات
    bool validReading = !isnan(t) && !isnan(h) && t > -40 && t < 80 && h > 0 && h < 100;
    
    if (validReading) {
      successfulReadings++;
      lastSuccessfulRead = currentMillis;
      
      // حفظ في التاريخ عشان chart
      tempHistory[historyIndex] = t;
      humHistory[historyIndex] = h;
      historyIndex = (historyIndex + 1) % historySize;
    } else {
      failedReadings++;
      Serial.println("DHT Sensor Read Error!");
    }
    
    // كشف الخطر
    bool fireDanger = (flameDigital == HIGH);
    bool gasDanger = (mqDigital == LOW);
    bool anyDanger = (fireDanger || gasDanger);
    
    // تفعيل الإنذار
    triggerAlert(fireDanger, gasDanger);
    
    // طباعة التنبيهات
    if (fireDanger && !previousFireState) {
      Serial.println("FIRE ALERT - IMMEDIATE!\n");
    }
    
    if (gasDanger && !previousGasState) {
      Serial.println("GAS/SMOKE ALERT - EVACUATE! \n");
    }
    
    previousFireState = fireDanger;
    previousGasState = gasDanger;
    
    // عرض البيانات
    Serial.printf("Time: %-50s \n", getTimeStamp().c_str());
    
    if (validReading) {
      Serial.printf("Temp: %5.1f°C (Avg: %.1f°C) |  Humidity: %5.1f%%  \n", 
                    t, getAverageTemp(), h);
    } else {
      Serial.printf("Temp: ERROR        |  Humidity: ERROR           \n");
    }
    
    Serial.printf("Flame: D=%d A=%4d    |  Gas: D=%d A=%4d          \n", 
                  flameDigital, flameAnalog, mqDigital, mqAnalog);
    
    Serial.printf("Status: %-48s \n", 
                  anyDanger ? "  DANGER DETECTED!" : " All Systems Normal");
    
    // إرسال البيانات إلى Firebase
    if (Firebase.ready()) {
      String timestamp = getTimeStamp();
      FirebaseJson json;
      
      json.set("timestamp", timestamp);
      json.set("temperature", validReading ? t : -999);
      json.set("humidity", validReading ? h : -999);
      json.set("temperature_avg", validReading ? getAverageTemp() : -999);
      json.set("humidity_avg", validReading ? getAverageHum() : -999);
      json.set("flame/digital", flameDigital);
      json.set("flame/analog", flameAnalog);
      json.set("gas/digital", mqDigital);
      json.set("gas/analog", mqAnalog);
      json.set("danger", anyDanger ? 1 : 0);
      json.set("fire_detected", fireDanger ? 1 : 0);
      json.set("gas_detected", gasDanger ? 1 : 0);
      json.set("sensor_status", validReading ? "OK" : "ERROR");
      json.set("uptime", getUptime());
      json.set("wifi_signal", WiFi.RSSI());
      
      // تحديث القراءات الحالية
      if (Firebase.setJSON(fbdo, "/Sensors", json)) {
        // نجح الإرسال
      } else {
        firebaseErrors++;
        Serial.printf(" Firebase Error: %s\n", fbdo.errorReason().c_str());
      }
      
      // حفظ في السجل التاريخي
      String logPath = "/SensorLogs/" + String(currentMillis);
      Firebase.setJSON(fbdo, logPath.c_str(), json);
      
      // حفظ الإنذارات في قاعدة منفصلة
      if (anyDanger) {
        FirebaseJson alertJson;
        alertJson.set("timestamp", timestamp);
        alertJson.set("type", fireDanger ? "FIRE" : "GAS");
        alertJson.set("severity", "HIGH");
        alertJson.set("message", fireDanger ? "Fire detected!" : "Gas-Smoke detected!");
        
        String alertPath = "/Alerts/" + String(currentMillis);
        Firebase.setJSON(fbdo, alertPath.c_str(), alertJson);
      }
    }
  }
  
  delay(10);
}
