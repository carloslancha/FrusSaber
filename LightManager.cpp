/*
   LightManager.cpp

   Created on: 23 april 2017
   author:    Carlos Lancha (carlitoslancha@gmail.com)
   Source :   https://github.com/carloslancha/FrusSaber
*/

#include "Config.h"

#include "LightManager.h"

#include <Adafruit_NeoPixel.h>

LightManager::LightManager() {
  light = new Adafruit_NeoPixel(STRIP_PIXELS, LIGHT_PIN, NEO_GRB + NEO_KHZ800);
}

void LightManager::init() {
  #ifdef DEBUG_LIGHT
    Serial.println("Initializing Light Manager");
  #endif

  flashColor[0] = FLASH_COLOR0;
  flashColor[1] = FLASH_COLOR1;
  flashColor[2] = FLASH_COLOR2;
  flashDuration = FLASH_DURATION;

  mainColor[0] = MAIN_COLOR0;
  mainColor[1] = MAIN_COLOR1;
  mainColor[2] = MAIN_COLOR2;

  light->begin();

  #ifdef DEBUG_LIGHT
    Serial.println("Light Manager ready");
  #endif
}

void LightManager::flash() {
  #ifdef DEBUG_LIGHT
    Serial.println("Flashing Light");
  #endif

  setColor(flashColor, 0);
  delay(flashDuration);
  setColor(mainColor, 0);
}

void LightManager::setColor(int color[3], uint8_t wait) {
  for (uint16_t i = 0; i < light->numPixels(); i++) {
    light->setPixelColor(i, light->Color(color[0], color[1], color[2]));
    light->show();
    delay(wait);
  }
}

void LightManager::turnOff() {
  #ifdef DEBUG_LIGHT
    Serial.println("Turning Light Off");
  #endif
  
  int offColor[3] = {0, 0, 0};
  setColor(offColor, 75);
}

void LightManager::turnOn() {
  #ifdef DEBUG_LIGHT
    Serial.println("Turning Light On");
  #endif

  setColor(mainColor, 75);
}
