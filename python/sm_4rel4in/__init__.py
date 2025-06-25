import struct
import smbus2

__version__ = "1.0.5"
_CARD_BASE_ADDRESS = 0x0e
_STACK_LEVEL_MAX = 7
_IN_CH_COUNT = 4
_RELAY_COUNT = 4
_ENC_CH_COUNT = 2
_FREQ_SIZE_BYTES = 2
_COUNT_SIZE_BYTES = 4
_CRT_SIZE = 2

_I2C_MEM_REVISION_HW_MAJOR_ADD = 122
_I2C_MEM_REVISION_HW_MINOR_ADD = 123
_I2C_MEM_RELAY_VAL = 0x00
_I2C_MEM_RELAY_SET = 0x01
_I2C_MEM_RELAY_CLR = 0x02
_I2C_MEM_DIG_IN = 0x03
_I2C_MEM_AC_IN = 0x04
_I2C_MEM_LED_VAL = 5
_I2C_MEM_LED_SET = 6
_I2C_MEM_LED_CLR = 7
_I2C_MEM_LED_MODE = 8  # //0-auto, 1 - manual;
_I2C_MEM_EDGE_ENABLE = 9
_I2C_MEM_ENC_ENABLE = 10
_I2C_MEM_PULSE_COUNT_START = 13
_I2C_MEM_ENC_COUNT_START = 37
_I2C_MEM_PPS = 29
_I2C_MEM_PULSE_COUNT_RESET = 61
_I2C_MEM_ENC_COUNT_RESET = 63
_I2C_MEM_PWM_IN_FILL = 45
_I2C_MEM_IN_FREQUENCY = 53
_I2C_CRT_IN = 72
_I2C_CRT_IN_RMS = 80
_CRT_SCALE = 1000.0

LED_AUTO = 0
LED_MANUAL = 1


