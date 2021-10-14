const char P_weath[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <meta http-equiv="Content-type" content="text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css"/>
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            var set_real_time;
            function set_weather(submit) {
                let server = "/weather?weatherKey0=" + val('weatherKey0')
                    + "&weatherKey1=" + val('weatherKey1')
                    +"&weatherHost=" + val('weatherHost')
                    + "&cityID0=" + val('cityID0')
                    + "&cityID1=" + val('cityID1')
                    + "&weatherLang=" + val('weatherLang')
                    + "&displayForecast=" + val_sw('displayForecast')
                    + "&displayCityName=" + val_sw('displayCityName')
                    + "&displayForecastNow=" + val_sw('displayForecastNow')
                    + "&displayForecastToday=" + val_sw('displayForecastToday')
                    + "&displayForecastTomorrow=" + val_sw('displayForecastTomorrow')
                    + "&timeStartViewWeather=" + val('timeStartViewWeather')
                    + "&timeEndViewWeather=" + val('timeEndViewWeather')
                    + "&personalCityName=" + val('personalCityName');
                send_request(submit, server);
            }
            function set_weather_up(submit) {
                server = "/weatherUpdate?update=ok";
                send_request(submit, server);
            }
            function set_weath_on(submit) {
                server = "/weathOn?displayForecast=" + val_sw("displayForecast");
                send_request(submit, server);
            }
        </script>
    </head>
    <body onload = "load('/configs_weath');">
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
                    <span class="ico-text" onclick="get_html('time.vz');">Время</span>
                    <span class="ico-text ico-text-active" onclick="get_html('weather.vz');">Погода</span>
                    <span class="ico-text" onclick="get_html('sgp.vz');">SGP</span>
                    <span class="ico-text" onclick="get_html('setup.vz');">Настройки</span>
                    <span class="ico-text" onclick="get_html('help.vz');">Помощь</span>
                    <span class="ico-text-right">&nbsp;</span>
                </nav>
                <div class="unit-body">
                    <fieldset>
                        <legend>Настройки погоды</legend>
                        <div class="informer">
                            <span class="opt_cn">Чтобы получить прогноз погоды,</span>
                            <a href = "https://www.weatherbit.io/" align = "center" target = "_blank">weatherbit.io </a>
                            <a href = "https://openweathermap.org/" align = "center" target = "_blank">openweathermap.org </a>
                            <span class="opt_cn">здесь мы берем ключи API.</span>
                        </div>
                        <div id="main_unit">
                            <table class = "table">
                                <tr>
                                    <td align = "right"><span class="opt_cn">Погодный сервер</span></td>
                                    <td><select id = "weatherHost">
                                        <option value = "0"> weatherbit.io </option>
                                        <option value = "1"> openweathermap.org </option>
                                    </select></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">API weatherbit ключ</span></td>
                                    <td><input type="text" class="field" id="weatherKey0" value="{{weatherKey0}}" class="form-control" pattern="[0-9a-zA-Z.]{1,32}"></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Название города</span></td>
                                    <td><input type="text" class="field" id="cityID0" value="{{cityID0}}" class="form-control" pattern="[ 0-9a-zA-Zа-яА-Я._]{1,32}"></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">API openweathermap ключ</span></td>
                                    <td><input type="text" class="field" id="weatherKey1" value="{{weatherKey1}}" class="form-control" pattern="[0-9a-zA-Z.]{1,32}"></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Название или код города</span></td>
                                    <td><input type="text" class="field" id="cityID1" value="{{cityID1}}" class="form-control" pattern="[ 0-9a-zA-Zа-яА-Я._]{1,32}"></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Отображаемое название города</span></td>
                                    <td><input type="text" class="field form-control" id="personalCityName" value="{{personalCityName}}" pattern=".{1,25}"></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Язык прогноза</span></td>
                                    <td><select id = "weatherLang">
                                        <option value = "en"> English </option>
                                        <option value = "uk"> Український </option>
                                        <option value = "ru"> Русский </option>
                                        <option value = "de"> Deutsch </option>
                                        <option value = "pl"> Polski </option>
                                    </select></td>
                                </tr>
                            </table>
                            <table class = "table-centered">
                                <tr>
                                    <td class="right opt_cn">Получать и выводить погоду на экран</span></td>
                                    <td>
                                        <input type="checkbox" id="displayForecast" onclick="set_weath_on(this);">
                                        <span class="switch-btn"></span>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="right opt_cn">Отображать название города</span></td>
                                    <td>
                                        <input type="checkbox" id="displayCityName">
                                        <span class="switch-btn"></span>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="right opt_cn">Отображать прогноз на сейчас</td>
                                    <td>
                                        <input type="checkbox" id="displayForecastNow">
                                        <span class="switch-btn"></span>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="right opt_cn">Отображать прогноз на сегодня</td>
                                    <td>
                                        <input type="checkbox" id="displayForecastToday">
                                        <span class="switch-btn"></span>
                                    </td>
                                </tr>
                                <tr>
                                    <td class="right opt_cn">Получать и выводить прогноз на завтра</td>
                                    <td>
                                        <input type="checkbox" id="displayForecastTomorrow">
                                        <span class="switch-btn"></span>
                                    </td>
                                </tr>
                            </table>
                            <table class="table-centered">
                                <tr>
                                    <td align = "right"><span class="opt_cn">Показывать погоду с:</span></td>
                                    <td><select id = "timeStartViewWeather">
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
                                    </select></td>
                                    <td align = "right"><span class="opt_cn">до:</span></td>
                                    <td><select id = "timeEndViewWeather">
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
                                        <option value = "24"> полуночи </option>
                                    </select></td>
                                </tr>
                            </table>
                        </div>
                    </fieldset>
                    <fieldset>
                        <legend>Данные сервера</legend>
                            <table class = "table">
                                <tr>
                                    <td align = "right"><span class="opt_cn">Название города :</span></td>
                                    <td><span class="opt_cn">{{location_name}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Регион :</span></td>
                                    <td><span class="opt_cn">{{location_region}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Страна :</span></td>
                                    <td><span class="opt_cn">{{location_country}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Время обновления :</span></td>
                                    <td><span class="opt_cn">{{location_localtime}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Рассвет :</span></td>
                                    <td><span class="opt_cn">{{location_sunrise}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Закат :</span></td>
                                    <td><span class="opt_cn">{{location_sunset}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Температура :</span></td>
                                    <td><span class="opt_cn">{{location_temp}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Ощущается как :</span></td>
                                    <td><span class="opt_cn">{{location_app_temp}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Влажность (%) :</span></td>
                                    <td><span class="opt_cn">{{location_rh}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Давление (mb) :</span></td>
                                    <td><span class="opt_cn">{{location_pres}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Скорость ветра (m/s) :</span></td>
                                    <td><span class="opt_cn">{{location_wind_spd}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Направление ветра :</span></td>
                                    <td><span class="opt_cn">{{location_wind_cdir_full}}</span></td>
                                </tr>

                                <tr>
                                    <td align = "right"><span class="opt_cn">Облачность (%) :</span></td>
                                    <td><span class="opt_cn">{{location_clouds}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Видимость (km) :</span></td>
                                    <td><span class="opt_cn">{{location_vis}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Индекс UV (0-11+) :</span></td>
                                    <td><span class="opt_cn">{{location_uv}}</span></td>
                                </tr>
                                <tr>
                                    <td align = "right"><span class="opt_cn">Описание погоды :</span></td>
                                    <td><span class="opt_cn">{{location_weather_description}}</span></td>
                                </tr>
                            </table>
                    </fieldset>
                </div>
                <div class="unit-footer">
                    <div class="form-buttons">
                        <span class="save_button" onclick="set_weather_up(this);">Обновить прогноз погоды</span>
                        <span class="save_button" onclick="set_weather(this); load_weath();">Сохранить настройки и обновить погоду</span>
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
