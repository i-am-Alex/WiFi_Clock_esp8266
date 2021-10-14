const char P_css[] PROGMEM = R"=====(
@charset "utf-8";
html {
    height: 100%;
    width: 100%;
}
body {
    color: #4e4e4e;
    background: #e8e8e8;
    height: 100%;
    width: 100%;
    font-size: 10pt;
    font-family: 'Roboto', sans-serif;
    margin: 0 auto;
}
table {
    font-size: 10pt;
      font-weight: 450;
}
header {
    height: 54px;
    margin-bottom: 22px;
}
.header-block {
    padding-top: 6px;
    position: fixed;
    font-size: 11pt;
    font-weight: bold;
    text-align: center;
    line-height: 150%;
    height: 54px;
    border-bottom: 1px solid #b3b3b3;
    width: 100%;
    background-color: #f4f4f4;
    -webkit-box-shadow: 0 5px 15px rgba(0, 0, 0, 0.16);
    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.16);
}
.central-unit {
    margin-left: auto;
    margin-right: auto;
    width: 500px;
    max-width: 100%;
    display: block;
    box-shadow: 3px 8px 15px rgba(0, 0, 0, 0.4);
    border: 1px solid #d9d9d9;
}
.main-menu {
    background: #eeeeee;
    font-weight: 500;
    font-size: 0;
    display: block;
    width: 100%;
}
.unit-body {
    background-color: white;
    padding: 10px;
    margin-left: auto;
    margin-right: auto;
    max-width: 100%;
    display: block;
}
.unit-footer {
    display: block;
    background: #f8f8f8;
}
footer {
    height: 41px;
}
.informer {
    background-color: #f5f5f5;
    margin: 5px;
    padding: 1%;
    border: 1px solid #d9d9d9;
    border-radius: 2px;
    font-style: italic;
    text-align: justify;
    font-size: 0.8em;
    display: block;
}
fieldset {
    border: 1px solid #d9d9d9;
    border-radius: 4px;
    margin-top: 5px;
    margin-bottom: 5px;
    padding: 5px 5px 10px 5px;
    background-color: #f5f5f5;
}
fieldset legend {
    font-weight: 600;
    text-align: left;
    padding-left: 4px;
    padding-right: 5px;
    margin-left: 6px;
}
#main_unit {
    text-align: center;
}
.ico-text {
    font-size: 10pt;
    color: #434343;
    cursor: pointer;
    background-color: #eeeeee;
    transition: 0.5s linear;
    padding: 6px 5px 6px 8px;
    border-bottom: 1px solid #d9d9d9;
    display: table-cell;
}
.ico-text:hover {
    background-color: white;
}
.ico-text-active {
    background-color: white;
    border-bottom: 1px solid white;
}
.ico-text-right {
    display: table-cell;
    width: inherit;
    font-size: 10pt;
    color: #434343;
    background-color: #eeeeee;
    border-bottom: 1px solid #d9d9d9;
}

.switch {
    font-weight: 450;
    display: block;
    margin: 6px 0 6px 0;
  }
.opt_cn {
    font-weight: 450;
}
.table {
    border-collapse: collapse;
    margin-left: auto;
    margin-right: auto;
}
.table-centered {
    font-weight: 450;
    margin-left: auto;
    margin-right: auto;
}
.table-centered > * > tr > td {
    text-align: left;
}
.left {
    text-align: left !important;
}
.right {
    text-align: right !important;
}
.center {
    text-align: center !important;
}
.table > * > tr > td  {
    border: 1px solid #a2a9b1;
    padding: 0.5em 0.65em;
    text-align: left;
}
.table > * > tr > th  {
    border: 1px solid #a2a9b1;
    padding: 0.5em 0.65em;
    text-align: center;
}
input[type=text].field, input[type=password].field {
    padding: 3px;
    margin: 1px;
    border: 1px solid #575757;
    border-radius: 2px;
}
input:focus[type=text].field{
    border: 1px solid #03d703
}
input:invalid[type=text] {
    border:1px solid red !important;
}
.save_button {
    background-color: rgb(221 239 255);
    padding: 6px;
    border-radius: 3px;
    border: 1px solid #c6c6c6;
    font-weight: bold;
    line-height: 100%;
    cursor: pointer;
    display: inline-block;
    z-index: 1005;
}
.save_button:hover {
    box-shadow: 0 0 10px 2px rgba(0,0,0,0.3);
}
.form-buttons {
    text-align: right;
    padding: 12px;
}
.center {
    text-align:center;
}
.link__img{
    text-decoration: none;
}
.footer-band {
    margin: auto;
    position: fixed;
    left: 0;
    bottom: 0;
    background: #b6b6b6;
    width: 100%;
    text-align: center;
    padding: 5px 0 5px 0;
}
.footer-band-labels {
    width: 500px;
    margin-left: auto;
    margin-right: auto;
}
.footer-left {
    float: left;
    padding-right: 15px;
}
.footer-right {
    float: right;
    padding-left: 15px;
}
.login-main {
    height: 75%;
    display: flex;
    align-items: center;
    justify-content: center;
}
.login-form-title {
    text-align: center;
    font-size: 16pt;
    font-weight: 450;
    padding: 10px 0 10px 0;
}
::selection {
    background: green; color: #fff
}
::-moz-selection {
    background: green; color: #fff
}
select {
    height: 22px;
    padding: 0 5px 0;
    border-radius: 2px;
    border: 1px solid #909090;
}
a {
    color: blue;
}
a:visited {
    color: blue;
}
a:hover {
    color: #1b63ff;
}
.board-container {
    display: flex;
    justify-content: space-between;
    flex-wrap: wrap;
}
.board-block-full {
    width: 100%;
}
.board-block-half {
    width: 50%;
}
.board-block {
    box-sizing: border-box;
    display: flex;
    justify-content: flex-start;
    padding: 5px;
}
.board-block-form {
    border: 1px solid #eaeaea;
    border-radius: 5px;
    background-color: #fbfbfb;
    font-size: 26pt;
    color: #2c7087;
    font-weight: 500;
    width: 100%;
}
.main-time-font {
    font-size: 65pt;
    text-align: center;
    font-weight: 600;
}
.board-block-title {
    padding: 6px 0 3px 10px;
    font-weight: 450;
    font-size: 11pt;
    color: #4f4f4f;
}
.board-block-text {
    font-weight: 400;
    font-size: 10pt;
    color: #3f3f3f;
}
.board-block-data {
    color: #2c7087;
    font-weight: 600;
    font-size: 37pt;
    text-align: right;
    padding: 6px 17px 6px 0;
}
)=====";
