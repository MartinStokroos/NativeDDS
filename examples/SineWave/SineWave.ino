/*
 *
 * File: SineWave.ino
 * Purpose: Native Direct Digital Synthesizer library example project
 * Version: 1.0.0
 * Date: 22-10-2018
 * URL: https://github.com/MartinStokroos/NativeDDS
 * License: MIT License
 *
 *
 * Copyright (c) M.Stokroos 2018
 *
 * This sketch demonstrates the Native DDS library. Native DDS is a Direct Digital Synthesizer
 * algorithm that runs in software on the Arduino. In this example the sine wave output frequency
 * is set to 1Hz. The pulse width modulated sine wave is available on pin 3.
 *
 * In this example the native DDS runs at 1kHz and this frequency is the 'clock frequency' for the DDS.
 * In theory the maximum output frequency at 1kHz clock frequency is 500Hz. Practically, with output 
 * filtering, the maximum output frequency is limited to about 1/5 of the clock frequency (200Hz).
 * For more information read: A Technical Tutorial on Digital Signal Synthesis, Ken Gentile and Rick Cushing, 
 * Analog Devices, 1999.
 *
 * NOTE: The DDS output is sent to a PWM output pin with the analogWrite function. analogWrite sets the duty-cycle
 * of a PWM signal corresponding to the wanted average output voltage level. The analogWrite PWM-frequency on pin 3
 * is approximately 490Hz. Therefore the maximum usable DDS output frequency is even lower. To get a smooth output
 * wave, output filtering, for example with a low pass RC-filter, is required.
 *
 * The loop timing can be observed with an oscilloscope on pin 13.
 * Please, also check the other example projects of the NativeDDS library.
 *
*/

#include <NativeDDS.h>

#define LPERIOD 1000    // loop period time in us. In this case 1.0ms
#define PWM_OUT 3       // define PWM output pin
#define DEBUG_PIN 13    // define debug PIN for timing observation (=LED pin)

double frequency = 1.0; // output frequency in Hz
unsigned long nextLoop;
int output;
DDS_1Ch mySine; // create an instance of DDS_8Bit_1Ch


void setup() {
	// run once:
	pinMode(PWM_OUT, OUTPUT);
	pinMode(DEBUG_PIN, OUTPUT);

	mySine.begin(frequency, 0, LPERIOD*1.0e-6); // init the DDS (frequency, startphase, sample clock period)

	nextLoop = micros() + LPERIOD; // Set the loop timer variable for the next loop interval.
	}



void loop() {
	// run repeatedly:

	#if defined(EIGHTBIT)
		digitalWrite(DEBUG_PIN, HIGH);  // for checking loop period time and loop execution time (signal high time)
		mySine.update();  //update the DDS
		digitalWrite(DEBUG_PIN, LOW);
		//measured execution time <10us
		analogWrite(PWM_OUT, mySine.out1 + 127); // add 127 to convert to unsigned.
	#else
		digitalWrite(DEBUG_PIN, HIGH);  // debugging: check loop period time and loop execution time (signal high time)
		mySine.update();  // update the DDS
		digitalWrite(DEBUG_PIN, LOW);
		//measured execution time <20us
		output = mySine.out1 + 511; // add 511 to convert to unsigned.
		analogWrite(PWM_OUT, output>>2); // convert to 8-bit and write to analog out.
	#endif


	while(nextLoop > micros());  // wait until the end of the time interval
	nextLoop += LPERIOD;  // set next loop time at current time + LOOP_PERIOD
	}

// end of SineWave.ino

