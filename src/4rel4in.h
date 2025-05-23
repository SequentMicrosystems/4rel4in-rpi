#ifndef REL4IN4_H_
#define REL4IN4_H_

#include <stdint.h>

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#define CHANNEL_NR_MIN 1
#define RELAY_CH_NO 4
#define LED_CH_NO 4
#define IN_CH_NO 4
#define COUNT_SIZE 4
#define ENC_COUNT_SIZE 4
#define ENC_NO 2
#define SCAN_FREQ_SIZE 2
#define PWM_IN_FILL_SIZE 2
#define PWM_IN_FILL_SCALE ((float)100)
#define IN_FREQENCY_SIZE 2
#define CRT_SIZE 2
#define ANALOG_VAL_SIZE 2
#define THERMISTOR_SIZE 2
#define CRT_SCALE ((float)1000)
#define RES_SCALE ((float)0.1)
#define TEMP_SCALE ((float)100)
#define RETRY_TIMES	10

#define CALIBRATION_KEY 0xaa
#define RESET_CALIBRATION_KEY 0x55
#define ZERO_CURRENT_KEY 0x11


enum
{
	I2C_MEM_RELAY_VAL = 0,
	I2C_MEM_RELAY_SET,
	I2C_MEM_RELAY_CLR,
	I2C_MEM_DIG_IN,
	I2C_MEM_AC_IN,
	I2C_MEM_LED_VAL,
	I2C_MEM_LED_SET,
	I2C_MEM_LED_CLR,
	I2C_MEM_LED_MODE, //0-auto, 1 - manual;
	I2C_MEM_EDGE_ENABLE,
	I2C_MEM_ENC_ENABLE,
	I2C_MEM_SCAN_FREQ,
	I2C_MEM_PULSE_COUNT_START = I2C_MEM_SCAN_FREQ + SCAN_FREQ_SIZE,
	I2C_MEM_PPS = I2C_MEM_PULSE_COUNT_START + (IN_CH_NO * COUNT_SIZE),
	I2C_MEM_ENC_COUNT_START = I2C_MEM_PPS + IN_CH_NO * IN_FREQENCY_SIZE,
	I2C_MEM_PWM_IN_FILL = I2C_MEM_ENC_COUNT_START + (ENC_NO * ENC_COUNT_SIZE),
	I2C_MEM_IN_FREQENCY = I2C_MEM_PWM_IN_FILL + (IN_CH_NO * PWM_IN_FILL_SIZE),
	I2C_MEM_IN_FREQENCY_END = I2C_MEM_IN_FREQENCY + (IN_CH_NO * IN_FREQENCY_SIZE)
		- 1,
	I2C_MEM_PULSE_COUNT_RESET,//2 bytes to be one modbus register
	I2C_MEM_ENC_COUNT_RESET = I2C_MEM_PULSE_COUNT_RESET + 2,//2 bytes to be one modbus register
	I2C_MODBUS_SETINGS_ADD = I2C_MEM_ENC_COUNT_RESET + 2,
	I2C_NBS1,
	I2C_MBS2,
	I2C_MBS3,
	I2C_MODBUS_ID_OFFSET_ADD,
	I2C_MEM_EXTI_ENABLE,
	I2C_MEM_BUTTON, //bit0 - state, bit1 - latch

	// INDUSTRIAL VERSION ADDED 1
	I2C_CRT_IN_VAL1_ADD , // current vales scaled as A/100 (1 = 0.01A) 16-bit signed integer
	I2C_CRT_IN_RMS_VAL1_ADD  = I2C_CRT_IN_VAL1_ADD + ANALOG_VAL_SIZE * IN_CH_NO, //current RMS values scaled as A/100 16bit unsigned integer
	I2C_MEM_CALIB_VALUE = I2C_CRT_IN_RMS_VAL1_ADD + ANALOG_VAL_SIZE * IN_CH_NO, // floating point value expressing the current in A
	I2C_MEM_CALIB_CHANNEL = I2C_MEM_CALIB_VALUE + 4,
	I2C_MEM_CALIB_KEY, //set calib point -> 0xaa; reset calibration on the channel -> 0x55; save zero current offset -> 0x11
	I2C_MEM_CALIB_STATUS,
	I2C_MEM_WDT_RESET_ADD,
	I2C_MEM_WDT_INTERVAL_SET_ADD,
	I2C_MEM_WDT_INTERVAL_GET_ADD = I2C_MEM_WDT_INTERVAL_SET_ADD + 2,
	I2C_MEM_WDT_INIT_INTERVAL_SET_ADD = I2C_MEM_WDT_INTERVAL_GET_ADD + 2,
	I2C_MEM_WDT_INIT_INTERVAL_GET_ADD = I2C_MEM_WDT_INIT_INTERVAL_SET_ADD + 2,
	I2C_MEM_WDT_RESET_COUNT_ADD = I2C_MEM_WDT_INIT_INTERVAL_GET_ADD + 2,
	I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD = I2C_MEM_WDT_RESET_COUNT_ADD + 2,
	I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD,
	I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD = I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD + 4,

