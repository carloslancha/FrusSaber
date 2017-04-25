/*
   LightManager.cpp

   Created on: 23 april 2017
   author:    Carlos Lancha (carlitoslancha@gmail.com)
   Source :   https://github.com/carloslancha/FrusSaber
*/

#ifndef LightManager_h
#define LightManager_h

#include "Arduino.h"

class Adafruit_NeoPixel;

class LightManager {
  public:
    LightManager();

    void init();

    void flash();

    void turnOff();

    void turnOn();

  protected:
    int flashColor[3];

    int flashDuration;

    Adafruit_NeoPixel *light;

    int mainColor[3];

    void setColor(int color[3], uint8_t wait);
};

#endif
