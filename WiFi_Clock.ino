
#define ver "0.1"

#define DIN_PIN   13                                                                    //GPIO 13 / D7
#define CS_PIN    15                                                                    //GPIO 15 / D8
#define CLK_PIN   14                                                                    //GPIO 14 / D5
#define buzzerPin 12                                                                    //GPIO 12 / D6

#define MAX_DIGITS 16

#define BUT_PIN   16

#define brightPin A0

// заглушки
void printTime();
void printStringWithShift(const char* s, int shiftDelay);

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Adafruit_Sensor.h>
#include <SimpleDHT.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "Adafruit_SGP30.h"

#include "globals.h"
#include "soundModule.h"
#include "max7219.h"
#include "fonts.h"
#include "rtc.h"


// WEB страницы
#include "P_js.h"
#include "P_css.h"
#include "P_index.h"
#include "P_time.h"
#include "P_weath.h"
#include "P_setup.h"
#include "P_sgp.h"
#include "P_help.h"
#include "P_auth.h"
#include "P_wifi.h"

Ticker blinker;
ESP8266HTTPUpdateServer httpUpdater;
WiFiClient ESPclient;
ESP8266WebServer server(80);    // Веб сервер
File fsUploadFile;
IPAddress apIP(192, 168, 4, 1);

// =====================================================================================
String ssid[2]      = {"LocalWire"}; // Назва локального WiFi
String password[2]  = {"12345678"}; // Пароль локального WiFi
String ssidAP       = "WiFi-Clock"; // Назва точки доступу
String passwordAP   = "11223344";
String auth         = "1234asdf5678";

// переменные сервера погоды
String weatherHost0 = "api.weatherbit.io";
String weatherHost1 = "api.openweathermap.org";
String weatherKey0  = "";
String weatherKey1  = "";
String cityID0      = "Kyiv";
String cityID1      = "703448";
boolean authOn = true;
boolean weatherHost = 0;
char personalCityName[51] = "";
String weatherLang = "uk"; // Мова отримання прогнозу погоди
String location_name = "";
String location_region = "";
String location_country = "";
String location_localtime = "";
float  location_temp;
float  location_app_temp;
int    location_rh;
float  location_pres;
float  location_wind_spd;
String location_wind_cdir_full = "";
String location_sunrise = "";
String location_sunset = "";
int    location_clouds;
int    location_vis;
int    location_uv;
String location_weather_description = "";
String cityName;

String weatherString;
String weatherStringZ;
bool animNotWeather = true;

// --------------------------------------------
String uuid = "";
String api_key = "";
int sensors_ID0 = 3300;    //88733 Frankfurt
int sensors_ID1 = 0;   //88459 Frankfurt
int sensors_ID2 = 0;
float nMon00 = 0.0;
float nMon01 = 0.0;
float nMon02 = 0.0;

// =====================================================================================
byte function[12] = {1,  2, 1,  3, 1,  4, 5, 6, 7, 8, 0, 0};
byte period[12]   = {10, 5, 10, 1, 10, 5, 5, 5, 5, 5, 0, 0};
byte fnCount = 0;
byte oldCount = 0;
unsigned long fnTimer;
bool endString = true;


// индикаторы

byte fontCLOCK = 8;      // 0-крупный, 1-крупный цифровой, 2-полу жирный, 3-полу жирный цифровой, 4-обычный, 5-обычный цифровой, 6-узкий, 7-узкий цифровой.
byte animPoint = 4;
byte aliData = 8;
byte volBrightnessD  = 8;
byte volBrightnessN  = 2;
bool volBrightnessAuto = 0;
byte levelBridhtness = 0;
int lowLivelBrightness = 0;
int upLivelBrightness = 1023;
byte timeDay = 7;
byte timeNight = 21;

byte timeStartViewWeather = 6;
byte timeEndViewWeather = 23;
byte timeScrollSpeed = 20;

// ---------- Настройка оновлення часу
IPAddress timeServerIP;
String ntpServerName = "ntp3.time.in.ua";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];


bool stopAlarm = false;

int pinDHT = 2;

SimpleDHT22 dht22;

boolean WIFI_connected = false;


WiFiUDP udp;
unsigned int localPort = 2390;
unsigned long epochNM;

#define LEAP_YEAR(Y) (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100)||!((1970+Y)%400)))   // Високосні літа

String jsonConfig = "{}";
String jsonAlarm = "{}";
String jsonTime = "{}";

// ---------- Змінні для роботи локального годинника
float timeZone = 2.0;                                                                       //  часовий пояс
float hourCorr;
bool isDayLightSaving = true;
int displayData = 1;                      // 0 - Не отображать, 1 - отображать статику, 2 - отображать в бегущей строке
long localEpoc = 0;
long localMillisAtUpdate = 0;

int g_hour, g_minute, g_second, g_month = 1, g_day, g_dayOfWeek, g_year;

bool statusUpdateNtpTime = 0;                                                               // якщо не "0" - то останнє оновленя часу було вдалим
String y, mon, wd, d, h, m, s, mes;
uint8_t hourTest[3], minuteTest[3];

String date;

bool clockNight = 0;

// ----------
byte dig[MAX_DIGITS] = {0};
byte digold[MAX_DIGITS] = {0};
byte digtrans[MAX_DIGITS] = {0};
int dx = 0;
int dy = 0;
byte del = 0;

bool updateOTAEnable = 1;
// пароль обновления
String passwordOTA  = "11223344";


#include "sgpModule.h"


unsigned long minCount;
unsigned long minCount2;
unsigned long weatherCount;
unsigned long weatherZCount;
bool displayForecast = true;
bool displayCityName = true;           // отображать название города
bool displayForecastNow = true;        // отображать прогноз на сейчас
bool displayForecastToday = true;      // отображать прогноз на сегодня
bool displayForecastTomorrow = true;   // отображать прогноз на завтра
bool updateForecastNot = true;
int updateForecast = 0;
int updateForecasttomorrow = 0;

float data00; // данные первого датчика
float data01; // данные второго датчика
float data02; // данные третьего датчика
float data03; // данные 4 датчика
float data04; // данные пятого датчика

bool pressSys = 1;
byte humidity;                        // влажность для прогноза
float pressure;                       // давление для прогноза
float temp;                           // температура для прогноза

bool ds18b20Found = false;
bool dhtFound = false;

float tempDht = 0;
float humiDht = 0;

float corr00 = 0;
float corr01 = 0;
float corr02 = 0;
float corr03 = 0;
float corr04 = 0;

byte sensore[5] = {10,6,7,10,0};     //NONE=0, DS18B20=1, Si7021=2, BMP280=3, BME280=4, DHT=5, MQTT1=6, MQTT2=7, MQTT3=8; =9, AHTx0=10, THING1=11, THING2=12, THING3=13, THING4=14, THING5=15, nMon00=16, nMon00=17, nMon00=18,;
byte param0 = 0;      //  0-темп.дом(tD), 1-темп.улица(tU), 2-темп.Н(tН), 3-(tT), 4-(tL) 
byte param1 = 1;      // 10-влажность1(hD), 11-(h1), 12-(h2), 13-(h3), 14-(h4)
byte param2 = 2;      // 20-давление(P), 21-целое число(С) 22-батарейка(V), 23-батарейка(A)
byte param3 = 10;
byte param4 = 20;


