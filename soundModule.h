
void bip() {
  if (!isActiveBuzzer) {
    tone(buzzerPin, 2000, 40);
    delay(250);
    noTone(buzzerPin);
  } else {
    digitalWrite(buzzerPin, HIGH);
    delay(120);
    digitalWrite(buzzerPin, LOW);
    delay(120);
  }
}

void bip(uint8_t count) {
  for(uint8_t i = 0; i < count; i++) {
    bip();
  }
}

void checkNeedHourSound() {
  if(minute == 0 && second == 0 && secFr == 0 && (hour >= kuOn && hour < kuOff)) {     
    bip();
    bip();
  }
}
