/*
 *
 * File: WobblingServo.ino
 * Purpose: Native Direct Digital Synthesizer library example project
 * Version: 1.0.0
 * First release date: 23-10-2018
 * Last edit date: 10-08-2020
 * 
 * URL: https://github.com/MartinStokroos/NativeDDS
 * License: MIT License
 *
 * Version history:
 * v1.0.1, 10-08-2020 - select 8-bit/10bit DDS from main c file. Does not work (yet).
 *
 * This sketch demonstrates the Native DDS library. Native DDS is a Direct Digital Synthesizer
 * algorithm that runs in software on the Arduino. In this example the DDS sine wave output frequency
 * is set to 0.1Hz. A servo motor connected to pin 3 slowly modulates between MIN_ANGLE and
 * MAX_ANGLE.
 * 
 * NOTE: The DDS output wave is remapped to the angle setpoint of a servo motor attached to pin 3. The servo is
 * PWM-controlled at a frequency of 50Hz. It is not meaningful to update the servo-position at a rate higher than 50Hz. 
 * The DDS output wave sample rate is down converted to 10Hz for updating the servo.
 *
*/


//#define DDS_8BIT  //Why does defining it here won't work? Now it still should be done from NativeDDS.h
#include "NativeDDS.h"
#include <Servo.h>

#define LPERIOD 1000    // loop period time in us. In this case 1.0ms => f_clk=1kHz 
#define SERVO_PIN 3     // define PWM output pin
#define DEBUG_PIN 13    // define debug PIN for timing observation (=LED pin)
#define MAX_ANGLE 120
#define MIN_ANGLE 60

double frequency = 0.1; // output frequency in Hz
int k=0, servoAngle;
unsigned long nextLoop;

DDS_1Ch mySine; // create an instance of DDS_1Ch
Servo myServo;  // create an instance of Servo


void setup() {
  pinMode(DEBUG_PIN, OUTPUT);  
  myServo.attach(SERVO_PIN);
  myServo.write( (MIN_ANGLE + MAX_ANGLE)/2 );  //Set the servo to the mid-position

  mySine.begin(frequency, 0, LPERIOD*1.0e-6);    //Initialize the DDS (frequency, startphase, sample clock period)

  nextLoop = micros() + LPERIOD;    //Set the loop timer variable for the next loop interval.
}



void loop() {
  // run repeatedly:
  digitalWrite(DEBUG_PIN, HIGH);  //For checking loop period time and execution (signal high) time with an oscilloscope. 
  
  mySine.update();  //DDS update

  if(k>=100) {   //update servo at 1/100 of loop rate (=10Hz)
    servoAngle = map(mySine.out1, -127, 127, MIN_ANGLE, MAX_ANGLE); // remap DDS output to servo degrees servo displacement.
    myServo.write(servoAngle);
    k=0;
  }
  k++;
  
  digitalWrite(DEBUG_PIN, LOW);
  
  while(nextLoop > micros());  //Wait until the end of the time interval 
    nextLoop += LPERIOD;  // set next loop time at current time + LOOP_PERIOD
} // measured execution time <20us

// end of WobblingServo.ino