//bool setTMes = true;
bool alarm_stat = 0;
bool alarm_hold = 0;
byte alarm_numer = 255;
byte alarme[5][3] {{12, 30, 0}, {7, 15, 0}, {22, 55, 0}, {0, 30, 0}, {0, 0, 0}}; //1-часы, 2-минуты, 3-откл(0)/1раз(11)/пон-пят(8)/пон-сб(9)/сб-вс(10)/вс(1)/пон(2)/вто(3)/сре(4)/чет(5)/пят(6)/сб(7)/всегда(12)

bool firstStart = 0;
bool apStart=0;
byte amountNotStarts = 0;
String jsonLine = "";

bool rtcStat = false;



byte errorRTC;
bool butStat = 0;
byte butMode = 0; // 0 - не нажата, 1 - нажата один раз, 2 - нажата два раза, 3 - 5 секунд нажата, 4 - 30 секунд нажата.
byte butFlag = 0; // 1 - кнопка нажата, 0 - не нажата
int butCount = 0; // счетчик времени нажатия кнопки
int butMillis = 0;
bool runningLine = 0;

String tJanuary, tFebruary, tMarch, tApril, tMay, tJune, tJuly, tAugust, tSeptember, tOctober, tNovember, tDecember;
String tMonday, tTuesday, tWednesday, tThursday, tFriday, tSaturday, tSunday;

String space = "";
bool startLine = false;
String textLine;

//======================================================================================
void setup() {
  Wire.begin();
  Serial.begin(115200);
  if (printCom) {
    Serial.println("");
  }
  pinMode(BUT_PIN, INPUT);
  digitalWrite(BUT_PIN, !butStat);
  delay(500);
  SPIFFS.begin();
  loadConfig();
  loadAlarm();

  initLang();
  initMAX7219();
  sendCmdAll(CMD_SHUTDOWN, 1);
  sendCmdAll(CMD_INTENSITY, 1);
  
  // задаем количество пробелов в зависимости от кол-ва знаков
  for(int i = 0; i < MAX7219_NUM; i++) {
    space += "   ";
  }
  
  Wire.beginTransmission(0x67);
  errorRTC = Wire.endTransmission();
  if (errorRTC == 0) {
    rtcAddr = 0x67;
    if(printCom) {
      Serial.println("YES!!! find RTC module addr: 0x67!");
    }
  } else {
    Wire.beginTransmission(0x68);
    errorRTC = Wire.endTransmission();
    if(errorRTC == 0) {
      rtcAddr = 0x68;
      if(printCom) {
        Serial.println("YES!!! find RTC module addr: 0x68!");
      }
    } else {
      rtcStat = false;
    }
  }
  if(rtcStat) {
    if(printCom) {
      Serial.println("RTC START");
    }
    getRTCDateTime();
    hour = rtcStruct.hour;
    minute = rtcStruct.minute;
    second = rtcStruct.second;
    day = rtcStruct.day;
    month = rtcStruct.month;
    year = rtcStruct.year;
    dayOfWeek = rtcStruct.dayOfWeek;
    if (printCom) {
      Serial.println("RTC update: " + String(hour) + ":" + String(minute) + ":" + String(second) + "    " + String(day) + "." + String(month) + "." + String(year) + " D=" + String(dayOfWeek));
    }
  } else if (printCom) {
    Serial.println("RTC module off!");
  }
  
  // ------------------
  sensorsDht();

  sensors();
  server_init();
  // ----------
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
  udp.begin(localPort);
  pinMode(buzzerPin, OUTPUT);
  // ---------- Підключення до WiFi
  wifiConnect();
  blinker.attach(0.05, ledPrint);
  // ***********  OTA SETUP
  
  if(updateOTAEnable) {
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("ESP-ZAL");
    //ArduinoOTA.setPassword((const char *)"123");
    ArduinoOTA.onEnd([](){ESP.restart();});
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR && printCom) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR && printCom) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR && printCom) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR && printCom) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR && printCom) {
        Serial.println("End Failed");
      }
      ESP.restart();
    });
    ArduinoOTA.begin();
  }
  if (WiFi.status() == WL_CONNECTED) {
    if (displayForecast) {
      if (!weatherHost) {
        getWeatherData0();
        getWeatherDataz0();
      } else {
        getWeatherData1();
        getWeatherDataz1();
      }
    }
  }

  if(!sgp.begin()){
    if(printCom){
      Serial.println("Sensor SGP-30 not found :(");
    }
    sgpFound = false;
  } else {
    if(printCom){
      printTime();
      Serial.print("Found SGP30 serial #");
      Serial.print(sgp.serialnumber[0], HEX);
      Serial.print(sgp.serialnumber[1], HEX);
      Serial.println(sgp.serialnumber[2], HEX);
    }   
    sgpFound = true;
  }
}

