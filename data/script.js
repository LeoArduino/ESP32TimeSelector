// ESP32 Time selector v1.0
// 

var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

var sliderNumber;
var sliderValue;
var slidervalue1;
var humSet;

var timer;
var timerNumber;
var timerNumber2;
var timerValue;
var startTijd1;
var stopTijd1; 
var weekJson1; 

var channel1;

var sv1;
var stt1;
var spt1;
var week1;

var sv1;


window.addEventListener('load', onload);
function onload(event) {
    initWebSocket();
}

function getValues(){
    websocket.send("getValues");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    getValues();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function updateSlider(element) {
    sliderNumber = element.id.charAt(element.id.length-1);
    sliderValue = document.getElementById(element.id).value;
   let value;
   if(sliderValue == 0) { value = "OFF" };
   if (sliderValue == 1) { value = "timer"}; 
   if (sliderValue == 2) { value = "ON" };
    document.getElementById("sliderValue"+sliderNumber).innerHTML = value;
    document.getElementById("slider"+sliderNumber).value = sliderValue;
 //   console.log("sliderNumber:"+sliderNumber);
 //   console.log("sliderValue"+sliderValue);
send_data();
}


function TimeSend() {
    document.getElementById("startTijd1").addEventListener("change", function() {
        startTijd1 = this.value;
    send_data();
    });
    document.getElementById("stopTijd1").addEventListener("change", function() {
        stopTijd1 = this.value;
    send_data();
    });    
}


function toggleCheckbox(){ 
  var D1 = document.getElementById("Zo1").checked;  if (D1 == true){ var Zo1 = 1; } else { Zo1 = 0};
  var D2 = document.getElementById("Ma1").checked;  if (D2 == true){ var Ma1 = 1; } else { Ma1 = 0};
  var D3 = document.getElementById("Di1").checked;  if (D3 == true){ var Di1 = 1; } else { Di1 = 0};
  var D4 = document.getElementById("Wo1").checked;  if (D4 == true){ var Wo1 = 1; } else { Wo1 = 0};
  var D5 = document.getElementById("Do1").checked;   if (D5 == true){ var Do1 = 1; } else { Do1 = 0};
  var D6 = document.getElementById("Vr1").checked;   if (D6 == true){ var Vr1 = 1; } else { Vr1 = 0};
  var D7 = document.getElementById("Za1").checked;   if (D7 == true){ var Za1 = 1; } else { Za1 = 0};
    var week1 = [Zo1,Ma1,Di1,Wo1,Do1,Vr1,Za1];
    weekJson1 =JSON.stringify(week1);
 //   console.log(weekJson1);
   send_data();
}

function send_data(){
   const jsonsend = {'sliderNumber' : sliderNumber, 'sliderValue': sliderValue,'startTijd1':startTijd1,'stopTijd1':stopTijd1,'week1': weekJson1 };
console.log(jsonsend);
 const myJSON = JSON.stringify(jsonsend);
 console.log("myJSON", myJSON);
    websocket.send(myJSON);
}

function onMessage(event) {
//console.log("Incomming data",event.data);
let allValues = JSON.parse(event.data);
console.log("allValues JSON Received: ", allValues);
sv1 = allValues.sliderValue1;
stt1 = allValues.startTijd1;
spt1 = allValues.stopTijd1;
week1 = allValues.week1;
channel1 = allValues.channel1;
    let value1;
    if(sv1 == 0) { value1 = "OFF" };
    if (sv1 == 1) { value1 = "timer" };
    if (sv1 == 2) { value1 = "ON" };
    document.getElementById("sliderValue"+1).innerHTML = value1;
    document.getElementById("slider"+1).value= sv1;
    document.getElementById("startTijd1").value = stt1;
    document.getElementById("stopTijd1").value = spt1;

    if (week1[1] == '1'){ document.getElementById("Zo1").checked = true;} else{document.getElementById("Zo1").checked = false; };
    if (week1[3] == '1'){ document.getElementById("Ma1").checked = true;} else{document.getElementById("Ma1").checked = false; };
    if (week1[5] == '1'){ document.getElementById("Di1").checked = true;} else{document.getElementById("Di1").checked = false; };
    if (week1[7] == '1'){ document.getElementById("Wo1").checked = true;} else{document.getElementById("Wo1").checked = false; };
    if (week1[9] == '1'){ document.getElementById("Do1").checked = true;} else{document.getElementById("Do1").checked = false; };
    if (week1[11] == '1'){document.getElementById("Vr1").checked = true;} else{document.getElementById("Vr1").checked = false; };
    if (week1[13] == '1'){document.getElementById("Za1").checked = true;} else{document.getElementById("Za1").checked = false; };
    


// Set the right color for the water drop indicator
    if (channel1 == 0) {document.getElementById("druppel1").style.color = "white";
    }else {document.getElementById("druppel1").style.color = "blue"};
    }




