[![4rel4in-rpi](readmeres/sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The Sequent Microsystems [Four Relays four HV Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/four-relays-four-inputs-for-raspberry-pi) V 4.0 or higher can be accessed thru Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **4rel4in** command.

Example:
```bash
~$ 4rel4in 0 cfg485wr 1 9600 1 0 1
```
Set Modbus RTU , Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address base: 1
```bash
~$ 4rel4in -h cfg485wr
```
display the full set of options

## Slave Address
The slave address is add with the "stack level" jumpers. For example the jumpers configuration for stack level 1  (one jumper in position ID0) slave address offset to 1 corespond to slave address 2.

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address | Description |
| --- | --- | --- | --- |
|   RELAY1 | 0001 | 0x00 | Relay #1 control |
|	RELAY2|0002 | 0x01 | Relay #2 control |
|	RELAY3|0003 | 0x02 | Relay #3 control |
|	RELAY4|0004 | 0x03 | Relay #4 control |
|	LED1_MAN| 0005 | 0x04 | LED1 control type (0 copy the input state/ 1 manual)|
|	ED2_MAN|0006 | 0x05 | LED2 control type |
|	LED3_MAN|0007 | 0x06 | LED3 control type|
|	LED4_MAN|0008 | 0x07 | LED4 control type|
|	LED1|0009 | 0x08 | LED1  turn coil| 
|	LED2|0010 | 0x09 | LED2  turn coil|
|	LED3|0011 | 0x0a | LED3  turn coil|
|	LED4|0012 | 0x0b | LED4  turn coil|
|	IN1_COUNT_ENABLE|0013 | 0x0c | Counter enable on input #1|
|	IN2_COUNT_ENABLE|0014 | 0x0d |Counter enable on input #2|
|	IN3_COUNT_ENABLE|0015 | 0x0e |Counter enable on input #3|
|	IN4_COUNT_ENABLE|0016 | 0x0f |Counter enable on input #4|
|	ENCODER1_ENABLE|0017 | 0x10 | Encoder enable on inputs #1/2|
|	ENCODER2_ENABLE|0018|0x011| Encoder enable on inputs #3/4|

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address | Description |
| --- | --- | --- | --- |
| IN_1 | 10001 | 0x00 | input #1 state|
| IN_2 | 10002 | 0x01 |input #2 state|
| IN_3 | 10003 | 0x02 |input #3 state|
| IN_4 | 10004 | 0x03 |input #4 state|
| AC_IN_1 | 10005 | 0x04 | filtred input #1 state (sense AC signals)|
| AC_IN_2 | 10006 | 0x05 |filtred input #2 state|
| AC_IN_3 | 10007 | 0x06 |filtred input #3 state|
| AC_IN_4 | 10008 | 0x07 |filtred input #4 state|
| BUTTON | 10008 | 0x07 |Push button state|


### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Description |
| --- | --- | --- | --- | --- |
| COUNT1_L| 30001 | 0x00 |  | Input #1 low 16bits value| 
| COUNT1_H| 30002 | 0x01 |  | Input #1 high 16bits value| 
| COUNT2_L| 30003 | 0x02 |  | Input #2 low 16 bits value| 
| COUNT2_H| 30004 | 0x03 |  |Input #2 high 16bits value| 
| COUNT3_L| 30005 | 0x04 |  |Input #3 low 16bits value| 
| COUNT3_H| 30006 | 0x05 |  |Input #3 high 16bits value| 
| COUNT4_L| 30007 | 0x06 |  |Input #4 low 16bits value| 
| COUNT4_H| 30008 | 0x07 |  |Input #4 high 16bits value| 
| PPS1| 30009 | 0x08 | Hz | Input #1 conts per second |
| PPS2| 30010 | 0x09 | Hz | Input #2 conts per second |
| PPS3| 30011 | 0x0a | Hz | Input #3 conts per second |
| PPS4| 30012 | 0x0b | Hz | Input #4 conts per second |
| ENC_VAL1_L| 30013 | 0x0c |  | Encoder #1 counter low 16bits|
| ENC_VAL1_H| 30014 | 0x0d |  |Encoder #1 counter high 16bits|
| ENC_VAL2_L| 30015 | 0x0e |  |Encoder #2 counter low 16bits|
| ENC_VAL2_H| 30016 | 0x0f |  |Encoder #2 counter high 16bits|
| PWM1_FILL| 30017 | 0x10 | %/100 | Fill factor for PWM input on channel 1|
| PWM2_FILL| 30018 | 0x11 | %/100 | Fill factor for PWM input on channel 2|
| PWM3_FILL| 30019 | 0x12 | %/100 | Fill factor for PWM input on channel 3|
| PWM4_FILL| 30020 | 0x13 | %/100 | Fill factor for PWM input on channel 4|
| PWM1_FREQ| 30021 | 0x14 | Hz | Frequency for PWM input on channel 1 |
| PWM2_FREQ| 30022 | 0x15 | Hz | Frequency for PWM input on channel 1 |
| PWM3_FREQ| 30023 | 0x16 | Hz | Frequency for PWM input on channel 1 |
| PWM4_FREQ| 30024 | 0x17 | Hz | Frequency for PWM input on channel 1 |


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range | Description |
| --- | --- | --- | --- | --- | --- |
| RESET_COUNT_CHANNEL | 40001 | 0x00 |  | 1..4 | Reset counter for specified input number |
| HR_RESET_ENCODER_CHANNEL | 40002 | 0x01 |  | 1..2 | Reset counter for specified Encoder number|


## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
