const char P_help[] PROGMEM =  R"=====(
<!DOCTYPE html>
<html lang="ru">
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function set_printCom(submit) {
                let server = "/printCom?printCom=" + val_sw('printCom');
                send_request(submit, server);
            }
            function resetConfig(submit,texts) {
                if(confirm(texts)) {
                    let server = "/resetConfig?device=ok";
                    send_request(submit, server);
                    return true;
                } else {
                    return false;
                }
            }
            function restart(submit, texts){
                if(confirm(texts)){
                    let server="/restart?device=ok";
                    send_request(submit, server);
                    return true;
                }else{
                    return false;
                }
}
        </script>
    </head>
    <body onload = "load('/configs');">
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
                    <span class="ico-text" onclick="get_html('weather.vz');">Погода</span>
                    <span class="ico-text" onclick="get_html('sgp.vz');">SGP</span>
                    <span class="ico-text" onclick="get_html('setup.vz');">Настройки</span>
                    <span class="ico-text ico-text-active" onclick="get_html('help.vz');">Помощь</span>
                    <span class="ico-text-right">&nbsp;</span>
                </nav>
                <div class="unit-body">
                    <fieldset>
                        <legend>Если что-то пошло не так...</legend>
                        <div class="informer"><span class="opt_cn">ВНИМАНИЕ!!! Некоторые изменения могут быть необратимыми!</span>
                        </div>
                        <div id="main_unit">
                            <label class="switch">
                                <span class="opt_cn">Отображать выполняемые функции и их результат в COM порт</span>
                                <input class="checkbox" type="checkbox" id="printCom" name="switch-btn" onclick="set_printCom(this);">
                                <span class="switch-btn"></span>
                            </label>
                        </div>
                        <hr>
                        <div class="center">
                            <div>
                                <b><span class="opt_cn">Update: select and download the firmware (bin)</span></b>
                                <form method="POST" action="/update" enctype="multipart/form-data">
                                    <div>
                                        <input type="file"  name="update" style="height: 33px;">
                                        <input type="submit" class="save_button" value="download" onclick="this.value='Please wait...';">
                                    </div>
                                </form>
                            </div>
                        </div>
                        <div class="informer">
                            <span class="opt_cn">Если вы ввели что-то не так и не можете изменить его, верните устройство к настройкам по умолчанию. Файл конфигурации будет удален. Устройство перезагрузится!</span>
                        </div>
                    </fieldset>
                </div>
                <div class="unit-footer">
                    <div class="form-buttons">
                        <span class="save_button" onclick="restart(this,'Перезагрузить ESP?');">Перезагрузить устройство</span>
                        <span class="save_button" onclick="resetConfig(this,'Сбросить настройки устройства?');">Сброс настроек по умолчанию</span>
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
