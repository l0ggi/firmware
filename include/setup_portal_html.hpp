#include <Arduino.h>

const char captive_portal[] PROGMEM = R"(<!DOCTYPE HTML>
<html lang="de">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      button{
          background-color: #4CAF50; /* Green */
          border: none;
          color: white;
          padding: 15px 32px;
          text-align: center;
          text-decoration: none;
          display: inline-block;
          font-size: 16px;
          box-shadow: 0 12px 16px 0 rgba(0,0,0,0.24), 0 17px 50px 0 rgba(0,0,0,0.19);
        }
      body{
        background: #404040; 
        text-align: center;
        }
      h2 {
        color: #D3D3D3;
      }
      input {
          width: 100%;
          max-width: 25vw;
          padding: 12px 20px;
          margin: 8px 0;
          display: inline-block;
          border: 1px solid #ccc;
          border-radius: 4px;
          box-sizing: border-box;
      }
    </style>
    <title>Wohoo! Time to set up your l0ggi</title>
  </head>
  <body>
    <h2>Please enter your WiFi Information</h2>
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
