void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            // convertong payload to string
            String sPayload = "";
            int i = 0;
            while( payload[i] != NULL){
              sPayload += char(payload[i]);
              i++;
            }
            // Serial.println(sPayload);
            // Serial.printf("Payload: %s\n", payload);

            // converting info from colorpicker
            if(payload[0] == '#') {
              uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
              int cRed = ((rgb >> 16) & 0xFF);
              int cGreen = ((rgb >> 8) & 0xFF);
              int cBlue = ((rgb >> 0) & 0xFF);
              // convert values to HSV
              rgb2hsv(cRed, cGreen, cBlue);
              // save values in RGB
              rgbColors[0] = cRed;
              rgbColors[1] = cGreen;
              rgbColors[2] = cBlue;
            }

            // converting info from sliders
            else if(payload[0] == 'T'){
              globalDelay = (int) strtol((const char *) &payload[1], NULL, 10);
            }
            else if(payload[0] == 'S'){
              globalScale = (int) strtol((const char *) &payload[1], NULL, 10);
            }

            // some fx needs resetting values
            else if(sPayload == "allblack"){
              one_color_all(0, 0, 0);
              currentMode = sPayload;
            }
            else if(sPayload == "running"){
              currentMode = sPayload;
              runningPosition = 0;
            }
            else if(sPayload == "rainbowcycle"){
              currentMode = sPayload;
              rainbowIndex = 0;
            }
            else if(sPayload == "colorbouncefade"){
              idex = 0;
              currentMode = sPayload;
            }
            else {
              currentMode = sPayload;
            }
            break;
    }

}
