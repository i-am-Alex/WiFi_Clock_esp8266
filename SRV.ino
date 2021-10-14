void server_init(void) {
  server.on("/", auth_auth);
  server.on("/index.vz", authIndex);
  server.on("/wifi.vz", auth_wifi);
  server.on("/time.vz", auth_time);
  server.on("/weather.vz", auth_weather);
  server.on("/setup.vz", auth_setup);
  server.on("/sgp.vz", auth_sgp);
  server.on("/help.vz", auth_help);
  server.on("/style.css", [](){server.send_P ( 200, "text/css", P_css);});
  server.on("/function.js", [](){server.send_P ( 200, "text/plain", P_js);});
  server.on("/favicon.ico", [](){server.send(200, "text/html", "");});
  server.on("/configs.json", handle_ConfigJSON);    // формування configs.json сторінки для передачі данних в web інтерфейс
  server.on("/index-data.json", get_IndexDataJson);
  server.on("/configs_wifi.json", get_ConfigWifiJson);
  server.on("/configs_time.json", handle_ConfigTimeJson);
  server.on("/configs_weath.json", handle_ConfigWeathJson);
  server.on("/configs_setup.json", handle_ConfigSetupJson);
  server.on("/configs_sgp.json", handle_ConfigSgpJson);
  server.on("/ssid", handle_Set_Ssid);
  server.on("/ntp", handle_ntp);         // Установка часової зони по запиту типа http://192.168.2.100/timeZone?timeZone=3
  server.on("/set_time", handle_set_time);
  server.on("/timepc", handle_timepc);
  server.on("/weatherUpdate", handle_weather_update);
  server.on("/weather", handle_weather);    // Установка сервера погоди по запиту типа http://192.168.2.100/weatherHost?weatherHost=api.openweathermap.org
  server.on("/weathOn", handle_weath_on);
  server.on("/setup", handle_setup);
  server.on("/sgp", handle_sgp);
  server.on("/mess", handleMessage);
  server.on("/restart", handle_Restart);            // перезавантаження модуля по запиту типу http://192.168.1.11/restart?device=ok
  server.on("/stopalarm", handle_stopAlarm);
  server.on("/resetConfig", handle_resetConfig);
  server.on("/printCom", handle_set_printCom);
  server.onNotFound([]() {(404, "text/plain", "FileNotFound");});
  httpUpdater.setup(&server);
  server.begin();
}

void sendAuthPage() {
  server.send_P(200, "text/html", P_auth);
}

//======================================================================================================
void auth_auth() {
  if(!authOn) {
    server.send_P(200, "text/html", P_index);
  } else {
    sendAuthPage();
  }
}

//======================================================================================================
void authIndex() {
  server.send_P(200, "text/html", P_index);
}

//======================================================================================================
void auth_wifi() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_wifi);
  } else {
    sendAuthPage();
  }
}

//======================================================================================================
void auth_time() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_time);
  } else {
    sendAuthPage();
  }
}
//======================================================================================================
void auth_weather() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_weath);
  } else {
    sendAuthPage();
  }
}
//======================================================================================================
void auth_setup() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_setup);
  } else {
    sendAuthPage();
  }
}
//======================================================================================================
void auth_sgp() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_sgp);
  } else {
    sendAuthPage();
  }
}

//======================================================================================================
void auth_help() {
  if(server.arg("auth")==auth || !authOn) {
    server.send_P(200, "text/html", P_help);
  } else {
    sendAuthPage();
  }
}

//==========================================
bool isNotVerified() {
  if(authOn && server.arg("auth") != auth) {
    server.send_P(404, "text/html", "404, you are not authorized!");
    return true;
  }
  return false;
}