//======================================================================================
void reconnect(){
  if(printCom) {
    printTime();
    Serial.println("Start Reconnect void...");
  }
  if(WiFi.status() == WL_CONNECTED) {  // && !ESPclient.connected()
    if(printCom) {
      printTime();
      Serial.print("MQTT reconnection...");
    }
  }
}
void ledPrint(){
  updateTime();   
  if(endString){
    if(function[fnCount]==1) showAnimClock();
    if(!startLine){
      if(function[fnCount]==2){
        if(displayData==1){
          showSimpleDate();
        } else if(displayData==2 && hour>=timeStartViewWeather && hour<timeEndViewWeather){
          convertDw();
          convertMonth();
          textLine = space + dw + ", " + String(day) + " " + _month + " " + String(year) + space;
          startLine=true;
        } else fnTimer=millis();
      } else if(function[fnCount]==3){
        if(hour>=timeStartViewWeather && hour<timeEndViewWeather && displayForecast){
          //if((timeDay<=timeNight?(hour>=timeDay && hour<=timeNight):(hour>=timeDay || hour<timeNight)) || !clockNight){
            if(updateForecastNot){
              textLine=space + tWeatrTN + space;
              startLine=true;
            }
          else {
            if(updateForecast > 30 && updateForecast < 360) weatherString = space + tWeatrTN + " - " + String(updateForecast) + "мин." + space;
            else if(updateForecast >= 360) weatherString = space + tWeatrNot + space;
            textLine=weatherString;
            if(updateForecasttomorrow<30) textLine+=weatherStringZ;
            startLine=true;
          }
          //} else fnTimer=millis();
        } else fnTimer=millis();
      } else if(function[fnCount]>3 && function[fnCount]<9){
        if(sensore[function[fnCount]-4]){
          showSimple(function[fnCount]-4);
        } else fnTimer=millis();
      }
    }
  }
}
//======================================================================================
//======================================================================================
void loop() {
  // дозволяємо HTTP серверу відповідать на запити
  server.handleClient();
  
  if(updateOTAEnable) {
    ArduinoOTA.handle();
  }
  
  if(fnTimer < millis() && endString) {
    fnCount++;
    if(fnCount >= 12) {
      fnCount=0;
    }
    fnTimer = millis() + (period[fnCount] * 1000);
  }
  if(startLine) {
    printStringWithShift(textLine.c_str(), timeScrollSpeed);
    startLine = false;
  }
                                
  buttonInter();

  // сигнал кожду годину
  checkNeedHourSound();
  if(secFr==0 && second==10 && !alarm_stat) {
    sensorsAll();
  }
  //----------- РОБОТА З БУДИЛЬНИКОМ------------------------------------------------------
  if(secFr==0) {
    if(second>0 && alarms()){
      if(!alarm_stat && alarm_numer!=255 && !alarm_hold) alarm_stat=1;
    } else if(alarm_stat){
      alarm_stat=0;
      if(alarme[alarm_numer][2]==11) alarme[alarm_numer][2]=0;
    } else if(alarm_hold!=0);
  }
  if(alarm_stat){
    if(secFr==0 && second>1 && second<=59){
      invert();
      refreshAll();
      bip();
      bip();
    }
  }
  //------------- РОБОТА З ЯСКРАВІСТЮ ЕКРАНУ --------------------------------
  if(secFr==0){
    if(volBrightnessAuto){
      //levelBridhtness = map(analogRead(brightPin), 1023, 0, 0, 15);
      int br=analogRead(brightPin);
      if(lowLivelBrightness<=upLivelBrightness){
        if(br<lowLivelBrightness) lowLivelBrightness=br;
        if(br>upLivelBrightness) upLivelBrightness=br;
      } else{
        if(br<upLivelBrightness) upLivelBrightness=br;
        if(br>lowLivelBrightness) lowLivelBrightness=br;
      }
      levelBridhtness=map(br,lowLivelBrightness, upLivelBrightness, volBrightnessN, volBrightnessD);
      sendCmdAll(CMD_INTENSITY, levelBridhtness);
    } else{
      if(hour>=timeDay && hour<timeNight) sendCmdAll(CMD_INTENSITY, volBrightnessD);
      else sendCmdAll(CMD_INTENSITY, volBrightnessN);
    }
  }
  buttonHandling();

  // --------------------------------------------------------------------------------------------------------
  if(secFr == 0) {
    if(second == 10 && minute == 1 && hour == 3){
      if(rtcStat) {
        getRTCDateTime();
        hour = rtcStruct.hour;
        minute = rtcStruct.minute;
        second = rtcStruct.second;
        day = rtcStruct.day;
        month = rtcStruct.month;
        year = rtcStruct.year;
        dayOfWeek = rtcStruct.dayOfWeek;
        if(printCom) {
          Serial.println("RTC update: " + String(hour) + ":" + String(minute) + ":" + String(second) + "  " + String(day) + "." + String(month) + "." + String(year) + " D=" + String(dayOfWeek));
        }
      }
    }
    if(!alarm_stat && WIFI_connected) {
      // ---------- Проверка погоды каждые пол часа. -------------------------------------
      if(displayForecast && (millis()>(weatherCount+900000) || (updateForecast && millis()>(weatherCount+60000)))){
        weatherCount=millis();
        if(!weatherHost) {
          getWeatherData0();
        } else {
          getWeatherData1();
        }
        //if(!MQTTclient.connected() && mqttOn) reconnect();
      }
      if(displayForecast && displayForecastTomorrow && (millis()>(weatherZCount+900000) || (updateForecasttomorrow && millis()>(weatherZCount+60000)))){
        weatherZCount=millis();
        if(!weatherHost) getWeatherDataz0();
        else getWeatherDataz1();
        //if(!MQTTclient.connected() && mqttOn) reconnect();
      }
      //----------- Отправка NMon, Thing, MQTT, SGP(опрос) каждую миунту -------------------------------
      if(millis()>(minCount+60000)){
        minCount = millis();
        if(sgpFound) {
          sgp30();
        }
      }
    }
    if(millis()>(minCount2+60000)){
      minCount2=millis();
      if((WiFi.status()!=WL_CONNECTED || !WIFI_connected) && !alarm_stat && firstStart){
        WIFI_connected=false;
        WiFi.disconnect();
        if(minute%5==1){
          wifiConnect();
          if(WiFi.status()==WL_CONNECTED) WIFI_connected=true;
        }
      }
      if((statusUpdateNtpTime==0 || (minute==02 && second==43)) && !alarm_stat) timeUpdateNTP();
    }
  }
  // ---------- якщо мережа WiFi доступна то виконуємо наступні функції ----------------------------
  if(secFr==0 && butMode != 0) { 
    // если отработали все функции работы с кнопкой, то состояние сбросится автоматом
    Serial.println("BUT MODE RESET");
    butMode = 0;
  }
}

//======================================================================================
void showSimple(byte num) {
  if(num == 0) {
    if(param0 < 10) {
      showSimpleTemperature(param0, data00);
    } else if(param0 < 20) {
      showSimpleHumidity(param0, data00);
    } else if(param0 == 20) {
      showSimplePressure(param0, data00);
    } else if(param0 == 21) {
      showSimpleNumeric(param0, data00);
    }
  } else if(num == 1) {
    if(param1 < 10) showSimpleTemperature(param1, data01);
    else if(param1 < 20) showSimpleHumidity(param1, data01);
    else if(param1 == 20) showSimplePressure(param1, data01);
    else if(param1 == 21) showSimpleNumeric(param1, data01);
  } else if(num == 2) {
    if(param2 < 10) showSimpleTemperature(param2, data02);
    else if(param2 < 20) showSimpleHumidity(param2, data02);
    else if(param2 == 20) showSimplePressure(param2, data02);
    else if(param2 == 21) showSimpleNumeric(param2, data02);
  } else if(num == 3) {
    if(param3 < 10) showSimpleTemperature(param3, data03);
    else if(param3 < 20) showSimpleHumidity(param3, data03);
    else if(param3 == 20) showSimplePressure(param3, data03);
    else if(param3 == 21) showSimpleNumeric(param3, data03);
  } else if(num == 4) {
    if(param4 < 10) showSimpleTemperature(param4, data04);
    else if(param4 < 20) showSimpleHumidity(param4, data04);
    else if(param4 == 20) showSimplePressure(param4, data04);
    else if(param4 == 21) showSimpleNumeric(param4, data04);
  }
}

//==========ВИВІД НА ЕКРАН ТЕМПЕРАТУРИ========================================
void showSimpleTemperature(byte znakT, float temp0) {
  int temp1 = int(temp0);
  int temp2 = int(temp0 * 10 * (temp0 > 0 ? 1 : -1)) % 10;
  int znak = znakT % 10;
  byte digPos[10] {3, 4, 5, 6, 17, 18, 19, 20, 21, 22};
  byte indent = aliData * (MAX7219_NUM - 4);
  dx = dy = 0;
  clr();
  showDigit((temp0 < 0.0 ? digPos[znak*2+1]:digPos[znak*2]), indent, znaki5x8); // друкуємо D+ альбо D-
  if(temp1 <= -10.0 || temp1 >= 10) showDigit((temp1 < 0 ? (temp1 * -1) / 10 : temp1 / 10), 4 + indent, dig5x8);
  showDigit((temp1 < 0 ? (temp1 * -1) % 10 : temp1 % 10), 10 + indent, dig5x8);
  showDigit(2, 16 + indent, znaki5x8);
  showDigit(temp2, 18 + indent, dig5x8);      
  showDigit(0, 24 + indent, znaki5x8);
  showDigit(1, 27 + indent, znaki5x8);
  refreshAll();
}

