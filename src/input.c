#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "4rel4in.h"
#include "comm.h"
#include "thread.h"

typedef enum
{
	COUNT_EDGES = 0,
	COUNT_ENCODER,
} COUNT_TYPE;

int inChGet(int dev, u8 channel, OutStateEnumType *state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > IN_CH_NO))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DIG_IN, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int inGet(int dev, int *val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_DIG_IN, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

int acInChGet(int dev, u8 channel, OutStateEnumType *state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > IN_CH_NO))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_AC_IN, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int acInGet(int dev, int *val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_AC_IN, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static int doInRead(int argc, char *argv[]);
const CliCmdType CMD_IN_READ =
{
	"inrd",
	2,
	&doInRead,
	"\tinrd:		Read inputs  status\n",
	"\tUsage:		4rel4in <stack> inrd <channel[1..4]>\n",
	"\tUsage:		4rel4in <stack> inrd\n",
	"\tExample:		4rel4in 0 inrd 2; Read Status of Input channel #2 \n"};

static int doInRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != inChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
		return OK;
	}
	else if (argc == 3)
	{
		if (OK != inGet(dev, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doAcInRead(int argc, char *argv[]);
const CliCmdType CMD_AC_IN_READ =
{
	"acinrd",
	2,
	&doAcInRead,
	"\tacinrd:		Read inputs status when the signal is AC voltage\n",
	"\tUsage:		4rel4in <stack> acinrd <channel[1..4]>\n",
	"\tUsage:		4rel4in <stack> acinrd\n",
	"\tExample:		4rel4in 0 acinrd 2; Read Status of Input channel #2 \n"};

static int doAcInRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != acInChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
		return OK;
	}
	else if (argc == 3)
	{
		if (OK != acInGet(dev, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

//==================== COUNTERS ==========================

int cfgCntChGet(int dev, u8 channel, OutStateEnumType *state, COUNT_TYPE type)
{
	u8 buff[2];
	int chMax = IN_CH_NO;
	int add = I2C_MEM_EDGE_ENABLE;

	if (NULL == state)
	{
		return ERROR;
	}

	if (COUNT_ENCODER == type)
	{
		chMax = ENC_NO;
		add = I2C_MEM_ENC_ENABLE;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > chMax))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, add, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int cfgCntGet(int dev, int *val, COUNT_TYPE type)
{
	u8 buff[2];
	int add = I2C_MEM_EDGE_ENABLE;

	if (NULL == val)
	{
		return ERROR;
	}
	if (COUNT_ENCODER == type)
	{
		add = I2C_MEM_ENC_ENABLE;
	}
	if (OK != i2cMem8Read(dev, add, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

int cfgCntChSet(int dev, u8 channel, OutStateEnumType state, COUNT_TYPE type)
{
	u8 buff[2];
	int chMax = IN_CH_NO;
	int add = I2C_MEM_EDGE_ENABLE;

	if (COUNT_ENCODER == type)
	{
		chMax = ENC_NO;
		add = I2C_MEM_ENC_ENABLE;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > chMax))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, add, buff, 1))
	{
		return ERROR;
	}
	if (state)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (u8)(1 << (channel - 1));
	}

	return i2cMem8Write(dev, add, buff, 1);
}

int cfgCntSet(int dev, int val, COUNT_TYPE type)
{
	u8 buff[2];
	int add = I2C_MEM_EDGE_ENABLE;

	if (COUNT_ENCODER == type)
	{
		add = I2C_MEM_ENC_ENABLE;
	}
	buff[0] = (u8)val;
	return i2cMem8Write(dev, add, buff, 1);

}

int cntGet(int dev, u8 channel, int *val, COUNT_TYPE type)
{
	u8 buff[COUNT_SIZE];
	uint32_t uContor = 0;
	int32_t contor = 0;
	int chMax = IN_CH_NO;
	int add = I2C_MEM_PULSE_COUNT_START;

	if (COUNT_ENCODER == type)
	{
		chMax = ENC_NO;
		add = I2C_MEM_ENC_COUNT_START;
	}
	if (NULL == val)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > chMax))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}
	if (OK
		!= i2cMem8Read(dev, add + (channel - 1) * COUNT_SIZE, buff, COUNT_SIZE))
	{
		return ERROR;
	}
	if (COUNT_ENCODER == type)
	{
		memcpy(&contor, buff, COUNT_SIZE);
		*val = contor;
	}
	else
	{
		memcpy(&uContor, buff, COUNT_SIZE);
		*val = uContor;
	}

	return OK;
}

int cntReset(int dev, u8 channel, COUNT_TYPE type)
{
	u8 buff[2];
	int chMax = IN_CH_NO;
	int add = I2C_MEM_PULSE_COUNT_RESET;

	if (COUNT_ENCODER == type)
	{
		chMax = ENC_NO;
		add = I2C_MEM_ENC_COUNT_RESET;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > chMax))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}

	buff[0] = channel;
	return i2cMem8Write(dev, add, buff, 1);
}

