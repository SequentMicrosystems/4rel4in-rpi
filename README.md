[![4relind-rpi](readmeres/sequent.jpg)](https://www.sequentmicrosystems.com)

[![4relind-rpi](readmeres/4-REL-4-IN.jpg)]([https://www.sequentmicrosystems.com](https://sequentmicrosystems.com/collections/all-io-cards/products/four-relays-four-inputs-for-raspberry-pi))

# 4rel4in-rpi
Drivers to control Sequent Microsystems [Four Relays four HV Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/four-relays-four-inputs-for-raspberry-pi) v4.0+ and [Smart Relays with Universal Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/smart-relays-with-universal-inputs-for-raspberry-pi)

The Version 4.0 of the card adds the following features related to the optocoupled inputs:

 * Transitions counts for each input up to 4kHz
 * Improved processing of AC input signals.
 * Input counters with cumulative or pps (pulse per second) reading of input transitions (up to 4KHz).
 * 2 x Quadrature encoder inputs speed up to 4000 pulse per second.
 * PWM inputs on each channel, reading the fill factor and frequency less than 1% resolution for signals up to 100Hz and 5% for 500Hz.
 * Programmable LEDs for inputs.
 * Modbus RTU access for all features.

## Setup (first usage of a Sequent Microsystems card)
 - Enable the I2C communication:
```bash
sudo raspi-config
```
Go to the "Interface Options">>"I2C" menu and select "Yes"

 - Update your Raspberry PI and install the tools you need:
```bash
sudo apt-get udate
sudo apt-get upgrade
sudo apt-get install git
```


## Install

```bash
cd
git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
cd 4rel4in-rpi/
sudo make install
```

## Usage
Now you can access all the functions of the card through the command `4rel4in`. Use -h option for help:
```bash
4rel4in -h
```
## Update
If you clone the repository any update can be made with the following commands:

```bash
cd ~/4rel4in-rpi/  
git pull
sudo make install
```

## [Python](https://github.com/SequentMicrosystems/4rel4in-rpi/tree/main/python)

## [Modbus RTU](https://github.com/SequentMicrosystems/4rel4in-rpi/blob/main/MODBUS.md)

## [Node RED](https://github.com/SequentMicrosystems/4relind-rpi/tree/main/node-red-contrib-sm-4relind)