//==========ВИВІД НА ЕКРАН ВОЛОГОСТІ========================================
void showSimpleHumidity(byte znakT, float humi0){
  if(humi0>0){
    int humi1 = int(humi0);
    int humi2 = int(humi0*10*(humi0>0?1:-1))%10;
    int znak = znakT%10;
    byte digPos[5] {7, 23, 24, 25, 26};
    byte indent = aliData * (MAX7219_NUM - 4);
    dx = dy = 0;
    clr();
    showDigit(digPos[znak], indent, znaki5x8);     // друкуємо знак вологості
    if (humi1 >= 10) showDigit(humi1/10, 6 + indent, dig5x8);
    showDigit((humi1-(humi1/10)*10), 12 + indent, dig5x8);
    showDigit(2, 18 + indent, znaki5x8);
    showDigit(humi2, 20 + indent, dig5x8);
    showDigit(8, 26 + indent, znaki5x8);
    refreshAll();
  }
}

//==========ВИВІД НА ЕКРАН ТИСКУ========================================
void showSimplePressure(byte znakT, float press0){
  if(pressSys==1 && press0>815) press0 /= 1.3332239;
  if(pressSys!=1 && press0<815) press0 /= 0.7500615613026439;
  if(press0 > 0){
    int press1 = (int) press0 / 1000;
    int press2 = ((int) press0 - press1 * 1000) / 100;
    int press3 = ((int) press0 - press1 * 1000 - press2 * 100) / 10;
    int press4 = (int)press0 % 10;
    byte indent = aliData * (MAX7219_NUM - 4);
    dx = dy = 0;
    clr();
    showDigit(9, 0 + indent, znaki5x8);     // друкуємо знак тиску
    if(press1 > 0) showDigit(press1, 5 + indent, dig5x8);
    showDigit(press2, (press1 > 0 ? 10 : 6) + indent, dig5x8);
    showDigit(press3, (press1 > 0 ? 16 : 12) + indent, dig5x8);
    showDigit(press4, (press1 > 0 ? 22 : 18) + indent, dig5x8);
    showDigit((pressSys == 1 ? 10 : 15), (press1 > 0 ? 28 : 24) + indent, znaki5x8);
    showDigit((pressSys == 1 ? 11 : 16), (press1 > 0 ? (pressSys == 1 ? 33 : 32) : (pressSys == 1 ? 29 : 28)) + indent, znaki5x8);
    refreshAll();
  }
}

//==========ВИВІД НА ЕКРАН ДОДАТКОВИХ ДАННИХ========================================
void showSimpleNumeric(byte znakT, float numer0) {
  byte indent = aliData * (MAX7219_NUM - 4);
  dx = dy = 0;
  clr();
  showDigit((numer0 < 0.0 ? 32 : 31), 0 + indent, znaki5x8);
  float numer1 = numer0 * (numer0 >= 0 ? 1 : -1);
  if (numer1 >= 10000) {
    showDigit((int)numer1 % 10, 28 + indent, dig4x8);
    showDigit((int)(numer1 / 10) % 10, 23 + indent, dig4x8);
    showDigit((int)(numer1 / 100) % 10, 18 + indent, dig4x8);
    showDigit((int)(numer1 / 1000) % 10, 13 + indent, dig4x8);
    showDigit((int)(numer1 / 10000) % 10, 8 + indent, dig4x8);
  } else if (numer1 >= 1000) {
    showDigit((int)(numer1 * 10) % 10, 28 + indent, dig4x8);
    showDigit(2, 26 + indent, znaki5x8);
    showDigit((int)numer1 % 10, 21 + indent, dig4x8);
    showDigit((int)(numer1 / 10) % 10, 16 + indent, dig4x8);
    showDigit((int)(numer1 / 100) % 10, 11 + indent, dig4x8);
    showDigit((int)(numer1 / 1000) % 10, 6 + indent, dig4x8);
  } else {
    showDigit((int)(numer1 * 100) % 10, 28 + indent, dig4x8);
    showDigit((int)(numer1 * 10) % 10, 23 + indent, dig4x8);
    showDigit(2, 21 + indent, znaki5x8);
    showDigit((int)numer1 % 10, 16 + indent, dig4x8);
    if (numer1 >= 10) showDigit((int)(numer1 / 10) % 10, 11 + indent, dig4x8);
    if (numer1 >= 100) showDigit((int)(numer1 / 100) % 10, 6 + indent, dig4x8);
  }
  refreshAll(); 
}

//==========ВИВІД НА ЕКРАН ДАТИ=========================================================
void showSimpleDate() {
  bool nonsens = false;
  byte digPos[8] {0, 5, 10, 12, 17, 22, 23, 28};
  if(year % 10 == 1) {
    digPos[6]++;
    if(month%10 == 1) {
      digPos[3]++;
      digPos[2]++;
      digPos[1]++;
      digPos[0]++;
    }
    if(month / 10 == 1) {
      digPos[2]++;
      digPos[1]++;
      digPos[0]++;
    }
    if(day % 10 == 1) {
      digPos[0]++;
    }
  } else {
    if(month % 10 == 1) {
      digPos[5]--;
      digPos[4]--;
      } else if(month / 10 == 1) {
      digPos[5]--;
      digPos[4]--;
      digPos[3]--;
    }
    if(month % 10 == 1 && month / 10 == 1) {
      digPos[2]++;
      digPos[1]++;
      digPos[0]++;
    }
    if(month % 10 != 1 && month / 10 != 1 && (day % 10 == 1 || day / 10 == 1)) {
      digPos[5]--;
      digPos[4]--;
      digPos[3]--;
      digPos[2]--;
      digPos[1]--;
    } else if(day % 10 == 1) {
      digPos[0]++;
    }
    if(month % 10 != 1 && month / 10 != 1 && day % 10 != 1 && day / 10 == 1) {
      nonsens = true;
    }
  }
  byte indent = aliData * (MAX7219_NUM - 4);
  dx = dy = 0;
  clr();
  showDigit(nonsens?10:day / 10, digPos[0] + indent, dig4x8);
  showDigit(day % 10, digPos[1] + indent, dig4x8);
  showDigit(2, digPos[2] + indent, znaki5x8);
  showDigit(month / 10, digPos[3] + indent, dig4x8);
  showDigit(month % 10, digPos[4] + indent, dig4x8);
  showDigit(2, digPos[5] + indent, znaki5x8);
  showDigit((year - 2000) / 10, digPos[6] + indent, dig4x8);
  showDigit((year - 2000) % 10, digPos[7] + indent, dig4x8);
  refreshAll();
}