int cntPpsGet(int dev, u8 channel, int *val)
{
	u8 buff[COUNT_SIZE];
	uint16_t uContor = 0;

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > IN_CH_NO))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}
	if (OK
		!= i2cMem8Read(dev, I2C_MEM_PPS + (channel - 1) * IN_FREQENCY_SIZE, buff,
			IN_FREQENCY_SIZE))
	{
		return ERROR;
	}
	memcpy(&uContor, buff, IN_FREQENCY_SIZE);
	*val = uContor;
	return OK;
}

static int doCfgCountRead(int argc, char *argv[]);
const CliCmdType CMD_CFG_COUNT_READ =
	{
		"cfgcntrd",
		2,
		&doCfgCountRead,
		"\tcfgcntrd:		Read the configuration of the count function\n",
		"\tUsage:		4rel4in <stack> cfgcntrd <channel[1..4]>\n",
		"\tUsage:		4rel4in <stack> cfgcntrd\n",
		"\tExample:		4rel4in 0 cfgcntrd 2; Readconfiguration for counting on Input channel #2 coud be 0/1 -> disabled/enabled \n"};

static int doCfgCountRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cfgCntChGet(dev, pin, &state, COUNT_EDGES))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
		return OK;
	}
	else if (argc == 3)
	{
		if (OK != cfgCntGet(dev, &val, COUNT_EDGES))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doCfgCountWrite(int argc, char *argv[]);
const CliCmdType CMD_CFG_COUNT_WRITE =
	{
		"cfgcntwr",
		2,
		&doCfgCountWrite,
		"\tcfgcntwr:		Set counting function On/Off\n",
		"\tUsage:		4rel4in <stack> cfgcntwr <channel[1..4]> <0/1>\n",
		"\tUsage:		4rel4in <stack> cfgcntwr <value[0..15]>\n",
		"\tExample:		4rel4in 0 cfgcntwr 2 1; Enable counting function on input channel #2\n"};

static int doCfgCountWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;

	if ( (argc != 4) && (argc != 5))
	{
		return ARG_CNT_ERR;
	}

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 5)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number out of range\n");
			return ARG_RANGE_ERROR;
		}

		if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
		{
			printf("Invalid state for counting enable (0/1)!\n");
			return ARG_RANGE_ERROR;
		}
		state = (OutStateEnumType)atoi(argv[4]);

		if (OK != cfgCntChSet(dev, pin, state, COUNT_EDGES))
		{
			printf("Fail to write counting config! \n");
			return IO_ERROR;
		}

		return OK;
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid config value [0-15]!\n");
			return ARG_RANGE_ERROR;
		}

		if (OK != cfgCntSet(dev, val, COUNT_EDGES))
		{
			printf("Fail to write configuration register!\n");
			return IO_ERROR;
		}

		return OK;
	}
	return ARG_CNT_ERR;
}

static int doCountRead(int argc, char *argv[]);
const CliCmdType CMD_COUNT_READ =
{
	"cntrd",
	2,
	&doCountRead,
	"\tcntrd:		Read the counter of one input channel\n",
	"\tUsage:		4rel4in <stack> cntrd <channel[1..4]>\n",
	"",
	"\tExample:		4rel4in 0 cntrd 2; Read conter for Input channel #2  \n"};

