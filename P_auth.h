const char P_auth[] PROGMEM =  R"=====(
<html lang="ru">
    <head>
        <meta http-equiv = "Content-type" content = "text/html; charset=utf-8">
        <title>Часы информер WiFi_Clock</title>
        <link rel="shortcut icon" href="#" />
        <link rel = "stylesheet" type = "text/css" href = "/style.css">
        <script type = "text/javascript" src = "/function.js"></script>
        <script type = "text/javascript">
            function send_token() {
                let server = "/index.vz?auth=" + val('auth');
                window.location.href = server;
            }
        </script>
    </head>
    <body>
        <header>
            <div class="header-block">
                <span class="opt_cn">Часы информер WiFi_Clock_{{ver}}</span><br>
                <span class="opt_cn">Время на ESP:</span>
            </div>
        </header>
        <main class="login-main">
            <div class="central-unit">
                <div class="unit-body">
                    <div class="login-form-title">Авторизация пользователя</div>
                        <div id="main_unit">
                            <fieldset>
                                <div class="opt_cn">Для доступа к данному устройству необходимо пройти авторизацию.</div>
                                <div class="center" style="padding: 20px 0 20px 0;">
                                    <table class="table-centered">
                                        <tr>
                                            <td align = "right"><span class="opt_cn">Введите токен авторизации</span></td>
                                            <td><input type="text" class="field form-control" id="auth" pattern="[-\w\s#@/]{1,15}" placeholder="authorization token"></td>
                                        </tr>
                                    </table>
                                </div>
                            </fieldset>
                        <span class="save_button" onclick="send_token(this);">Войти</span>
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
