#ifndef MANAGER_H
#define MANAGER_H

#include <WiFiManager.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <FS.h>

extern bool wm_nonblocking;
extern WiFiManager wm;


void setupWiFiManager();
void saveParamCallback();
String getParam(String name);

#endif