//==========ВИВІД НА ЕКРАН АНІМАЦІЙНОГО ГОДИННИКА=======================================
void showAnimClock() {
  if(fontCLOCK > 7) {
    showAnimClock2();
    return;
  }
  byte indent = (hour < 10 ? 12 : 15) + 4 * (MAX7219_NUM - 4);
  byte digPos[5] = {(indent-(fontCLOCK<2?14:fontCLOCK<6?12:10)), (indent-(fontCLOCK<2?7:fontCLOCK<6?6:5)), (indent+3), (indent+(fontCLOCK<2?10:fontCLOCK<6?9:8)), indent};
  int digHt = 16;
  int num=hour<10?1:0;
  int i;
  if(del==0){
    del=digHt;
    for(i=num; i<4; i++) digold[i]=dig[i];
    dig[0]=hour/10;
    dig[1]=hour%10;
    dig[2]=minute/10;
    dig[3]=minute%10;
    for(i=num; i<4; i++) digtrans[i]=(dig[i]==digold[i])?0:digHt;
  } else del--;
  clr();
  for(i=num; i<4; i++){
    if(digtrans[i]==0){
      dy=0;
      if(fontCLOCK==0) showDigit(dig[i], digPos[i], dig6x8);
      else if(fontCLOCK==1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if(fontCLOCK==2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if(fontCLOCK==3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if(fontCLOCK==4) showDigit(dig[i], digPos[i], dig5x8);
      else if(fontCLOCK==5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if(fontCLOCK==6) showDigit(dig[i], digPos[i], dig4x8);
      else if(fontCLOCK==7) showDigit(dig[i], digPos[i], dig4x8dig);
    } else{
      dy=digHt-digtrans[i];
      if(fontCLOCK==0) showDigit(digold[i], digPos[i], dig6x8);
      else if(fontCLOCK==1) showDigit(digold[i], digPos[i], dig6x8dig);
      else if(fontCLOCK==2) showDigit(digold[i], digPos[i], dig5x8rn);
      else if(fontCLOCK==3) showDigit(digold[i], digPos[i], dig5x8rndig);
      else if(fontCLOCK==4) showDigit(digold[i], digPos[i], dig5x8);
      else if(fontCLOCK==5) showDigit(digold[i], digPos[i], dig5x8dig);
      else if(fontCLOCK==6) showDigit(digold[i], digPos[i], dig4x8);
      else if(fontCLOCK==7) showDigit(digold[i], digPos[i], dig4x8dig);
      dy=-digtrans[i];
      if(fontCLOCK==0) showDigit(dig[i], digPos[i], dig6x8);
      else if(fontCLOCK==1) showDigit(dig[i], digPos[i], dig6x8dig);
      else if(fontCLOCK==2) showDigit(dig[i], digPos[i], dig5x8rn);
      else if(fontCLOCK==3) showDigit(dig[i], digPos[i], dig5x8rndig);
      else if(fontCLOCK==4) showDigit(dig[i], digPos[i], dig5x8);
      else if(fontCLOCK==5) showDigit(dig[i], digPos[i], dig5x8dig);
      else if(fontCLOCK==6) showDigit(dig[i], digPos[i], dig4x8);
      else if(fontCLOCK==7) showDigit(dig[i], digPos[i], dig4x8dig);
      digtrans[i]--;
    }
  }
  dy=0;
  int flash=millis()%2000;
  if(animPoint%2) flash=flash%1000;
  else flash=flash/2;
  if (!alarm_stat){
    if(!WIFI_connected){
      if(flash<500){
        setCol(digPos[4], 0xC0); setCol(digPos[4]+1, 0xC0); // полная точка
      }
    } else if(!statusUpdateNtpTime){
      if(flash<149){
        setCol(digPos[4], 0x66); setCol(digPos[4]+1, 0x66);
      }
    } else if(animPoint<3){      // Простая 
      if(flash<499 || animPoint==0){
        setCol(digPos[4], 0x66); setCol(digPos[4]+1, 0x66);
      }
    } else if(animPoint==3 || animPoint==4){ // MAX1
      if(flash<500){
        setCol(digPos[4], 0x62); setCol(digPos[4]+1, 0x62);
      } else{
        setCol(digPos[4], 0x46); setCol(digPos[4]+1, 0x46);
      }
    } else if(animPoint==5 || animPoint==6){ // MAX2
      if((flash>=200 && flash<400) || flash>=600){
        setCol(digPos[4], 0x66); setCol(digPos[4]+1, 0x66);
      }
      if(flash>=0 && flash<200){
        setCol(digPos[4], 0x24); setCol(digPos[4]+1, 0x24);
      }
      if(flash>=400 && flash<600){
        setCol(digPos[4], 0x42); setCol(digPos[4]+1, 0x42);
      }
    } else if(animPoint==7 || animPoint==8){ // Мерцание 
      if((flash>=(animPoint==5?180:200) && flash<(animPoint==5?360:400)) || flash>=(animPoint==5?540:600)){
        setCol(digPos[4], 0x66); setCol(digPos[4]+1, 0x66);
      }
      if(flash>=0 && flash<(animPoint==5?180:200)){
        setCol(digPos[4], 0x24); setCol(digPos[4]+1, 0x42);
      }
      if(flash>=(animPoint==5?360:400) && flash<(animPoint==5?540:600)){
        setCol(digPos[4], 0x42); setCol(digPos[4]+1, 0x24);
      }
    } else if(animPoint==9 || animPoint==10){ // Вращение 
      if(flash<250){
        setCol(digPos[4], 0x06); setCol(digPos[4]+1, 0x60);
      } else if(flash>=250 && flash<500){
        setCol(digPos[4], 0x42); setCol(digPos[4]+1, 0x42); 
      } else if(flash>=500 && flash<750){
        setCol(digPos[4], 0x60); setCol(digPos[4]+1, 0x06);  
      } else if(flash>=750){
        setCol(digPos[4], 0x24); setCol(digPos[4]+1, 0x24);
      }
    }
    if(displayForecast && updateForecast && WIFI_connected) {
      setCol(00, flash<500?0x80:0x00);
    }
    if(displayForecastTomorrow && displayForecast && updateForecasttomorrow && WIFI_connected) {
      setCol((MAX7219_NUM*8-1), flash < 500 ? 0x80 : 0x00); 
    }
  } else {
    setCol(digPos[4], 0x66);
    setCol(digPos[4]+1, 0x66);
  }
  refreshAll();
}

//=========================================================================================
void showAnimClock2() {
  int num = hour < 10 ? 1 : 0;
  int indent = 4 * (MAX7219_NUM - 4) / 2 - (1 * num);
  byte digPos[6] = {0, 6, 13, 19, 25, 29};
  int digHt=16;
  int i;
  if(del==0){
    del=digHt;
    for(i=num; i<4; i++) digold[i]=dig[i];
    dig[0]=hour/10;
    dig[1]=hour%10;
    dig[2]=minute/10;
    dig[3]=minute%10;
    for(i=num; i<4; i++) digtrans[i]=(dig[i]==digold[i])?0:digHt;
  } else del--;
  clr();
  for(i=num; i<4; i++){
    if(digtrans[i]==0){
      dy=0;
      showDigit(dig[i], digPos[i]+indent-(i==1&&num?2:0), fontCLOCK==8?dig5x8sec:dig5x7sec);
    } else{
      dy=digHt-digtrans[i];
      showDigit(digold[i], digPos[i]+indent-(i==1&&num?2:0), fontCLOCK==8?dig5x8sec:dig5x7sec);
      dy=-digtrans[i];
      showDigit(dig[i], digPos[i]+indent-(i==1&&num?2:0), fontCLOCK==8?dig5x8sec:dig5x7sec);
      digtrans[i]--;
    }
  }
  dy=0;
  int flash=millis()%2000;
  flash=flash/2;
  if (!alarm_stat) {
    if(flash<500) {
      setCol(11+indent-(num?1:0), 0x80);
    } else {
      setCol(12+indent-(num?1:0), 0x80);
    }
    if(displayForecast && updateForecast && WIFI_connected) {
      setCol(00, flash<500?0x80:0x00);
    }
    if(displayForecastTomorrow && displayForecast && updateForecasttomorrow && WIFI_connected) {
      setCol((MAX7219_NUM*8-1), flash<500?0x80:0x00); 
    }
  } else {
    setCol(11+indent-(num?1:0), 0x80);
    setCol(12+indent-(num?1:0), 0x80);
  }
  showDigit((second/10)%10, digPos[4]+indent+(num?1:0), fontCLOCK==8?dig3x7:dig3x6);
  showDigit(second%10, digPos[5]+indent+(num?1:0), fontCLOCK==8?dig3x7:dig3x6);
  refreshAll();
}

//=================================================
void showAnimWifi(byte probaWifi) {
  byte digPos[2] = {18, 25};
  int digHt = 16;
  int num = 2;
  int ii;
  if (del == 0) {
    del = digHt;
    for (ii = 0; ii < num; ii++) digold[ii] = dig[ii];
    dig[0] = probaWifi / 10;
    dig[1] = probaWifi % 10;
    for (ii = 0; ii < num; ii++)  digtrans[ii] = (dig[ii] == digold[ii]) ? 0 : digHt;
  } else del--;
  clr();
  for (ii = 0; ii < num; ii++) {
    if (digtrans[ii] == 0) {
      dy = 0;
      showDigit(dig[ii], digPos[ii], dig6x8);
    } else {
      dy = digHt - digtrans[ii];
      showDigit(digold[ii], digPos[ii], dig6x8);
      dy = - digtrans[ii];
      showDigit(dig[ii], digPos[ii], dig6x8);
      digtrans[ii]--;
    }
  }
  dy = 0;
  refreshAll();
}

//==========ДРУКУВАННЯ БІГУЧОЇ СТРОКИ *s - текст, shiftDelay - швидкість==========================================
void printStringWithShift(const char* s, int shiftDelay) {
  endString=false;
  while(*s){                                                  // коли працює ця функція, основний цикл зупиняється
    printCharWithShift(*s, shiftDelay);
    s++;
    if (updateOTAEnable) {
      ArduinoOTA.handle();
    }
    server.handleClient();                                      // зберігаемо можливість відповіді на HTML запити під час бігучої стоки
    buttonInter();
    if (butMode != 0) {
      clr();
      refreshAll();
      runningLine = 0;
      return;
    }
  }
  endString=true;
}

//==========ДРУКУВАННЯ БІГУЧОГО СИМВОЛУ с - символ, shiftDelay - швидкість=====================================
void printCharWithShift(unsigned char c, int shiftDelay) {
  c = convert_UA_RU_PL_DE(c);
  if (c < ' ') return;
  c -= 32;
  int w = showChar(c, fontUA_RU_PL_DE);
  for (int i = 0; i < w + 1; i++) {
    delay(shiftDelay);
    scrollLeft();
    refreshAll();
  }
}

//======================================================================================
int showChar(char ch, const uint8_t *data) {
  int len = pgm_read_byte(data);
  int i, w = pgm_read_byte(data + 1 + ch * len);
  for (i = 0; i < w; i++)
    scr[MAX7219_NUM * 8 + i] = pgm_read_byte(data + 1 + ch * len + 1 + i);
  scr[MAX7219_NUM * 8 + i] = 0;
  return w;
}

//======================================================================================
void showDigit(char ch, int col, const uint8_t *data) {
  if (dy < -8 | dy > 8) return;
  int len = pgm_read_byte(data);
  int w = pgm_read_byte(data + 1 + ch * len);
  col += dx;
  for (int i = 0; i < w; i++) {
    if (col + i >= 0 && col + i < 8 * MAX7219_NUM) {
      byte v = pgm_read_byte(data + 1 + ch * len + 1 + i);
      if (!dy) scr[col + i] = v; else scr[col + i] |= dy > 0 ? v >> dy : v << -dy;
    }
  }
}

//======================================================================================
void setCol(int col, byte v) {
  if (dy < -8 | dy > 8) return;
  col += dx;
  if (col >= 0 && col < 8 * MAX7219_NUM) {
    if (!dy) scr[col] = v; else scr[col] |= dy > 0 ? v >> dy : v << -dy;
  }
}

//==========
void saveChrMas(String string_t, byte lenght_off, byte number_s) {
  byte lenght = string_t.length();
  if (lenght > lenght_off) return;
  const char *s = string_t.c_str();

  for (int i = 0; i < lenght; i++) {
    //snprintf(*memory_date_mes[number_s], 1, "%s", *s);
    s++;
  }
}

//==========ОНОВЛЕННЯ ЛОКАЛЬНОГО ЧАСУ (ЛОКАЛЬНІ ЧАСИ)===============================================================
void updateTime(){
  long curEpoch=localEpoc+((millis()-localMillisAtUpdate)/1000);
  long epoch=round(curEpoch+86400L);
  epoch=(epoch%86400L);
  hour=((epoch%86400L)/3600)%24;
  minute=(epoch%3600)/60;
  second=epoch%60;
  if(second!=lastSecond) { 
    // на початку нової секунди скидаємо secFr в "0"
    lastSecond=second;
    secFr=0;
    if(lastMinute!=minute){
      if(hour==0 && minute==0 && second==0){
        day++;
        if(day==32 || (day==31 && (month==4 || month==6 || month==9 || month==11)) || (month==2 && ((day==29 && year%4!=0) || (day==30 && year%4==0)))){
          day=1;
          month++;
          if(month>12){
            month=1;
            year++;
          }
        }
        dayOfWeek++;
        if(dayOfWeek>7) dayOfWeek=1;
      }
      lastMinute=minute;
    }
  } else secFr++; 
}

//==========ОНОВЛЕННЯ МЕРЕЖЕВОГО ЧАСУ (перевірка в три проходи)====================================================
void timeUpdateNTP() {
  if (!WIFI_connected) {
    return;
  }
  if (printCom) {
    printTime();
  }
  statusUpdateNtpTime = 1;
  for (int timeTest = 0; timeTest < 3; timeTest++) {
    getNTPtime();
    if (printCom) {
      if (timeTest) Serial.print("          ");
      Serial.println("Proba #" + String(timeTest + 1) + "   " + String(g_hour) + ":" + ((g_minute < 10) ? "0" : "") + String(g_minute) + ":" + ((g_second < 10) ? "0" : "") + String(g_second));
    }
    // updateTime();

    hourTest[timeTest] = g_hour;
    minuteTest[timeTest] = (g_minute || (g_minute == 59 ? 0 : g_minute++));
    if (statusUpdateNtpTime == 0) {
      if (printCom) {
        printTime();
        Serial.print("ERROR TIME!!!\r\n");
      }
      return;
    }
    if (timeTest > 0) {
      if ((hourTest[timeTest] != hourTest[timeTest - 1] || minuteTest[timeTest] != minuteTest[timeTest - 1])) {
        statusUpdateNtpTime = 0;
        if (printCom) {
          printTime();
          Serial.print("ERROR TIME!!!\r\n");
        }
        return;
      }
    }
  }
  hour = g_hour;
  minute = g_minute;
  second = g_second;
  day = g_day;
  dayOfWeek = g_dayOfWeek;
  month = g_month;
  year = g_year;
  if (rtcStat) {
    rtcStruct.hour = hour;
    rtcStruct.minute = minute;
    rtcStruct.second = 0;
    rtcStruct.year = year;
    rtcStruct.month = month;
    rtcStruct.day = day;
    rtcStruct.dayOfWeek = dayOfWeek;
    setRTCDateTime();
  }
  localMillisAtUpdate = millis();
  localEpoc = (hour * 60 * 60 + minute * 60 + second);
//  saveTime();
  if (printCom) {
    printTime();
    Serial.println((day < 10 ? "0" : "") + String(day) + "." + (month < 10 ? "0" : "") + String(month) + "." + String(year) + " DW = " + String(dayOfWeek));
    Serial.println("          Time update OK.");
  }
}

//==========ОТРИМАННЯ ДАТИ ТА ЧАСУ ВІД СЕРВЕРА ТОЧНОГО ЧАСУ =============================================================
void getNTPtime() {
  WiFi.hostByName(ntpServerName.c_str(), timeServerIP);
  int cb;
  for (int i = 0; i < 3; i++) {
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;
    udp.beginPacket(timeServerIP, 123);                     //NTP порт 123
    udp.write(packetBuffer, NTP_PACKET_SIZE);
    udp.endPacket();
    delay(800);                                             // чекаємо пів секуни
    cb = udp.parsePacket();
    if (!cb && printCom) Serial.println("          no packet yet..." + String (i + 1));
    if (!cb && i == 2) {                                             // якщо час не отримано
      statusUpdateNtpTime = 0;
      return;                                             // вихіз з getNTPtime()
    }
    if (cb) i = 3;
  }
  if (cb) {                                                  // якщо отримали пакет з серверу
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;        // Unix час станом на 1 січня 1970. в секундах, то 2208988800:
    unsigned long epoch = secsSince1900 - seventyYears;
    epochNM = epoch - (millis() / 1000);
    boolean summerTime;
    if (month < 3 || month > 10) summerTime = false;            // не переходимо на літній час в січні, лютому, листопаді і грудню
    if (month > 3 && month < 10) summerTime = true;             // Sommerzeit лічимо в квіні, травні, червені, липні, серпені, вересені
    if (month == 3 && (hour + 24 * day) >= (3 + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (3 + 24 * (31 - (5 * year / 4 + 1) % 7))) summerTime = true;
    epoch += (int)(timeZone * 3600 + (3600 * (isDayLightSaving && summerTime)));
    hourCorr = timeZone + (isDayLightSaving && summerTime);
    g_year = 0;
    int days = 0;
    uint32_t time;
    time = epoch / 86400;
    g_hour = (epoch % 86400L) / 3600;
    g_minute = (epoch % 3600) / 60;
    g_second = epoch % 60;
    g_dayOfWeek = (((time) + 4) % 7) + 1;
    while ((unsigned)(days += (LEAP_YEAR(g_year) ? 366 : 365)) <= time) {
      g_year++;
    }
    days -= LEAP_YEAR(g_year) ? 366 : 365;
    time -= days;
    days = 0;
    g_month = 0;
    uint8_t monthLength = 0;
    for (g_month = 0; g_month < 12; g_month++) {
      if (g_month == 1) {
        if (LEAP_YEAR(g_year)) monthLength = 29;
        else monthLength = 28;
      }
      else monthLength = monthDays[g_month];
      if (time >= monthLength) time -= monthLength;
      else break;
    }
    g_month++;
    g_day = time + 1;
    g_year += 1970;
    return;
  }
  if (printCom) Serial.println("Nie ma czasu(((");
}

//=========================================================================================================
void wifiConnect() {
  if (printCom) {
    printTime();
    Serial.print("Connecting WiFi (ssid0=" + String(ssid[0].c_str()) + "  pass0=" + String(password[0].c_str()) + ") ");
  }
  if(WiFi.status() == WL_CONNECTED){
    WIFI_connected = true;
    if(printCom) Serial.print(" IP adress : ");
    if(printCom) Serial.println(WiFi.localIP());
    firstStart=1;
    timeUpdateNTP();
    amountNotStarts=0;
    return;
  }
  if (!firstStart) printStringWithShift("WiFi", 15);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid[0].c_str(), password[0].c_str());
  for (int i = 1; i < 21; i++) {
    if (WiFi.status() == WL_CONNECTED){
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
      WIFI_connected = true;
      if(printCom) Serial.print(" IP adress : ");
      if(printCom) Serial.println(WiFi.localIP());
      if(!firstStart){
        String aaa=WiFi.localIP().toString() + space;
        clr();
        printStringWithShift(" IP: ", 15);
        printStringWithShift(aaa.c_str(), 25);
      }
      firstStart=1;
      timeUpdateNTP();
      amountNotStarts=0;
      return;
    }
    if(printCom) Serial.print(".");
    if(!firstStart){
      int j=0;
      while (j<500){
        if(j%10==0) showAnimWifi(i);
        j++;
        delay(1);
      }
    }
    delay (800);
  }
  if (printCom) {
    printTime();
    Serial.print("/nConnecting WiFi (ssid1=" + String(ssid[1].c_str()) + "  pass1=" + String(password[1].c_str()) + ") ");
  }
  if (!firstStart) printStringWithShift("WiFi", 15);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid[1].c_str(), password[1].c_str());
  for (int i = 1; i < 21; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      WIFI_connected = true;
      if(printCom) Serial.print(" IP adress : ");
      if(printCom) Serial.println(WiFi.localIP());
      if(!firstStart){
        String aaa=WiFi.localIP().toString() + space;
        clr();
        printStringWithShift(" IP: ", 15);
        printStringWithShift(aaa.c_str(), 25);
      }
      firstStart=1;
      timeUpdateNTP();
      amountNotStarts=0;
      return;
    }
    if(printCom) Serial.print(".");
    if(!firstStart){
      int j=0;
      while (j<500){
        if(j%10==0) showAnimWifi(i);
        j++;
        delay(1);
      }
    }
    delay (800);
  }
  WiFi.disconnect();
  if (printCom) Serial.println(" Not connected!!!");
  amountNotStarts++;
  if (printCom) {
    Serial.print("Amount of the unsuccessful connecting = ");
    Serial.println(amountNotStarts);
  }
  if (amountNotStarts > 21) {
    amountNotStarts = 0;
    firstStart = 0;
    ESP.reset();
  }
  if (!firstStart) {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssidAP.c_str(), passwordAP.c_str());
    if (printCom) {
      printTime();
      Serial.println("Start AP mode!!!");
      Serial.print("          Wifi AP IP : ");
      Serial.println(WiFi.softAPIP());
    }
    updateTime();
    String aaa = tPoint + " " + ssidAP;
    if(passwordAP != "") aaa += ",  " + tPass + ": " + passwordAP;
    aaa += ",  " + tIp + ": 192.168.4.1";
    clr();
    printStringWithShift(aaa.c_str(), 35);
    //firstStart=1;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sensorsAll() {
  if(printCom) Serial.println("======== START GET SENSORS DATA =======================");
  sensorsDht();
  sensors();
  if(printCom) Serial.println("======== END ==========================================");
}

//-------------------------------------------------------------------------
void sensors() {
  data00 = (sensore[0]==0?0:sensore[0]==5?(param0>4?humiDht:tempDht):0);
  if(sensore[0]) {
    data00 += corr00;
  }
  data01 = (sensore[1]==0?0:sensore[1]==5?(param1>4?humiDht:tempDht):0);
  if(sensore[1]) {
    data01 += corr01;
  }
  if(printCom){
    if(sensore[0]){
      printTime();
      Serial.println("sensore[0](+corr) = " + String(data00));
    }
    if(sensore[1]){
      printTime();
      Serial.println("sensore[1](+corr) = " + String(data01));
    }
  }
  if(setSgpCorr){
    if(setSgpCorr==4 && dhtFound){
      sgpCorrTemp = tempDht;
      sgpCorrHumi = humiDht;
    }
  }

  if((sensore[0]==5 && sensore[2]==5 && dhtFound)){
    if(param4==21&&sensore[4]==99){
      float absH=(6.112*(pow(2.718281828,(17.67*data00)/(data00+243.5)))*data02*2.1674)/(273.15+data00);
      if(printCom){
        printTime();
        Serial.println("Absolute Humi = " + String(absH) + "g/m3");
      }
      data04=absH;
    }
    if(param3==3&&sensore[3]==99){
      float ans=(data00-(14.55+0.114*data00)*(1-(0.01*data02))-pow(((2.5+0.007*data00)*(1-(0.01*data02))),3)-(15.9+0.117*data00)*pow((1-(0.01*data02)),14));
      if(printCom){
        printTime();
        Serial.println("Dew point = " + String(ans) + "*C");
      }
      data03=ans;
    }
  }
}

//--------------------------------------------------------------------------
void sensorsDht() {   //5
  dhtFound=false;
  int err = SimpleDHTErrSuccess;
  int err2 = SimpleDHTErrSuccess;
  byte temp11 = 0;
  byte humi11 = 0;
  if((err2 = dht22.read2(pinDHT, &tempDht, &humiDht, NULL)) == SimpleDHTErrSuccess) {
    humiDht = int(humiDht);
    if(printCom) {
      printTime();
      Serial.println("Temperature DHT22: " + String(tempDht) + " *C,  Humidity: " + String(humiDht) + " %");
    }
    dhtFound=true;
  }
}

void printTime() {
  if (printCom) {
    Serial.print((hour < 10 ? "0" : "") + String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second) + "  ");
  }
}

//--------------------------------------------------------------------------
byte alarms() {
  for (byte i = 0; i < 5; i++) {
    if (alarme[i][0] == hour && alarme[i][1] == minute && (alarme[i][2] == dayOfWeek || (alarme[i][2] == 8 && (dayOfWeek > 1 && dayOfWeek < 7)) || (alarme[i][2] == 9 && dayOfWeek > 1) || (alarme[i][2] == 10 && (dayOfWeek == 1 || dayOfWeek == 7)) || alarme[i][2] > 10)) {
      alarm_numer = i;
      return 1;
    }
  }
  alarm_numer = 255;
  return 0;
}

//------------ function urlencode for weather parameters --------------------
String urlencode(String str) {    // функция взята от http://forum.amperka.ru/members/benben.19545/
  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  return encodedString;
}

//------------ function chr_to_str --------------------
String chr_to_str(String str) {
  String chr_to_str = "";
  for (int i = 0; i < str.length(); i++) {
    chr_to_str += str.charAt(i);
  }
  return chr_to_str;
}

//---------------------------------------------------------------------------
void buttonInter() {
  if (digitalRead(BUT_PIN) == butStat && butCount == 0 && butFlag == 0  && butMode == 0) {
    butCount = millis();
    butFlag = 1;
  }
  if ((millis() - butCount) >= 30000 && butFlag == 1 && butMode == 0) {
    butMode = 4;
    butFlag = 0;
    butCount = 0;
  }
  if (digitalRead(BUT_PIN) == !butStat && (millis() - butCount) >= 10000 && butFlag == 1 && butMode == 0) {
    butMode = 3;
    butFlag = 0;
    butCount = 0;
  }
  if (digitalRead(BUT_PIN) == !butStat && (millis() - butCount) < 10000 && (millis() - butCount) > 800 && (butFlag == 1 || butFlag == 2) && butMode == 0) {
    butMode = 1;
    butFlag = 0;
    butCount = 0;
  }
  if (digitalRead(BUT_PIN) == !butStat && (millis() - butCount) <= 800 && butFlag == 1 && butMode == 0) {
    butFlag = 2;
  }
  if (digitalRead(BUT_PIN) == butStat && (millis() - butCount) <= 800  && butFlag == 2) {
    butMode = 2;
    butFlag = 0;
    butCount = 0;
  }
}

//----------------------------------------------------------------
void buttonHandling() {
  if (alarm_stat &&  (butMode || stopAlarm)) { // если будильник работает, то любое нажатие выключает его
    alarm_stat = 0;
    alarm_hold = 1;
    stopAlarm = false;
    butMode = 0;
    if (alarme[alarm_numer][2] == 11) {
      alarme[alarm_numer][2] = 0;
    }
  }
  if (butMode == 4) { // если кнопка нажата была более 30 секунд то возврат к заводским установкам
    butMode = 0;
    Serial.println("Ta to jest KAPUT!!!!!!");
    bip();
    bip();
    bip();
    bip();
    SPIFFS.remove("/config.json");
    if (printCom) {
      printTime();
      Serial.println("ESP erase Config file");
    }
    delay(3000);
    ESP.reset();
  }
  if (butMode == 3) { // если кнопка была нажата более 10 секунд но менее 30, то будет рестарт часов
    butMode = 0;
    Serial.println("Reset ESP!!!");
    bip();
    bip();
    bip();
    ESP.reset();
  }
  if (butMode == 1) {
    bip();
    clr();
    refreshAll();
    if(sensore[0]) {
      showSimple(0);
      delay(1500);
    }
    if(sensore[1]) {
      showSimple(1);
      delay(1500);
    }
    if(sensore[2]) {
      showSimple(2);
      delay(1500);
    }
    if(sensore[3]) {
      showSimple(3);
      delay(1500);
    }
    if(sensore[4]) {
      showSimple(4);
      delay(1500);
    }
    butMode = 0;
    clr();
    refreshAll();
  }
  if (butMode == 2) { // При двойном нажатии на кнопку выводится прогноз погоды
    bip();
    bip();
    butMode = 0;
    clr();
    refreshAll();
    printStringWithShift(weatherString.c_str(), timeScrollSpeed);
    printStringWithShift(weatherStringZ.c_str(), timeScrollSpeed);
    clr();
    refreshAll();
  }
}