String getJsonHeaderPart() {
  String json = "\"ver\":\""
      + String(ver)
      + "\",\"time\":\""
      + (String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  return json;
}

void get_IndexDataJson() {
  String json = "{"
      + getJsonHeaderPart()
      + "\",\"ip\":\""
      + WiFi.localIP().toString()
      + "\",\"printCom\":\""
      + (printCom == 1 ? "checked" : "")
      + "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================
void handle_ConfigJSON() {
  if(isNotVerified()) {
    return;
  }
  String json = "{"
      + getJsonHeaderPart()
      + "\",\"ip\":\""
      + WiFi.localIP().toString()
      + "\",\"printCom\":\""
      + (printCom==1?"checked":"")
      + "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================
void get_ConfigWifiJson() {
  if(isNotVerified()) {
    return;
  }
  String json = "{"
   + getJsonHeaderPart()
   + "\",\"auth\":\""
   + auth
   + "\",\"ssid0\":\""
   + ssid[0]
   + "\",\"password0\":\""
   + password[0]
   + "\",\"ssid1\":\""
   + ssid[1]
   + "\",\"password1\":\""
   + password[1]
   + "\",\"ssidAP\":\""
   + ssidAP
   + "\",\"passwordAP\":\""
   + passwordAP
   + "\",\"authOn\":\""
   + (authOn==1 ? "checked" : "")
   + "\"}";
  server.send(200, "text/json", json);
}
//======================================================================================================
void handle_ConfigTimeJson(){
  if(isNotVerified()) {
    return;
  }
  String json = "{";
  json += getJsonHeaderPart();
  json += "\",\"ntpServerName\":\"";
  json += ntpServerName;
  json += "\",\"timeZone\":\"";
  json += timeZone;
  json += "\",\"isDayLightSaving\":\"";
  json += (isDayLightSaving==1?"checked":"");
  json += "\",\"rtcStat\":\"";
  json += (rtcStat==1?"checked":"");
  json += "\",\"setTMes\":\"";
  json += "\",\"al_0_0\":\"";
  json += alarme[0][0];
  json += "\",\"al_0_1\":\"";
  json += alarme[0][1];
  json += "\",\"al_0_2\":\"";
  json += alarme[0][2];
  json += "\",\"al_1_0\":\"";
  json += alarme[1][0];
  json += "\",\"al_1_1\":\"";
  json += alarme[1][1];
  json += "\",\"al_1_2\":\"";
  json += alarme[1][2];
  json += "\",\"al_2_0\":\"";
  json += alarme[2][0];
  json += "\",\"al_2_1\":\"";
  json += alarme[2][1];
  json += "\",\"al_2_2\":\"";
  json += alarme[2][2];
  json += "\",\"al_3_0\":\"";
  json += alarme[3][0];
  json += "\",\"al_3_1\":\"";
  json += alarme[3][1];
  json += "\",\"al_3_2\":\"";
  json += alarme[3][2];
  json += "\",\"al_4_0\":\"";
  json += alarme[4][0];
  json += "\",\"al_4_1\":\"";
  json += alarme[4][1];
  json += "\",\"al_4_2\":\"";
  json += alarme[4][2];
  json += "\",\"t0\":\"";
  json += hour;
  json += "\",\"t1\":\"";
  json += minute;
  json += "\",\"d0\":\"";
  json += day;
  json += "\",\"d1\":\"";
  json += month;
  json += "\",\"d2\":\"";
  json += year;
  json += "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================  
void handle_ConfigWeathJson(){
  if(isNotVerified()) {
    return;
  }
  int sr = location_sunrise.substring(0, 2).toInt() + (int)hourCorr;
  if(sr>23) sr -= 24;
  if(sr<0) sr += 24;
  String sunrise = String(sr) + location_sunrise.substring(2, 5);
  int ss = location_sunset.substring(0, 2).toInt() + (int)hourCorr;
  if(ss>23) ss -= 24;
  if(ss<0) ss += 24;
  String sunset = String(ss) + location_sunset.substring(2, 5);
  int st = location_localtime.substring(11, 13).toInt() + (int)hourCorr;
  int ly = location_localtime.substring(0, 4).toInt();
  byte lm = location_localtime.substring(5, 7).toInt();
  byte ld = location_localtime.substring(8, 10).toInt(); 
  if(st>23) {
    st -= 24;
    ld++;
    if(ld==32 || (ld==31 && (lm==4 || lm==6 || lm==9 || lm==11)) || (lm==2 && ((ld==29 && ly%4!=0) || (ld==30 && ly%4==0)))) {
      ld=1;
      lm++;
      if(lm>12){
        lm=1;
        ly++;
      }
    }
  }
  if(st<0) {
    st += 24;
    ld--;
    if(ld<1) {
      ld = 0 + ((lm==5 || lm==7 || lm==10 || lm==12 || (lm==3 && ly%4==0))?30:(lm==3 && ly%4!=0)?29:31);
      lm--;
      if(lm<1){
        lm=12;
        ly--;
      }
    }
  }
  String lt = String(ly) + "-" + (lm<10?"0":"") + String(lm) + "-" + (ld<10?"0":"") + String(ld) + " " + (st<10?"0":"") + String(st) + location_localtime.substring(13, 16);
  String json = "{";
  json += getJsonHeaderPart();
  json += "\",\"weatherKey0\":\"";
  json += weatherKey0;
  json += "\",\"weatherKey1\":\"";
  json += weatherKey1;
  json += "\",\"weatherHost\":\"";
  json += weatherHost;
  json += "\",\"cityID0\":\"";
  json += cityID0;
  json += "\",\"cityID1\":\"";
  json += cityID1;
  json += "\",\"personalCityName\":\"";
  json += personalCityName;
  json += "\",\"weatherLang\":\"";
  json += weatherLang;
  json += "\",\"displayForecast\":\"";
  json += (displayForecast==1?"checked":"");
  json += "\",\"displayCityName\":\"";
  json += (displayCityName==1?"checked":"");
  json += "\",\"displayForecastNow\":\"";
  json += (displayForecastNow==1?"checked":"");
  json += "\",\"displayForecastToday\":\"";
  json += (displayForecastToday==1?"checked":"");
  json += "\",\"displayForecastTomorrow\":\"";
  json += (displayForecastTomorrow==1?"checked":"");
  json += "\",\"animNotWeather\":\"";
  json += (animNotWeather==1?"checked":"");
  json += "\",\"timeStartViewWeather\":\"";
  json += timeStartViewWeather;
  json += "\",\"timeEndViewWeather\":\"";
  json += timeEndViewWeather;
  json += "\",\"location_name\":\"";
  json += location_name;
  json += "\",\"location_region\":\"";
  json += location_region;
  json += "\",\"location_country\":\"";
  json += location_country;
  json += "\",\"location_localtime\":\"";
  json += lt;
  json += "\",\"location_temp\":\"";
  json += location_temp;
  json += "\",\"location_app_temp\":\"";
  json += location_app_temp;
  json += "\",\"location_rh\":\"";
  json += location_rh;
  json += "\",\"location_pres\":\"";
  json += location_pres;
  json += "\",\"location_wind_spd\":\"";
  json += location_wind_spd;
  json += "\",\"location_wind_cdir_full\":\"";
  json += location_wind_cdir_full;
  json += "\",\"location_sunrise\":\"";
  json += sunrise;
  json += "\",\"location_sunset\":\"";
  json += sunset;
  json += "\",\"location_clouds\":\"";
  json += location_clouds;
  json += "\",\"location_vis\":\"";
  json += location_vis;
  json += "\",\"location_uv\":\"";
  json += location_uv;
  json += "\",\"location_weather_description\":\"";
  json += location_weather_description;
  json += "\",\"uuid\":\"";
  json += uuid;
  json += "\",\"api_key\":\"";
  json += api_key;
  json += "\",\"sensors_ID0\":\"";
  json += sensors_ID0;
  json += "\",\"sensors_ID1\":\"";
  json += sensors_ID1;
  json += "\",\"sensors_ID2\":\"";
  json += sensors_ID2;
  json += "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================
void handle_ConfigSetupJson(){
  if(isNotVerified()) {
    return;
  }
  String json = "{";
  json += getJsonHeaderPart();
  json += "\",\"kuOn\":\"";
  json += kuOn;
  json += "\",\"kuOff\":\"";
  json += kuOff;
  json += "\",\"timeDay\":\"";
  json += timeDay;
  json += "\",\"volBrightnessD\":\"";
  json += volBrightnessD;
  json += "\",\"volBrightnessN\":\"";
  json += volBrightnessN;
  json += "\",\"timeNight\":\"";
  json += timeNight;
  json += "\",\"timeScrollSpeed\":\"";
  json += 100 - timeScrollSpeed;
  json += "\",\"clockNight\":\"";
  json += (clockNight==1?"checked":"");
  json += "\",\"volBrightnessAuto\":\"";
  json += (volBrightnessAuto==1?"checked":"");
  json += "\",\"lowLivelBrightness\":\""; 
  json += lowLivelBrightness;
  json += "\",\"upLivelBrightness\":\"";
  json += upLivelBrightness;
  json += "\",\"lba\":\"";
  json += levelBridhtness; 
  json += "\",\"isActiveBuzzer\":\"";
  json += (isActiveBuzzer == 1 ? "checked" : "");
  json += "\",\"sensor00\":\"";
  json += sensore[0];
  json += "\",\"corr00\":\"";
  json += corr00;
  float Td=data00;
  if(param0==20){
    if(pressSys==1 && Td>815) Td /= 1.3332239;
    if(pressSys!=1 && Td<815) Td /= 0.7500615613026439;
  }
  json += "\",\"Td\":\"";
  json += Td;
  json += "\",\"sensor01\":\"";
  json += sensore[1];
  json += "\",\"corr01\":\"";
  json += corr01;
  float Tu=data01;
  if(param1==20){
    if(pressSys==1 && Tu>815) Tu /= 1.3332239;
    if(pressSys!=1 && Tu<815) Tu /= 0.7500615613026439;
  }
  json += "\",\"Tu\":\"";
  json += Tu;
  json += "\",\"sensor02\":\"";
  json += sensore[2];
  json += "\",\"corr02\":\"";
  json += corr02;
  float Th=data02;
  if(param2==20){
    if(pressSys==1 && Th>815) Th /= 1.3332239;
    if(pressSys!=1 && Th<815) Th /= 0.7500615613026439;
  }
  json += "\",\"Th\":\"";
  json += Th;
  json += "\",\"sensor03\":\"";
  json += sensore[3];
  json += "\",\"corr03\":\"";
  json += corr03;
  float Hd=data03;
  if(param3==20){
    if(pressSys==1 && Hd>815) Hd /= 1.3332239;
    if(pressSys!=1 && Hd<815) Hd /= 0.7500615613026439;
  }
  json += "\",\"Hd\":\"";
  json += Hd;
  json += "\",\"sensor04\":\"";
  json += sensore[4];
  json += "\",\"corr04\":\"";
  json += corr04;
  float Pu=data04;
  if(param4==20){
    if(pressSys==1 && Pu>815) Pu /= 1.3332239;
    if(pressSys!=1 && Pu<815) Pu /= 0.7500615613026439;
  }
  json += "\",\"Pu\":\"";
  json += Pu;
  json += "\",\"pressSys\":\"";
  json += pressSys;
  json += "\",\"param0\":\"";
  json += param0;
  json += "\",\"param1\":\"";
  json += param1;
  json += "\",\"param2\":\"";
  json += param2;
  json += "\",\"param3\":\"";
  json += param3;
  json += "\",\"param4\":\"";
  json += param4;
  json += "\",\"MAX7219_ROTATE\":\"";
  json += MAX7219_ROTATE;
  json += "\",\"MAX7219_NUM\":\"";
  json += MAX7219_NUM;
  json += "\",\"fontCLOCK\":\"";
  json += fontCLOCK;
  json += "\",\"animPoint\":\"";
  json += animPoint;
  json += "\",\"aliData\":\"";
  json += aliData;
  json += "\",\"butStat\":\"";
  json += butStat;
  json += "\",\"displayData\":\"";
  json += displayData;
  json += "\",\"function00\":\"";
  json += function[0];
  json += "\",\"function01\":\"";
  json += function[1];
  json += "\",\"function02\":\"";
  json += function[2];
  json += "\",\"function03\":\"";
  json += function[3];
  json += "\",\"function04\":\"";
  json += function[4];
  json += "\",\"function05\":\"";
  json += function[5];
  json += "\",\"function06\":\"";
  json += function[6];
  json += "\",\"function07\":\"";
  json += function[7];
  json += "\",\"function08\":\"";
  json += function[8];
  json += "\",\"function09\":\"";
  json += function[9];
  json += "\",\"function10\":\"";
  json += function[10];
  json += "\",\"function11\":\"";
  json += function[11];
  json += "\",\"period00\":\"";
  json += period[0];
  json += "\",\"period01\":\"";
  json += period[1];
  json += "\",\"period02\":\"";
  json += period[2];
  json += "\",\"period03\":\"";
  json += period[3];
  json += "\",\"period04\":\"";
  json += period[4];
  json += "\",\"period05\":\"";
  json += period[5];
  json += "\",\"period06\":\"";
  json += period[6];
  json += "\",\"period07\":\"";
  json += period[7];
  json += "\",\"period08\":\"";
  json += period[8];
  json += "\",\"period09\":\"";
  json += period[9];
  json += "\",\"period10\":\"";
  json += period[10];
  json += "\",\"period11\":\"";
  json += period[11];
  json += "\",\"sgp\":\"";
  if(sgpFound){
    json += "SGP30";
  } else json += "";
  json += "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================
void handle_ConfigSgpJson() {
  if(isNotVerified()) {
    return;
  }
  String json = "{"
      + getJsonHeaderPart()
      + "\",\"sgpCo2\":\""
      + sgpCo2
      + "\",\"textCo2\":\""
      + sgpCo2Message[sgpValues.co2Livel]
      + "\",\"sgpCo2LivelAlarm\":\""
      + sgpCo2LivelAlarm
      + "\",\"eCo2AlarmEsp\":\""
      + (eCo2AlarmEsp==1?"checked":"")
      + "\",\"eCo2Led\":\""
      + (eCo2Led==1?"checked":"")
      + "\",\"sgpTvoc\":\""
      + sgpTvoc
      + "\",\"textTvoc\":\""
      + sgpTvocMessage[sgpValues.tvocLivel]
      + "\",\"sgpTvocLivelAlarm\":\""
      + sgpTvocLivelAlarm
      + "\",\"tvocAlarmEsp\":\""
      + (tvocAlarmEsp==1?"checked":"")
      + "\",\"tvocLed\":\""
      + (tvocLed==1?"checked":"")
      + "\",\"setSgpCorr\":\""
      + setSgpCorr
      + "\",\"sgpCorrTemp\":\""
      + sgpCorrTemp
      + "\",\"sgpCorrHumi\":\""
      + sgpCorrHumi
      + "\"}";
  server.send(200, "text/json", json);
}

//======================================================================================================
void handle_Set_Ssid(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("ssid0")!="") ssid[0] = server.arg("ssid0").c_str();
  password[0] = server.arg("password0").c_str();
  ssid[1] = server.arg("ssid1").c_str();
  password[1] = server.arg("password1").c_str();
  if(server.arg("ssidAP")!="") ssidAP = server.arg("ssidAP").c_str();
  passwordAP = server.arg("passwordAP").c_str();
  if(server.arg("auth")!="") auth = server.arg("auth").c_str();
  if(server.arg("authOn")!="") authOn = server.arg("authOn").toInt();
  saveConfig();
  if(printCom) {
    printTime();
    Serial.println("Set ssid0: " + ssid[0] + ",  Set password0: " + password[0] + ",  ssid1: " + ssid[1] + ",  password1: " + password[1] + ",  ssidAP: " + ssidAP + ",  AP password: " + passwordAP + ",  Set auth: " + auth + ",  Set authOn: " + authOn);
  }
  server.send(200, "text/plain", "OK");
  ESP.reset();
}

void handle_ntp() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("ntpServerName")!="") {
    ntpServerName = server.arg("ntpServerName").c_str();
  }
  if(server.arg("timeZone")!="") {
    timeZone = server.arg("timeZone").toFloat();
  }
  if(server.arg("isDayLightSaving")!="") {
    isDayLightSaving = server.arg("isDayLightSaving").toInt();
  }
  if(server.arg("rtcStat")!="") {
    rtcStat = server.arg("rtcStat").toInt();
  }
//  if(server.arg("setTMes")!="") setTMes = server.arg("setTMes").toInt();
  if(server.arg("al_0_0")!="") {
    alarme[0][0]=server.arg("al_0_0").toInt();
  }
  if(server.arg("al_0_1")!="") {
    alarme[0][1]=server.arg("al_0_1").toInt();
  }
  if(server.arg("al_0_2")!="") {
    alarme[0][2]=server.arg("al_0_2").toInt();
  }
  if(server.arg("al_1_0")!="") {
    alarme[1][0]=server.arg("al_1_0").toInt();
  }
  if(server.arg("al_1_1")!="") {
    alarme[1][1]=server.arg("al_1_1").toInt();
  }
  if(server.arg("al_1_2")!="") {
    alarme[1][2]=server.arg("al_1_2").toInt();
  }
  if(server.arg("al_2_0")!="") {
    alarme[2][0]=server.arg("al_2_0").toInt();
  }
  if(server.arg("al_2_1")!="") {
    alarme[2][1]=server.arg("al_2_1").toInt();
  }
  if(server.arg("al_2_2")!="") {
    alarme[2][2]=server.arg("al_2_2").toInt();
  }
  if(server.arg("al_3_0")!="") {
    alarme[3][0]=server.arg("al_3_0").toInt();
  }
  if(server.arg("al_3_1")!="") {
    alarme[3][1]=server.arg("al_3_1").toInt();
  }
  if(server.arg("al_3_2")!="") {
    alarme[3][2]=server.arg("al_3_2").toInt();
  }
  if(server.arg("al_4_0")!="") {
    alarme[4][0]=server.arg("al_4_0").toInt();
  }
  if(server.arg("al_4_1")!="") {
    alarme[4][1]=server.arg("al_4_1").toInt();
  }
  if(server.arg("al_4_2")!="") {
    alarme[4][2]=server.arg("al_4_2").toInt();
  }
  if(printCom) {
    printTime();
    Serial.println("Set NTP Server Name: " + ntpServerName + ",  NTP Time Zone: " + String(timeZone) + ",  isDayLightSaving: " + String(isDayLightSaving));
  }
  alarm_hold = 0;
  saveAlarm();
  timeUpdateNTP();
  server.send(200, "text/plain", "OK"); 
}

//======================================================================================================
void handle_set_time(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("t0")!="") hour = server.arg("t0").toInt();
  if(server.arg("t1")!="") minute = server.arg("t1").toInt();
  if(server.arg("d0")!="") day = server.arg("d0").toInt();
  if(server.arg("d1")!="") month = server.arg("d1").toInt();
  if(server.arg("d2")!="") year = server.arg("d2").toInt();
  if(printCom) {
    printTime();
    Serial.println("Set manual time: " + String(hour) + ":" + String(minute) + "      " + String(day) + "-" + String(month) + "-" + String(year));
  }
  server.send(200, "text/plain", "OK");
  
  localEpoc = (hour * 60 * 60 + minute * 60);
  showSimpleDate();
  
  rtcStruct.hour = hour;
  rtcStruct.minute = minute;
  rtcStruct.second = 0;
  rtcStruct.month = month;
  rtcStruct.day = day;
  rtcStruct.year = year;
  printTime();
  setRTCDateTime();
}

//======================================================================================================
void handle_timepc(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("hours")!="") {
    hour = server.arg("hours").toInt();
  }
  if(server.arg("minute")!="") {
    minute = server.arg("minute").toInt();
  }
  if(server.arg("sec")!="") {
    second = server.arg("sec").toInt();
  }
  if(server.arg("tz")!="") {
    timeZone = server.arg("tz").toFloat();
  }
  if(server.arg("day")!="") {
    day = server.arg("day").toInt();
  }
  if(server.arg("month")!="") {
    month = server.arg("month").toInt();
  }
  if(server.arg("year")!="") {
    year = server.arg("year").toInt();
  }
  if(printCom) {
    printTime();
    Serial.println("Set Date/Time from PC - "+String(day)+"."+String(month)+"."+String(year)+" "+String(hour)+":"+String(minute)+":"+String(second)+" timeZone="+String(timeZone));
  }
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  saveConfig();
  server.send(200, "text/plain", "OK"); 
}

//======================================================================================================
void weather_update() {
  if(displayForecast) {
    if(!weatherHost) {
      getWeatherData0();
      getWeatherDataz0();
    } else {
      getWeatherData1();
      getWeatherDataz1();
    }
  }
}

//======================================================================================================
void handle_weath_on() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("displayForecast")!="") {
    displayForecast = server.arg("displayForecast").toInt(); 
  }
  if(printCom) {
    printTime();
    Serial.println("displayForecast = " + String(displayForecast));
  }
  if(displayForecast) {
    weather_update();
  }
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}

