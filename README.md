# WS2812WemosWeb

WS2812 strip controller based on Wemos D1 mini (ESP8266EX)

### Requirements
[FastLED](https://github.com/FastLED/FastLED) library (3.2.0) \
[arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets) library (2.2.0)

### Usage
1. Install ESP core: [link to copy and paste in IDE settings](http://arduino.esp8266.com/stable/package_esp8266com_index.json)
2. Install libraries
3. Select LOLIN(WEMOS) D1 R2 & mini in board manager
4. Open `WS2812WemosWeb.ino` and write your WiFi ssid and password (you can add more than 1 WiFi AP)
5. Upload the firmware
6. Open serial monitor and select 74880 Baud rate
7. Copy esp8266 local IP to your browser (or use mDNS: http://esp8266.local)

### More
You can find more led effects [here](https://github.com/AlexGyver/WS2812_FX/blob/master/%D0%BF%D1%80%D0%BE%D1%88%D0%B8%D0%B2%D0%BA%D0%B8/WS2812_FX_rndChange_light_fixed/LED_EFFECT_FUNCTIONS.ino)

Add buttons in UI, read comments in `index.html`
If you want to visualize IP on the strip, modify `showIP()` function in `utils.ino`
