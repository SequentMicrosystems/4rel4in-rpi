[![ioplus-rpi](../readmeres/sequent.jpg)](https://www.sequentmicrosystems.com)

# sm_4rel4in

This is the python library to control the [Four Relays four HV Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/four-relays-four-inputs-for-raspberry-pi).

## Install

```bash
~$ sudo apt-get update
~$ sudo apt-get install build-essential python-pip python-dev python-smbus git
~$ git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
~$ cd 4rel4in-rpi/python/
~/4rel4in-rpi/python$ sudo python setup.py install
```
If you use python3.x repace the last line with:
```
~/4rel4in-rpi/python$ sudo python3 setup.py install
```
## Update

```bash
~$ cd 4rel4in-rpi/
~/4rel4in-rpi$ git pull
~$ cd ioplus-rpi/python/
~/4rel4in-rpi/python$ sudo python setup.py install
```
If you use python3.x repace the last line with:
```
~/4rel4in-rpi/python$ sudo python3 setup.py install
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
  * Read
* Parameters
  * 
* Returns
  * 

#### **
* Description
  * 
* Parameters
  * 
* Returns
  * 
