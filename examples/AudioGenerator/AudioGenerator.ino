/*
 *
 * File: AudioGenerator.ino
 * Purpose: Native Direct Digital Synthesizer library example project
 * Version: 1.0.3
 * Release date: 24-10-2018
 * Last edit date: 02-11-2020
 * 
 * URL: https://github.com/MartinStokroos/NativeDDS
 * License: MIT License
 *
 * Version history:
 * v1.0.0, 24 Oct 2018 - initial release
 * v1.0.1, 18 Nov 2018 - textual changes in comments
 * v1.0.2, 10-08-2020 - select 8-bit/10bit DDS from main c file but it does not work (yet).
 * v1.0.3, 02-11-2020 - library update for seperate instance methods for 8-bit and 10-bit DDS
 * 
 * This sketch demonstrates the Native DDS library. Native DDS is a Direct Digital Synthesizer
 * algorithm that runs in software on the Arduino. In this example, a 220Hz and a 440Hz sine wave are 
 * simultaneously generated with the 2-channel module of the DDS. The pulse width modulated sine waves 
 * are available on pin 3 and pin 11.
 *
 * In this example the native DDS runs at about 32kHz and this frequency is the update frequency (clock frequency)
 * of the DDS. In theory the maximum output frequency for a 32kHz clocked DDS is 16kHz. With output filtering, the 
 * maximum output frequency is limited to about 1/5 of the clock frequency (~6kHz). For more information read: 
 * A Technical Tutorial on Digital Signal Synthesis, Ken Gentile and Rick Cushing, Analog Devices, 1999.
 *
 * To get a smooth output wave, a low pass filter (RC or multistage LC), is required.
 *
 * The loop timing can be observed with an oscilloscope on pin 13. (define DEBUG and get digitalWriteFast).
 * Please, also check the other example projects of the NativeDDS library.
 *
*/

#include "NativeDDS.h"

//#define DEBUG

#if defined(DEBUG)
#include <digitalWriteFast.h>   // library for high performance digital reads and writes by J.R. Raines
			        // see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1267553811/0
			        // and http://code.google.com/p/digitalwritefast/
#endif


#define PWM_OUT1 3      // define PWM output pin
#define PWM_OUT2 11     // define PWM output pin
#define DEBUG_PIN 13    // define debug PIN for timing observation (=LED pin)

const float Ts = 3.1875E-5; // Timer2 period
float f = 220.0; // Wanted DDS output frequency

DDS_8bit_2Ch mySine;	// Create instance of DDS_8bit_2Ch for a dual channel DDS.



void setup() {
	pinMode(PWM_OUT1, OUTPUT);  // PWM output 1
	pinMode(PWM_OUT2, OUTPUT);  // PWM output 2
	pinMode(DEBUG_PIN, OUTPUT); // debug PIN for timing observation (=LED pin)

	/* Setup for Timer 2 for mode 1, Phase Correct PWM mode.
 	*
	* analogWrite can no longer be used on pin 3 and 11.
	* set prescaler to 1
	* foc = fclk/(N*510) = 16e6/(1*510) = 31372.549Hz
	*/
	bitSet(TCCR2B, CS20);
	bitClear(TCCR2B, CS21);
	bitClear(TCCR2B, CS22);
	bitClear(TCCR2A, COM2A0); // clear Compare Match pin11
	bitSet(TCCR2A, COM2A1);
	bitClear(TCCR2A, COM2B0); // clear Compare Match pin3
	bitSet(TCCR2A, COM2B1);
	bitSet(TCCR2A, WGM20);    // mode 1, phase correct PWM
	bitClear(TCCR2A, WGM21);
	bitClear(TCCR2B, WGM22);
	bitSet(TIMSK2, TOIE2);    // enable Timer2 Interrupt

	mySine.begin(f, 2*f, 0, 0, Ts);  // Initialize DDS module. f1=220Hz, f2=440Hz.
}



void loop() {
  // put your main code here
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
* Timer2 ISR, running at 31372.549Hz, and is also the update rate for the DDS
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ISR(TIMER2_OVF_vect) {
	#if defined(DEBUG)
		digitalWriteFast(DEBUG_PIN, true);  // check the loop period time and the execution time in the loop.
	#endif

	mySine.update();
	OCR2B = mySine.uout1;  // PWM_OUT1 directly updated by writing the OCR2B register value (220Hz)
	OCR2A = mySine.uout2;  // PWM_OUT2 directly updated by writing the OCR2A register value (440Hz)

	#if defined(DEBUG)
		digitalWriteFast(DEBUG_PIN, false);
	#endif
}

// end of AudioGenerator.ino
