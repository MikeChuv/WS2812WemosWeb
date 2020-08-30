// show local esp IP address on a strip
void showIP(IPAddress espIP, int timeout){
  int ip1 = espIP[3] % 10;
  int ip0 = espIP[3] / 10;
  for (int i = 0; i < ip0; i++){
    leds[i].setRGB(200, 100, 100);
  }
  for (int i = ip0 + 2; i < (ip0 + 2 + ip1) ; i++){
    leds[i].setRGB(200, 100, 100);
  }
  LEDS.show();
  delay(timeout * 1000);
  one_color_all(0, 0, 0);
}

// map float values for adapting effect scale
float map_float(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// Convert RGB to HSV
void rgb2hsv(int cRed, int cGreen, int cBlue){
  // Serial.printf("RGB: %d %d %d\n", cRed, cGreen, cBlue);
  float fRed = (float)cRed / 255;
  float fGreen = (float)cGreen / 255;
  float fBlue = (float)cBlue / 255;
  float cmax = max(fGreen, fBlue);
  cmax = max(cmax, fRed);
  float cmin = min(fGreen, fBlue);
  cmin = min(cmin, fRed);
  float delta = cmax - cmin;
  if (delta == 0){
    cHue = 0;
  }
  else if((cmax == fRed) && (fGreen >= fBlue)){
    cHue = round(42.5 * (fGreen - fBlue) / delta);
  }
  else if((cmax == fRed) && (fGreen < fBlue)){
    cHue = round(42.5 * (((fGreen - fBlue) / delta) + 6));
  }
  else if(cmax == fGreen){
    cHue = round(42.5 * (((fBlue - fRed) / delta) + 2));
  }
  else if(cmax == fBlue){
    cHue = round(42.5 * (((fRed - fGreen) / delta) + 4));
  }
  if(cmax == 0){
    cSat = 0;
  }
  else{
    cSat = round(255 * delta / cmax);
  }
  cVal = round(255 * cmax);
  // Serial.printf("HSV: %d %d %d\n", cHue, cSat, cVal);
}
