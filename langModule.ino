

//==========КОНВЕРТАЦІЯ СИМВОЛІВ В РАЗІ ВИКОРИСТАННЯ УКРАЇНСЬКИХ ЛІТЕР==================
byte dualChar = 0;
unsigned char convert_UA_RU_PL_DE(unsigned char _c) {
  unsigned char c = _c;
  // конвертирование латиницы
  if (c == 208) {
    dualChar = 1;
    return 0;
  }
  else if (c == 209 || c == 210) {
    dualChar = 2;
    return 0;
  }
  if (c == 32 && dualChar != 3) {
    dualChar = 3;
    return c;
  }
  if (dualChar == 1) {
    if (c >= 144 && c < 192) {
      c += 48;
    }
    dualChar = 0;
    return c;
  }
  if (dualChar == 2) {
    if (c >= 128 && c < 144) {
      c += 112;
    }
    switch (_c) {
      case 144: c = 133; break;
      case 145: c = 149; break;
    }
    dualChar = 0;
    return c;
  }
  // конвертирование польского и немецкого
  if (c == 195) {
    dualChar = 4;
    return 0;
  }
  if (c == 196) {
    dualChar = 5;
    return 0;
  }
  if (c == 197) {
    dualChar = 6;
    return 0;
  }
  if (dualChar == 4) {
    switch (_c) {
      case 132: c = 177; break;
      case 147: c = 166; break;
      case 150: c = 179; break;
      case 156: c = 181; break;
      case 159: c = 183; break;
      case 164: c = 178; break;
      case 179: c = 167; break;
      case 182: c = 180; break;
      case 188: c = 182; break;
    }
    dualChar = 0;
    return c;
  }
  if (dualChar == 5) {
    if (c >= 132 && c < 136) {
      c += 26;
    }
    switch (_c) {
      case 152: c = 168; break;
      case 153: c = 169; break;
    }
    dualChar = 0;
    return c;
  }
  if (dualChar == 6) {
    if (c >= 129 && c < 133) {
      c += 33;
    }
    if (c >= 154 && c < 156) {
      c += 16;
    }
    if (c >= 185 && c < 189) {
      c -= 13;
    }
    dualChar = 0;
    return c;
  }
}

// ===========================КОНВЕРТАЦІЯ НАЗВ ДНІВ ТИЖНЯ НА УКРАЇНСЬКУ МОВУ============================================
void convertDw(){
  switch(dayOfWeek){
    case 1 : dw = tSunday;    break;
    case 2 : dw = tMonday;    break;
    case 3 : dw = tTuesday;   break;
    case 4 : dw = tWednesday; break;
    case 5 : dw = tThursday;  break;
    case 6 : dw = tFriday;    break;
    case 7 : dw = tSaturday;  break;
  }
}
// ===========================КОНВЕРТАЦІЯ НАЗВ МІСЯЦІВ НА УКРАЇНСЬКУ МОВУ============================================
void convertMonth(){
  switch(month){
    case 1 : _month = tJanuary;      break;
    case 2 : _month = tFebruary;     break;
    case 3 : _month = tMarch;        break;
    case 4 : _month = tApril;        break;
    case 5 : _month = tMay;          break;
    case 6 : _month = tJune;         break;
    case 7 : _month = tJuly;         break;
    case 8 : _month = tAugust;       break;
    case 9 : _month = tSeptember;    break;
    case 10 : _month = tOctober;     break;
    case 11 : _month = tNovember;    break;
    case 12 : _month = tDecember;    break;
  }
}