static int doCountRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntGet(dev, pin, &val, COUNT_EDGES))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}

		printf("%d\n", val);

		return OK;
	}

	return ARG_CNT_ERR;
}

static int doCountReset(int argc, char *argv[]);
const CliCmdType CMD_COUNT_RESET =
{
	"cntrst",
	2,
	&doCountReset,
	"\tcntrst:		Reset the counter of one input channel\n",
	"\tUsage:		4rel4in <stack> cntrst <channel[1..4]>\n",
	"",
	"\tExample:		4rel4in 0 cntrst 2; Reset the conter for Input channel #2\n"};

static int doCountReset(int argc, char *argv[])
{
	int pin = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntReset(dev, pin, COUNT_EDGES))
		{
			printf("Fail to write!\n");
			return IO_ERROR;
		}

		return OK;
	}

	return ARG_CNT_ERR;
}

static int doCountPPSRead(int argc, char *argv[]);
const CliCmdType CMD_COUNT_PPS_READ =
	{
		"cntppsrd",
		2,
		&doCountPPSRead,
		"\tcntppsrd:		Read the counter pulse per second of one input channel\n",
		"\tUsage:		4rel4in <stack> cntppsrd <channel[1..4]>\n",
		"",
		"\tExample:		4rel4in 0 cntppsrd 2; Read pulse per second for Input channel #2  \n"};

static int doCountPPSRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntPpsGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}

		printf("%d\n", val);

		return OK;
	}

	return ARG_CNT_ERR;
}

// ================= ENCODER commands ==============================

static int doCfgEncoderRead(int argc, char *argv[]);
const CliCmdType CMD_CFG_ENCODER_READ =
	{
		"cfgencrd",
		2,
		&doCfgEncoderRead,
		"\tcfgencrd:		Read the configuration of the quadrature encoder function\n",
		"\tUsage:		4rel4in <stack> cfgencrd <channel[1..2]>\n",
		"\tUsage:		4rel4in <stack> cfgencrd\n",
		"\tExample:		4rel4in 0 cfgcntrd 2; Read configuration for encoder channel #2 (in ch #3, #4)  coud be 0/1 -> disabled/enabled \n"};

static int doCfgEncoderRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO / 2))
		{
			printf("Encoder channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cfgCntChGet(dev, pin, &state, COUNT_ENCODER))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
		return OK;
	}
	else if (argc == 3)
	{
		if (OK != cfgCntGet(dev, &val, COUNT_ENCODER))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doCfgEncoderWrite(int argc, char *argv[]);
const CliCmdType CMD_CFG_ENCODER_WRITE =
	{
		"cfgencwr",
		2,
		&doCfgEncoderWrite,
		"\tcfgencwr:		Set quadrature encoder function On/Off\n",
		"\tUsage:		4rel4in <stack> cfgencwr <channel[1..2]> <0/1>\n",
		"\tUsage:		4rel4in <stack> cfgencwr <value[0..3]>\n",
		"\tExample:		4rel4in 0 cfgencwr 2 1; Enable encoder function on channel #2 (in ch 1&2\n"};

static int doCfgEncoderWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;

	if ( (argc != 4) && (argc != 5))
	{
		return ARG_CNT_ERR;
	}

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 5)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO / 2))
		{
			printf("Input channel number out of range\n");
			return ARG_RANGE_ERROR;
		}

		if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
		{
			printf("Invalid state for encoder enable (0/1)!\n");
			return ARG_RANGE_ERROR;
		}
		state = (OutStateEnumType)atoi(argv[4]);

		if (OK != cfgCntChSet(dev, pin, state, COUNT_ENCODER))
		{
			printf("Fail to write encoder config! \n");
			return IO_ERROR;
		}

		return OK;
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x03)
		{
			printf("Invalid config value [0-3]!\n");
			return ARG_RANGE_ERROR;
		}

		if (OK != cfgCntSet(dev, val, COUNT_ENCODER))
		{
			printf("Fail to write configuration register!\n");
			return IO_ERROR;
		}

		return OK;
	}
	return ARG_CNT_ERR;
}