class SM4rel4in:
    def __init__(self, stack=0, i2c=1):
        """Initialize the SM4rel4in object and check the card presence.

        Args:
            stack (int): Card stack level [0..7] set by the jumpers
            i2c (int): I2C port number, 1 - Raspberry default, 7 - rock pi 4, etc.
        """
        if stack < 0 or stack > _STACK_LEVEL_MAX:
            raise ValueError('Invalid stack level!')
        self._hw_address_ = _CARD_BASE_ADDRESS + stack
        self._i2c_bus_no = i2c
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            self._card_rev_major = bus.read_byte_data(self._hw_address_, _I2C_MEM_REVISION_HW_MAJOR_ADD)
            self._card_rev_minor = bus.read_byte_data(self._hw_address_, _I2C_MEM_REVISION_HW_MINOR_ADD)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()

    def _get_i16(self, address):
        bus = smbus2.SMBus(self._i2c_bus_no)
        buf = bus.read_i2c_block_data(self._hw_address_, address, 2)
        i16_value = struct.unpack("h", bytearray(buf))[0]
        bus.close()
        return i16_value

    def set_relay(self, relay, val):
        """Set one relay state.

        Args:
            relay (int): The relay number [1..4]
            val (int): The new state of the relay 0 = turn off else turn on
        """
        if relay < 1 or relay > _RELAY_COUNT:
            raise ValueError('Invalid relay number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            if val != 0:
                bus.write_byte_data(self._hw_address_, _I2C_MEM_RELAY_SET, relay)
            else:
                bus.write_byte_data(self._hw_address_, _I2C_MEM_RELAY_CLR, relay)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def set_all_relays(self, val):
        """Set the state of all relays as a 4 bits bit-map.

        Args:
            val (int): The new state of all 4 relays 0 = all off, 15 = all on
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_RELAY_VAL, 0x0f & val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_relay(self, relay):
        """Read one relay state.

        Args:
            relay (int): Relay number [1..4]

        Returns:
            int: The state of the relay 0 or 1
        """
        if relay < 1 or relay > _RELAY_COUNT:
            raise ValueError('Invalid relay number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_RELAY_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if (val & (1 << (relay - 1))) != 0:
            return 1
        return 0

    def get_all_relays(self):
        """Read the state of all 4 relays.

        Returns:
            int: Relays state as bitmap [0..15]
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_RELAY_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_in(self, channel):
        """Read the state of one digital input channel.

        Args:
            channel (int): Input channel number [1..4]

        Returns:
            int: The state of the input 0/1
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_DIG_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if (val & (1 << (channel - 1))) != 0:
            return 1
        return 0

    def get_all_in(self):
        """Read the state of all digital input channels.

        Returns:
            int: The state of all inputs as a 4 bits value
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_DIG_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_ac_in(self, channel):
        """Read the filtered digital inputs. The filter cuts 50Hz so you can detect AC signals presence.

        Args:
            channel (int): Input channel [1..4]

        Returns:
            int: The presence of an AC or DC signal on the input (0/1)
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_AC_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if (val & (1 << (channel - 1))) != 0:
            return 1
        return 0

    def get_all_ac_in(self):
        """Read all the inputs, filtered.

        Returns:
            int: The state of all inputs as a bitmap [0..15]
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_AC_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_count_cfg(self, channel):
        """Read the counting function activation state.

        Args:
            channel (int): Input channel [1..4]

        Returns:
            int: 1 if the counting function for specified channel is enabled, 0 if not
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_EDGE_ENABLE)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if (val & (1 << (channel - 1))) != 0:
            return 1
        return 0

    def set_count_cfg(self, channel, state):
        """Turn ON or OFF the counting function for specified input channel.

        Args:
            channel (int): Input channel number [1..4]
            state (int): 0 - turn off; 1 turn on
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_EDGE_ENABLE)
            if state != 0:
                val |= 1 << (channel - 1)
            else:
                val &= ~(1 << (channel - 1))
            bus.write_byte_data(self._hw_address_, _I2C_MEM_EDGE_ENABLE, val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()

    def get_count(self, channel):
        """Read the counter for the specified channel.

        Args:
            channel (int): Input channel number [1..4]

        Returns:
            int: The counter as an unsigned 32 bits integer
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            buff = bus.read_i2c_block_data(self._hw_address_,
                                           _I2C_MEM_PULSE_COUNT_START + (channel - 1) * _COUNT_SIZE_BYTES,
                                           _COUNT_SIZE_BYTES)
            count = int.from_bytes(buff, "little")
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return count

    def reset_count(self, channel):
        """Reset the counter for the specified input channel.

        Args:
            channel (int): Input channel number [1..4]
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_PULSE_COUNT_RESET, channel)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_pps(self, channel):
        """Read the speed or pulse per second for specified input channel.

        Args:
            channel (int): Input channel number [1..4]

        Returns:
            int: Pulse per seconds as 16bits unsigned integer
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_word_data(self._hw_address_, _I2C_MEM_PPS + (channel - 1) * _FREQ_SIZE_BYTES)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_encoder_cfg(self, channel):
        """Read the quadrature encoder settings. The card supports two quadrature encoders one on inputs 1 and 2 and one on inputs 3 and 4.

        Args:
            channel (int): Quadrature encoder number [1..2]

        Returns:
            int: 0 encoder disabled, 1 encoder enabled
        """
        if channel < 1 or channel > _ENC_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..2]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_ENC_ENABLE)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if (val & (1 << (channel - 1))) != 0:
            return 1
        return 0

    def set_encoder_cfg(self, channel, state):
        """Enable or disable quadrature encoder function for specified encoder channel.

        Args:
            channel (int): Quadrature encoder number [1..2]
            state (int): 0 - disable; 1 - enable
        """
        if channel < 1 or channel > _ENC_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..2]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_ENC_ENABLE)
            if state != 0:
                val |= 1 << (channel - 1)
            else:
                val &= ~(1 << (channel - 1))
            bus.write_byte_data(self._hw_address_, _I2C_MEM_ENC_ENABLE, val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()

    def get_encoder(self, channel):
        """Get the encoder count as 32 bits signed integer.

        Args:
            channel (int): Quadrature encoder number [1..2]

        Returns:
            int: Encoder value
        """
        if channel < 1 or channel > _ENC_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..2]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            buff = bus.read_i2c_block_data(self._hw_address_,
                                           _I2C_MEM_ENC_COUNT_START + (channel - 1) * _COUNT_SIZE_BYTES,
                                           _COUNT_SIZE_BYTES)
            count = int.from_bytes(buff, "little", signed=True)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return count

    def reset_encoder(self, channel):
        """Reset the specified encoder value.

        Args:
            channel (int): Quadrature encoder number [1..2]
        """
        if channel < 1 or channel > _ENC_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..2]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_ENC_COUNT_RESET, channel)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_frequency(self, channel):
        """Read the frequency in Hz of a PWM signal fed in the specified input channel.

        Args:
            channel (int): Input channel number [1..4]

        Returns:
            int: Frequency in Hz
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_word_data(self._hw_address_, _I2C_MEM_IN_FREQUENCY + (channel - 1) * _FREQ_SIZE_BYTES)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_pwm_fill(self, channel):
        """Read the fill factor in percent of a PWM signal fed in the specified input channel.

        Args:
            channel (int): Input channel number [1..4]

        Returns:
            float: Fill factor in % with two decimal places
        """
        if channel < 1 or channel > _IN_CH_COUNT:
            raise ValueError('Invalid input channel number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_word_data(self._hw_address_, _I2C_MEM_PWM_IN_FILL + (channel - 1) * _FREQ_SIZE_BYTES)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val/100
    
    def get_crt(self, channel):
        """Read the current(A) for one relay output channel.

        Args:
            channel (int): Relay channel number [1..4]

        Returns:
            float: Current value in amperes(A)
        """
        if channel < 1 or channel > _RELAY_COUNT:
            raise ValueError('Invalid relay number, number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = self._get_i16(_I2C_CRT_IN + (channel - 1) * _CRT_SIZE)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return float(val) / _CRT_SCALE
    
    def get_crt_rms(self, channel):
        """Read the RMS current(A) value for one relay output channel.

        Args:
            channel (int): Relay channel number [1..4]

        Returns:
            float: RMS current value in amperes(A)
        """
        if channel < 1 or channel > _RELAY_COUNT:
            raise ValueError('Invalid relay number, number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = self._get_i16(_I2C_CRT_IN_RMS + (channel - 1) * _CRT_SIZE)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return float(val) / _CRT_SCALE

    def get_led_cfg(self, led):
        """Read the configuration for LEDs on the card. Four of the card LEDs are dual purpose, they can follow the inputs state (auto mode) or can be software controlled (manual mode).

        Args:
            led (int): LED number [1..4]

        Returns:
            int: 0 LED follow the input state (auto), 1 LED can be controlled in software (manual)
        """
        if led < 1 or led > _IN_CH_COUNT:
            raise ValueError('Invalid led number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_LED_MODE)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if val & (1 << (led - 1)) != 0:
            return 1
        return 0

    def set_led_cfg(self, led, mode):
        """Configure the corresponding LED working mode.

        Args:
            led (int): LED number [1..4]
            mode (int): 0 - auto; 1 - manual
        """
        if led < 1 or led > _IN_CH_COUNT:
            raise ValueError('Invalid led number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_LED_MODE)
            if mode == 0:
                val &= ~(1 << (led - 1))
            else:
                val |= 1 << (led - 1)
            bus.write_byte_data(self._hw_address_, _I2C_MEM_LED_MODE, val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def set_led(self, led, val):
        """Set the corresponding LED state (on/off).

        Args:
            led (int): LED number [1..4]
            val (int): 0 - LED off; 1 - LED on
        """
        if led < 1 or led > _IN_CH_COUNT:
            raise ValueError('Invalid led number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            if val != 0:
                bus.write_byte_data(self._hw_address_, _I2C_MEM_LED_SET, led)
            else:
                bus.write_byte_data(self._hw_address_, _I2C_MEM_LED_CLR, led)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def set_all_leds(self, val):
        """Set the state of all 4 LEDs.

        Args:
            val (int): Bitmap of all 4 LEDs [0..15]
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_LED_VAL, 0x0f & val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_led(self, led):
        """Read the state of the corresponding LED.

        Args:
            led (int): LED number [1..4]

        Returns:
            int: 0 - LED is off, 1 - LED is on
        """
        if led < 1 or led > _IN_CH_COUNT:
            raise ValueError('Invalid led number number must be [1..4]!')
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_LED_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        if val & (1 << (led - 1)) != 0:
            return 1
        return 0

    def get_all_leds(self):
        """Read the state of all LEDs.

        Returns:
            int: The state of all 4 LEDs [0..15]
        """
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_LED_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