//-----------------------------------------------------
void initLang() {
  if (weatherLang == "uk") {
    tNow = "Зараз";
    tPress0 = "гПа";
    tPress = "ммРс";
    tSpeed = "м/с";
    tMin = "хв.";
    tCurr = "Сьогодні";
    tTom = "Завтра";
    tYour = "Ваш";
    tPoint = "Підключіться до";
    tIp = "та перейдіть за адресою";
    tPass = "пароль";
    tWeatrNot = "   Немає оновлень погоди більше 6 годин!!!   ";
    tWeatrTN = "Немає оновлень погоди.";
    tJanuary = "січня";
    tFebruary = "лютого";
    tMarch = "березня";
    tApril = "квітня";
    tMay = "травня";
    tJune = "червня";
    tJuly = "липня";
    tAugust = "серпня";
    tSeptember = "вересня";
    tOctober = "жовтня";
    tNovember = "листопада";
    tDecember = "грудня";
    tMonday = "Понеділок";
    tTuesday = "Вівторок";
    tWednesday = "Середа";
    tThursday = "Четвер";
    tFriday = "П'ятниця";
    tSaturday = "Субота";
    tSunday = "Неділя";
    tAlarm = "УВАГА!!!   ";
    sgpCo2Message[0] = "Допустимий рівень повітря";
    sgpCo2Message[1] = "Важке повітря. Необхідно провітрювання";
    sgpCo2Message[2] = "Можлива сонливість, втома, головний біль";
    sgpCo2Message[3] = "Серйозне погіршення здоров'я !!!";
    sgpCo2Message[4] = "Гранично допустима концентрація протягом 8 годин!!!";
    sgpTvocMessage [0] = "Допустимий рівень ЛОС";
    sgpTvocMessage [1] = "Рекомендується вентиляція";
    sgpTvocMessage [2] = "Рекомендується інтенсивна вентиляція";
    sgpTvocMessage [3] = "Інтенсивна вентиляція і провітрювання необхідні";
    sgpTvocMessage [4] = "Дуже інтенсивна вентиляція суворо необхідна !!!";
  } else if (weatherLang == "ru") {
    tNow = "Сейчас";
    tPress0 = "гПа";
    tPress = "ммРс";
    tSpeed = "м/с";
    tMin = "мин.";
    tCurr = "Сегодня";
    tTom = "Завтра";
    tYour = "Ваш";
    tPoint = "Подключитесь к точке доступа";
    tIp =  "и введите в браузере адрес";
    tPass = "пароль";
    tWeatrNot = "   Нет обновления погоды более 6 часов!!!   ";
    tWeatrTN = "Нет обновления погоды.";
    tJanuary = "января";
    tFebruary = "февраля";
    tMarch = "марта";
    tApril = "апреля";
    tMay = "мая";
    tJune = "июня";
    tJuly = "июля";
    tAugust = "августа";
    tSeptember = "сентября";
    tOctober = "октября";
    tNovember = "ноября";
    tDecember = "декабря";
    tMonday = "Понедельник";
    tTuesday = "Вторник";
    tWednesday = "Среда";
    tThursday = "Четверг";
    tFriday = "Пятница";
    tSaturday = "Суббота";
    tSunday = "Воскресенье";
    tAlarm = "ВНИМАНИЕ!!!   ";
    sgpCo2Message[0] = "Допустимый уровень возуха";
    sgpCo2Message[1] = "Тяжелый воздух. Необходимо проветривание";
    sgpCo2Message[2] = "Возможно сонливость, усталость, головная боль";
    sgpCo2Message[3] = "Серьезное ухудшение здоровья!!!";
    sgpCo2Message[4] = "Предельно допустимая концентрация в течении 8 часов";
    sgpTvocMessage[0] = "Допустимый уровень ЛОС";
    sgpTvocMessage[1] = "Рекомендуется вентиляция";
    sgpTvocMessage[2] = "Рекомендуется интенсивная вентиляция";
    sgpTvocMessage[3] = "Интенсивная вентиляция и проветривание необходимы";
    sgpTvocMessage[4] = "Очень интенсивная вентиляция строжайше необходима!!!";
  } else if (weatherLang == "en") {
    tNow = "now";
    tPress0 = "gPa";
    tPress = "mmHg";
    tSpeed = "m/s";
    tMin = "min.";
    tCurr = "Today";
    tTom = "Tomorrow";
    tYour = "Your";
    tPoint = "Connect to access point the";
    tIp = "and enter in the browser the address";
    tPass = "password";
    tWeatrNot = "   There is no weather update for more than 6 hours !!!   ";
    tWeatrTN = "No weather updates.";
    tJanuary = "January";
    tFebruary = "February";
    tMarch = "Martha";
    tApril = "April";
    tMay = "May";
    tJune = "June";
    tJuly = "July";
    tAugust = "August";
    tSeptember = "September";
    tOctober = "October";
    tNovember = "November";
    tDecember = "December";
    tMonday = "Monday";
    tTuesday = "Tuesday";
    tWednesday = "Wednesday";
    tThursday = "Thursday";
    tFriday = "Friday";
    tSaturday = "Saturday";
    tSunday = "Sunday";
    tAlarm = "ATTENTION!!!   ";
    sgpCo2Message[0] = "Permissible level of air";
    sgpCo2Message[1] = "Heavy air. Ventilation is necessary";
    sgpCo2Message[2] = "Drowsiness, fatigue, headache is possible";
    sgpCo2Message[3] = "Serious deterioration of health !!!";
    sgpCo2Message[4] = "Maximum permissible concentration within 8 hours";
    sgpTvocMessage [0] = "Allowable VOC Level";
    sgpTvocMessage [1] = "Recommended ventilation";
    sgpTvocMessage [2] = "Recommended intensive ventilation";
    sgpTvocMessage [3] = "Intensive ventilation and ventilation are necessary";
    sgpTvocMessage [4] = "Very intensive ventilation is strictly necessary !!!";
  }
}
