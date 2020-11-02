/*
* 
* File: NativeDDS.cpp
* Purpose: Native Direct Digital Synthesizer library functions
* Version: 1.2.0
* Release date: 22-10-2018
* Last edit date: 01-11-2020
*
* Version history:
*
* v1.2.0, 01-11-2020 - now with seperate instance methods for 8-bit and 10-bit DDS and signed/unsigned wave outputs.
* v1.0.2, 10-08-2020 - changed the option switch DDS_8BIT/DDS_10BIT
*
* URL: https://github.com/MartinStokroos/NativeDDS
* License: MIT License
*
*
* Copyright (c) M.Stokroos 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation 
* files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, 
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
* Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#include "NativeDDS.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/* * * init functions * * */
void DDS_8bit_1Ch::begin(float _freq, float _startPhase, float _deltat) {
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate the DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// set the starting phase
	if(startPhase <= 0 || startPhase>2*PI){
		startPhase = 0;
	}
	phaseAccu = (unsigned long)(startPhase*pow(2,32)/(2*PI));
}

void DDS_8bit_2Ch::begin(float _freq1, float _freq2, float _startPhase1, float _startPhase2, float _deltat) {
	freq1 = _freq1;
	freq2 = _freq2;
	deltat = _deltat;
	startPhase1 = _startPhase1;
	startPhase2 = _startPhase2;
	// calculate the DDS tuning word
	tuningWord1 = (unsigned long)(pow(2,32)*freq1*deltat);
	tuningWord2 = (unsigned long)(pow(2,32)*freq2*deltat);
	// set the starting phase
	if(startPhase1 <= 0 || startPhase1 > 2*PI){
		startPhase1 = 0;
	}
	if(startPhase2 <= 0 || startPhase2 > 2*PI){
		startPhase2 = 0;
	}
	phaseAccu1 = (unsigned long)(startPhase1*pow(2,32)/(2*PI));
	phaseAccu2 = (unsigned long)(startPhase2*pow(2,32)/(2*PI));
}

void DDS_8bit_IQ::begin(float _freq, float _startPhase, float _deltat) {
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// set the starting phase
	phaseAccu = startPhase*pow(2,32)/(2*PI);
}

void DDS_8bit_3Ph::begin(float _freq, float _startPhase, float _deltat){
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate the DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// set the starting phase
	phaseAccu = startPhase*pow(2,32)/(2*PI);;
}


/* * * Periodic update functions * * */
void DDS_8bit_1Ch::update(void){
	byte phaseIndex;

	phaseAccu += tuningWord; // update the phase accumulator
	phaseIndex = (byte)(phaseAccu>>24); // truncate the phase accumulator to obtain the phase index pointer
	uout1 = pgm_read_byte_near(sinlut256+phaseIndex); // read amplitude from look-up table
	out1 = uout1 - 0x7F; // convert to signed int.
}

void DDS_8bit_2Ch::update(void){
	byte phaseIndex1, phaseIndex2;

	phaseAccu1 += tuningWord1;
	phaseAccu2 += tuningWord2;
	phaseIndex1 = (byte)(phaseAccu1>>24);
	phaseIndex2 = (byte)(phaseAccu2>>24);
	uout1 = pgm_read_byte_near(sinlut256+phaseIndex1);
	out1 = uout1 - 0x7F;
	uout2 = pgm_read_byte_near(sinlut256+phaseIndex2);
	out2 = uout2 - 0x7F;
}

void DDS_8bit_IQ::update(void){
	byte phaseIndexI, phaseIndexQ;

	phaseAccu += tuningWord;
	phaseIndexI = (byte)(phaseAccu>>24);
	phaseIndexQ = (byte)((phaseAccu+PHASE_OFFS_90)>>24); //add fixed phase offset for +90deg.
	uouti = pgm_read_byte_near(sinlut256+phaseIndexI);
	outi = uouti - 0x7F;
	uoutq = pgm_read_byte_near(sinlut256+phaseIndexQ);
	outq = uoutq - 0x7F;
}

void DDS_8bit_3Ph::update(void){
	byte phaseIndexU, phaseIndexV, phaseIndexW;

	phaseAccu += tuningWord;
	phaseIndexU = (byte)(phaseAccu>>24);
	phaseIndexV = (byte)((phaseAccu+PHASE_OFFS_120)>>24); //add fixed phase offset for +120 deg.
	phaseIndexW = (byte)((phaseAccu+PHASE_OFFS_240)>>24); //add fixed phase offset for +240 deg.
	uoutu = pgm_read_byte_near(sinlut256+phaseIndexU);
	outu = uoutu - 0x7F;
	uoutv = pgm_read_byte_near(sinlut256+phaseIndexV);
	outv = uoutv - 0x7F;
	uoutw = pgm_read_byte_near(sinlut256+phaseIndexW);
	outw = uoutw - 0x7F;
}


