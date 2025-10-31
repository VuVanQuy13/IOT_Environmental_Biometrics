var gateway = `ws://${window.location.hostname}/ws`;
var webSocket;
window.addEventListener('load' , onload);

function onload() {
    initWebSocket();
}

function initWebSocket() {
    console.log("Trying to open WebSocket connection ...");
    webSocket = new WebSocket(gateway);
    webSocket.onopen = onOpen;
    webSocket.onclose = onClose;
    webSocket.onmessage = onMessage;
}

function onOpen() {
    webSocket.send("getValueFirts");
    console.log("Connection Opened");
}

function onClose() {
    console.log("Connection Closed !");
    setTimeout(initWebSocket , 2000);
}

function onMessage(event) {
    let sensorRes = JSON.parse(event.data);

    // Render meansure Web 
    if(sensorRes.temperature !== undefined) {
        console.log("Nhiet do: ", sensorRes.temperature);
        console.log("Do Am: ", sensorRes.humidity);
        console.log("Do Am Dat: ", sensorRes.soil);
        console.log("Anh Sang: ", sensorRes.light);
        console.log("Do Bui: ", sensorRes.dust);
        console.log("Co2: ", sensorRes.CO2);
        console.log("Nh3: ", sensorRes.NH3);
        console.log("No: ", sensorRes.NO);

        document.getElementById("temperature").innerHTML = sensorRes.temperature;
        document.getElementById("humidity").innerHTML = sensorRes.humidity;
        document.getElementById("soil_moisture").innerHTML = sensorRes.soil;
        document.getElementById("light").innerHTML = sensorRes.light;
        document.getElementById("dust").innerHTML = sensorRes.dust;
        document.getElementById("co2").innerHTML = sensorRes.CO2;
        document.getElementById("nh3").innerHTML = sensorRes.NH3;
        document.getElementById("no").innerHTML = sensorRes.NO;

    }

    // Thresold
    if(sensorRes.sensor1 !== undefined) {
        document.getElementById("sensor1").innerHTML = sensorRes.sensor1;
        document.getElementById("slider1").value = sensorRes.sensor1;

        document.getElementById("sensor2").innerHTML = sensorRes.sensor2;
        document.getElementById("slider2").value = sensorRes.sensor2;

        document.getElementById("sensor3").innerHTML = sensorRes.sensor3;
        document.getElementById("slider3").value = sensorRes.sensor3;
        
        document.getElementById("sensor4").innerHTML = sensorRes.sensor4;
        document.getElementById("slider4").value = sensorRes.sensor4;
        
        document.getElementById("sensor5").innerHTML = sensorRes.sensor5;
        document.getElementById("slider5").value = sensorRes.sensor5;
        
        document.getElementById("sensor6").innerHTML = sensorRes.sensor6;
        document.getElementById("slider6").value = sensorRes.sensor6;
        
        document.getElementById("sensor7").innerHTML = sensorRes.sensor7;
        document.getElementById("slider7").value = sensorRes.sensor7;
        
        document.getElementById("sensor8").innerHTML = sensorRes.sensor8;
        document.getElementById("slider8").value = sensorRes.sensor8;
    }

}

function handleChangeSlider(element) {
    const sliderNum = element.id.charAt(element.id.length - 1);
    const sliderValue = document.getElementById(element.id).value;

    document.getElementById("sensor" + sliderNum).innerHTML = sliderValue;
    webSocket.send(sliderNum + "s" + sliderValue)
}