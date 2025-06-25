# Welcome to SM4rel4in’s documentation!

# Install

```bash
sudo pip install SM4rel4in
```

or

```bash
sudo pip3 install SMm4rel4in
```

# Update

```bash
sudo pip install SM4rel4in -U
```

or

```bash
sudo pip3 install SM4rel4in -U
```

# Initiate class

```console
$ python
Python 3.11.8 (main, Feb 12 2024, 14:50:05) [GCC 13.2.1 20230801] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import sm_4rel4in
>>> rel = sm_4rel4in.SM4rel4in()
>>> rel.get_all_relays(1, 10)
1
>>>
```

# Documentation

<a id="module-sm_4rel4in"></a>

### *class* sm_4rel4in.SM4rel4in(stack=0, i2c=1)

Bases: `object`

#### get_ac_in(channel)

Read the filtered digital inputs. The filter cuts 50Hz so you can detect AC signals presence.

* **Parameters:**
  **channel** (*int*) – Input channel [1..4]
* **Returns:**
  The presence of an AC or DC signal on the input (0/1)
* **Return type:**
  int

#### get_all_ac_in()

Read all the inputs, filtered.

* **Returns:**
  The state of all inputs as a bitmap [0..15]
* **Return type:**
  int

#### get_all_in()

Read the state of all digital input channels.

* **Returns:**
  The state of all inputs as a 4 bits value
* **Return type:**
  int

#### get_all_leds()

Read the state of all LEDs.

* **Returns:**
  The state of all 4 LEDs [0..15]
* **Return type:**
  int

#### get_all_relays()

Read the state of all 4 relays.

* **Returns:**
  Relays state as bitmap [0..15]
* **Return type:**
  int

#### get_count(channel)

Read the counter for the specified channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]
* **Returns:**
  The counter as an unsigned 32 bits integer
* **Return type:**
  int

#### get_count_cfg(channel)

Read the counting function activation state.

* **Parameters:**
  **channel** (*int*) – Input channel [1..4]
* **Returns:**
  1 if the counting function for specified channel is enabled, 0 if not
* **Return type:**
  int

#### get_crt(channel)

Read the current(A) for one relay output channel.

* **Parameters:**
  **channel** (*int*) – Relay channel number [1..4]
* **Returns:**
  Current value in amperes(A)
* **Return type:**
  float

#### get_crt_rms(channel)

Read the RMS current(A) value for one relay output channel.

* **Parameters:**
  **channel** (*int*) – Relay channel number [1..4]
* **Returns:**
  RMS current value in amperes(A)
* **Return type:**
  float

#### get_encoder(channel)

Get the encoder count as 32 bits signed integer.

* **Parameters:**
  **channel** (*int*) – Quadrature encoder number [1..2]
* **Returns:**
  Encoder value
* **Return type:**
  int

#### get_encoder_cfg(channel)

Read the quadrature encoder settings. The card supports two quadrature encoders one on inputs 1 and 2 and one on inputs 3 and 4.

* **Parameters:**
  **channel** (*int*) – Quadrature encoder number [1..2]
* **Returns:**
  0 encoder disabled, 1 encoder enabled
* **Return type:**
  int

#### get_frequency(channel)

Read the frequency in Hz of a PWM signal fed in the specified input channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]
* **Returns:**
  Frequency in Hz
* **Return type:**
  int

#### get_in(channel)

Read the state of one digital input channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]
* **Returns:**
  The state of the input 0/1
* **Return type:**
  int

#### get_led(led)

Read the state of the corresponding LED.

* **Parameters:**
  **led** (*int*) – LED number [1..4]
* **Returns:**
  0 - LED is off, 1 - LED is on
* **Return type:**
  int

#### get_led_cfg(led)

Read the configuration for LEDs on the card. Four of the card LEDs are dual purpose, they can follow the inputs state (auto mode) or can be software controlled (manual mode).

* **Parameters:**
  **led** (*int*) – LED number [1..4]
* **Returns:**
  0 LED follow the input state (auto), 1 LED can be controlled in software (manual)
* **Return type:**
  int

#### get_pps(channel)

Read the speed or pulse per second for specified input channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]
* **Returns:**
  Pulse per seconds as 16bits unsigned integer
* **Return type:**
  int

#### get_pwm_fill(channel)

Read the fill factor in percent of a PWM signal fed in the specified input channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]
* **Returns:**
  Fill factor in % with two decimal places
* **Return type:**
  float

#### get_relay(relay)

Read one relay state.

* **Parameters:**
  **relay** (*int*) – Relay number [1..4]
* **Returns:**
  The state of the relay 0 or 1
* **Return type:**
  int

#### reset_count(channel)

Reset the counter for the specified input channel.

* **Parameters:**
  **channel** (*int*) – Input channel number [1..4]

#### reset_encoder(channel)

Reset the specified encoder value.

* **Parameters:**
  **channel** (*int*) – Quadrature encoder number [1..2]

#### set_all_leds(val)

Set the state of all 4 LEDs.

* **Parameters:**
  **val** (*int*) – Bitmap of all 4 LEDs [0..15]

#### set_all_relays(val)

Set the state of all relays as a 4 bits bit-map.

* **Parameters:**
  **val** (*int*) – The new state of all 4 relays 0 = all off, 15 = all on

#### set_count_cfg(channel, state)

Turn ON or OFF the counting function for specified input channel.

* **Parameters:**
  * **channel** (*int*) – Input channel number [1..4]
  * **state** (*int*) – 0 - turn off; 1 turn on

#### set_encoder_cfg(channel, state)

Enable or disable quadrature encoder function for specified encoder channel.

* **Parameters:**
  * **channel** (*int*) – Quadrature encoder number [1..2]
  * **state** (*int*) – 0 - disable; 1 - enable

#### set_led(led, val)

Set the corresponding LED state (on/off).

* **Parameters:**
  * **led** (*int*) – LED number [1..4]
  * **val** (*int*) – 0 - LED off; 1 - LED on

#### set_led_cfg(led, mode)

Configure the corresponding LED working mode.

* **Parameters:**
  * **led** (*int*) – LED number [1..4]
  * **mode** (*int*) – 0 - auto; 1 - manual

#### set_relay(relay, val)

Set one relay state.

* **Parameters:**
  * **relay** (*int*) – The relay number [1..4]
  * **val** (*int*) – The new state of the relay 0 = turn off else turn on

<!-- vi:se ts=4 sw=4 et: -->
