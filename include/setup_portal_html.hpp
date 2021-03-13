#include <Arduino.h>

const char captive_portal[] PROGMEM = R"(<!DOCTYPE HTML>
<html lang="de">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      button{width:11em;height:2.5em}
      body{background: #87cefa; text-align: center;}
    </style>
    <title>Wohoo! Time to set up your l0ggi</title>
  </head>
  <body>
    <h2>Zugangsdaten</h2>
    <form>
      <p>
        <label>SSID:<br>
          <input name="WIFI_SSID" placeholder="SSID" required>
        </label>
      </p>
      <p>
        <label>Passwort:<br>
          <input name="WIFI_PW" placeholder="Password" required>
        </label>
      </p>
    </form>
    <button>
      Save & Setup
    </button>
    <script>
      document.querySelector('button').addEventListener('click', async () =>{
        let elem = document.querySelector('form');
          let resp = await fetch('/store', {method: 'post', body: new FormData(elem)});
          let json = await resp.json();
          document.body.innerHTML = json;
      });
    </script>
  </body>
</html>)";

const char ANSWER_GOT_CREDITS[] PROGMEM = R"("<h3>Credits were sent to your l0ggi. He'll try now to connect to your network..</h3>")";