//======================================================================================================
void handle_weather_update() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("update") == "ok") {
    weather_update();
    bip();
    server.send(200, "text/plain", "OK");
  }
}

//======================================================================================================
void handle_weather() {
  if(server.arg("weatherHost")!="") {
    weatherHost = server.arg("weatherHost").toInt();
  }
  if(server.arg("weatherKey0")!="") {
    weatherKey0 = server.arg("weatherKey0").c_str();
  }
  if(server.arg("weatherKey1")!="") {
    weatherKey1 = server.arg("weatherKey1").c_str();
  }
  if(server.arg("cityID0")!="") {
    cityID0 = server.arg("cityID0").c_str();
  }
  if(server.arg("cityID1")!="") {
    cityID1 = server.arg("cityID1").c_str();
  }
  if(server.arg("weatherLang")!="") {
    weatherLang = server.arg("weatherLang").c_str();
  }
  if(server.arg("displayForecast")!="") {
    displayForecast = server.arg("displayForecast").toInt();
  }
  if(server.arg("displayCityName")!="") {
    displayCityName = server.arg("displayCityName").toInt();
  }
  if(server.arg("displayForecastNow")!="") {
    displayForecastNow = server.arg("displayForecastNow").toInt();
  }
  if(server.arg("displayForecastToday")!="") {
    displayForecastToday = server.arg("displayForecastToday").toInt();
  }
  if(server.arg("displayForecastTomorrow")!="") {
    displayForecastTomorrow = server.arg("displayForecastTomorrow").toInt();
  }
  if(server.arg("timeStartViewWeather")!="") {
    timeStartViewWeather = server.arg("timeStartViewWeather").toInt();
  }
  if(server.arg("timeEndViewWeather")!="") {
    timeEndViewWeather = server.arg("timeEndViewWeather").toInt();
  }
  if(server.arg("timeScrollSpeed")!="") {
    timeScrollSpeed = 100 - server.arg("timeScrollSpeed").toInt();
  }
  uuid = server.arg("uuid").c_str();
  api_key = server.arg("api_key").c_str();
  sensors_ID0 = server.arg("sensors_ID0").toInt();
  sensors_ID1 = server.arg("sensors_ID1").toInt();
  sensors_ID2 = server.arg("sensors_ID2").toInt();
  if(server.arg("personalCityName")!="") {
    snprintf(personalCityName, 51, "%s", server.arg("personalCityName").c_str());
  }
  if(printCom) {
    printTime();
    Serial.print("Set Weather Server: ");
    if(!weatherHost) {
      Serial.print(weatherHost0);
    } else  {
      Serial.print(weatherHost1);
    }
    Serial.println(", Key0: " + weatherKey0 + ", Key1: " + weatherKey1 + ", City ID0: " + cityID0 + ", City ID1: " + cityID1 + ", weatherLang: " + weatherLang);
    Serial.println("          displayCityName: " + String(displayCityName) + ", displayForecastNow: " + String(displayForecastNow) + ", displayForecastTomorrow: " + String(displayForecastTomorrow) + ", personalCityName: " + String(personalCityName));
    Serial.println("          timeStartViewWeather: " + String(timeStartViewWeather) + ", timeEndViewWeather: " + String(timeEndViewWeather) + ", timeScrollSpeed: " + String(timeScrollSpeed) + ", uuid: " + String(uuid) + ", api_key: " + String(api_key) + ", sensors_ID0: " + String(sensors_ID0) + ", sensors_ID1: " + String(sensors_ID1) + ", sensors_ID2: " + String(sensors_ID2));
  }
  
  saveConfig();
  initLang();
  weather_update();
  
  server.send(200, "text/plain", "OK");
}

