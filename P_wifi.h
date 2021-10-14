const char P_wifi[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function setSsid(submit) {
                let server = "/ssid?ssid0=" + val('ssid0')
                    + "&password0=" + encodeURIComponent(val('password0'))
                    + "&ssid1=" + val('ssid1')
                    + "&password1=" + encodeURIComponent(val('password1'))
                    + "&ssidAP=" + val('ssidap')
                    + "&passwordAP=" + encodeURIComponent(val('passwordAp'))
                    + "&auth=" + val('auth')
                    + "&authOn=" + val_sw('authOn');
                send_request(submit,server);
                alert("Изменения вступят в силу после перезагрузки.");
            }
            function sendTest(submit) {
                let server = "/mess?text=" + val("test");
                send_request(submit, server);
            }
        </script>
    </head>
    <body onload = "load('/configs_wifi');">
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
                    <span class="ico-text ico-text-active" onclick="get_html('wifi.vz');">Wifi</span>
                    <span class="ico-text" onclick="get_html('time.vz');">Время</span>
                    <span class="ico-text" onclick="get_html('weather.vz');">Погода</span>
                    <span class="ico-text" onclick="get_html('sgp.vz');">SGP</span>
                    <span class="ico-text" onclick="get_html('setup.vz');">Настройки</span>
                    <span class="ico-text" onclick="get_html('help.vz');">Помощь</span>
                    <span class="ico-text-right">&nbsp;</span>
                </nav>
                <div class="unit-body">
                    <div id="main_unit">
                        <fieldset>
                            <legend>Настройки WIFI</legend>
                            <div class="informer">
                                <span class="opt_cn">Введите настройки интернет соединения для обновления времени, получения данных о погоде.</span>
                            </div>
                            <p>
                                <span class="opt_cn">Подключение к локальной WiFi сети</span>
                            </p>
                            <div class="center">
                                <table id="table__font" class="table">
                                    <tr>
                                        <th><span class="opt_cn"></span></th>
                                        <th><span class="opt_cn"><b>SSID WiFi сети</b></span></th>
                                        <th><span class="opt_cn"><b>Пароль</b></span></th>
                                    </tr>
                                    <tr>
                                        <td align="right"><span class="opt_cn">1</span></td>
                                        <td><input type="text" class="field form-control" id="ssid0" value="{{ssid0}}" pattern="[-\w\s#@/]{1,15}" placeholder="WiFi network name"></td>
                                        <td><input class="field form-control" id="password0" value="{{password0}}" pattern=".{8,15}" onfocus="this.type='text'" type="password"></td>
                                    </tr>
                                    <tr>
                                        <td align="right"><span class="opt_cn">2</span></td>
                                        <td><input type="text" class="field form-control" id="ssid1" value="{{ssid1}}" pattern="[-\w\s#@/]{1,15}" placeholder="WiFi network name"></td>
                                        <td><input class="field form-control" id="password1" value="{{password1}}" pattern=".{8,15}" onfocus="this.type='text'" type="password"></td>
                                    </tr>
                                </table>
                            </div>
                        </fieldset>

                        <fieldset>
                            <legend>Точка доступа часов (IP_192.168.4.1)</legend>
                             <div class="center">
                                <table class="table-centered">
                                    <tr>
                                        <td class="right"><span class="opt_cn">Введите имя точки доступа часов</span></td>
                                        <td><input type="text" class="field" id="ssidap" value="{{ssidAP}}" class="form-control" pattern="[-\w\s#@/]{1,15}" placeholder="access point name"></td>
                                    </tr>
                                    <tr>
                                        <td class="right"><span class="opt_cn">Пароль к точке доступа</span></td>
                                        <td><input class="field" id="passwordAp" value="{{passwordAP}}" class="form-control" pattern=".{0,15}" onfocus="this.type='text'" type="password" class="form-control"></td>
                                    <tr>
                                </table>
                            </div>
                        </fieldset>
                        <fieldset>
                            <legend>Авторизация WEB сервера</legend>
                            <div class="center">
                                <table class="table-centered">
                                    <tr>
                                        <td class="right"><span class="opt_cn">Использовать авторизацию</span></td>
                                        <td><input type="checkbox" lass="checkbox" id="authOn" name="switch-btn"></td>
                                    </tr>
                                    <tr>
                                        <td class="right"><span class="opt_cn">Токен авторизации</span></td>
                                        <td><input class="field form-control" id="auth" value="{{auth}}" onfocus="this.type='text'" type="password"></td>
                                    </tr>
                                </table>
                            </div>
                        </fieldset>
                        <fieldset>
                            <legend>Тестовое сообщение</legend>
                            <input type="text" class="field form-control" id="test" name="test" value="TEST" placeholder="Введите сообщение">
                            <span class="save_button" onclick="sendTest(this);">Отправить</span>
                        </fieldset>
                    </div>
                </div>
                <div class="unit-footer">
                    <div class="form-buttons">
                        <span class="save_button" onclick="setSsid(this);">Сохранить настройки</span>
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