	I2C_MEM_DIAG_RASP_V = I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD + 4,
	I2C_MEM_DIAG_RASP_V1,
	I2C_MEM_DIAG_SNS_VCC,
	I2C_MEM_DIAG_SNS_VCC1,
// END INDUSTRIAL VERSION ADDED 1


	I2C_MEM_REVISION_HW_MAJOR_ADD = 0x78,
	I2C_MEM_REVISION_HW_MINOR_ADD,
	I2C_MEM_REVISION_MAJOR_ADD,
	I2C_MEM_REVISION_MINOR_ADD,

	I2C_MEM_TH_RES_START_ADD,
	I2C_MEM_TH_RES_END_ADD = I2C_MEM_TH_RES_START_ADD + ANALOG_VAL_SIZE * IN_CH_NO,
	I2C_MEM_TH_TEMP_START_ADD = I2C_MEM_TH_RES_END_ADD,

	SLAVE_BUFF_SIZE = 256,
};

#define ERROR	-1
#define OK		0
#define FAIL	-1
#define ARG_CNT_ERR -2
#define ARG_RANGE_ERROR -3
#define IO_ERROR -4

#define SLAVE_OWN_ADDRESS_BASE 0x0e

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef enum
{
	OFF = 0,
	ON,
	STATE_COUNT
} OutStateEnumType;

typedef struct
{
	const char *name;
	const int namePos;
	int (*pFunc)(int, char**);
	const char *help;
	const char *usage1;
	const char *usage2;
	const char *example;
} CliCmdType;

typedef struct __attribute__((packed))
{
	unsigned int mbBaud :24;
	unsigned int mbType :4;
	unsigned int mbParity :2;
	unsigned int mbStopB :2;
	unsigned int add :8;
} ModbusSetingsType;
int doBoardInit(int stack);
int getHw(void);
// RS-485 CLI structures
extern const CliCmdType CMD_RS485_READ;
extern const CliCmdType CMD_RS485_WRITE;

extern const CliCmdType CMD_RELAY_TEST;
extern const CliCmdType CMD_RELAY_READ;
extern const CliCmdType CMD_RELAY_WRITE;

extern const CliCmdType CMD_IN_READ;
extern const CliCmdType CMD_AC_IN_READ;
extern const CliCmdType CMD_BL_READ;
extern const CliCmdType CMD_BUTTON_READ;
extern const CliCmdType CMD_CFG_COUNT_READ;
extern const CliCmdType CMD_CFG_COUNT_WRITE;
extern const CliCmdType CMD_COUNT_READ;
extern const CliCmdType CMD_COUNT_RESET;
extern const CliCmdType CMD_COUNT_PPS_READ;
extern const CliCmdType CMD_CFG_ENCODER_READ;
extern const CliCmdType CMD_CFG_ENCODER_WRITE;
extern const CliCmdType CMD_ENCODER_READ;
extern const CliCmdType CMD_ENCODER_RESET;
extern const CliCmdType CMD_PWM_READ;
extern const CliCmdType CMD_IN_FREQ_READ;
extern const CliCmdType CMD_CFG_EXTI_READ;
extern const CliCmdType CMD_CFG_EXTI_WRITE;
extern const CliCmdType CMD_CRT_READ;
extern const CliCmdType CMD_CRT_RMS_READ;
extern const CliCmdType CMD_CRT_OFFSET;
extern const CliCmdType CMD_CRT_CAL;
extern const CliCmdType CMD_CRT_CAL_RESET;
extern const CliCmdType CMD_THERM_RES_READ;
extern const CliCmdType CMD_THERM_TEMP_READ;
extern const CliCmdType *gCmdArray[];

#endif
