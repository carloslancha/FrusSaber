/*
   MotionManager.h

   Created on: 23 april 2017
   author:    Carlos Lancha (carlitoslancha@gmail.com)
   Source :   https://github.com/carloslancha/FrusSaber
*/

#ifndef MotionManager_h
#define MotionManager_h

#include "Arduino.h"

class MPU6050;
class Quaternion;

class MotionManager {
  public:
    MotionManager();

    void init();

    bool isClash();

    bool isSwing();

    void update();

  protected:

    int clashDebounce;

    int clashTimer;

    unsigned short int devStatus;

    bool dmpReady;

    unsigned char fifoBuffer[64];

    MPU6050 *mpu;

    unsigned int mpuFifoCount;

    unsigned short int mpuIntStatus;

    unsigned int packetSize;

    Quaternion *quaternion;

    Quaternion *quaternion_last;

    Quaternion *quaternion_reading;
};

#endif
