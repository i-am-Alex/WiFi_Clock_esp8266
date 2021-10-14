int BCD2DEC(int x) { return ((x)>>4)*10+((x)&0xf); }
int DEC2BCD(int x) { return (((x)/10)<<4)+((x)%10); }

#define I2CStart(x)   Wire.beginTransmission(x)
#define I2CStop()     Wire.endTransmission()
#define I2CWrite(x)   Wire.write(x)
#define I2CRead()     Wire.read()
#define I2CReq(x,y)   Wire.requestFrom(x,y)
#define I2CReady      while(!Wire.available()) {};

#define DS_RTC_TIME    0x00
#define DS_RTC_DOW     0x03
#define DS_RTC_DATE    0x04
#define DS_RTC_MEM     0x08

int rtcAddr = 0x68;

typedef struct {
  int hour, minute, second, month, day, dayOfWeek, year;
} RtcStruct;

RtcStruct rtcStruct;

void setRTCDateTime() {
  I2CStart(rtcAddr);
  I2CWrite(DS_RTC_TIME);
  I2CWrite(DEC2BCD(rtcStruct.second));
  I2CWrite(DEC2BCD(rtcStruct.minute));
  I2CWrite(DEC2BCD(rtcStruct.hour));
  I2CWrite(DEC2BCD(rtcStruct.dayOfWeek));
  I2CWrite(DEC2BCD(rtcStruct.day));
  I2CWrite(DEC2BCD(rtcStruct.month));
  I2CWrite(DEC2BCD(rtcStruct.year - 2000));
  I2CStop();
}

void getRTCDateTime(void) {
  int v;
  I2CStart(rtcAddr);
  I2CWrite(DS_RTC_TIME);
  I2CStop();
  I2CReq(rtcAddr, 7);
  I2CReady;
  v = I2CRead() & 0x7f;
  rtcStruct.second = BCD2DEC(v);
  v = I2CRead() & 0x7f;
  rtcStruct.minute = BCD2DEC(v);
  v = I2CRead() & 0x3f;
  rtcStruct.hour = BCD2DEC(v);
  v = I2CRead() & 0x07;
  rtcStruct.dayOfWeek = BCD2DEC(v);
  v = I2CRead() & 0x3f;
  rtcStruct.day = BCD2DEC(v);
  v = I2CRead() & 0x3f;
  rtcStruct.month = BCD2DEC(v);
  v = I2CRead() & 0xff;
  rtcStruct.year = BCD2DEC(v) + 2000;
  I2CStop();
}
