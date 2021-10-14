bool loadConfig(){                                                // Завантаження даних збереженних в файлі config.json
  File configFile = SPIFFS.open("/config.json", "r");              // Відкриваемо файл на читання
  if(!configFile) {                                                // якщо файл не знайдено ствоюємого його та записуємо в ньго данні з наших змінних
    if(printCom) Serial.println("Failed to open config file");
    saveConfig();                                                  // Створюємо файл
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  size_t size = configFile.size();                                 // Перевіряємо ромір файлу, будемо використовувати файл довжиною в 1024 байта
  if(printCom) {
    Serial.println("size config.file = " + (String) size);
  }
  if(size > 4096) {
    if(printCom) {
      Serial.println("Config file size is too large");
    }
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  jsonConfig = configFile.readString();                            // завантажуємо файл конфігурації в глобальну змінну
  DynamicJsonDocument doc(8192);                                    // Резервуємо память для json обекту буфер може розти по мірі необхідності переважно для ESP8266 
  deserializeJson(doc, jsonConfig);
  configFile.close();
  
  ssidAP = doc["ssidAP"].as<String>();
  passwordAP = doc["passwordAP"].as<String>();
  ssid[0] = doc["ssid0"].as<String>();
  password[0] = doc["password0"].as<String>();
  ssid[1] = doc["ssid1"].as<String>();
  password[1] = doc["password1"].as<String>();
  auth = doc["auth"].as<String>();
  authOn = doc["authOn"];
  kuOn = doc["kuOn"];
  kuOff = doc["kuOff"];
  weatherHost = doc["weatherHost"];
  weatherKey0 = doc["weatherKey0"].as<String>();
  weatherKey1 = doc["weatherKey1"].as<String>();
  cityID0 = doc["cityID0"].as<String>();
  cityID1 = doc["cityID1"].as<String>();
  snprintf(personalCityName, 51, "%s", (doc["PSN"].as<String>()).c_str());
  weatherLang = doc["weatherLang"].as<String>();
  displayForecast = doc["displayForecast"];
  displayCityName = doc["displayCityName"];
  displayForecastNow = doc["displayForecastNow"];
  displayForecastToday = doc["displayForecastToday"];
  displayForecastTomorrow = doc["displayForecastTomorrow"];
  animNotWeather = doc["animNotWeather"];
  timeStartViewWeather = doc["timeStartViewWeather"];
  timeEndViewWeather = doc["timeEndViewWeather"];
  timeScrollSpeed = doc["timeScrollSpeed"];
  MAX7219_ROTATE = doc["MAX7219_ROTATE"];
  updateOTAEnable = doc["updateOTAEnable"];
  volBrightnessD = doc["volBrightnessD"];
  volBrightnessN = doc["volBrightnessN"];
  timeDay = doc["timeDay"];
  timeNight = doc["timeNight"];
  volBrightnessAuto = doc["volBrightnessAuto"];
  lowLivelBrightness = doc["lowLivelBrightness"];
  upLivelBrightness = doc["upLivelBrightness"];
  clockNight = doc["clockNight"];
  isActiveBuzzer = doc["isActiveBuzzer"];
  function[0] = doc["function00"];
  function[1] = doc["function01"];
  function[2] = doc["function02"];
  function[3] = doc["function03"];
  function[4] = doc["function04"];
  function[5] = doc["function05"];
  function[6] = doc["function06"];
  function[7] = doc["function07"];
  function[8] = doc["function08"];
  function[9] = doc["function09"];
  function[10] = doc["function10"];
  function[11] = doc["function11"];
  period[0] = doc["period00"];
  period[1] = doc["period01"];
  period[2] = doc["period02"];
  period[3] = doc["period03"];
  period[4] = doc["period04"];
  period[5] = doc["period05"];
  period[6] = doc["period06"];
  period[7] = doc["period07"];
  period[8] = doc["period08"];
  period[9] = doc["period09"];
  period[10] = doc["period10"];
  period[11] = doc["period11"];
  printCom = doc["printCom"];
  sensore[0] = doc["sensor00"];
  sensore[1] = doc["sensor01"];
  sensore[2] = doc["sensor02"];
  sensore[3] = doc["sensor03"];
  sensore[4] = doc["sensor04"];
  param0 = doc["param0"];
  param1 = doc["param1"];
  param2 = doc["param2"];
  param3 = doc["param3"];
  param4 = doc["param4"];
  pressSys = doc["pressSys"];
  fontCLOCK = doc["fontCLOCK"];
  aliData = doc["aliData"];
  animPoint = doc["animPoint"];
  corr00 = doc["corr00"];
  corr01 = doc["corr01"];
  corr02 = doc["corr02"];
  corr03  = doc["corr03"];
  corr04 = doc["corr04"];
  MAX7219_NUM = doc["MAX7219_NUM"];
  rtcStat = doc["rtcStat"];
  uuid  = doc["uuid"].as<String>();
  api_key = doc["api_key"].as<String>();
  sensors_ID0 = doc["sensors_ID0"];
  sensors_ID1 = doc["sensors_ID1"];
  sensors_ID2 = doc["sensors_ID2"];
  displayData = doc["displayData"];
  butStat = doc["butStat"];
  sgpCo2LivelAlarm = doc["sgpCo2LivelAlarm"];
  eCo2AlarmEsp = doc["eCo2AlarmEsp"];
  eCo2Led = doc["eCo2Led"];
  sgpTvocLivelAlarm = doc["sgpTvocLivelAlarm"];
  tvocAlarmEsp = doc["tvocAlarmEsp"];
  tvocLed = doc["tvocLed"];
  setSgpCorr = doc["setSgpCorr"];

printCom = true;
updateOTAEnable = true;

  if(MAX7219_NUM == 0) {
    MAX7219_NUM = 4;
  }
  
  if(printCom) {
    printTime();
    Serial.print("Load Config : ");
    Serial.println(jsonConfig);
  }
  return true;
}

//================================================================================================================================================
bool loadAlarm(){                                                // Завантаження даних збереженних в файлі config.json
  File configFile = SPIFFS.open("/alarm.json", "r");              // Відкриваемо файл на читання
  if(!configFile) {                                                // якщо файл не знайдено ствоюємого його та записуємо в ньго данні з наших змінних
    if(printCom) Serial.println("Failed to open alarm file");
    saveAlarm();                                                  // Створюємо файл
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  size_t size = configFile.size();                                 // Перевіряємо ромір файлу, будемо використовувати файл довжиною в 1024 байта
  if(printCom) Serial.println("size alarme.file = " + (String) size);
  if(size > 2048) {
    if(printCom) Serial.println("Config file size is too large");
    configFile.close();
    return false;                                                  // Повернення з помилкою
  }
  jsonAlarm = configFile.readString();                            // завантажуємо файл конфігурації в глобальну змінну
  DynamicJsonDocument doc(4096);                                   // Резервуємо память для json обекту буфер може розти по мірі необхідності переважно для ESP8266 
  deserializeJson(doc, jsonAlarm);
  configFile.close();
  ntpServerName = doc["ntpServerName"].as<String>();
  timeZone = doc["timeZone"];                                      // Так отримуємо число
  isDayLightSaving = doc["isDayLightSaving"];
  alarme[0][0] = doc["al_0_0"];
  alarme[0][1] = doc["al_0_1"];
  alarme[0][2] = doc["al_0_2"];
  alarme[1][0] = doc["al_1_0"];
  alarme[1][1] = doc["al_1_1"];
  alarme[1][2] = doc["al_1_2"];
  alarme[2][0] = doc["al_2_0"];
  alarme[2][1] = doc["al_2_1"];
  alarme[2][2] = doc["al_2_2"];
  alarme[3][0] = doc["al_3_0"];
  alarme[3][1] = doc["al_3_1"];
  alarme[3][2] = doc["al_3_2"];
  alarme[4][0] = doc["al_4_0"];
  alarme[4][1] = doc["al_4_1"];
  alarme[4][2] = doc["al_4_2"];
  rtcStat = doc["rtcStat"];

  if(printCom) {
    printTime();
    Serial.print("Load Alarm(config): ");
    Serial.println(jsonAlarm);
  }
  return true;
}

//=================================================================
bool saveConfig(){
  DynamicJsonDocument doc(8192);
  deserializeJson(doc, jsonConfig);
  doc["ssidAP"] = ssidAP;
  doc["passwordAP"] = passwordAP;
  doc["ssid0"] = ssid[0];
  doc["password0"] = password[0];
  doc["ssid1"] = ssid[1];
  doc["password1"] = password[1];
  doc["auth"] = auth;
  doc["authOn"] = authOn;
  doc["kuOn"] = kuOn;
  doc["kuOff"] = kuOff;
  doc["weatherHost"] = weatherHost;
  doc["weatherKey0"] = weatherKey0;
  doc["weatherKey1"] = weatherKey1;
  doc["cityID0"] = cityID0;
  doc["cityID1"] = cityID1;
  doc["PSN"] = chr_to_str(personalCityName);
  doc["weatherLang"] = weatherLang;
  doc["displayForecast"] = displayForecast;
  doc["displayCityName"] = displayCityName;
  doc["displayForecastNow"] = displayForecastNow;
  doc["displayForecastToday"] = displayForecastToday;
  doc["displayForecastTomorrow"] = displayForecastTomorrow;
  doc["timeStartViewWeather"] = timeStartViewWeather;
  doc["timeEndViewWeather"] = timeEndViewWeather;
  doc["timeScrollSpeed"] = timeScrollSpeed;
  doc["MAX7219_ROTATE"] = MAX7219_ROTATE;
  doc["updateOTAEnable"] = updateOTAEnable;
  doc["volBrightnessD"] = volBrightnessD;
  doc["volBrightnessN"] = volBrightnessN;
  doc["timeDay"] = timeDay;
  doc["timeNight"] = timeNight;
  doc["volBrightnessAuto"] = volBrightnessAuto;
  doc["lowLivelBrightness"] = lowLivelBrightness;
  doc["upLivelBrightness"] = upLivelBrightness;
  doc["clockNight"] = clockNight;
  doc["isActiveBuzzer"] = isActiveBuzzer;
  doc["function00"] = function[0];
  doc["function01"] = function[1];
  doc["function02"] = function[2];
  doc["function03"] = function[3];
  doc["function04"] = function[4];
  doc["function05"] = function[5];
  doc["function06"] = function[6];
  doc["function07"] = function[7];
  doc["function08"] = function[8];
  doc["function09"] = function[9];
  doc["function10"] = function[10];
  doc["function11"] = function[11];
  doc["period00"] = period[0];
  doc["period01"] = period[1];
  doc["period02"] = period[2];
  doc["period03"] = period[3];
  doc["period04"] = period[4];
  doc["period05"] = period[5];
  doc["period06"] = period[6];
  doc["period07"] = period[7];
  doc["period08"] = period[8];
  doc["period09"] = period[9];
  doc["period10"] = period[10];
  doc["period11"] = period[11];
  doc["printCom"] = printCom;
  doc["sensor00"] = sensore[0];
  doc["sensor01"] = sensore[1];
  doc["sensor02"] = sensore[2];
  doc["sensor03"] = sensore[3];
  doc["sensor04"] = sensore[4];
  doc["param0"] = param0; 
  doc["param1"] = param1;
  doc["param2"] = param2;
  doc["param3"] = param3;
  doc["param4"] = param4;
  doc["pressSys"] = pressSys;
  doc["fontCLOCK"] = fontCLOCK;
  doc["aliData"] = aliData;
  doc["animPoint"] = animPoint;
  doc["corr00"] = corr00;
  doc["corr01"] = corr01;
  doc["corr02"] = corr02;
  doc["corr03"] = corr03;
  doc["corr04"] = corr04;
  doc["MAX7219_NUM"] = MAX7219_NUM;
  doc["rtcStat"] = rtcStat;
  doc["uuid"] = uuid;
  doc["api_key"] = api_key;
  doc["sensors_ID0"] = sensors_ID0;
  doc["sensors_ID1"] = sensors_ID1;
  doc["sensors_ID2"] = sensors_ID2;
  doc["displayData"] = displayData;
  doc["butStat"] = butStat;
  doc["sgpCo2LivelAlarm"] = sgpCo2LivelAlarm;
  doc["eCo2AlarmEsp"] = eCo2AlarmEsp;
  doc["eCo2Led"] = eCo2Led;
  doc["sgpTvocLivelAlarm"] = sgpTvocLivelAlarm;
  doc["tvocAlarmEsp"] = tvocAlarmEsp;
  doc["tvocLed"] = tvocLed;
  doc["setSgpCorr"] = setSgpCorr;

  jsonConfig = "";
  if(serializeJson(doc, jsonConfig) == 0) {
    if(printCom) {
      Serial.println(F("Failed to write to jsonConfig"));
    }
  }
  
  // Відкриваємо файл для запису
  File configFile = SPIFFS.open("/config.json", "w");
  if(!configFile) {
    configFile.close();
    return false;
  }
  
  if(serializeJson(doc, configFile) == 0) {
    if(printCom) {
      Serial.println(F("Failed to write to file"));
    }
  } 
  
  if(printCom) {
    printTime();
    Serial.print("Save Config : ");
    Serial.println(jsonConfig);
  }
  configFile.close();
  bip();
  return true;
}

//=================================================================
bool saveAlarm() {
  DynamicJsonDocument doc(4096);
  deserializeJson(doc, jsonAlarm);
  doc["ntpServerName"] = ntpServerName;
  doc["timeZone"] = timeZone;
  doc["isDayLightSaving"] = isDayLightSaving;
  doc["al_0_0"] = alarme[0][0];
  doc["al_0_1"] = alarme[0][1];
  doc["al_0_2"] = alarme[0][2];
  doc["al_1_0"] = alarme[1][0];
  doc["al_1_1"] = alarme[1][1];
  doc["al_1_2"] = alarme[1][2];
  doc["al_2_0"] = alarme[2][0];
  doc["al_2_1"] = alarme[2][1];
  doc["al_2_2"] = alarme[2][2];
  doc["al_3_0"] = alarme[3][0];
  doc["al_3_1"] = alarme[3][1];
  doc["al_3_2"] = alarme[3][2];
  doc["al_4_0"] = alarme[4][0];
  doc["al_4_1"] = alarme[4][1];
  doc["al_4_2"] = alarme[4][2];
  doc["rtcStat"] = rtcStat;
  
  jsonAlarm = "";
  if(serializeJson(doc, jsonAlarm) == 0) {
    if(printCom) {
      Serial.println(F("Failed to write to jsonConfig"));
    }
  }

  // Відкриваємо файл для запису
  File configFile = SPIFFS.open("/alarm.json", "w");
  if(!configFile) {
    configFile.close();
    return false;
  }

  if(serializeJson(doc, configFile) == 0) {
    if(printCom) {
      Serial.println(F("Failed to write to file"));
    }
  } 

  if(printCom) {
    printTime();
    Serial.print("Save Alarm : ");
    Serial.println(jsonAlarm);
  }
  configFile.close();
  bip();
  return true;
}
