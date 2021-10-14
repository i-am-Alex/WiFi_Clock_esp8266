
Adafruit_SGP30 sgp;

boolean sgpFound = false;
int sgpCo2 = 400;

byte sgpCo2LivelAlarm = 2;
boolean eCo2AlarmEsp = true;

boolean eCo2Led = true;
String sgpCo2Message[] = {"","","","",""};
int sgpTvoc = 0;
byte sgpTvocLivelAlarm = 2;

boolean tvocAlarmEsp = true;

boolean tvocLed = true;

String sgpTvocMessage[] = {"","","","",""};

byte setSgpCorr = 0;  //0-нет коррекции, 1-BME-280, 2-Si7021, 3-AHTx0, 4-DHT11/22

float sgpCorrTemp = 22.1; // [°C]
float sgpCorrHumi = 45.2; // [%RH]

typedef struct {
  byte co2Livel = 0; //0=400-699, 1=700-999, 2=1000-2499, 3=2500-4999, 4=5000++
  int co2Livel1 =  700;
  int co2Livel2 = 1000;
  int co2Livel3 = 2500;
  int co2Livel4 = 5000;
  byte tvocLivel = 0; //0=0-64, 1=65-219, 2=220-659, 3=660-2199, 4=2200++
  int tvocLivel1 = 65;
  int tvocLivel2 = 220;
  int tvocLivel3 = 660;
  int tvocLivel4 = 2200;
} SgpValues;

SgpValues sgpValues;

uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void sgp30() {
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  if(setSgpCorr) {
    sgp.setHumidity(getAbsoluteHumidity(sgpCorrTemp, sgpCorrHumi));
  }
  printTime();
  if(!sgp.IAQmeasure()) {
    if(printCom) {
      Serial.println("Measurement failed");
    }
    return;
  }
  sgpCo2 = sgp.eCO2;
  sgpTvoc = sgp.TVOC;
  if(printCom) {
    Serial.print("TVOC = " + String(sgpTvoc) + " ppb,     ");
  }
  if(printCom) {
    Serial.print("eCO2 = " + String(sgpCo2) + " ppm,     ");
  }
  if(sgpCo2 < sgpValues.co2Livel1) {
    sgpValues.co2Livel = 0;
  } else if(sgpCo2 < sgpValues.co2Livel2) {
    sgpValues.co2Livel = 1;
  } else if(sgpCo2 < sgpValues.co2Livel3) {
    sgpValues.co2Livel = 2;
  } else if(sgpCo2 < sgpValues.co2Livel4) {
    sgpValues.co2Livel = 3;
  } else {
    sgpValues.co2Livel = 4;
  }
  if(sgpTvoc < sgpValues.tvocLivel1) {
    sgpValues.tvocLivel = 0;
  } else if(sgpTvoc < sgpValues.tvocLivel2) {
    sgpValues.tvocLivel = 1;
  } else if(sgpTvoc < sgpValues.tvocLivel3) {
    sgpValues.tvocLivel = 2;
  } else if(sgpTvoc < sgpValues.tvocLivel4) {
    sgpValues.tvocLivel = 3;
  } else {
    sgpValues.tvocLivel = 4;
  }
  
  if(!sgp.IAQmeasureRaw()) {
    if(printCom) {
      Serial.println("Raw Measurement failed");
    }
  } else {
    if(printCom) {
      Serial.print("Raw H2 = " + String(sgp.rawH2) + ",     ");
    }
    if(printCom) {
      Serial.println("Raw Ethanol = " + String(sgp.rawEthanol));
    }
    if (minute % 5 == 0) {
      uint16_t TVOC_base, eCO2_base;
      if(!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
        if(printCom){
          printTime();
          Serial.println("Failed to get baseline readings");
        }
        return;
      }
      if(printCom) {
        printTime();
        Serial.print("****Baseline values: eCO2: 0x" + String (eCO2_base, HEX));
        Serial.println(" & TVOC: 0x" + String(TVOC_base, HEX));
      }
    }
  }
  
  String livelCo2 = tAlarm + sgpCo2Message[sgpValues.co2Livel] + " eCO2 = " + String(sgpCo2) + " ppm               ";
  if(sgpValues.co2Livel >= sgpCo2LivelAlarm) {
    if(eCo2AlarmEsp) {
      bip(4);
      clr();
      printStringWithShift(("        " + livelCo2).c_str(), timeScrollSpeed);
    }
  }
  
  String livelTvoc = tAlarm + sgpTvocMessage[sgpValues.tvocLivel] + " TVOC = " + String(sgpTvoc) + " ppb               ";
  if(sgpValues.tvocLivel >= sgpTvocLivelAlarm) {
    if(tvocAlarmEsp) {
      bip(4);
      clr();
      printStringWithShift(("        " + livelTvoc).c_str(), timeScrollSpeed);
    }
  }
}
