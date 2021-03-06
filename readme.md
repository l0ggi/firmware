# l0ggi firmware
based on a prior firmware developed for a doughbox with similar hardware (uses a l0ggi + shield)

## Basic concept
l0ggi has a built in webserver and allows clients to request the measured data. l0ggi is currently not optimized in any way. as the current pcb approach does not include a battery for supply, also the power draw can be high. 

### dependencies
- AsyncTCP-esphome
- ESPAsyncTCP-esphome
- ESPAsyncWebServer-esphome
- SimpleDHT
- U8g2
- U8glib-HAL
- arduino
### folder structure
``data\`` htm(l) files delivered from internal webserver

``include\`` headerfiles for source. includes the config.h files

``lib\`` not used in this project

``src\`` c/cpp source

``test\`` not used in this project

### configuration
l0ggi is programmed with the main goal of one firmware to fit all, therefore all 'end-user' config can be done in the captive client after booting the l0ggi for the first time

Anyways the firmware has config files:

``config.h``  primary config file for l0ggi firmware, comments in file itself

``webserver_config.h`` config file for just the webserver. WiFi AP settings are here, like IP address, wifi name, wifi pass (note, these are the settings for the setupserver, the production-server can be set up by just connecting to the setupserver and insert the details.)

``constants.h`` defines the structs used in this project

note, all ``.h`` files are settings-files, the ``.hpp`` headers are used as headers for function files

### state machines
l0ggi knows different state machines. 

``main state machine`` the main loop controlls the time machine (pretty basic, it measures the runtime by adding the runtime to a time struct. works really well somehow) and calls the webserver_loop with the currently measured values. 
The time machine exectues subroutines at a 1Hz clock rate:
- Read temp sensor data,
- Display Update

(1s for each temperature measurement is pretty fast as this device should measure for multiple years) 

``web server state machine`` obviously the webserver has its own state machine, so a restful asyncronus interface is possible. the webserver switches between the configuration mode and the default mode.
In configuration mode, it serves a static page and a dns server catching all requests and sending them to the l0ggi itself, so the connected devices is forced to open the config page
Also a few RESTful endpoints are active to allow to post the configuration details

In production mode the webserver knows 
- a RESTful interface
- a Websocket 
- an Eventhandler

Documentation for restful, websocket and eventhandler following as soon as finished.