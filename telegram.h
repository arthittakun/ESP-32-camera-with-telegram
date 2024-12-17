
#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

extern WiFiClientSecure clientTCP;
extern UniversalTelegramBot bot;

// Declare functions
void configInitCamera();
bool loadConfigFiletelegram();
String sendPhotoTelegram(String text);

#endif // TELEGRAM_BOT_H
