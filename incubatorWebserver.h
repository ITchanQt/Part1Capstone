const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        * {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Poppins', sans-serif;
    }
    .logo {
    padding: 20px;
    font-size: 25px;
    color: #0C0C0C;
    text-decoration: none;
    font-weight: 600;
}

    .timeBox {
    padding: -15px;
    position: absolute;
    display: flex;
    justify-content: space-around;
    align-items: center;
    width: 250px;
    font-size: 10px;
    color: #0C0C0C;
    margin-left: 4rem;
    margin-bottom: 10rem;
    }
    #btnShowData {
        position: absolute;
        border: none;
        background: #059212;
        font-size: 12px;
        margin-left: 19rem;
        margin-bottom: 20rem;
        border-radius: 5px;
        padding: 1px;
        cursor: pointer;
        height: 2rem;
        width: 70px;
    }
    
    body {
        background: #f9ffbe;
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
    }

.modal {
            display: none;
            position: fixed;
            z-index: 1;
            padding: 100px;
            left: 0;
            right: 0;
            top: 0;
            bottom: 0;
            overflow: auto;
            background-color: rgb(0,0,0);
            background-color: rgba(0,0,0,0.4);
        }
        .modal-content {
            background-color: #fefefe;
            margin: 15% 5% 10% 5%;
            padding: 20px;
            border: 1px solid #888;
        }
        .close {
            color: #aaa;
            float: right;
            font-size: 28px;
            font-weight: bold;
        }
        .close:hover,
        .close:focus {
            color: black;
            text-decoration: none;
            cursor: pointer;
        }

        .modalHeader {
          margin-right: auto;
          margin-left: auto;
        }

        table {
            margin-right: auto;
            margin-left: auto;
            width: 80%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
.btnContainer{
    margin-top: 2rem;
}

.btnBox {

        border: #9BEC00 solid 1px;
        background: #F3FF90;
        margin: 1rem auto;
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: space-evenly;
        width: 25rem;
        height: 23rem;
        padding: 4rem;
        box-shadow: 5px 5px rgba(0, 0, 0, 0.1);
        border-radius: 5px;
      }
      .btnBox p {
        font-size: 2.5rem;
        color: #0C0C0C;
        margin: 0;
      }
      .btnBox button,
      .btnBox .onandoffTurner button {
        font-size: 15px;
        height: 2.3rem;
        width: 14rem;
        margin-top: 0.5rem;
        background-color: #059212;
        color: #fff;
        border-radius: 3px;
        cursor: pointer;
        border: none;
      }
      .btnBox  button:hover,
      .btnBox .onandoffTurner button:hover {
        background-color: #9BEC00;
        color: #0C0C0C;
      }

      .btnContainer h4 {
        padding-bottom: 10px;
      }

      .TandH_Readings {
        margin: auto;
        width: 20rem;
        height: 12rem;
        border: #9BEC00 solid 1px;
        background: #F3FF90;
        display: flex;
        box-shadow: 5px 5px rgba(0, 0, 0, 0.1);
        border-radius: 5px;
        justify-content: center;
        flex-direction: column;
      }

      .TandH_Readings .waterLevel, .temp,
      .humi { 
        padding: 9px;
      }

      .dht-labels {
        font-size: 15px;
      }
      
      @media screen and (max-width: 1200px) {

      }

      @media screen and (max-width: 600px) {
        .modal {
            display: none;
            position: fixed;
            z-index: 1;
            padding: 25px;
            left: 0;
            right: 0;
            top: 0;
            bottom: 0;
            overflow: auto;
            background-color: rgb(0,0,0);
            background-color: rgba(0,0,0,0.4);
        }
      }

      .notification {
        position: fixed;
        top: 10px;
        right: 10px;
        padding: 10px;
        color: #fff;
        border-radius: 5px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.3);
        z-index: 1000;
      }
      .notification.success {
        background-color: #4caf50;
      }
      .notification.warning {
        background-color: #f44336;
      }
          </style>
