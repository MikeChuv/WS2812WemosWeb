/*
LED effects functions
more at
https://github.com/AlexGyver/WS2812_FX
*/


void one_color_all(int cred, int cgrn, int cblu) {       //-SET ALL LEDS TO ONE COLOR
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].setRGB( cred, cgrn, cblu);
  }
  LEDS.show();
}




void RunningLights(byte red, byte green, byte blue, int WaveDelay, float Position) {
    for (int i = 0; i < LED_COUNT; i++) {
      // sine wave, 3 offset waves make a "rainbow"!
      //(sin(i + Position) * offset1 + offset2) / 255
      // default offset1 = 127
      // default offset2 = 128
      // Position is between 0 and TWO_PI
      setPixel(i, ((sin(i + Position) * 127 + 128) / 255)*red,
                  ((sin(i + Position) * 127 + 128) / 255)*green,
                  ((sin(i + Position) * 127 + 128) / 255)*blue);
    }
    LEDS.show();
    delay(WaveDelay);
}




void setPixel(int Pixel, byte red, byte green, byte blue) {
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}




// requires Wheel() function
void rainbowCycle(int SpeedDelay, uint16_t jIndex) {
  byte *c;
  for (uint16_t i = 0; i < LED_COUNT; i++) {
    c = Wheel(((i * 256 / LED_COUNT) + jIndex) & 255);
    setPixel(i, *c, *(c + 1), *(c + 2));
  }
  LEDS.show();
  delay(SpeedDelay);
}
byte * Wheel(byte WheelPos) {
  static byte c[3];
  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }
  return c;
}




// requires setPixelHeatColor() function
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT];
  int cooldown;
  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < LED_COUNT; i++) {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);

    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = LED_COUNT - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if ( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }
  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < LED_COUNT; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  LEDS.show();
  delay(SpeedDelay);
}




