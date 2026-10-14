#pragma once

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>

<html>
  <head>
    <title>RC Car Controller</title>
    <link id="favicon" rel="icon" href="favicon.ico">

    <style>
      body {
        margin: 0;
        height: 100%;
        overflow: hidden;
        background: #111;
        color: white;
        font-family: Arial;
        text-align: center;
      }

      footer {
        text-align: center;
        bottom: 0;
        left: 0;
        width: 100%;
        position: fixed;
      }

      #connectBtn {
        margin: 20px;
      }

      .controls {
        display: flex;
        justify-content: center;
        align-items: center;
        gap: 20px;
        margin: 40px auto;
        width: fit-content;
      }

      #joystick {
        position: relative;
        width: 300px;
        height: 300px;
        margin: 0;
        background: #222;
        border-radius: 50%;
        overflow: hidden;
      }

      #settingsPanel {
        height: 100%; 
        width: 0; 
        position: fixed; 
        z-index: 1;
        top: 0;
        left: 0;
        background-color: #111; 
        overflow-x: hidden; 
        padding-top: 60px; 
        transition: 0.5s; 
      }

      #settingsPanel #closebtn {
        position: absolute;
        top: 0;
        right: 25px;
        font-size: 36px;
        margin-left: 50px;
      }

      #openbtn {
        position: absolute;
        left: 0px;
        top: 0px;
        margin: 20px;
        font-size: 56px;
      }

      .allbuttons {
        margin: 20px;
        padding: 15px 30px;
        font-size: 18px;
        border-radius: 10px;
        border: none;
      }

      .allbuttons:active {
        transform: translateY(4px);
      }

      #deleteUUIDsBTN {
        font-size: 56px;
      }

      #iosWarn {
        position: fixed;
        top: 0; left: 0;
        width: 100%;
        height: 100%;
        background: rgba(240, 33, 33, 0.575);
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        z-index: 9999;
      }
      
      #iosWarn-content {
        padding: 20px;
        border-radius: 8px;
        text-align: center;
      }
      
      .hidden {
        display: none !important;
      }

      .switch {
        position: relative;
        display: inline-block;
        width: 60px;
        height: 34px;
      }

      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        -webkit-transition: .4s;
        transition: .4s;
      }

      .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        -webkit-transition: .4s;
        transition: .4s;
      }

      input:checked + .slider {
        background-color: #2196F3;
      }

      input:checked + .slider:before {
        -webkit-transform: translateX(26px);
        -ms-transform: translateX(26px);
        transform: translateX(26px);
      }

      .slider.round {
        border-radius: 34px;
      }

      .slider.round:before {
        border-radius: 50%;
      }

      #needle {
        transform-origin: bottom center;
        transform: rotate(var(--angle));
        clip-path: polygon(50% 0, 45% 100%, 55% 100%);
        width: 20px;
        height: 200px;
        background: aliceblue;
        position: absolute;
        top: 40%;
        scale: 2;
      }

      @media (max-width: 1900px) and (orientation: landscape) {
        body {
          touch-action: none;
          overflow: hidden;
          scroll-behavior: none;
        }

        footer {
          visibility: hidden;
          display: none;
          font-size: 0;
        }
      }
    </style>
  </head>

  <body>
    <div id="settingsPanel">
      <a class="allbuttons" id="closebtn" onclick="closeNav()">&times;</a>
      
      <button class="allbuttons" id="connectBtn" style="margin: 5px" onclick="connect()">Connect</button>
      <div id="status">Disconnected</div>
      <div id="errorLbl" onclick="this.innerText=null"></div>

      <br><br>

      <a class="hidden">
        <div>Enable gyroscope</div>
        <label class="switch">
          <input type="checkbox" onchange="enableOrientationEvent(this)">
          <span class="slider round"></span>
        </label>
      
        <br>
        <div id="gyroA">A</div>
        <div id="gyroB">B</div>
        <div id="gyroG">G</div>
      </a>


      <div style="position: absolute; bottom: 75px; left: 50%" id="VersionText"></div>
    </div>
    
    <a class="allbuttons" id="openbtn" onclick="openNav()">&#9881;</a>

    <div id="jsControls" class="controls">
      <div id="joystick"></div>
    </div>

    <div id="gyroControls" class="controls hidden">
      <button class="allbuttons" onclick="referenceGot = false">Zero gyro</button>
      <button class="allbuttons hidden" onclick="gyroStart = !gyroStart">Start / Stop</button>

      <div id="needle" class="hidden"></div>
    </div>


    <script src="joystick.js"></script>
    <script>
      let siteVersion = 3;

      let showOutput = false;

      let ws;

      const statusText = document.getElementById("status");


      // gyro varis
      let gyroStart = true;
      let referenceGot = false;
      let lastestA = 0;
      let lastestB = 0;
      let lastestG = 0;
      let refA = 0;
      let refB = 0;
      let refG = 0;


			////////////////////////////////////////////////////////////
			// Websocket event handlers

			function wsCloseEvent(event) {
				statusText.innerText = "Disconnected";
			}

			function wsErrorEvent(event) {
				console.warn(event.data);
			}

			function wsMessageEvent(event) {
				console.log(event.data);
			}

			function wsOpenEvent(event) {
				statusText.innerText = "Connected";
			}

			////////////////////////////////////////////////////////////



      function connect() {
				if (!ws) {
        	ws = new WebSocket("ws://esp32.local:8080");
					ws.addEventListener("close", wsCloseEvent);
					ws.addEventListener("error", wsErrorEvent);
					ws.addEventListener("message", wsMessageEvent);
					ws.addEventListener("open", wsOpenEvent);
				}
				else {
					ws.close();
          ws = null;
				}
      };

      function openNav() {
        document.getElementById("settingsPanel").style.width = "350px";
      }

      function closeNav() {
        document.getElementById("settingsPanel").style.width = "0";
      }

      function getReferences() {
        refA = lastestA;
        refB = lastestB;
        refG = lastestG;
        referenceGot = true;
      }

      function hangleOrientationChange(event) {
        if (event.alpha == null || event.beta == null || event.gamma == null) { return; }
        lastestA = event.alpha;
        lastestB = event.beta;
        lastestG = event.gamma;

        if (!referenceGot) { getReferences(); }
        if (gyroStart) { gyroSend(); }

        document.getElementById("gyroA").innerText = lastestA - refA;
        document.getElementById("gyroB").innerText = lastestB - refB;
        document.getElementById("gyroG").innerText = lastestG - refG;
      }
      
      async function enableOrientationEvent(state) {
        (state.checked ? document.getElementById("gyroControls") : document.getElementById("jsControls")).classList.remove("hidden");
        (!state.checked ? document.getElementById("gyroControls") : document.getElementById("jsControls")).classList.add("hidden");

        if (state.checked) {
          if (typeof DeviceOrientationEvent.requestPermission === 'function') {
            // Handle iOS devices.
            const permission = await DeviceOrientationEvent.requestPermission();
            if (permission === "granted") {
              window.addEventListener("deviceorientation", hangleOrientationChange);
            }
          } else {
            // Handle regular non iOS devices.
            window.addEventListener("deviceorientation", hangleOrientationChange);
          }
        }
        else {
          window.removeEventListener("deviceorientation", hangleOrientationChange);
        }
      }

      window.onload = () => {
        //enableOrientationEvent();
        //screen.orientation.lock("landscape");
      
        // Version number
        document.getElementById("VersionText").innerText = "v" + siteVersion;
      }

      const manager = nipplejs.create({
        zone: document.getElementById('joystick'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'white',
        size: 200
      });

      let sending = false;

      function gyroSend() {
        send("gyro," + (lastestA - refA) + "," + (lastestB - refB) + "," + (lastestG - refG));
        return;
      }

      function jsSend(x, y) {
        send("js," + Math.round((x + 1) * 127.5).toString() + "," + (255-Math.round((y + 1) * 127.5)).toString());
        return;
      }

      function send(dataToSend) {
				if (showOutput) { console.log(dataToSend); }
        if (ws && ws.readyState === WebSocket.OPEN) {
          ws.send(dataToSend);
        }
      }

      manager.on('move', function (evt, data) {
        function applyDeadzone(v) {
          return Math.abs(v) < 0.1 ? 0 : v;
        }
        
        if (!data.vector) return;
      
        let x = applyDeadzone(data.vector.x);
        let y = applyDeadzone(-data.vector.y);

        jsSend(x,y);
      });

      manager.on('end', function () {
        jsSend(0, 0);
      });
    

      const keys = new Set();

      document.addEventListener('keydown', (e) => {
        const key = e.key.toLowerCase();

        if (!['w', 'a', 's', 'd'].includes(key)) return;

        if (!keys.has(key)) {
          keys.add(key);
          updateKeyboardJoystick();
        }
      });

      document.addEventListener('keyup', (e) => {
        const key = e.key.toLowerCase();

        if (!['w', 'a', 's', 'd'].includes(key)) return;

        if (keys.has(key)) {
          keys.delete(key);
          updateKeyboardJoystick();
        }
      });

      function updateKeyboardJoystick() {
        let x = 0;
        let y = 0;

        if (keys.has('a')) x--;
        if (keys.has('d')) x++;
        if (keys.has('w')) y--;
        if (keys.has('s')) y++;

        const length = Math.hypot(x, y);

        if (length) {
          x /= length;
          y /= length;
        }
        
        // 100 = maximum travel for your 200px joystick
        manager[0].setPosition(null, {
          x: x * 100,
          y: y * 100
        });

        jsSend(x, y);
      }
    </script>
  </body>
</html>
)rawliteral";