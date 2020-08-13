/*
 * File: SineWave.ino
 * Purpose: Native Direct Digital Synthesizer library example project
 * Version: 1.1.1
 * First release date: 22-10-2018
 * Last edit date: 10-08-2020
 * 
 * URL: https://github.com/MartinStokroos/NativeDDS
 * License: MIT License
 *
 * Version history:
 * v1.0.0, 22 Oct 2018 - initial release
 * v1.1.0, 18 Nov 2018 - looptime=200us (5kHz). Pin3 + pin11 PWM frequency=31250Hz and the output frequency=100Hz.
 * v1.1.1, 10-08-2020 - select 8-bit/10bit DDS from main c file but it does not work (yet).
 * 
 * This sketch demonstrates the Native DDS library. Native DDS is a Direct Digital Synthesizer
 * algorithm that runs in software on the Arduino. In this example the output frequency is set to 100Hz.
 * The pulse width modulated sine wave is available on pin 3.
 *
 * In this example the DDS runs at 5kHz (the update frequency of the DDS). In theory the maximum output frequency 
 * at 5kHz update rate is 2500Hz. To get a smooth output wave, a low pass filter (RC or multistage LC), is required.
 * For more information please read: A Technical Tutorial on Digital Signal Synthesis, Ken Gentile and Rick Cushing, 
 * Analog Devices, 1999.
 *
 * NOTE: The DDS output is sent to the PWM output pin with the analogWrite function. analogWrite sets the duty-cycle
 * of the PWM signal corresponding to the wanted average output voltage level. The default PWM-frequency of pin 3
 * is approximately 490Hz. Because of this low PWM frequency, the maximum usable DDS output frequency is even lower.
 * For this reason the pwm frequency has been set to much a higher frequency than the DDS update frequency in this example.
 *
 * The loop timing can be observed with an oscilloscope on pin 13 (uncomment the lines with a digitalWrite).
 * Please, also check the other example projects of the NativeDDS library.
 *
*/

//#define DDS_8BIT  //Why does defining it here won't work? Now it still should be done from NativeDDS.h
#include "NativeDDS.h"

#define LPERIOD 200     // loop period time in us. Update freq. is 5kHz
#define PWM_OUT 3       // define PWM output pin
#define DEBUG_PIN 13    // define debug PIN for timing observation (=LED pin)

double frequency = 100.0; // output frequency in Hz
unsigned long nextLoop;
int output;

DDS_1Ch mySine; // create instance of DDS_1Ch



void setup() {
	// run once:
	pinMode(PWM_OUT, OUTPUT);
	pinMode(DEBUG_PIN, OUTPUT);
  TCCR2B = (TCCR2B & 0xF8)|0x01;    // set Pin3 + Pin11 PWM frequency to 31250Hz

	mySine.begin(frequency, 0, LPERIOD*1.0e-6); // initilize the DDS (frequency, startphase, update period time)

	nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
	}



void loop() {
	// run repeatedly:
  
	#ifdef DDS_8BIT
		//digitalWrite(DEBUG_PIN, HIGH);  // for checking loop period time and loop execution time (signal high time)
		
		mySine.update();  //update the DDS, measured execution time <10us
	
		//digitalWrite(DEBUG_PIN, LOW);
		analogWrite(PWM_OUT, mySine.out1 + 127); // add 127 to convert to unsigned.
   
	#elif defined (DDS_10BIT)
		//digitalWrite(DEBUG_PIN, HIGH);  // debugging: check loop period time and loop execution time (signal high time)
		
		mySine.update();  // update the DDS, measured execution time <20us
		
		//digitalWrite(DEBUG_PIN, LOW);
		output = mySine.out1 + 511; // add 511 to convert to unsigned.
		analogWrite(PWM_OUT, output>>2); // convert back to to 8-bit and write to analog out.
	#endif

	while(nextLoop > micros());  // wait until the end of the time interval
	nextLoop += LPERIOD;  // set next loop time at current time + LOOP_PERIOD
	}

// end of SineWave.ino

