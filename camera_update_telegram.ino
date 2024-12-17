#define BLYNK_TEMPLATE_ID "TMPL6BOxc6ZR7"
#define BLYNK_TEMPLATE_NAME "home"
#define BLYNK_AUTH_TOKEN "ypfXsFyiNdiSpXmtEJ9xlRqwRjz1d3e8"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "manader.h"
#include "telegram.h"
#include "camera_config.h"
#define TRIGGER_PIN 0

unsigned long buttonPressedTime = 0;  
unsigned long debounceDelay = 50;     
bool buttonState = LOW;              
bool lastButtonState = LOW;  

BlynkTimer timer;

// ฟังก์ชัน BLYNK_WRITE สำหรับ V0
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    sendPhotoTelegram("capure photo");
  }
}



void setup() {
  Serial.begin(115200);
//  wm.resetSettings();
  setupWiFiManager();
  configInitCamera();
  clientTCP.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  wm.setConfigPortalTimeout(180);

  bool res = wm.autoConnect("camera_esp", "admin_one");

  if (!res) {
    Serial.println("Failed to connect or hit timeout");
  } else {
    Serial.println("Connected to WiFi!");
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi.SSID().c_str(), WiFi.psk().c_str(), "blynk.cloud", 80);
  loadConfigFiletelegram();
  sendPhotoTelegram("เริ่มต้นระบบกล้อง");
}
unsigned long check =  0;
void loop() {
  if (wm_nonblocking) wm.process();

  unsigned long currentMillis = millis();  

  bool reading = digitalRead(TRIGGER_PIN);  
  if (reading != lastButtonState) {
    buttonPressedTime = millis();  
  }
  if (reading == HIGH && (millis() - buttonPressedTime) > debounceDelay) {
    if ((millis() - buttonPressedTime) >= 7000) { 
      wm.resetSettings();
      ESP.restart();
      wm.setConfigPortalTimeout(180);
      if (!wm.startConfigPortal("camera_esp", "ad_camera")) {
        Serial.println("Failed to connect or hit timeout");
      }
    } else if ((millis() - buttonPressedTime) >= 1000) { // ถ้ากด 1 วินาที
      wm.setConfigPortalTimeout(180);
      if (!wm.startConfigPortal("camera_esp", "admin")) {
        Serial.println("Failed to connect or hit timeout");
      }
    }
  }

  lastButtonState = reading;  


  Blynk.run();
  timer.run();
}
