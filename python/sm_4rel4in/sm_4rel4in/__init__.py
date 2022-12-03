import smbus2

__version__ = "1.0.4"
_CARD_BASE_ADDRESS = 0x0e
_STACK_LEVEL_MAX = 7
_IN_CH_COUNT = 4
_RELAY_COUNT = 4
_ENC_CH_COUNT = 2
_FREQ_SIZE_BYTES = 2
_COUNT_SIZE_BYTES = 4

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

LED_AUTO = 0
LED_MANUAL = 1


class SM4rel4in:

    def __init__(self, stack=0, i2c=1):
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

    def set_relay(self, relay, val):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_RELAY_VAL, 0x0f & val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_relay(self, relay):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_RELAY_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_in(self, channel):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_DIG_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_ac_in(self, channel):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_AC_IN)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

    def get_count_cfg(self, channel):
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

    def get_led_cfg(self, led):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            bus.write_byte_data(self._hw_address_, _I2C_MEM_LED_VAL, 0x0f & val)
        except Exception as e:
            bus.close()
            raise Exception("Fail to write with exception " + str(e))
        bus.close()

    def get_led(self, led):
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
        bus = smbus2.SMBus(self._i2c_bus_no)
        try:
            val = bus.read_byte_data(self._hw_address_, _I2C_MEM_LED_VAL)
        except Exception as e:
            bus.close()
            raise Exception("Fail to read with exception " + str(e))
        bus.close()
        return val

