
static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};        // Кількість днів у місяцях

String weatherMain = "";
String weatherDescription = "";
String weatherLocation = "";
String country;
String tempz;
int clouds;
int windDeg;
float windSpeed;
String dw, _month;

//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  https://www.weatherbit.io                                                 //
//===============================================================================================================================//
void getWeatherData0() {
  if(weatherKey0=="" || !displayForecast) return;
  if(!WIFI_connected) {
    updateForecast++;
    return;
  }
  if(printCom) {
    Serial.println("======== START GET WEATHER FROM WEATHERBIT.IO =========");
    printTime();
  }  
  location_name = "";
  location_region = "";
  location_country = "";
  location_localtime = "";
  location_temp = 0;
  location_app_temp = 0;
  location_rh = 0;
  location_pres = 0;
  location_wind_spd = 0;
  location_wind_cdir_full = "";
  location_sunrise = "";
  location_sunset = "";
  location_clouds = 0;
  location_vis = 0;
  location_uv = 0;
  location_weather_description = "";
  if(ESPclient.connect(weatherHost0.c_str(), 80)){}
  else {
    if(printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/current/daily?city="+urlencode(cityID0)+"&lang="+weatherLang+"&key="+weatherKey0;
  if(printCom) Serial.print(reqline);
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
      if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(37) + 1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject data = doc["data"][0];
  location_rh = data["rh"]; // 69
  location_pres = data["pres"]; // 999.3
  if(pressSys == 1) location_pres /= 1.3332239;
  const char* data_timezone = data["timezone"]; // "Europe/Kiev"
  location_region = data_timezone;
  const char* data_ob_time = data["ob_time"]; // "2019-09-19 17:57"
  location_localtime = data_ob_time;
  const char* data_country_code = data["country_code"]; // "UA"
  location_country = data_country_code;
  location_clouds = data["clouds"]; // 4
  const char*   data_city_name = data["city_name"]; // "Kiev"
  location_name = data_city_name;
  location_wind_spd = data["wind_spd"]; // 1
  const char* data_wind_cdir_full = data["wind_cdir_full"]; // "пі́вдень-пі́вдень-схід"
  location_wind_cdir_full = data_wind_cdir_full;
  location_vis = data["vis"]; // 5
  const char*   data_sunset = data["sunset"]; // "16:01"
  location_sunset = data_sunset;
  location_uv = data["uv"]; // 0
  int           data_wind_dir = data["wind_dir"]; // 166
  const char*   data_sunrise = data["sunrise"]; // "03:39"
  location_sunrise = data_sunrise;   //int         data_dni = data["dni"]; // 0
  JsonObject    data_weather = data["weather"];
  const char*   data_weather_description = data_weather["description"]; // "ясного неба"
  location_weather_description = data_weather_description;
  location_temp = data["temp"]; // 10.6
  location_app_temp = data["app_temp"]; // 10.6
  String windDegString;
  if(data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211";    //"Північний";
  if(data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234";    //"Північно-східний";
  if(data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230";    //"Східний";
  if(data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235";    //"Південно-східний";
  if(data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210";    //"Південний";
  if(data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232";    //"Південно-західний";
  if(data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231";    //"Західний";
  if(data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233";    //"Північно-західний";
  weatherString = space;
  if(displayCityName){
    String PCN=personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(displayForecastNow){
    weatherString += tNow + ":    \212 " + String(location_temp, 1)+" ("+String(location_app_temp,1)+")"+("\202")+"C";
    weatherString += "     \213 " + String(location_rh) + "%"; 
    weatherString += "     \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0);
    weatherString += "     \214 " + windDegString + String(location_wind_spd, 1) + tSpeed;
    weatherString += "     \216 " + String(location_clouds) + "%     " + data_weather_description + space;
  }
  updateForecast = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}

//===============================================================================================================================//
//                              БЕРЕМО ПОГОДУ З САЙТУ  openweathermap.org                                                     //
//===============================================================================================================================//
void getWeatherData1(){
  if(weatherKey1=="" || !displayForecast) return;
  if(!WIFI_connected){
    updateForecast++;
    return;
  }
  if(printCom){                                                            //
    Serial.println("======== START GET WEATHER FROM OPENWEATHERMAP.ORG ====");
    printTime();
  } 
  location_name = "";
  location_region = "";
  location_country = "";
  location_localtime = "";
  location_temp = 0;
  location_app_temp = 0;
  location_rh = 0;
  location_pres = 0;
  location_wind_spd = 0;
  location_wind_cdir_full = "";
  location_sunrise = "";
  location_sunset = "";
  location_clouds = 0;
  location_vis = 0;
  location_uv = 0;
  location_weather_description = "";
  if(ESPclient.connect(weatherHost1.c_str(), 80)){}
  else{
    if(printCom){
      Serial.println(" Not connection server!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/weather?id="+urlencode(cityID1)+"&lang="+weatherLang+"&units=metric&appid="+weatherKey1;
  if(printCom) Serial.print(reqline); 
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY){
       line = http.getString();
      }
    } else {
      if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(13) + 751; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecast++;
    return;
  }
  JsonObject weather_0 = doc["weather"][0];
  const char*   data_weather_description = weather_0["description"]; // "fog"
  location_weather_description = data_weather_description;
  JsonObject main = doc["main"];
  location_temp = main["temp"]; // 10.34
  location_pres = main["pressure"]; // 1023
  if(pressSys == 1) location_pres /= 1.3332239;
  location_rh = main["humidity"]; // 100
  float location_temp_min = main["temp_min"]; // 7
  float location_temp_max = main["temp_max"]; // 12.22
  location_vis = doc["visibility"]; // 1000
  location_vis /= 1000;
  location_wind_spd = doc["wind"]["speed"]; // 1
  int data_wind_dir = doc["wind"]["deg"]; // 230
  location_clouds = doc["clouds"]["all"]; // 20
  JsonObject sys = doc["sys"];
  const char* data_country_code = sys["country"]; // "UA"
  location_country = data_country_code;
  const char*   data_city_name = doc["name"]; // "Kyiv"
  location_name = data_city_name;
  String windDegString;
  if(data_wind_dir >= 345 || data_wind_dir <= 22)  windDegString = "\211";    //"Північний";
  if(data_wind_dir >= 23  && data_wind_dir <= 68)  windDegString = "\234";    //"Північно-східний";
  if(data_wind_dir >= 69  && data_wind_dir <= 114) windDegString = "\230";    //"Східний";
  if(data_wind_dir >= 115 && data_wind_dir <= 160) windDegString = "\235";    //"Південно-східний";
  if(data_wind_dir >= 161 && data_wind_dir <= 206) windDegString = "\210";    //"Південний";
  if(data_wind_dir >= 207 && data_wind_dir <= 252) windDegString = "\232";    //"Південно-західний";
  if(data_wind_dir >= 253 && data_wind_dir <= 298) windDegString = "\231";    //"Західний";
  if(data_wind_dir >= 299 && data_wind_dir <= 344) windDegString = "\233";    //"Північно-західний";
  weatherString = space;
  if(displayCityName){
    String PCN=personalCityName;
    if(PCN.length() > 0) weatherString += PCN;
    else weatherString += String(location_name);
    weatherString += ", ";
  }
  if(displayForecastNow){
    weatherString += tNow + ":    \212 "+String(location_temp, 1)+" ("+String(location_temp_min,1)+"..."+String(location_temp_max,1)+")"+("\202")+"C";
    weatherString += "     \213 " + String(location_rh) + "%";
    weatherString += "     \215 " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0) ;
    weatherString += "     \214 " + windDegString + String(location_wind_spd, 1) + tSpeed;
    weatherString += "     \216 " + String(location_clouds) + "%     " + data_weather_description + space;
  }
  updateForecast = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}

// ============================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту https://www.weatherbit.io     // 
// ============================================================================//
void getWeatherDataz0() {
  if(weatherKey0=="" || !displayForecastTomorrow) return;
  if(!WIFI_connected) {
    updateForecasttomorrow++;
    return;
  }
  if(printCom){
    Serial.println("======== START GET FORECAST FROM WEATHERBIT.IO ========");
    printTime();
  } 
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+urlencode(cityID0)+"&lang="+weatherLang+"&days=2&key="+weatherKey0;
  if(printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode = http.GET();
   if(httpCode > 0) {
     if(printCom) Serial.printf("  [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      line = http.getString();
     }
   } else {
     if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(37) + 2321;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject data_0 = doc["data"][0];
  JsonObject data_0_weather = data_0["weather"];
  const char* data_0_weather_description = data_0_weather["description"]; // "Помірний дощ"
  float data_0_max_temp = data_0["max_temp"]; // 13.4
  float data_0_min_temp = data_0["min_temp"]; // 10.9
  JsonObject data_1 = doc["data"][1];
  int data_1_rh = data_1["rh"]; // 75
  int data_1_clouds = data_1["clouds"]; // 58
  float data_1_wind_spd = data_1["wind_spd"]; // 3.75302
  JsonObject data_1_weather = data_1["weather"];
  const char* data_1_weather_description = data_1_weather["description"]; // "Світло душ дощ"
  float data_1_max_temp = data_1["max_temp"]; // 16.3
  float data_1_min_temp = data_1["min_temp"]; // 10
  weatherStringZ = "";
  if(displayForecastToday){
    if(hour<18) weatherStringZ += tCurr + ":";
    if(hour<12) weatherStringZ += "   \212" + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  ";
    if(hour<18) weatherStringZ += "   " + String(data_0_weather_description) + "     ";
  }
  if(displayForecastTomorrow) {
    weatherStringZ += tTom + ":   \212" + String(data_1_min_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";
    weatherStringZ += "     \213 " + String(data_1_rh) + "%";
    weatherStringZ += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;
    weatherStringZ += "     " + String(data_1_weather_description);
    weatherStringZ += space;
  }
  if(printCom) Serial.println("          Getting weather forecast for tomorrow - is OK.");
  updateForecasttomorrow = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}

// =======================================================================//
//               Беремо ПРОГНОЗ!!! погоди з сайту openweathermap.org      //
// =======================================================================//
void getWeatherDataz1(){
  if(weatherKey1=="" || !displayForecastTomorrow) return;
  if(!WIFI_connected) {
    updateForecasttomorrow++;
    return;
  }
  if(printCom){
    Serial.println("======== START GET FORECAST FROM OPENWEATHERMAP.ORG ===");
    printTime();
  }
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost1+"/data/2.5/forecast/daily?id="+urlencode(cityID1)+"&units=metric&appid="+weatherKey1+"&lang="+weatherLang+"&cnt=2";
  if(printCom) Serial.print(reqline);   
  if(http.begin(ESPclient, reqline)){
   int httpCode = http.GET();
   if (httpCode > 0) {
     if(printCom) Serial.printf("    [HTTP] GET... code: %d\n", httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      line = http.getString();
     }
   } else {
     if(printCom){
        Serial.printf("    [HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        Serial.println("======== END ==========================================");
      }
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    if(printCom){
      Serial.printf("    [HTTP] Unable to connect\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    if(printCom){
      printTime();
      Serial.printf("[HTTP] The answer is empty\n");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + 3*JSON_OBJECT_SIZE(6) + 2*JSON_OBJECT_SIZE(10) + 1281;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if(!doc.capacity()){
    if(printCom){
      printTime();
      Serial.println("Parse weather forecast - FAILED!!!");
      Serial.println("======== END ==========================================");
    }
    updateForecasttomorrow++;
    return;
  }
  JsonObject city = doc["city"];
  JsonObject list_0 = doc["list"][0];
  JsonObject list_0_temp = list_0["temp"];
  float data_0_min_temp = list_0_temp["min"]; // 9.63
  float data_0_max_temp = list_0_temp["max"]; // 9.69
  JsonObject list_0_weather_0 = list_0["weather"][0];
  const char* data_0_weather_description = list_0_weather_0["description"]; // "облачно"
  JsonObject list_1 = doc["list"][1];
  JsonObject list_1_temp = list_1["temp"];
  float data_1_day_temp = list_1_temp["day"]; // 18.13
  float data_1_min_temp = list_1_temp["min"]; // 10.67
  float data_1_max_temp = list_1_temp["max"]; // 20.32
  int data_1_rh = list_1["humidity"]; // 56
  JsonObject list_1_weather_0 = list_1["weather"][0];
  const char* data_1_weather_description = list_1_weather_0["description"]; // "слегка облачно"
  float data_1_wind_spd = list_1["speed"]; // 0.7
  int data_1_clouds = list_1["clouds"]; // 38
  weatherStringZ = "";
  if(displayForecastToday){
    if(hour<18) weatherStringZ += tCurr + ":";
    if(hour<12) weatherStringZ += "   \212" + String(data_0_min_temp, 1) + "...." + String(data_0_max_temp, 1) + "\202" + "C  ";
    if(hour<18) weatherStringZ += "   " + String(data_0_weather_description) + "     ";
  }
  if(displayForecastTomorrow) {
    weatherStringZ += tTom + ":   \212" + String(data_1_min_temp, 1) + "...." + String(data_1_day_temp, 1) + "...." + String(data_1_max_temp, 1) + "\202" + "C";
    weatherStringZ += "     \213 " + String(data_1_rh) + "%";
    weatherStringZ += "     \214 " + String(data_1_wind_spd, 1) + tSpeed;
    weatherStringZ += "     \216 " + String(data_1_clouds) + "%     "  + String(data_1_weather_description);
    weatherStringZ += space;
  }
  updateForecasttomorrow = 0;
  updateForecastNot = false;
  if(printCom){
    printTime();
    Serial.println("line =" + line);
    Serial.println("======== END ==========================================");
  }
}
