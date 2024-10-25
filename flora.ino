#pragma region imports
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif
#include <FastLED.h>
// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file
#pragma endregion 
#pragma region MPU
/*
Code stripped from MPU6050/Examples/MPU6050_DMP6
*/

/******
MPU
Something you learn is that .ino files are just concatted together and then thrown into compiler (file with same name is on top tho)
******/
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;   // set true if DMP init was successful
uint8_t mpuIntStatus;    // holds actual interrupt status byte from MPU
uint8_t devStatus;       // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // count of all bytes currently in FIFO
uint8_t fifoBuffer[64];  // FIFO storage buffer

// orientation/motion vars
Quaternion quant;         // [w, x, y, z]         quaternion container
VectorInt16 aa;       // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;   // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;  // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;  // [x, y, z]            gravity vector
float euler[3];       // [psi, theta, phi]    Euler angle container
float ypr[3];         // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;  // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
uint32_t color;

#define degree  180 / M_PI
#pragma endregion MPU
#pragma region LED
/******
Constants
******/
//G22 - SCL esp
//G21 - SDA esp

#define DATA_PIN 15// 6 for nano
#define NUM_LEDS 1
#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 10
#define FLORA_LED_PIN 8
//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
/********
Global Variables
*/
bool blinkState = false;
CRGB leds[NUM_LEDS];

#pragma endregion LED

void setup() {
  FastLED.addLeds<NEOPIXEL, FLORA_LED_PIN>(leds, NUM_LEDS);
}

void loop() {
  // put your main code here, to run repeatedly:
  leds[0] = CRGB::Red;

  FastLED.show();

}