//======================================================================================================
void handle_setup(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("timeDay")!="") timeDay = server.arg("timeDay").toInt();
  if(server.arg("volBrightnessD")!="") volBrightnessD = server.arg("volBrightnessD").toInt();
  if(server.arg("timeNight")!="") timeNight = server.arg("timeNight").toInt();
  if(server.arg("volBrightnessN")!="") volBrightnessN = server.arg("volBrightnessN").toInt();
  if(server.arg("volBrightnessAuto")!="") volBrightnessAuto = server.arg("volBrightnessAuto").toInt();
  if(server.arg("lowLivelBrightness")!="") lowLivelBrightness = server.arg("lowLivelBrightness").toInt();
  if(server.arg("upLivelBrightness")!="") upLivelBrightness = server.arg("upLivelBrightness").toInt();
  if(server.arg("isActiveBuzzer")!="") isActiveBuzzer = server.arg("isActiveBuzzer").toInt();
  if(server.arg("clockNight")!="") clockNight = server.arg("clockNight").toInt();
  if(server.arg("MAX7219_ROTATE")!="") MAX7219_ROTATE = server.arg("MAX7219_ROTATE").toInt();
  if(server.arg("MAX7219_NUM")!="") MAX7219_NUM = server.arg("MAX7219_NUM").toInt();
  if(server.arg("kuOn")!="") kuOn = server.arg("kuOn").toInt();
  if(server.arg("kuOff")!="") kuOff = server.arg("kuOff").toInt();
  if(server.arg("sensor00")!="" && server.arg("param0")!=""){
    int sens = server.arg("sensor00").toInt();
    int param = server.arg("param0").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<19))&&param<24)){
      sensore[0] = server.arg("sensor00").toInt();
      param0 = server.arg("param0").toInt();
    }
  }
  if(server.arg("sensor01")!="" && server.arg("param1")!=""){
    int sens = server.arg("sensor01").toInt();
    int param = server.arg("param1").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<19))&&param<24)){
      sensore[1] = sens;
      param1 = param;
    }
  }
  if(server.arg("sensor02")!="" && server.arg("param2")!=""){
    int sens = server.arg("sensor02").toInt();
    int param = server.arg("param2").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<19))&&param<24)){
      sensore[2] = server.arg("sensor02").toInt();
      param2 = server.arg("param2").toInt();
    }
  }
  if(server.arg("sensor03")!="" && server.arg("param3")!=""){
    int sens = server.arg("sensor03").toInt();
    int param = server.arg("param3").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<19))&&param<24)||(sens==99&&param==3)){
      sensore[3] = server.arg("sensor03").toInt();
      param3 = server.arg("param3").toInt();
    }
  }
  if(server.arg("sensor04")!="" && server.arg("param4")!=""){
    int sens = server.arg("sensor04").toInt();
    int param = server.arg("param4").toInt();
    if(sens==0||(sens==1&&param>=0&&param<5)||((sens==2||sens==5||sens==10)&&param>=0&&param<15)||(sens==3&&(param>=0&&param<5)||param==20)||(sens==4&&((param>=0&&param<15)||param==20))||(((sens>5&&sens<10)||(sens>10&&sens<19))&&param<24)||(sens==99&&param==21)){
      sensore[4] = server.arg("sensor04").toInt();
      param4 = server.arg("param4").toInt();
    }
  }
  if(server.arg("pressSys")!="") pressSys = server.arg("pressSys").toInt();
  if(server.arg("fontCLOCK")!="") fontCLOCK = server.arg("fontCLOCK").toInt();
  if(server.arg("animPoint")!="") animPoint = server.arg("animPoint").toInt();
  if(server.arg("aliData")!="") aliData = server.arg("aliData").toInt();
  if(server.arg("timeScrollSpeed")!="") timeScrollSpeed = 100 - server.arg("timeScrollSpeed").toInt();
  if(server.arg("corr00")!="") corr00 = server.arg("corr00").toFloat();
  if(server.arg("corr01")!="") corr01 = server.arg("corr01").toFloat();
  if(server.arg("corr02")!="") corr02 = server.arg("corr02").toFloat();
  if(server.arg("corr03")!="") corr03 = server.arg("corr03").toFloat();
  if(server.arg("corr04")!="") corr04 = server.arg("corr04").toInt();
  if(server.arg("displayData")!="") displayData = server.arg("displayData").toInt();
  if(server.arg("butStat")!="") butStat = server.arg("butStat").toInt();

  if(server.arg("function00")!="") function[0] = server.arg("function00").toInt();
  if(server.arg("function01")!="") function[1] = server.arg("function01").toInt();
  if(server.arg("function02")!="") function[2] = server.arg("function02").toInt();
  if(server.arg("function03")!="") function[3] = server.arg("function03").toInt();
  if(server.arg("function04")!="") function[4] = server.arg("function04").toInt();
  if(server.arg("function05")!="") function[5] = server.arg("function05").toInt();
  if(server.arg("function06")!="") function[6] = server.arg("function06").toInt();
  if(server.arg("function07")!="") function[7] = server.arg("function07").toInt();
  if(server.arg("function08")!="") function[8] = server.arg("function08").toInt();
  if(server.arg("function09")!="") function[9] = server.arg("function09").toInt();
  if(server.arg("function10")!="") function[10] = server.arg("function10").toInt();
  if(server.arg("function11")!="") function[11] = server.arg("function11").toInt();

  if(server.arg("period00")!="") period[0] = server.arg("period00").toInt();
  if(server.arg("period01")!="") period[1] = server.arg("period01").toInt();
  if(server.arg("period02")!="") period[2] = server.arg("period02").toInt();
  if(server.arg("period03")!="") period[3] = server.arg("period03").toInt();
  if(server.arg("period04")!="") period[4] = server.arg("period04").toInt();
  if(server.arg("period05")!="") period[5] = server.arg("period05").toInt();
  if(server.arg("period06")!="") period[6] = server.arg("period06").toInt();
  if(server.arg("period07")!="") period[7] = server.arg("period07").toInt();
  if(server.arg("period08")!="") period[8] = server.arg("period08").toInt();
  if(server.arg("period09")!="") period[9] = server.arg("period09").toInt();
  if(server.arg("period10")!="") period[10] = server.arg("period10").toInt();
  if(server.arg("period11")!="") period[11] = server.arg("period11").toInt();
  
  if(printCom){
    printTime();
    Serial.println("timeDay: " + String(timeDay) + ", volBrightnessD: " + String(volBrightnessD) + ", timeNight: " + String(timeNight) + ", volBrightnessN: " + String(volBrightnessN) + ",  kuOn: " + String(kuOn) + ",  kuOff: " + String(kuOff) + ", MAX7219_ROTATE: " + String(MAX7219_ROTATE) + ", clockNight: " + String(clockNight) + ", isActiveBuzzer: "+String(isActiveBuzzer));
    Serial.println("          sensore[0]: "+String(sensore[0])+", sensore[1]: "+String(sensore[1])+", sensore[2]: "+String(sensore[2])+", sensore[3]: "+String(sensore[3])+",  sensore[4]: "+String(sensore[4]));
    Serial.println("          param0: "+String(param0)+", param1: "+String(param1)+", param2: "+String(param2)+", param3: "+String(param3)+",  param4: "+String(param4));
    Serial.println("          func00: "+String(function[0])+", func01: "+String(function[1])+", func02: "+String(function[2])+", func03: "+String(function[3])+",  func04: "+String(function[4])+", func05: "+String(function[5])+", func06: "+String(function[6])+", func07: "+String(function[7])+", func08: "+String(function[8])+",  func09: "+String(function[9])+", func10: "+String(function[10])+",  func11: "+String(function[11]));
    Serial.println("          peri00: "+String(period[0])+", peri01: "+String(period[1])+", peri02: "+String(period[2])+", peri03: "+String(period[3])+",  peri04: "+String(period[4])+", peri05: "+String(period[5])+", peri06: "+String(period[6])+", peri07: "+String(period[7])+", peri08: "+String(period[8])+",  peri09: "+String(period[9])+", peri10: "+String(period[10])+",  peri11: "+String(period[11]));
  }
  saveConfig();
  sensorsAll(); 
  server.send(200, "text/plain", "OK");
}