/* * * init functions * * */
void DDS_10bit_1Ch::begin(float _freq, float _startPhase, float _deltat) {
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// set the starting phase
	if(startPhase <= 0 || startPhase > 2*PI){
		startPhase = 0;
	}
	phaseAccu = (unsigned long)(startPhase*pow(2,32)/(2*PI));
}

void DDS_10bit_2Ch::begin(float _freq1, float _freq2, float _startPhase1, float _startPhase2, float _deltat) {
	freq1 = _freq1;
	freq2 = _freq2;
	deltat = _deltat;
	startPhase1 = _startPhase1;
	startPhase2 = _startPhase2;
	// calculate the DDS tuning word
	tuningWord1 = (unsigned long)(pow(2,32)*freq1*deltat);
	tuningWord2 = (unsigned long)(pow(2,32)*freq2*deltat);
	// start phase
	if(startPhase1 <= 0 || startPhase1 > 2*PI){
		startPhase1 = 0;
	}
	if(startPhase2 <= 0 || startPhase2 > 2*PI){
		startPhase2 = 0;
	}
	phaseAccu1 = (unsigned long)(startPhase1*pow(2,32)/(2*PI));
	phaseAccu2 = (unsigned long)(startPhase2*pow(2,32)/(2*PI));
}

void DDS_10bit_IQ::begin(float _freq, float _startPhase, float _deltat){
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate the DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// set the starting phase
	phaseAccu = startPhase*pow(2,32)/(2*PI);
}

void DDS_10bit_3Ph::begin(float _freq, float _startPhase, float _deltat){
	freq = _freq;
	deltat = _deltat;
	startPhase = _startPhase;
	// calculate the DDS tuning word
	tuningWord = (unsigned long)(pow(2,32)*freq*deltat);
	// start phase
	phaseAccu = startPhase*pow(2,32)/(2*PI);;
}


/* * * Periodic update functions * * */
void DDS_10bit_1Ch::update(void){
	unsigned int phaseIndex;

	phaseAccu += tuningWord;
	phaseIndex = (phaseAccu>>22);
	uout1 = pgm_read_word(sinlut1024+phaseIndex);
	out1 = uout1 - 0x1FF;
}

void DDS_10bit_2Ch::update(void){
	unsigned int phaseIndex1, phaseIndex2;

	phaseAccu1 += tuningWord1;
	phaseAccu2 += tuningWord2;
	phaseIndex1 = (phaseAccu1>>22);
	phaseIndex2 = (phaseAccu2>>22);
	uout1 = pgm_read_word(sinlut1024+phaseIndex1);
	out1 = uout1 - 0x1FF;
	uout2 = pgm_read_word(sinlut1024+phaseIndex2);
	out2 = uout2 - 0x1FF;
}

void DDS_10bit_IQ::update(void){
	unsigned int phaseIndexI, phaseIndexQ;

	phaseAccu += tuningWord;
	phaseIndexI = (phaseAccu>>22);
	phaseIndexQ = ((phaseAccu+PHASE_OFFS_90)>>22); // adding a fixed phase offset for +90deg.
	uouti = pgm_read_word(sinlut1024+phaseIndexI);
	outi = uouti - 0x1FF;
	uoutq = pgm_read_word(sinlut1024+phaseIndexQ);
	outq = uoutq - 0x1FF;
}

void DDS_10bit_3Ph::update(void){
	unsigned int phaseIndexU, phaseIndexV, phaseIndexW;

	phaseAccu += tuningWord;
	phaseIndexU = (phaseAccu>>22);
	phaseIndexV = ((phaseAccu+PHASE_OFFS_120)>>22); // adding a fixed phase offset for +120deg.
	phaseIndexW = ((phaseAccu+PHASE_OFFS_240)>>22); // adding a fixed phase offset for +240deg.
	uoutu = pgm_read_word(sinlut1024+phaseIndexU);
	outu = uoutu - 0x1FF;
	uoutv = pgm_read_word(sinlut1024+phaseIndexV);
	outv = uoutv - 0x1FF;
	uoutw = pgm_read_word(sinlut1024+phaseIndexW);
	outw = uoutw - 0x1FF;
}

// end of NativeDDS.cpp
