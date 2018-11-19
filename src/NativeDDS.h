/*
* 
* File: NativeDDS.h
* Purpose: Native Direct Digital Synthesizer library functions
* Version: 1.0.0
* Date: 22-10-2018
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


#ifndef NativeDDS_H_
#define NativeDDS_H_

#include "Arduino.h"
#include "avr/pgmspace.h"

#define EIGHTBIT
//#define TENBIT

#define PHASE_OFFS_90 1073741824UL
#define PHASE_OFFS_120 1431655765UL
#define PHASE_OFFS_240 2863311531UL


#ifdef EIGHTBIT
// look-up table of 256 sine values
const int sinlut256[] PROGMEM = {
		127, 130, 133, 136, 139, 143, 146, 149,
		152, 155, 158, 161, 164, 167, 170, 173,
		176, 178, 181, 184, 187, 190, 192, 195,
		198, 200, 203, 205, 208, 210, 212, 215,
		217, 219, 221, 223, 225, 227, 229, 231,
		233, 234, 236, 238, 239, 240, 242, 243,
		244, 245, 247, 248, 249, 249, 250, 251,
		252, 252, 253, 253, 253, 254, 254, 254,
		254, 254, 254, 254, 253, 253, 253, 252,
		252, 251, 250, 249, 249, 248, 247, 245,
		244, 243, 242, 240, 239, 238, 236, 234,
		233, 231, 229, 227, 225, 223, 221, 219,
		217, 215, 212, 210, 208, 205, 203, 200,
		198, 195, 192, 190, 187, 184, 181, 178,
		176, 173, 170, 167, 164, 161, 158, 155,
		152, 149, 146, 143, 139, 136, 133, 130,
		127, 124, 121, 118, 115, 111, 108, 105,
		102, 99, 96, 93, 90, 87, 84, 81,
		78, 76, 73, 70, 67, 64, 62, 59,
		56, 54, 51, 49, 46, 44, 42, 39,
		37, 35, 33, 31, 29, 27, 25, 23,
		21, 20, 18, 16, 15, 14, 12, 11,
		10, 9, 7, 6, 5, 5, 4, 3,
		2, 2, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 2,
		2, 3, 4, 5, 5, 6, 7, 9,
		10, 11, 12, 14, 15, 16, 18, 20,
		21, 23, 25, 27, 29, 31, 33, 35,
		37, 39, 42, 44, 46, 49, 51, 54,
		56, 59, 62, 64, 67, 70, 73, 76,
		78, 81, 84, 87, 90, 93, 96, 99,
		102, 105, 108, 111, 115, 118, 121, 124
		};
#endif


#ifdef TENBIT
// look-up table with 1024 sine values
const int sinlut1024[] PROGMEM = {
		  511, 514, 517, 520, 524, 527, 530, 533,
		  536, 539, 542, 545, 549, 552, 555, 558,
		  561, 564, 567, 570, 574, 577, 580, 583,
		  586, 589, 592, 595, 598, 601, 605, 608,
		  611, 614, 617, 620, 623, 626, 629, 632,
		  635, 638, 641, 644, 647, 650, 653, 656,
		  659, 662, 665, 668, 671, 674, 677, 680,
		  683, 686, 689, 692, 695, 698, 701, 704,
		  707, 709, 712, 715, 718, 721, 724, 727,
		  729, 732, 735, 738, 741, 744, 746, 749,
		  752, 755, 757, 760, 763, 766, 768, 771,
		  774, 776, 779, 782, 784, 787, 790, 792,
		  795, 797, 800, 803, 805, 808, 810, 813,
		  815, 818, 820, 823, 825, 828, 830, 833,
		  835, 838, 840, 842, 845, 847, 849, 852,
		  854, 856, 859, 861, 863, 866, 868, 870,
		  872, 875, 877, 879, 881, 883, 885, 888,
		  890, 892, 894, 896, 898, 900, 902, 904,
		  906, 908, 910, 912, 914, 916, 918, 920,
		  921, 923, 925, 927, 929, 931, 932, 934,
		  936, 938, 939, 941, 943, 944, 946, 948,
		  949, 951, 952, 954, 956, 957, 959, 960,
		  962, 963, 965, 966, 967, 969, 970, 972,
		  973, 974, 976, 977, 978, 979, 981, 982,
		  983, 984, 985, 987, 988, 989, 990, 991,
		  992, 993, 994, 995, 996, 997, 998, 999,
		  1000, 1001, 1002, 1003, 1003, 1004, 1005, 1006,
		  1007, 1007, 1008, 1009, 1010, 1010, 1011, 1012,
		  1012, 1013, 1013, 1014, 1014, 1015, 1016, 1016,
		  1016, 1017, 1017, 1018, 1018, 1019, 1019, 1019,
		  1020, 1020, 1020, 1020, 1021, 1021, 1021, 1021,
		  1021, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
		  1022, 1022, 1022, 1022, 1022, 1022, 1022, 1022,
		  1021, 1021, 1021, 1021, 1021, 1020, 1020, 1020,
		  1020, 1019, 1019, 1019, 1018, 1018, 1017, 1017,
		  1016, 1016, 1016, 1015, 1014, 1014, 1013, 1013,
		  1012, 1012, 1011, 1010, 1010, 1009, 1008, 1007,
		  1007, 1006, 1005, 1004, 1003, 1003, 1002, 1001,
		  1000, 999, 998, 997, 996, 995, 994, 993,
		  992, 991, 990, 989, 988, 987, 985, 984,
		  983, 982, 981, 979, 978, 977, 976, 974,
		  973, 972, 970, 969, 967, 966, 965, 963,
		  962, 960, 959, 957, 956, 954, 952, 951,
		  949, 948, 946, 944, 943, 941, 939, 938,
		  936, 934, 932, 931, 929, 927, 925, 923,
		  921, 920, 918, 916, 914, 912, 910, 908,
		  906, 904, 902, 900, 898, 896, 894, 892,
		  890, 888, 885, 883, 881, 879, 877, 875,
		  872, 870, 868, 866, 863, 861, 859, 856,
		  854, 852, 849, 847, 845, 842, 840, 838,
		  835, 833, 830, 828, 825, 823, 820, 818,
		  815, 813, 810, 808, 805, 803, 800, 797,
		  795, 792, 790, 787, 784, 782, 779, 776,
		  774, 771, 768, 766, 763, 760, 757, 755,
		  752, 749, 746, 744, 741, 738, 735, 732,
		  729, 727, 724, 721, 718, 715, 712, 709,
		  707, 704, 701, 698, 695, 692, 689, 686,
		  683, 680, 677, 674, 671, 668, 665, 662,
		  659, 656, 653, 650, 647, 644, 641, 638,
		  635, 632, 629, 626, 623, 620, 617, 614,
		  611, 608, 605, 601, 598, 595, 592, 589,
		  586, 583, 580, 577, 574, 570, 567, 564,
		  561, 558, 555, 552, 549, 545, 542, 539,
		  536, 533, 530, 527, 524, 520, 517, 514,
		  511, 508, 505, 502, 498, 495, 492, 489,
		  486, 483, 480, 477, 473, 470, 467, 464,
		  461, 458, 455, 452, 448, 445, 442, 439,
		  436, 433, 430, 427, 424, 421, 417, 414,
		  411, 408, 405, 402, 399, 396, 393, 390,
		  387, 384, 381, 378, 375, 372, 369, 366,
		  363, 360, 357, 354, 351, 348, 345, 342,
		  339, 336, 333, 330, 327, 324, 321, 318,
		  315, 313, 310, 307, 304, 301, 298, 295,
		  293, 290, 287, 284, 281, 278, 276, 273,
		  270, 267, 265, 262, 259, 256, 254, 251,
		  248, 246, 243, 240, 238, 235, 232, 230,
		  227, 225, 222, 219, 217, 214, 212, 209,
		  207, 204, 202, 199, 197, 194, 192, 189,
		  187, 184, 182, 180, 177, 175, 173, 170,
		  168, 166, 163, 161, 159, 156, 154, 152,
		  150, 147, 145, 143, 141, 139, 137, 134,
		  132, 130, 128, 126, 124, 122, 120, 118,
		  116, 114, 112, 110, 108, 106, 104, 102,
		  101, 99, 97, 95, 93, 91, 90, 88,
		  86, 84, 83, 81, 79, 78, 76, 74,
		  73, 71, 70, 68, 66, 65, 63, 62,
		  60, 59, 57, 56, 55, 53, 52, 50,
		  49, 48, 46, 45, 44, 43, 41, 40,
		  39, 38, 37, 35, 34, 33, 32, 31,
		  30, 29, 28, 27, 26, 25, 24, 23,
		  22, 21, 20, 19, 19, 18, 17, 16,
		  15, 15, 14, 13, 12, 12, 11, 10,
		  10, 9, 9, 8, 8, 7, 6, 6,
		  6, 5, 5, 4, 4, 3, 3, 3,
		  2, 2, 2, 2, 1, 1, 1, 1,
		  1, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 0, 0, 0,
		  1, 1, 1, 1, 1, 2, 2, 2,
		  2, 3, 3, 3, 4, 4, 5, 5,
		  6, 6, 6, 7, 8, 8, 9, 9,
		  10, 10, 11, 12, 12, 13, 14, 15,
		  15, 16, 17, 18, 19, 19, 20, 21,
		  22, 23, 24, 25, 26, 27, 28, 29,
		  30, 31, 32, 33, 34, 35, 37, 38,
		  39, 40, 41, 43, 44, 45, 46, 48,
		  49, 50, 52, 53, 55, 56, 57, 59,
		  60, 62, 63, 65, 66, 68, 70, 71,
		  73, 74, 76, 78, 79, 81, 83, 84,
		  86, 88, 90, 91, 93, 95, 97, 99,
		  101, 102, 104, 106, 108, 110, 112, 114,
		  116, 118, 120, 122, 124, 126, 128, 130,
		  132, 134, 137, 139, 141, 143, 145, 147,
		  150, 152, 154, 156, 159, 161, 163, 166,
		  168, 170, 173, 175, 177, 180, 182, 184,
		  187, 189, 192, 194, 197, 199, 202, 204,
		  207, 209, 212, 214, 217, 219, 222, 225,
		  227, 230, 232, 235, 238, 240, 243, 246,
		  248, 251, 254, 256, 259, 262, 265, 267,
		  270, 273, 276, 278, 281, 284, 287, 290,
		  293, 295, 298, 301, 304, 307, 310, 313,
		  315, 318, 321, 324, 327, 330, 333, 336,
		  339, 342, 345, 348, 351, 354, 357, 360,
		  363, 366, 369, 372, 375, 378, 381, 384,
		  387, 390, 393, 396, 399, 402, 405, 408,
		  411, 414, 417, 421, 424, 427, 430, 433,
		  436, 439, 442, 445, 448, 452, 455, 458,
		  461, 464, 467, 470, 473, 477, 480, 483,
		  486, 489, 492, 495, 498, 502, 505, 508
};
#endif


#ifdef EIGHTBIT
class DDS_1Ch
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);

	float freq;
	float deltat;
	float startPhase;
	int out1;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};


class DDS_2Ch
{
public:
	void begin(float _freq1, float _freq2, float _startPhase1, float _startPhase2, float _deltat);
	void update(void);

	float freq1;
	float freq2;
	float deltat;
	float startPhase1;
	float startPhase2;
	int out1;
	int out2;
private:
	volatile unsigned long tuningWord1;
	volatile unsigned long tuningWord2;
	volatile unsigned long phaseAccu1;
	volatile unsigned long phaseAccu2;
};


class DDS_IQ
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);

	float freq;
	float deltat;
	float startPhase;
	int outi;
	int outq;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};


class DDS_3Ph
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);

	float freq;
	float deltat;
	float startPhase;
	int outu;
	int outv;
	int outw;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};
#endif


#ifdef TENBIT
class DDS_1Ch
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);
	//void updateFreq(void);
	//void updatePhase(void);

	float freq;
	float deltat;
	float startPhase;
	int out1;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};


class DDS_2Ch
{
public:
	void begin(float _freq1, float _freq2, float _startPhase1, float _startPhase2, float _deltat);
	void update(void);

	float freq1;
	float freq2;
	float deltat;
	float startPhase1;
	float startPhase2;
	int out1;
	int out2;

private:
	volatile unsigned long tuningWord1;
	volatile unsigned long tuningWord2;
	volatile unsigned long phaseAccu1;
	volatile unsigned long phaseAccu2;
};


class DDS_IQ
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);

	float freq;
	float deltat;
	float startPhase;
	int outi;
	int outq;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};


class DDS_3Ph
{
public:
	void begin(float _freq, float _startPhase, float _deltat);
	void update(void);

	float freq;
	float deltat;
	float startPhase;
	int outu;
	int outv;
	int outw;
private:
	volatile unsigned long tuningWord;
	volatile unsigned long phaseAccu;
};
#endif

#endif /* NativeDDS_H_ */