//======================================================================================================
void handle_sgp() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("eCo2Led")!="") {
    sgpCo2 = server.arg("eCo2Led").toInt();
  }
  if(server.arg("sgpCo2LivelAlarm")!="") {
    sgpCo2LivelAlarm = server.arg("sgpCo2LivelAlarm").toInt();
  }
  if(server.arg("eCo2AlarmEsp")!="") {
    eCo2AlarmEsp = server.arg("eCo2AlarmEsp").toInt();
  }
  if(server.arg("tvocLed")!="") {
    tvocLed = server.arg("tvocLed").toInt();
  }
  if(server.arg("sgpTvocLivelAlarm")!="") {
    sgpTvocLivelAlarm = server.arg("sgpTvocLivelAlarm").toInt();
  }
  if(server.arg("tvocAlarmEsp")!="") {
    tvocAlarmEsp = server.arg("tvocAlarmEsp").toInt();
  }
  if(server.arg("setSgpCorr")!="") {
    setSgpCorr = server.arg("setSgpCorr").toInt();
  }
  if(setSgpCorr == 99) {
    if(server.arg("sgpCorrTemp")!="") {
      sgpCorrTemp = server.arg("sgpCorrTemp").toFloat();
    }
    if(server.arg("sgpCorrHumi")!="") {
      sgpCorrHumi = server.arg("sgpCorrHumi").toFloat();
    }
  }
  if(printCom) {
    printTime();
    Serial.println("Set eCo2Led: " + String(eCo2Led) + ",  sgpCo2LivelAlarm: " + String(sgpCo2LivelAlarm) + ",  eCo2AlarmEsp: " + String(eCo2AlarmEsp));
    Serial.print("          tvocLed: " + String(tvocLed) + ",  sgpTvocLivelAlarm: " + String(sgpTvocLivelAlarm) + ",  tvocAlarmEsp: " + String(tvocAlarmEsp)+ ",  setSgpCorr: " + String(setSgpCorr));
    if(setSgpCorr == 99) {
      if(printCom) {
        Serial.println(",  sgpCorrTemp: " + String(sgpCorrTemp) + ",  sgpCorrMan: " + String(sgpCorrHumi));
      }
    } else {
      if(printCom) {
        Serial.println("");
      }
    }
  }
  
  saveConfig(); 
  server.send(200, "text/plain", "OK");
}

