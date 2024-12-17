#include "manader.h"
#define ESP_DRD_USE_SPIFFS true
#define JSON_CONFIG_FILE "/config.json"

bool shouldSaveConfig = false;
bool wm_nonblocking = false;

char Token_bot[120];
char Chat_id[40];

WiFiManager wm;


bool loadConfigFile() {
     SPIFFS.format();

  Serial.println("Mounting File System...");

  if (SPIFFS.begin(false) || SPIFFS.begin(true))
  {
    Serial.println("mounted file system");
    if (SPIFFS.exists(JSON_CONFIG_FILE))
    {
      Serial.println("reading config file");
      File configFile = SPIFFS.open(JSON_CONFIG_FILE, "r");
      if (configFile)
      {
        Serial.println("Opened configuration file");
        StaticJsonDocument<512> json;
        DeserializationError error = deserializeJson(json, configFile);
        serializeJsonPretty(json, Serial);
        if (!error)
        {
          Serial.println("Parsing JSON");

          strcpy(Token_bot, json["Token_bot"]);
          strcpy(Chat_id, json["Chat_id"]);
          return true;
        }
        else
        {
          Serial.println("Failed to load json config");
        }
      }
    }
  }
  else
  {
    Serial.println("Failed to mount FS");
  }

  return false;
}


void saveConfigFile() {
  Serial.println(F("Saving configuration..."));


  StaticJsonDocument<512> json;
  json["Token_bot"] = Token_bot;
  json["Chat_id"] = Chat_id;

  File configFile = SPIFFS.open(JSON_CONFIG_FILE, "w");
  if (!configFile)
  {
    Serial.println("failed to open config file for writing");
  }

  serializeJsonPretty(json, Serial);
  if (serializeJson(json, configFile) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  configFile.close();
}

void setupWiFiManager() {
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);
  delay(3000);
  Serial.println("\n Starting");
  bool forceConfig = false;
  bool spiffsSetup = loadConfigFile();
  if (!spiffsSetup)
  {
    Serial.println(F("Forcing config mode as there is no saved config"));
    forceConfig = true;
  }



  if (wm_nonblocking) wm.setConfigPortalBlocking(false);



  WiFiManagerParameter header_telegram("<hr>ตั้งค่าบอทTelegram <br><br>");
  wm.addParameter(&header_telegram);

  WiFiManagerParameter Token_bot_param("Token_bot", "ข้อมูล Token bot telegram", Token_bot, 120);
  WiFiManagerParameter Chat_id_param("Chat_id", "ข้อมูล Chat Id", Chat_id, 40);
  wm.addParameter(&Token_bot_param);
  wm.addParameter(&Chat_id_param);


 


  wm.setSaveParamsCallback(saveParamCallback);
  std::vector<const char *> menu = {"wifi", "restart"};
  wm.setMenu(menu);
  wm.setClass("invert");

  wm.setConfigPortalTimeout(180);

  bool res = wm.autoConnect("camera_esp", "ad_camera");

  if (!res) {
    Serial.println("Failed to connect or hit timeout");
  } else {
    Serial.println("Connected to WiFi!");
  }
  strncpy(Token_bot, Token_bot_param.getValue(), sizeof(Token_bot));
  strncpy(Chat_id, Chat_id_param.getValue(), sizeof(Chat_id));
  if (shouldSaveConfig)
  {
    saveConfigFile();
  }
}



void saveParamCallback() {
  shouldSaveConfig = true;
  Serial.println("[CALLBACK] saveParamCallback fired");
}

String getParam(String name) {
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}
