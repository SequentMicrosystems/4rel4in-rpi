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


