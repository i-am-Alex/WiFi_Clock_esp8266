const char P_time[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <meta http-equiv="Content-type" content="text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_ntp(submit) {
                let server = "/ntp?ntpServerName=" + val('ntpServerName')
                    + "&timeZone=" + val('timeZone')
                    + "&isDayLightSaving=" + val_sw('isDayLightSaving')
                    + "&rtcStat=" + val_sw('rtcStat')
                    + "&al_0_0=" + val('al_0_0')
                    + "&al_0_1=" + val('al_0_1')
                    + "&al_0_2=" + val('al_0_2')
                    + "&al_1_0=" + val('al_1_0')
                    + "&al_1_1=" + val('al_1_1')
                    + "&al_1_2=" + val('al_1_2')
                    + "&al_2_0=" + val('al_2_0')
                    + "&al_2_1=" + val('al_2_1')
                    + "&al_2_2=" + val('al_2_2')
                    + "&al_3_0=" + val('al_3_0')
                    + "&al_3_1=" + val('al_3_1')
                    + "&al_3_2=" + val('al_3_2')
                    + "&al_4_0=" + val('al_4_0')
                    + "&al_4_1=" + val('al_4_1')
                    + "&al_4_2=" + val('al_4_2')
                    + "&md_0_0=" + val('md_0_0')
                    + "&md_0_1=" + val('md_0_1')
                    + "&md_0_2=" + val('md_0_2')
                    + "&md_1_0=" + val('md_1_0')
                    + "&md_1_1=" + val('md_1_1')
                    + "&md_1_2=" + val('md_1_2')
                    + "&md_2_0=" + val('md_2_0')
                    + "&md_2_1=" + val('md_2_1')
                    + "&md_2_2=" + val('md_2_2')
                    + "&md_3_0=" + val('md_3_0')
                    + "&md_3_1=" + val('md_3_1')
                    + "&md_3_2=" + val('md_3_2')
                    + "&md_4_0=" + val('md_4_0')
                    + "&md_4_1=" + val('md_4_1')
                    + "&md_4_2=" + val('md_4_2')
                    + "&md_5_0=" + val('md_5_0')
                    + "&md_5_1=" + val('md_5_1')
                    + "&md_5_2=" + val('md_5_2')
                    + "&md_6_0=" + val('md_6_0')
                    + "&md_6_1=" + val('md_6_1')
                    + "&md_6_2=" + val('md_6_2')
                    + "&md_7_0=" + val('md_7_0')
                    + "&md_7_1=" + val('md_7_1')
                    + "&md_7_2=" + val('md_7_2')
                    + "&md_8_0=" + val('md_8_0')
                    + "&md_8_1=" + val('md_8_1')
                    + "&md_8_2=" + val('md_8_2')
                    + "&md_start=" + val('md_start')
                    + "&md_stop=" + val('md_stop')
                    + "&setTMes=" + val_sw('setTMes')
                send_request(submit, server);
            }
            function set_ntp_auto(submit) {
                let date = new Date();
                let day = date.getDate();
                let month = date.getMonth() + 1;
                let year = date.getFullYear();
                let hours = date.getHours( );
                let minute = date.getMinutes();
                let sec = date.getSeconds();
                let gmtHours = -date.getTimezoneOffset()/60;
                let server = "/timepc?hours="+hours+"&minute="+minute+"&sec="+sec+"&tz="+gmtHours+"&day="+day+"&month="+month+"&year="+year;
                send_request(submit,server);
            }
            function stop_alarm(submit) {
                let server = "/stopalarm?stopAlarm=ok";
                send_request(submit,server);
            }
            function set_time(submit) {
                let server = "/set_time?t0=" + val('t0') + "&t1=" + val('t1') + "&d0=" + val('d0') + "&d1=" + val('d1') + "&d2=" + val('d2');
                send_request(submit,server);
            }
        </script>
    </head>
    <body onload = "load('/configs_time');">
        <header>
            <div class="header-block">
                <span class="opt_cn">Часы информер WiFi_Clock_{{ver}}</span><br>
                <span class="opt_cn">Время на ESP:</span>
                <span id="time">{{time}}</span>
            </div>
        </header>
        <main>
            <div class="central-unit">
                <nav class="main-menu">
                    <span class="ico-text" onclick="get_html('index.vz');">Главная</span>
                    <span class="ico-text" onclick="get_html('wifi.vz');">Wifi</span>
                    <span class="ico-text ico-text-active" onclick="get_html('time.vz');">Время</span>
                    <span class="ico-text" onclick="get_html('weather.vz');">Погода</span>
                    <span class="ico-text" onclick="get_html('sgp.vz');">SGP</span>
                    <span class="ico-text" onclick="get_html('setup.vz');">Настройки</span>
                    <span class="ico-text" onclick="get_html('help.vz');">Помощь</span>
                    <span class="ico-text-right">&nbsp;</span>
                </nav>
                <div class="unit-body">
                    <fieldset>
                        <legend>Настройки Времени</legend>
                        <table class="table-centered">
                            <tr>
                                <td colspan="2" class="td-center">Сервер точного времени (пример) "time.nist.gov"</td>
                            </tr>
                            <tr>
                                <td class="right">Адрес сервера</td>
                                <td><input type="text" class="field form-control" id="ntpServerName" value="{{ntpServerName}}" pattern="[0-9a-zA-Z. ]{1,20}"></td>
                            </tr>
                            <tr>
                                <td class="right">Ваша часовая зона</td>
                                <td>
                                    <select id="timeZone">
                                        <option value="-12.00"> (GMT) -12:00 </option>
                                        <option value="-11.00"> (GMT) -11:00 </option>
                                        <option value="-10.00"> (GMT) -10:00 </option>
                                        <option value="-9.00" > (GMT) -09:00 </option>
                                        <option value="-8.00" > (GMT) -08:00 </option>
                                        <option value="-7.00" > (GMT) -07:00 </option>
                                        <option value="-6.00" > (GMT) -06:00 </option>
                                        <option value="-5.00" > (GMT) -05:00 </option>
                                        <option value="-4.00" > (GMT) -04:00 </option>
                                        <option value="-3.50" > (GMT) -03:30 </option>
                                        <option value="-3.00" > (GMT) -03:00 </option>
                                        <option value="-2.00" > (GMT) -02:00 </option>
                                        <option value="-1.00" > (GMT) -01:00 </option>
                                        <option value="0.00"  > (GMT) +00:00 </option>
                                        <option value="1.00"  > (GMT) +01:00 </option>
                                        <option value="2.00"  > (GMT) +02:00 </option>
                                        <option value="3.00"  > (GMT) +03:00 </option>
                                        <option value="3.50"  > (GMT) +03:30 </option>
                                        <option value="4.00"  > (GMT) +04:00 </option>
                                        <option value="4.50"  > (GMT) +04:30 </option>
                                        <option value="5.00"  > (GMT) +05:00 </option>
                                        <option value="5.50"  > (GMT) +05:30 </option>
                                        <option value="5.75"  > (GMT) +05:45 </option>
                                        <option value="6.00"  > (GMT) +06:00 </option>
                                        <option value="6.50"  > (GMT) +06:30 </option>
                                        <option value="7.00"  > (GMT) +07:00 </option>
                                        <option value="8.00"  > (GMT) +08:00 </option>
                                        <option value="8.50"  > (GMT) +08:30 </option>
                                        <option value="8.75"  > (GMT) +08:45 </option>
                                        <option value="9.00"  > (GMT) +09:00 </option>
                                        <option value="9.50"  > (GMT) +09:30 </option>
                                        <option value="10.00" > (GMT) +10:00 </option>
                                        <option value="10.50" > (GMT) +10:30 </option>
                                        <option value="11.00" > (GMT) +11:00 </option>
                                        <option value="12.00" > (GMT) +12:00 </option>
                                        <option value="12.75" > (GMT) +12:45 </option>
                                        <option value="13.00" > (GMT) +13:00 </option>
                                        <option value="14.00" > (GMT) +14:00 </option>
                                    </select>
                                </td>
                            <tr>
                            <tr>
                                <td class="right">Переход на летнее время</td>
                                <td>
                                    <input type="checkbox" id="isDayLightSaving">
                                    <span class="switch-btn"></span>
                                </td>
                            <tr>
                            <tr>
                                <td class="right">Использовать модуль RTC</td>
                                <td>
                                    <input type="checkbox" id="rtcStat">
                                    <span class="switch-btn"></span>
                                </td>
                            <tr>
                            <tr>
                                <td class="right"></td>
                                <td>

                                </td>
                            <tr>
                        </table>
                    </fieldset>
                    <fieldset>
                        <legend>Настройка будильника</legend>
                        <table class="table">
                            <tr>
                                <td><span class="opt_cn"><b> Номер </b></span></td>
                                <td><span class="opt_cn"><b> Часы </b></span></td>
                                <td><span class="opt_cn"><b> Минуты </b></span></td>
                                <td><span class="opt_cn"><b> Режим </b></span></td>
                            </tr>
                            <tr>
                                <td><span>1</span></td>
                                <td>
                                    <select id="al_0_0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_0_1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_0_2">
                                        <option value="0">Откл</option>
                                        <option value="11">Разово</option>
                                        <option value="8">ПОН-ПЯТ (2-6)</option>
                                        <option value="9">ПОН-СУБ (2-7)</option>
                                        <option value="10">СУБ-ВОС (6+1)</option>
                                        <option value="1">ВОС (1)</option>
                                        <option value="2">ПОН (2)</option>
                                        <option value="3">ВТО (3)</option>
                                        <option value="4">СРЕ (4)</option>
                                        <option value="5">ЧЕТ (5)</option>
                                        <option value="6">ПЯТ (6)</option>
                                        <option value="7">СУБ (7)</option>
                                        <option value="12">ВСЕГДА</option>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td><span>2</span></td>
                                <td>
                                    <select id="al_1_0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_1_1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_1_2">
                                        <option value="0">Откл</option>
                                        <option value="11">Разово</option>
                                        <option value="8">ПОН-ПЯТ (2-6)</option>
                                        <option value="9">ПОН-СУБ (2-7)</option>
                                        <option value="10">СУБ-ВОС (6+1)</option>
                                        <option value="1">ВОС (1)</option>
                                        <option value="2">ПОН (2)</option>
                                        <option value="3">ВТО (3)</option>
                                        <option value="4">СРЕ (4)</option>
                                        <option value="5">ЧЕТ (5)</option>
                                        <option value="6">ПЯТ (6)</option>
                                        <option value="7">СУБ (7)</option>
                                        <option value="12">ВСЕГДА</option>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td><span>3</span></td>
                                <td>
                                    <select id="al_2_0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_2_1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_2_2">
                                        <option value="0">Откл</option>
                                        <option value="11">Разово</option>
                                        <option value="8">ПОН-ПЯТ (2-6)</option>
                                        <option value="9">ПОН-СУБ (2-7)</option>
                                        <option value="10">СУБ-ВОС (6+1)</option>
                                        <option value="1">ВОС (1)</option>
                                        <option value="2">ПОН (2)</option>
                                        <option value="3">ВТО (3)</option>
                                        <option value="4">СРЕ (4)</option>
                                        <option value="5">ЧЕТ (5)</option>
                                        <option value="6">ПЯТ (6)</option>
                                        <option value="7">СУБ (7)</option>
                                        <option value="12">ВСЕГДА</option>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td><span>4</span></td>
                                <td>
                                    <select id="al_3_0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_3_1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_3_2">
                                        <option value="0">Откл</option>
                                        <option value="11">Разово</option>
                                        <option value="8">ПОН-ПЯТ (2-6)</option>
                                        <option value="9">ПОН-СУБ (2-7)</option>
                                        <option value="10">СУБ-ВОС (6+1)</option>
                                        <option value="1">ВОС (1)</option>
                                        <option value="2">ПОН (2)</option>
                                        <option value="3">ВТО (3)</option>
                                        <option value="4">СРЕ (4)</option>
                                        <option value="5">ЧЕТ (5)</option>
                                        <option value="6">ПЯТ (6)</option>
                                        <option value="7">СУБ (7)</option>
                                        <option value="12">ВСЕГДА</option>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td><span>5</span></td>
                                <td>
                                    <select id="al_4_0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_4_1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <select id="al_4_2">
                                        <option value="0">Откл</option>
                                        <option value="11">Разово</option>
                                        <option value="8">ПОН-ПЯТ (2-6)</option>
                                        <option value="9">ПОН-СУБ (2-7)</option>
                                        <option value="10">СУБ-ВОС (6+1)</option>
                                        <option value="1">ВОС (1)</option>
                                        <option value="2">ПОН (2)</option>
                                        <option value="3">ВТО (3)</option>
                                        <option value="4">СРЕ (4)</option>
                                        <option value="5">ЧЕТ (5)</option>
                                        <option value="6">ПЯТ (6)</option>
                                        <option value="7">СУБ (7)</option>
                                        <option value="12">ВСЕГДА</option>
                                    </select>
                                </td>
                            </tr>
                        </table>
                    </fieldset>
                    <fieldset>
                        <legend>Настройки Времени</legend>
                        <table>
                            <tr>
                                <td>
                                    <span>Время</span>
                                </td>
                                <td>
                                    <select id="t0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                    </select>
                                </td>
                                <td>
                                    <span>:</span>
                                </td>
                                <td>
                                    <select id="t1">
                                        <option value = "0"> 00 </option>
                                        <option value = "1"> 01 </option>
                                        <option value = "2"> 02 </option>
                                        <option value = "3"> 03 </option>
                                        <option value = "4"> 04 </option>
                                        <option value = "5"> 05 </option>
                                        <option value = "6"> 06 </option>
                                        <option value = "7"> 07 </option>
                                        <option value = "8"> 08 </option>
                                        <option value = "9"> 09 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                        <option value = "32"> 32 </option>
                                        <option value = "33"> 33 </option>
                                        <option value = "34"> 34 </option>
                                        <option value = "35"> 35 </option>
                                        <option value = "36"> 36 </option>
                                        <option value = "37"> 37 </option>
                                        <option value = "38"> 38 </option>
                                        <option value = "39"> 39 </option>
                                        <option value = "40"> 40 </option>
                                        <option value = "41"> 41 </option>
                                        <option value = "42"> 42 </option>
                                        <option value = "43"> 43 </option>
                                        <option value = "44"> 44 </option>
                                        <option value = "45"> 45 </option>
                                        <option value = "46"> 46 </option>
                                        <option value = "47"> 47 </option>
                                        <option value = "48"> 48 </option>
                                        <option value = "49"> 49 </option>
                                        <option value = "50"> 50 </option>
                                        <option value = "51"> 51 </option>
                                        <option value = "52"> 52 </option>
                                        <option value = "53"> 53 </option>
                                        <option value = "54"> 54 </option>
                                        <option value = "55"> 55 </option>
                                        <option value = "56"> 56 </option>
                                        <option value = "57"> 57 </option>
                                        <option value = "58"> 58 </option>
                                        <option value = "59"> 59 </option>
                                    </select>
                                </td>
                                <td>
                                    <span>Дата</span>
                                </td>
                                <td>
                                    <select id="d0">
                                        <option value = "0"> 0 </option>
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                        <option value = "13"> 13 </option>
                                        <option value = "14"> 14 </option>
                                        <option value = "15"> 15 </option>
                                        <option value = "16"> 16 </option>
                                        <option value = "17"> 17 </option>
                                        <option value = "18"> 18 </option>
                                        <option value = "19"> 19 </option>
                                        <option value = "20"> 20 </option>
                                        <option value = "21"> 21 </option>
                                        <option value = "22"> 22 </option>
                                        <option value = "23"> 23 </option>
                                        <option value = "24"> 24 </option>
                                        <option value = "25"> 25 </option>
                                        <option value = "26"> 26 </option>
                                        <option value = "27"> 27 </option>
                                        <option value = "28"> 28 </option>
                                        <option value = "29"> 29 </option>
                                        <option value = "30"> 30 </option>
                                        <option value = "31"> 31 </option>
                                    </select>
                                </td>
                                <td>
                                    <span>-</span>
                                </td>
                                <td>
                                    <select id="d1">
                                        <option value = "1"> 1 </option>
                                        <option value = "2"> 2 </option>
                                        <option value = "3"> 3 </option>
                                        <option value = "4"> 4 </option>
                                        <option value = "5"> 5 </option>
                                        <option value = "6"> 6 </option>
                                        <option value = "7"> 7 </option>
                                        <option value = "8"> 8 </option>
                                        <option value = "9"> 9 </option>
                                        <option value = "10"> 10 </option>
                                        <option value = "11"> 11 </option>
                                        <option value = "12"> 12 </option>
                                    </select>
                                </td>
                                <td>
                                    <span>-</span>
                                </td>
                                <td>
                                    <select id="d2">
                                        <option value = "2020"> 2020 </option>
                                        <option value = "2021"> 2021 </option>
                                        <option value = "2022"> 2022 </option>
                                        <option value = "2023"> 2023 </option>
                                        <option value = "2024"> 2024 </option>
                                        <option value = "2025"> 2025 </option>
                                        <option value = "2026"> 2026 </option>
                                        <option value = "2027"> 2027 </option>
                                        <option value = "2028"> 2028 </option>
                                        <option value = "2029"> 2029 </option>
                                        <option value = "2030"> 2030 </option>
                                        <option value = "2031"> 2031 </option>
                                    </select>
                                </td>
                            <tr>
                        </table>
                        <div class="right">
                            <span class="save_button" onclick="set_time(this);">Установить</span>
                        </div>
                    </fieldset>
                </div>
                <div class="unit-footer">
                    <div class="form-buttons">
                        <span class="save_button" onclick="set_ntp_auto(this);">Получить Дату/Время из ПК</span>
                        <span class="save_button" onclick="set_ntp(this);">Сохранить настройки</span>
                    </div>
                </div>
            </div>
        </main>
        <footer>
            <div class="footer-band">
                <div class="footer-band-labels">
                    <div class="footer-left">
                        <a href="https://github.com/i-am-Alex/WiFi_Clock_esp8266" class = "right">GitHub</a>
                    </div>
                    <div class="footer-right">
                        <a href="https://github.com/IZ76/WiFi_Clock" class = "right">GitHub</a>
                    </div>
                    <div class="footer-right">
                        <a href="http://forum.amperka.ru/threads/%D0%A7%D0%B0%D1%81%D1%8B-%D0%BD%D0%B0-esp8266-%D0%B8-max7219-android-%D1%83%D0%BF%D1%80%D0%B0%D0%B2%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5.12767/">Forum</a>
                    </div>
                    <div class="footer-right">
                        Original version:
                    </div>
                </div>
            </div>
        </footer>
    </body>
</html>
)=====";