void setPixelHeatColor(int Pixel, byte temperature) {
  byte t192 = round((temperature / 255.0) * 191);
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  if ( t192 > 0x80) {                    // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}





void new_rainbow_loop(int thisdelay) {
  ihue -= 1;
  fill_rainbow( leds, LED_COUNT, ihue );
  LEDS.show();
  delay(thisdelay);
}




void rainbow_loop(int thisdelay) {
  idex++;
  ihue = ihue + globalScale;
  if (idex >= LED_COUNT) {
    idex = 0;
  }
  if (ihue > 255) {
    ihue = 0;
  }
  leds[idex] = CHSV(ihue, cSat, cVal);
  LEDS.show();
  delay(thisdelay);
}




void random_burst(int thisdelay) {
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  leds[idex] = CHSV(ihue, cSat, cVal);
  LEDS.show();
  delay(thisdelay);
}




void random_color_pop(int thisdelay) {
  idex = random(0, LED_COUNT);
  ihue = random(0, 255);
  for (int i = 0 ; i < LED_COUNT; i++) {
    leds[i].setRGB(0, 0, 0);
  }
  leds[idex] = CHSV(ihue, cSat, cVal);
  LEDS.show();
  delay(thisdelay);
}




void rainbow_fade(int thisdelay) {
  ihue++;
  if (ihue > 255) {
    ihue = 0;
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(ihue, cSat, cVal);
  }
  LEDS.show();
  delay(thisdelay);
}




void rgb_propeller(int thisdelay) {
  idex++;
  int ghue = (cHue + 80) % 255;
  int bhue = (cHue + 160) % 255;
  int N3  = int(LED_COUNT / 3);
  int N6  = int(LED_COUNT / 6);
  int N12 = int(LED_COUNT / 12);
  for (int i = 0; i < N3; i++ ) {
    int j0 = (idex + i + LED_COUNT - N12) % LED_COUNT;
    int j1 = (j0 + N3) % LED_COUNT;
    int j2 = (j1 + N3) % LED_COUNT;
    leds[j0] = CHSV(cHue, cSat, cVal);
    leds[j1] = CHSV(ghue, cSat, cVal);
    leds[j2] = CHSV(bhue, cSat, cVal);
  }
  LEDS.show();
  delay(thisdelay);
}




// requires adjacent_cw() and adjacent_ccw() functions
void color_bounceFADE(int thisdelay) {
  if (bouncedirection == 0) {
    idex = idex + 1;
    if (idex == LED_COUNT) {
      bouncedirection = 1;
      idex = idex - 1;
    }
  }
  if (bouncedirection == 1) {
    idex = idex - 1;
    if (idex == 0) {
      bouncedirection = 0;
    }
  }
  int iL1 = adjacent_cw(idex);
  int iL2 = adjacent_cw(iL1);
  int iL3 = adjacent_cw(iL2);
  int iR1 = adjacent_ccw(idex);
  int iR2 = adjacent_ccw(iR1);
  int iR3 = adjacent_ccw(iR2);
  for (int i = 0; i < LED_COUNT; i++ ) {
    if (i == idex) {
      leds[i] = CHSV(cHue, cSat, 255);
    }
    else if (i == iL1) {
      leds[i] = CHSV(cHue, cSat, 150);
    }
    else if (i == iL2) {
      leds[i] = CHSV(cHue, cSat, 80);
    }
    else if (i == iL3) {
      leds[i] = CHSV(cHue, cSat, 20);
    }
    else if (i == iR1) {
      leds[i] = CHSV(cHue, cSat, 150);
    }
    else if (i == iR2) {
      leds[i] = CHSV(cHue, cSat, 80);
    }
    else if (i == iR3) {
      leds[i] = CHSV(cHue, cSat, 20);
    }
    else {
      leds[i] = CHSV(0, 0, 0);
    }
  }
  LEDS.show();
  delay(thisdelay);
}

int adjacent_cw(int i) {
  int r;
  if (i < LED_COUNT - 1) {
    r = i + 1;
  }
  else {
    r = 0;
  }
  return r;
}

int adjacent_ccw(int i) {
  int r;
  if (i > 0) {
    r = i - 1;
  }
  else {
    r = LED_COUNT - 1;
  }
  return r;
}



// requires antipodal_index() function
void ems_lightsALL(int thisdelay) {
  idex++;
  if (idex >= LED_COUNT) {
    idex = 0;
  }
  int idexR = idex;
  int idexB = antipodal_index(idexR);
  int thathue = (cHue + 160) % 255;
  leds[idexR] = CHSV(cHue, cSat, cVal);
  leds[idexB] = CHSV(thathue, cSat, cVal);
  LEDS.show();
  delay(thisdelay);
}

int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % LED_COUNT;
  }
  return iN;
}





void pulse_one_color_all(int thisdelay) {
  if (bouncedirection == 0) {
    ibright++;
    if (ibright >= 255) {
      bouncedirection = 1;
    }
  }
  if (bouncedirection == 1) {
    ibright = ibright - 1;
    if (ibright <= 1) {
      bouncedirection = 0;
    }
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(cHue, cSat, ibright);
  }
  LEDS.show();
  delay(thisdelay);
}




void sin_bright_wave(int thisdelay, int position, float scale) {
  tcount += scale;
  if (tcount > 3.14) {
    tcount = 0.0;
  }
  ibright = int(sin(tcount) * 255);
  leds[position] = CHSV(cHue, cSat, ibright);
  LEDS.show();
  delay(thisdelay);
}



void theaterChaseRainbow(int SpeedDelay, int j) {
  byte *c;
  for (int q = 0; q < 3; q++) {
    for (int i = 0; i < LED_COUNT; i = i + 3) {
      c = Wheel( (i + j) % 255);
      setPixel(i + q, *c, *(c + 1), *(c + 2)); //turn every third pixel on
    }
    FastLED.show();
    delay(SpeedDelay);
    for (int i = 0; i < LED_COUNT; i = i + 3) {
      setPixel(i + q, 0, 0, 0);    //turn every third pixel off
    }
  }
}
