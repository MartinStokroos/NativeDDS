# Native DDS Library for Arduino
This repository contains the *NativeDDS* library for Arduino. *NativeDDS* is a software implemented Direct Digital Synthesizer. *NativeDDS* generates sinusoidal waves for example for use in audio applications, motor drives, sine wave inverters or as a reference frequency source. The library is easy portable to other platforms.

## Function
The DDS frequency resolution is 32-bit. This means that the frequency can be tuned with steps of `f_update/(2^32)`, with `f_update = 1/timestep` (see below). The amplitude resolution can be set to eight bit or ten bit by defining `EIGHTBIT` or `TENBIT` in the `NativeDDS.h` file. The eight bit implementation is faster and will be precise enough for most applications.

The following library classes are implemented:

* `DDS_1Ch`
* `DDS_2Ch`
* `DDS_IQ`
* `DDS_3Ph`

`DDS_1Ch` is a single (output) channel DDS, `DDS_2Ch` is a dual channel DDS, `DDS_IQ` is a dual channel DDS with orthogonal outputs and `DDS_3Ph` is a three channel DDS.

The following methods exists:

`.begin()`

`.update()`

Initialize the class instance with the member function `begin()`. Also, `begin()` must be called each time when the frequency or phase has to be updated.

DDS_1Ch,  use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_2Ch,  use: `.begin(float frequency1, float frequency2, starting-phase1, float starting-phase2, timestep);`

DDS_IQ, use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_3Ph, use: `.begin(float frequency, float starting-phase, float timestep);`

For the member function `.begin()`, the variable `frequency` is in Hz and `starting-phase` in radians. `timestep` is the loop iteration time period in seconds with which `.update()` is repeatedly invoked.

The following public variables are defined for the outputs:

DDS_1Ch: `out1`

DDS_2Ch: `out1`, `out2`

DDS_IQ: `outi`, `outq`

DDS_3Ph: `outu`, `outv`, `outw`

## Usage
* First create an instance of the library object, for example here we define *mySin*:

```
DDS_1Ch mySine;
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

Right after the previous step, the instantaneous output value is obtained with: `int sample = mySine.out1;`

```
void loop() {
	...
	mySine.update();
	sample=mySine.out1
	analogWrite(PWM_OUT, sample + 127); // add 127 to convert to unsigned.
	...
}
```

## Examples
`SineWave.ino` - Example of a single channel DDS. The time base is generated in the main loop. The maximum usable output frequency is limited to about 100Hz.

`WobblingServo.ino` - A slowly sine-wave modulated servo motor application.

`AudioGenerator.ino` - A generator for audible frequencies. A timer interrupt has been used to create a fixed time base.

## Acknowledgement
A lot of time was saved in developing this library by using the "other" Arduino-IDE *Sloeber*. Sloeber is a wonderful Arduino plugin for Eclipse. Thanks to Jantje and his contributors! 
