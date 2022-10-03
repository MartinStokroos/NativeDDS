# Native DDS Library for Arduino v1.2
This repository contains the *NativeDDS* library for Arduino. *NativeDDS* is a software implemented Direct Digital Synthesizer. *NativeDDS* generates sinusoidal waves for example for use in audio applications, motor drives, sine wave inverters or for a reference frequency source. The library is easy portable to other platforms.

## Function
The DDS frequency resolution is 32-bit. This means that the frequency can be tuned in steps of `f_update/(2^32)`, with `f_update = 1/timestep` (see below). The amplitude resolution can be eight bits or ten bits by choosing the class instances `DDS_8bit_xx` or `DDS_10bit_xx`. The eight bit implementation is faster and will be precise enough in most applications.

The following library instance methods are implemented:

* `DDS_8bit_1Ch or DDS_10bit_1Ch`
* `DDS_8bit_2Ch or DDS_10bit_2Ch`
* `DDS_8bit_IQ or DDS_10bit_IQ`
* `DDS_8bit_3Ph or DDS_10bit_3Ph`

The explanation below will be continued for the 8-bit case.

`DDS_8bit_1Ch` is a single (output) channel DDS, `DDS_8bit_2Ch` is a dual channel DDS, `DDS_8bit_IQ` is a dual channel DDS generating orthogonal outputs and `DDS_8bit_3Ph` is a three output channel DDS.

The following methods exists:

`.begin()`

`.update()`

Initialize the class instance with the member function `begin()`. Also, `begin()` must be called each time when the frequency or phase has to be updated.

DDS_8bit_1Ch,  use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_8bit_2Ch,  use: `.begin(float frequency1, float frequency2, starting-phase1, float starting-phase2, timestep);`

DDS_8bit_IQ, use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_8bit_3Ph, use: `.begin(float frequency, float starting-phase, float timestep);`

For the member function `.begin()`, the variable `frequency` is in Hz and `starting-phase` in radians. `timestep` is the loop repeating time in seconds at which `.update()` is invoked.

The following public output variables are defined:

DDS_8bit_1Ch: `out1` and `uout1` for unsigned.

DDS_8bit_2Ch: `out1`, `out2` and `uout1`, `uout2` for unsigned. 

DDS_8bit_IQ: `outi`, `outq` and `uouti`, `uoutq` for unsigned.

DDS_8bit_3Ph: `outu`, `outv`, `outw` and `uoutu`, `uoutv`, `uoutw` for unsigned.

## Usage
* First create an instance of the library object, for example here we define *mySin*:

```
DDS_8bit_1Ch mySine;
```

* Initialize the DDS on some place in your setup function: 

```
void setup() {
	...
	mySine.begin(float frequency, float starting-phase, float timestep);
	...
}
```

* Call `mySine.update();` from the main loop or from an Interrupt Service Routine (ISR). Use `.update(void)` on the basis of a regular time interval to generate a jitter free output signal.

Right after the previous step, the instantaneous (unsigned) output value is obtained with: `int sample = mySine.uout1;`

```
void loop() {
	...
	mySine.update();
	sample=mySine.uout1
	analogWrite(PWM_OUT, sample);
	...
}
```

## Examples
`SineWave_8bit.ino` - Example of a single channel 8-bits DDS. The time base is generated in the main loop. The maximum usable output frequency is limited to about 100Hz.

`SineWave_10bit.ino` - Example of a single channel 10-bits DDS. The time base is generated in the main loop. The maximum usable output frequency is limited to about 100Hz.

`WobblingServo.ino` - A slowly sine-wave modulated servo motor application.

`AudioGenerator.ino` - A generator for audible frequencies. A timer interrupt has been used to create the time base.

## Acknowledgement
A lot of time was saved in developing this library by using the alternative Arduino-IDE [Sloeber](https://eclipse.baeyens.it/). Sloeber is a wonderful Arduino plugin for Eclipse. Thanks to Jantje and his contributors!
