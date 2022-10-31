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
#define RETRY_TIMES	10


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


	I2C_MEM_REVISION_HW_MAJOR_ADD = 0x78,
	I2C_MEM_REVISION_HW_MINOR_ADD,
	I2C_MEM_REVISION_MAJOR_ADD,
	I2C_MEM_REVISION_MINOR_ADD,

	SLAVE_BUFF_SIZE,
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
// RS-485 CLI structures
extern const CliCmdType CMD_RS485_READ;
extern const CliCmdType CMD_RS485_WRITE;

extern const CliCmdType CMD_RELAY_TEST;
extern const CliCmdType CMD_RELAY_READ;
extern const CliCmdType CMD_RELAY_WRITE;

extern const CliCmdType CMD_IN_READ;
extern const CliCmdType CMD_AC_IN_READ;
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
extern const CliCmdType *gCmdArray[];

#endif