</head>
<body>
        <a href="" class="logo"><h2>IOT Egg Incubator</h2></a>


    <!-- The Modal -->
    <div id="myModal" class="modal">
      <div class="modal-content">
          <span class="close" onclick="closeModal()">&times;</span>
          <h2 class="modalHeader">Daily Logs</h2>
          <table>
              <thead>
              <tr>
                  <th>Files</th>
                  <th>Actions</th>
              </tr>
              </thead>
              <tr >
                <td>Files</td>
                <td><button id="download">download</button></td>
              </tr>
          </table>
      </div>
  </div>
  
  <div class="btnContainer">
    <div class="btnBox" id="btnBox">
        <button onclick="openModal()" type="button" id="btnShowData" class="btnShowData">Show Data</button>
    <h4>Controls Temperature and Turner</h4>
      <p id="setTemp">%SETTEMP%</p>
        <button type="button" id="AddTemp">+</button>
        <button type="button" id="SubtTemp">-</button>
      <div class="onandoffTurner">
        <button type="button" id="btn_turner">Turner is  </button>
      </div>
    </div>
  </div>

  <div class="TandH_Readings">
    <div class="timeBox">
      <p>
          <span id="days">%DAYS%</span>
      </p>
      <span>/</span>
      <p>
          <span id="hours">%HOURS%</span>
      </p>
      <span>:</span>
      <p>
          <span id="minutes">%MINUTES%</span>
      </p>
      <span>:</span>
      <p>
          <span id="seconds">%SECONDS%</span>
      </p>
    </div>
    <div class="waterLevel">
        <h4 class="dht-labels">Water Level:</h4>
        <p id="waterLevel">%WATERLEVEL%</p>
    </div>
    <div class="temp">
      <h4 class="dht-labels">Temperature:</h4> 
      <p id="temperature">%TEMPERATURE%<span><sup class="units">&deg;C</sup></span></p>
    </div>
    <div class="humi">
      <h4 class="dht-labels">Humidity:</h4>
    <p id="humidity">%HUMIDITY%<span><sup class="units">%</sup></span></p>
    </div>
  </div>

<script>
function openModal() {
  document.getElementById("myModal").style.display = "block";
}

function closeModal() {
    document.getElementById("myModal").style.display = "none";
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("waterLevel").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/waterLevel", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("days").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/days", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hours").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/hours", true);
  xhttp.send();
}, 1000) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("minutes").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/minutes", true);
  xhttp.send();
}, 1000) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("seconds").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/seconds", true);
  xhttp.send();
}, 1000) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("btn_turner").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/btn_turner", true);
  xhttp.send();
}, 1000) ;

// New function to update setTemp value from server
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("setTemp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/setTemp", true);
  xhttp.send();
}, 1000);

function updateSetTemp(value) {
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "/setTemp?value=" + value, true);
  xhttp.send();
}

document.getElementById("AddTemp").addEventListener("click", function() {
  var currentTemp = parseInt(document.getElementById("setTemp").innerText);
  currentTemp++;
  document.getElementById("setTemp").innerText = currentTemp;
  updateSetTemp(currentTemp);
});

document.getElementById("SubtTemp").addEventListener("click", function() {
  var currentTemp = parseInt(document.getElementById("setTemp").innerText);
  currentTemp--;
  document.getElementById("setTemp").innerText = currentTemp;
  updateSetTemp(currentTemp);
});

      document.getElementById("btn_turner").addEventListener("click", function() {
        var btn = document.getElementById("btn_turner");
        var currentText = btn.innerText;
        var newText, newStat;

        if (currentText === "Turner is OFF") {
          newText = "Turner is ON";
          newStat = 1;
          showNotification("Turner is now ON", "success");
        } else {
          newText = "Turner is OFF";
          newStat = 0;
          showNotification("Turner is now OFF", "warning");
        }

        btn.innerText = newText;
        updateTurnerStat(newStat);
      });

      function updateTurnerStat(stat) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/setTurnerStat?value=" + stat, true);
        xhttp.send();
      }

      function showNotification(message, type) {
        var notification = document.createElement("div");
        notification.className = "notification " + type;
        notification.innerText = message;
        document.body.appendChild(notification);

        setTimeout(function() {
          notification.remove();
        }, 3000);
      }

</script>
</body>
</html>)rawliteral";
