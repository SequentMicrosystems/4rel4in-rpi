[![ioplus-rpi](../../readmeres/sequent.jpg)](https://www.sequentmicrosystems.com)

# sm_4rel4in

This is the python library to control the [Four Relays four HV Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/four-relays-four-inputs-for-raspberry-pi).

## Install

```bash
~$ sudo apt-get update
~$ sudo apt-get install build-essential python-pip python-dev python-smbus git
~$ git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
~$ cd 4rel4in-rpi/python/4rel4in/
~/4rel4in-rpi/python/4rel4in$ sudo python setup.py install
```
If you use python3.x repace the last line with:
```
~/4rel4in-rpi/python/4rel4in$ sudo python3 setup.py install
```
## Update

```bash
~$ cd 4rel4in-rpi/
~/4rel4in-rpi$ git pull
~$ cd ioplus-rpi/python/4rel4in/
~/4rel4in-rpi/python/4rel4in$ sudo python setup.py install
```
If you use python3.x repace the last line with:
```
~/4rel4in-rpi/python/4rel4in$ sudo python3 setup.py install
```
## Usage example

```bash
~$ python
Python 3.10.7 (main, Nov  7 2022, 22:59:03) [GCC 8.3.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import sm_4rel4in
>>> rel = sm_4rel4in.SM4rel4in(0)
>>> rel.set_relay(1, 1)
>>> rel.get_all_relays()
1
>>>
```

More usage example in the [examples](examples/) folder

## Functions prototype

### *class sm_4rel4in.SM4rel4in(stack = 0, i2c = 1)*
* Description
  * Init the SM4rel4in object and check the card presence 
* Parameters
  * stack : Card stack level [0..7] set by the jumpers
  * i2c : I2C port number, 1 - Raspberry default , 7 - rock pi 4, etc.
* Returns 
  * card object

#### *set_relay(relay, val)*
* Description
  * Set one relay state
* Parameters
  * *relay*: The relay number 1 to 4
  * *val*: The new state of the relay 0 = turn off else turn on
* Returns
  * none
  
#### *set_all_relays(val)*
* Description
  * Set the state of all relays as a 4 bits bit-map
* Parameters
  * *val*: The new state of all 4 relays 0 = all off, 15 = all on
* Returns
  * none
  
#### *get_relay(relay)*
* Description
  * Read one relay state
* Parameters
  * *relay* relay number [1..4]
* Returns
  * the state of the relay 0 or 1

#### *get_all_relays()*
* Description
  * Read the state of all 4 relays 
* Parameters
  * none
* Returns
  * relays state as bitmap [0..15]

#### *get_in(channel)*
* Description
  * Read the state of one digital input channel
* Parameters
  * *channel*
* Returns
  * the state of the input 0/1

#### *get_all_in()*
* Description
  * Read the state of all digital input channels
* Parameters
  * none
* Returns
  * the state of all inputs as a 4 bits value

#### *get_ac_in(channel)*
* Description
  * Read the filtred digital inputs. The filter cuts 50Hz sou you can detect AC signals presence.
* Parameters
  * *channel* input channel [1..4]
* Returns
  * the presence of an AC or DC signal on the input (0/1)

#### *get_all_ac_in()*
* Description
  * Read all the inputs, filtered.
* Parameters
  * none
* Returns
  * the state of all inputs as a bitmap [0..15]

#### *get_count_cfg(channel)*
* Description
  * Read the counting function activation state
* Parameters
  * *channel* input channel [1..4]
* Returns
  * *1* if the counting function for specified channel is enabled
  * *0* if not

#### *set_count_cfg(channel, state)*
* Description
  * Turns ON or OFF the counting function for specified input channel
* Parameters
  * *channel* input channel number [1..4]
  * *state* 0 - turn off; 1 turn on
* Returns
  * none

#### *get_count(channel)*
* Description
  * Read the counter for the specified channel
* Parameters
  * *channel* input channel number [1..4]
* Returns
  * the counter as a unsigned 32 bits integer

#### *reset_count(channel)*
* Description
  * Reset the counter for the specified input channel
* Parameters
  * *channel* input channel number [1..4]
* Returns
  * none

#### *get_pps(channel)*
* Description
  * Read the speed or pulse per second for specified input channel
* Parameters
  * *channel* input channel number [1..4]
* Returns
  * pulse per seconds as 16bits unsigned integer

#### *get_encoder_cfg(channel)*
* Description
  * Read the quadrature encodersettings. The card supports two quadrature encoders one on the inpits 1 and 2 and one on inputs 3 and 4
* Parameters
  * *channel* quadrature encoder number [1..2]
* Returns
  * *0* encoder enabled
  * *1* encoder disabled


#### *set_encoder_cfg(channel, state)*
* Description
  * Enable or disable quadrature encoder function for specified encoder channel
* Parameters
  * *channel* quadrature encoder number [1..2]
  * *state* 0 - disable; 1 - enable
* Returns
  * none


#### *get_encoder(channel)*
* Description
  * Get the encoder count as 32 bits signed integer
* Parameters
  * *channel* quadrature encoder number [1..2]
* Returns
  * encoder value


#### *reset_encoder(channel)*
* Description
  * Reset the specified encoder value
* Parameters
  * *channel* quadrature encoder number [1..2]
* Returns
  * none


#### *get_frequency(channel)*
* Description
  * Read the frequency in Hz of a PWM signal feed in the specified input channel
* Parameters
  * *channel* input channel number [1..4]
* Returns
  * frequency in Hz


#### *get_pwm_fill(channel)*
* Description
  * Read the fill factor in precents f a PWM signal feed in the specified input channel
* Parameters
  * *channel* input channel number [1..4]
* Returns
  * fill factor in % with two decimal places

#### *get_led_cfg(led)*
* Description
  * Read the configurartion for leds on the card. Four of the card leds are dual purpose, they can follow the inputs state (auto mode) or can be software controled (manual mode) 
* Parameters
  * *led* led number [1..4]
* Returns
  * *0*  led follow the input state (auto)
  * *1*  led can be controlled in software (manual)

#### *set_led_cfg(led, mode)*
* Description
  * Configure the coresponding led working mode
* Parameters
  * *led* led number [1..4]
  * *mode* 0 - auto; 1 - manual
* Returns
  * none

#### *set_led(led, val)*
* Description
  * Set the coresponding led state (on/off)
* Parameters
  * *led* led number [1..4]
  * *val* 0 - led off; 1 - led on
* Returns
  * none

#### *set_all_leds(val)*
* Description
  * Set the state of all 4 leds 
* Parameters
  * *val* - bitmap of all 4 leds [0..15]
* Returns
  * none

#### *get_led(led)*
* Description
  * Read the state of the coresponded led
* Parameters
  * *led* led number [1..4]
* Returns
  * 0 - led is off
  * 1 - led is on

#### *get_all_leds()*
* Description
  * Read the state of all leds 
* Parameters
  * none
* Returns
  * the state of all 4 leds [0..15]
