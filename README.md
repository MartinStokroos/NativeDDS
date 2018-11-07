# Native DDS Library for Arduino
This repository contains the *NativeDDS* library for Arduino. *NativeDDS* is a software implemented Direct Digital Synthesizer. *NativeDDS* generates sinusoidal waves for example for use in audio applications, motor drives, sine wave inverters or as a reference frequency source. The library is easy portable to other platforms.

## Functionality
The DDS frequency resolution is 32-bit. This means that the frequency can be tuned with steps of `f_clk/(2^32)`, with `f_clk = 1/timestep` (see below). The amplitude resolution can be set to eight bit or ten bit by defining `EIGHTBIT` or `TENBIT` in the `NativeDDS.h` file. The eight bit version is faster and will be precise enough for most applications.

The following library classes are defined:

- `DDS_1Ch`,    a single (output) channel DDS
- `DDS_2Ch`,    a dual channel DDS
- `DDS_IQ`,     a dual channel, single frequency, orthogonal output DDS
- `DDS_3Ph`,    a three channel DDS.

## Usage
- First create an instance of the library object, for example:

```
DDS_1Ch mySine;
```

- Initialize the class instance with the member function `begin()`. Also, `begin()` must be called each time when the frequency or phase has to be updated.

```
mySine.begin(float frequency, float starting-phase, float timestep);
```

The arguments of the member function `.begin()` may differ between classes:


DDS_1Ch, use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_2Ch use: `.begin(float frequency1, float frequency2, starting-phase1, float starting-phase2, timestep);`

DDS_IQ use: `.begin(float frequency, float starting-phase, float timestep);`

DDS_3Ph use: `.begin(float frequency, float starting-phase, float timestep);`


For the above member function `.begin()`, the variable `frequency` is in Hz and `starting-phase` in radians. `timestep` is the time period in seconds with which `.compute()` is repeatedly invoked.

- Use the class member function `.compute(void)` on the basis of a fixed time interval to generate a jitter free output signal. For example:

```
mySine.compute();
```

The instantaneous output value is obtained with:

```
int signal = mySine.out1;
```

The following outputs are defined:

DDS_1Ch: `out1`

DDS_2Ch: `out1`, `out2`

DDS_IQ: `outi`, `outq`

DDS_3Ph: `outu`, `outv`, `outw`

## Examples
`SineWave.ino` - Example with a single channel DDS. The time base is generated in the main loop. The maximum usable output frequency is limited to about 100Hz.

`WobblingServo.ino` - A slowly sine-wave modulated servo motor application.

`AudioGenerator.ino` - A generator for audible frequencies. A timer interrupt has been used to create a fixed time base.

## Acknowledgement
A lot of time was saved in developing this library by using the "other" Arduino IDE *Sloeber*. Sloeber is a wonderful Arduino plugin for Eclipse. Thanks to Jantje and his contributors! 