static int doEncoderRead(int argc, char *argv[]);
const CliCmdType CMD_ENCODER_READ =
{
	"encrd",
	2,
	&doEncoderRead,
	"\tencrd:		Read the counter of one encoder channel\n",
	"\tUsage:		4rel4in <stack> encrd <channel[1..2]>\n",
	"",
	"\tExample:		4rel4in 0 encrd 2; Read conter for encoder channel #2  \n"};

static int doEncoderRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO / 2))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntGet(dev, pin, &val, COUNT_ENCODER))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}

		printf("%d\n", val);

		return OK;
	}

	return ARG_CNT_ERR;
}

static int doEncoderReset(int argc, char *argv[]);
const CliCmdType CMD_ENCODER_RESET =
	{
		"encrst",
		2,
		&doEncoderReset,
		"\tencrst:		Reset the counter of one encoder input channel\n",
		"\tUsage:		4rel4in <stack> encrst <channel[1..2]>\n",
		"",
		"\tExample:		4rel4in 0 encrst 2; Reset the conter for encoder input channel #2\n"};

static int doEncoderReset(int argc, char *argv[])
{
	int pin = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO / 2))
		{
			printf("Encoder channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntReset(dev, pin, COUNT_ENCODER))
		{
			printf("Fail to write!\n");
			return IO_ERROR;
		}

		return OK;
	}

	return ARG_CNT_ERR;
}
//================== PWM IN ====================================
int cntPwmFillGet(int dev, u8 channel, float *val)
{
	u8 buff[COUNT_SIZE];
	uint16_t uContor = 0;

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > IN_CH_NO))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}
	if (OK
		!= i2cMem8Read(dev, I2C_MEM_PWM_IN_FILL + (channel - 1) * PWM_IN_FILL_SIZE, buff,
			PWM_IN_FILL_SIZE))
	{
		return ERROR;
	}
	memcpy(&uContor, buff, PWM_IN_FILL_SIZE);
	*val = (float)uContor / PWM_IN_FILL_SCALE;
	return OK;
}

int cntPwmFreqGet(int dev, u8 channel, int *val)
{
	u8 buff[COUNT_SIZE];
	uint16_t uContor = 0;

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > IN_CH_NO))
	{
		printf("Invalid input nr!\n");
		return ERROR;
	}
	if (OK
		!= i2cMem8Read(dev, I2C_MEM_IN_FREQENCY + (channel - 1) * IN_FREQENCY_SIZE, buff,
			IN_FREQENCY_SIZE))
	{
		return ERROR;
	}
	memcpy(&uContor, buff, IN_FREQENCY_SIZE);
	*val = uContor;
	return OK;
}


static int doPwmFillRead(int argc, char *argv[]);
const CliCmdType CMD_PWM_READ =
	{
		"pwmrd",
		2,
		&doPwmFillRead,
		"\tpwmrd:		Read the pwm fill factor(%) for one input channel\n",
		"\tUsage:		4rel4in <stack> pwmrd <channel[1..4]>\n",
		"",
		"\tExample:		4rel4in 0 pwmrd 2; Read fill PWM factor for Input channel #2  \n"};

static int doPwmFillRead(int argc, char *argv[])
{
	int pin = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntPwmFillGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}

		printf("%0.2f\n", val);

		return OK;
	}

	return ARG_CNT_ERR;
}


static int doFrequencyRead(int argc, char *argv[]);
const CliCmdType CMD_IN_FREQ_READ =
	{
		"freqrd",
		2,
		&doFrequencyRead,
		"\tfreqrd:		Read the frequency(Hz) for one input channel\n",
		"\tUsage:		4rel4in <stack> freqrd <channel[1..4]>\n",
		"",
		"\tExample:		4rel4in 0 freqrd 2; Read input frequency for Input channel #2  \n"};

static int doFrequencyRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > IN_CH_NO))
		{
			printf("Input channel number value out of range!\n");
			return ERROR;
		}

		if (OK != cntPwmFreqGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}

		printf("%d\n", val);

		return OK;
	}

	return ARG_CNT_ERR;
}
