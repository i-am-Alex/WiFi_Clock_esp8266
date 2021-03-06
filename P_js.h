const char P_js[] PROGMEM = R"=====(
let xhr=createXmlHttpObject();

function createXmlHttpObject() {
    let xhr;
    if(window.XMLHttpRequest) {
        xhr=new XMLHttpRequest();
    } else {
        xhr=new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xhr;
}
function load(submit) {
    if(xhr.readyState===0 || xhr.readyState===4){
        submit+=".json";
        let auth = document.location.search;
        if(auth.match(/auth=/)) {
            submit+=auth;
        }
        xhr.open('GET',submit,true);
        xhr.send(null);
        xhr.onload=function() {
            jsonResponse=JSON.parse(xhr.responseText);
            loadBlock(xhr.onload);
        }
    }
}
function loadBlock(){
    let data2 = JSON.parse(xhr.responseText);
    let data = document.getElementsByTagName('body')[0].innerHTML;
    let new_string;
    for (let key in data2){
        new_string=data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
        data=new_string;
    }
    document.getElementsByTagName('body')[0].innerHTML=new_string;
    let inputs=document.getElementsByTagName("input");
    let selects=document.getElementsByTagName("select");
    for (let key in data2) {
        if(data2[key]=='checked'){
            for (let i=0; i<inputs.length; i++) {
                if (inputs[i].id===key) {
                    inputs[i].checked="true";
                }
            }
        }
        for (let i=0; i<selects.length; i++) {
          if (selects[i].id===key) {
            document.getElementById(key).value=data2[key];
          }
        }
    }
    handleServerResponse();
}
function val(id) {
    return document.getElementById(id).value;
}
function val_sw(nameSwitch) {
    let switchOn = document.getElementById(nameSwitch);
    return (switchOn.checked) ? 1 : 0;
}
function send_request(submit, server) {
    server = convert(server);
    let request = new XMLHttpRequest();
    request.open("GET", server, true);
    request.send();
    save_status(submit, request);
}
function save_status(submit, request) {
    let old_submit = submit.value;
    request.onreadystatechange = function() {
        if(request.readyState!=4) {
            return;
        }
        submit.value = request.responseText;
        setTimeout(function() {
            submit.value=old_submit;
            submit_disabled(false);
        }, 1000);
    }
    submit.value='??????????????????...';
    submit_disabled(true);
}
function submit_disabled(request){
  let inputs = document.getElementsByTagName("input");
  for (let i = 0; i < inputs.length; i++) {
    if (inputs[i].type === 'submit') {
        inputs[i].disabled = request;
    }
  }
}
function toggle(target) {
  let curVal = document.getElementById(target).className;
  document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}
var set_real_time;
function handleServerResponse(){
    clearTimeout(set_real_time);
    let res = jsonResponse.time.split(":");
    real_time(hours=res[0],min=res[1],sec=res[2]);
    document.body.style.backgroundColor="rgb("+jsonResponse.rgb+")";
}
function real_time(hours, min, sec) {
    sec = Number(sec)+1;
    if (sec>=60) {
        min=Number(min)+1;sec=0;
    }
    if (min>=60) {
        hours=Number(hours)+1;
        min=0;
    }
    if (hours>=24) {
        hours=0
    };
    document.getElementById("time").innerHTML = hours+":"+(min<10?"0":"")+min+":"+(sec<10?"0":"")+sec;
    set_real_time = setTimeout("real_time("+hours+","+min+","+sec+");", 1000);
}
function load_time(submit) {
    server = "/Time";
    send_request(submit,server);
    load('/configs.json');
}
function get_html(submit) {
    let auth = document.location.search;
    if(auth.match(/auth=/)) submit += auth;
    window.location.href = submit;
}
function convert(submit) {
    let auth = document.location.search;
    if(auth.match(/auth=/)) {
        submit+=auth.replace("\?", "&");
    }
    return (submit);
}
)=====";
