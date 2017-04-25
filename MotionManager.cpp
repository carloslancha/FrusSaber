/*
   MotionManager.cpp

   Created on: 23 april 2017
   author:    Carlos Lancha (carlitoslancha@gmail.com)
   Source :   https://github.com/carloslancha/FrusSaber
*/

#include "Config.h"

#include "Arduino.h"

#include "I2Cdev.h"

#include "MotionManager.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

volatile bool mpuInterrupt = false;
void dmpDataReady() {
  mpuInterrupt = true;
}

MotionManager::MotionManager() {
  clashDebounce = 200;
  clashTimer = 0;
  devStatus = 0;
  mpu = new MPU6050();
  mpuIntStatus = 0;
  mpuFifoCount = 0;
  packetSize = 0;
  quaternion = new Quaternion();
  quaternion_last = new Quaternion();
  quaternion_reading = new Quaternion();

}

void MotionManager::init() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24;
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  #ifdef DEBUG_MPU
    Serial.println("Initializing Motion Manager");
  #endif
  mpu->initialize();

  #ifdef DEBUG_MPU
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  #endif

  #ifdef DEBUG_MPU
    Serial.println(F("Initializing DMP..."));
  #endif

  devStatus = mpu->dmpInitialize();

  mpu->setXGyroOffset(XGYROOFFSET);
  mpu->setYGyroOffset(YGYROOFFSET);
  mpu->setZGyroOffset(ZGYROOFFSET);
  mpu->setZAccelOffset(XACCELOFFSET);
  mpu->setZAccelOffset(YACCELOFFSET);
  mpu->setZAccelOffset(ZACCELOFFSET);

  if (devStatus == 0) {
    #ifdef DEBUG_MPU
      Serial.println(F("Enabling DMP..."));
    #endif
    mpu->setDMPEnabled(true);

    #ifdef DEBUG_MPU
      Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    #endif
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu->getIntStatus();

    #ifdef DEBUG_MPU
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
    #endif
    dmpReady = true;

    packetSize = mpu->dmpGetFIFOPacketSize();
  }
  else {
    #ifdef DEBUG_MPU
      Serial.print(F("DMP Initialization failed (code "));
      Serial.print(devStatus);
      Serial.println(F(")"));
    #endif
  }

  mpu->setIntMotionEnabled(true);
  mpu->setMotionDetectionThreshold(10);
  mpu->setMotionDetectionDuration(2);
  mpuIntStatus = mpu->getIntStatus();
}

boolean MotionManager::isClash() {
  boolean isClash = false;
  
  if (millis() - clashTimer > clashDebounce && mpuIntStatus > 60 && mpuIntStatus < 70) {
    #ifdef DEBUG_MPU
      Serial.println("Clash detected");
    #endif

    isClash = true;
    clashTimer = millis();
  }

  return isClash;
}

boolean MotionManager::isSwing() {
  boolean swingDetected = false;

  long absoluteMagnitude = abs(quaternion->w);

  if (absoluteMagnitude > 10) {
    #ifdef DEBUG_MPU
      Serial.println("Swing detected");
    #endif

    swingDetected = true;
  }

  return swingDetected;
}

void MotionManager::update() {
  if (!dmpReady) return;

  while (!mpuInterrupt && mpuFifoCount < packetSize) {
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu->getIntStatus();

  mpuFifoCount = mpu->getFIFOCount();

  if ((mpuIntStatus & 0x10) || mpuFifoCount == 1024) {
    mpu->resetFIFO();

    #ifdef DEBUG_MPU
      Serial.println(F("FIFO overflow!"));
    #endif
  }
  else if (mpuIntStatus & 0x02) {
    while (mpuFifoCount < packetSize) mpuFifoCount = mpu->getFIFOCount();

    mpu->getFIFOBytes(fifoBuffer, packetSize);

    mpuFifoCount -= packetSize;

    quaternion_last->w = quaternion_reading->w;

    mpu->dmpGetQuaternion(quaternion_reading, fifoBuffer);

    quaternion->w = quaternion_reading->w * 1000 - quaternion_last->w * 1000;
  }
}