//======================================================================================================
void handleMessage() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("text") != "") {
    server.send(200, "text/plain", "OK");
    String text = server.arg("text").c_str();
    bip(3);
    printStringWithShift((space + String(text) + space).c_str(), timeScrollSpeed);
    if(printCom) {
      printTime();
      Serial.println(text);
    }
  }
}

//======================================================================================================
void handle_stopAlarm(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("stopAlarm")=="ok") { 
    if(alarm_stat) {
      stopAlarm = true;
      if(printCom) {
        printTime();
        Serial.println("STOP ALARM");
      }
      bip();
    }
  }
  server.send(200, "text/plain", "OK");
}

//======================================================================================================
void handle_resetConfig() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("device") == "ok") {
    SPIFFS.remove("/config.json");
    if(printCom) {
      printTime();
      Serial.println("ESP erase Config file");
    }
    delay(3000);
    server.send(200, "text/plain", "OK");
    delay(3000);
    bip();
    ESP.reset();
  }
}

//======================================================================================================
void handle_set_printCom() {
  if(isNotVerified()) {
    return;
  }
  if(server.arg("printCom")!=""){
    printCom = server.arg("printCom").toInt();
    if(printCom){
      printTime();
      Serial.println("Set printCom: " + String(printCom));
    }
    saveConfig();
  }
  server.send(200, "text/plain", "OK"); 
}

//======================================================================================================
void handle_Restart(){
  if(isNotVerified()) {
    return;
  }
  if(server.arg("device") == "ok"){
    server.send(200, "text/plain", "OK"); 
    ESP.reset();
  }
}
