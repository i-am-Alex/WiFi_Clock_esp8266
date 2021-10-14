const char P_index[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "style.css">
        <script type = "text/javascript" src = "function.js"></script>
    </head>
    <body onload = "load('/index-data');">
        <header>
            <div class="header-block">
                <span class="opt_cn">Часы информер WiFi_Clock_{{ver}}</span><br>
            </div>
        </header>
        <main>
            <div class="central-unit">
                <nav class="main-menu">
                    <span class="ico-text ico-text-active" onclick="get_html('index.vz');">Главная</span>
                    <span class="ico-text" onclick="get_html('wifi.vz');">Wifi</span>
                    <span class="ico-text" onclick="get_html('time.vz');">Время</span>
                    <span class="ico-text" onclick="get_html('weather.vz');">Погода</span>
                    <span class="ico-text" onclick="get_html('sgp.vz');">SGP</span>
                    <span class="ico-text" onclick="get_html('setup.vz');">Настройки</span>
                    <span class="ico-text" onclick="get_html('help.vz');">Помощь</span>
                    <span class="ico-text-right">&nbsp;</span>
                </nav>
                <div class="unit-body">
                    <div class="board-container">
                        <div id="block-time" class="main-time board-block board-block-full">
                            <div class="board-block-form">
                                <div class="board-block-title">Время ESP</div>
                                <div id="time" class="main-time-font">{{time}}</div>
                            </div>
                        </div>
                        <div id="block-weather" class="board-block board-block-full">
                            <div class="board-block-form">
                                <div class="board-block-title">Погода</div>
                                <div class="board-block-text">Сервер: <span id="weath-server"></span></div>
                                <div id="weath">Прогноз недоступен</div>
                            </div>
                        </div>
                        <div id="block-temperature" class="board-block board-block-half">
                            <div class="board-block-form">
                                <div class="board-block-title">Датчик 1</div>
                                <div id="temp" class="board-block-data">-28.9 C</div>
                            </div>
                        </div>
                        <div id="block-humidity" class="board-block board-block-half">
                            <div class="board-block-form">
                                <div class="board-block-title">Датчик 2</div>
                                <div id="humid" class="board-block-data">148 %</div>
                            </div>
                        </div>
                        <div id="block-sgp30" class="board-block board-block-half">
                            <div class="board-block-form">
                                <div class="board-block-title">Датчик sgp30</div>
                                <div id="data-sgp30" class="board-block-data">148 %</div>
                            </div>
                        </div>
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
