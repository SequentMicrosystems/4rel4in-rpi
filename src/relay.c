#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "4rel4in.h"
#include "comm.h"
#include "thread.h"

int relayChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_VAL, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_VAL, buff, 1);
		break;
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int relayChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
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

int relaySet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_MEM_RELAY_VAL, buff, 1);
}

int relayGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_VAL, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static int doRelayWrite(int argc, char *argv[]);
const CliCmdType CMD_RELAY_WRITE =
{
	"relwr",
	2,
	&doRelayWrite,
	"\trelwr:		Set relays (AC switch) On/Off\n",
	"\tUsage:		4rel4in <stack> relwr <channel[1..4]> <0/1>\n",
	"\tUsage:		4rel4in <stack> relwr <value[0..15]>\n",
	"\tExample:		4rel4in 0 relwr 2 1; Turn relay #2 On\n"};

static int doRelayWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

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
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Relay channel number value out of range\n");
			return ARG_RANGE_ERROR;
		}

		/**/if ( (strcasecmp(argv[4], "up") == 0)
			|| (strcasecmp(argv[4], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[4], "down") == 0)
			|| (strcasecmp(argv[4], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
			{
				printf("Invalid relay state!\n");
				return ARG_RANGE_ERROR;
			}
			state = (OutStateEnumType)atoi(argv[4]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != relayChSet(dev, pin, state))
			{
				printf("Fail to write relay\n");
				return IO_ERROR;
			}
			if (OK != relayChGet(dev, pin, &stateR))
			{
				printf("Fail to read relay\n");
				return IO_ERROR;
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write relay\n");
			return IO_ERROR;
		}
		return OK;
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid relays value\n");
			return ARG_RANGE_ERROR;
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != relaySet(dev, val))
			{
				printf("Fail to write relay!\n");
				return IO_ERROR;
			}
			if (OK != relayGet(dev, &valR))
			{
				printf("Fail to read relay!\n");
				return IO_ERROR;
			}
		}
		if (retry == 0)
		{
			printf("Fail to write relay!\n");
			return IO_ERROR;
		}
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doRelayRead(int argc, char *argv[]);
const CliCmdType CMD_RELAY_READ =
{
	"relrd",
	2,
	&doRelayRead,
	"\trelrd:		Read relays  status\n",
	"\tUsage:		4rel4in <stack> relrd <channel[1..4]>\n",
	"\tUsage:		4rel4in <stack> relrd\n",
	"\tExample:		4rel4in 0 relrd 2; Read Status of Relay #2 \n"};

static int doRelayRead(int argc, char *argv[])
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
			printf("Relay channel number value out of range!\n");
			return ERROR;
		}

		if (OK != relayChGet(dev, pin, &state))
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
		if (OK != relayGet(dev, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doRelayTest(int argc, char* argv[]);
const CliCmdType CMD_RELAY_TEST =
{
	"reltest",
	2,
	&doRelayTest,
	"\treltest:		Turn ON and OFF the relays until press a key\n",
	"\tUsage:		4rel4in <stack> reltest\n",
	"",
	"\tExample:		4rel4in 0 reltest\n"};

static int doRelayTest(int argc, char* argv[])
{
	int dev = 0;
	int i = 0;
	int retry = 0;
	int trVal;
	int valR;
	int relayResult = 0;
	FILE* file = NULL;
	const u8 relayOrder[RELAY_CH_NO] ={ 1, 2, 3, 4};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (argc == 4)
	{
		file = fopen(argv[3], "w");
		if (!file)
		{
			printf("Fail to open result file\n");
			//return -1;
		}
	}
//relay test****************************
	if (strcasecmp(argv[2], "reltest") == 0)
	{
		trVal = 0;
		printf(
			"Are all relays and LEDs turning on and off in sequence?\nPress y for Yes or any key for No....");
		startThread();
		while (relayResult == 0)
		{
			for (i = 0; i < RELAY_CH_NO; i++)
			{
				relayResult = checkThreadResult();
				if (relayResult != 0)
				{
					break;
				}
				valR = 0;
				trVal = (u8)1 << (relayOrder[i] - 1);

				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) == 0))
				{
					if (OK != relayChSet(dev, relayOrder[i], ON))
					{
						retry = 0;
						break;
					}

					if (OK != relayGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write relay\n");
					if (file)
						fclose(file);
					return IO_ERROR;
				}
				busyWait(150);
			}

			for (i = 0; i < RELAY_CH_NO; i++)
			{
				relayResult = checkThreadResult();
				if (relayResult != 0)
				{
					break;
				}
				valR = 0xff;
				trVal = (u8)1 << (relayOrder[i] - 1);
				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) != 0))
				{
					if (OK != relayChSet(dev, relayOrder[i], OFF))
					{
						retry = 0;
					}
					if (OK != relayGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write relay!\n");
					if (file)
						fclose(file);
					return IO_ERROR;
				}
				busyWait(150);
			}
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	if (relayResult == YES)
	{
		if (file)
		{
			fprintf(file, "Relay Test ............................ PASS\n");
		}
		else
		{
			printf("Relay Test ............................ PASS\n");
		}
	}
	else
	{
		if (file)
		{
			fprintf(file, "Relay Test ............................ FAIL!\n");
		}
		else
		{
			printf("Relay Test ............................ FAIL!\n");
		}
	}
	if (file)
	{
		fclose(file);
	}
	relaySet(dev, 0);
	return OK;
}

//  ================= failsafe Enable/Disable functions ===========================
int failsafeEnChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1);
		break;
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int failsafeEnChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1))
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

int failsafeEnSet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1);
}

int failsafeEnGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_EN_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = 0x0f & buff[0];
	return OK;
}

static int doFailsafeEnWrite(int argc, char *argv[]);
const CliCmdType CMD_FAILSAFE_EN_WRITE =
{
	"fsenwr",
	2,
	&doFailsafeEnWrite,
	"\tfsenwr:		Set failsafe enable On/Off\n",
	"\tUsage:		4rel4in <stack> fsenwr <channel[1..4]> <0/1>\n",
	"\tUsage:		4rel4in <stack> fsenwr <value[0..15]>\n",
	"\tExample:		4rel4in 0 fsenwr 2 1; Turn failsafe enable #2 On\n"};

static int doFailsafeEnWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

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
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Relay channel number value out of range\n");
			return ARG_RANGE_ERROR;
		}

		/**/if ( (strcasecmp(argv[4], "up") == 0)
			|| (strcasecmp(argv[4], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[4], "down") == 0)
			|| (strcasecmp(argv[4], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
			{
				printf("Invalid relay state!\n");
				return ARG_RANGE_ERROR;
			}
			state = (OutStateEnumType)atoi(argv[4]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != failsafeEnChSet(dev, pin, state))
			{
				printf("Fail to write failsafe enable\n");
				return IO_ERROR;
			}
			if (OK != failsafeEnChGet(dev, pin, &stateR))
			{
				printf("Fail to read failsafe enable\n");
				return IO_ERROR;
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write failsafe enable\n");
			return IO_ERROR;
		}
		return OK;
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid failsafe enable value\n");
			return ARG_RANGE_ERROR;
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != failsafeEnSet(dev, val))
			{
				printf("Fail to write failsafe enable!\n");
				return IO_ERROR;
			}
			if (OK != failsafeEnGet(dev, &valR))
			{
				printf("Fail to read failsafe enable!\n");
				return IO_ERROR;
			}
		}
		if (retry == 0)
		{
			printf("Fail to write failsafe enable!\n");
			return IO_ERROR;
		}
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doFailsafeEnRead(int argc, char *argv[]);
const CliCmdType CMD_FAILSAFE_EN_READ =
{
	"fsenrd",
	2,
	&doFailsafeEnRead,
	"\tfsenrd:		Read failsafe enable status\n",
	"\tUsage:		4rel4in <stack> fsenrd <channel[1..4]>\n",
	"\tUsage:		4rel4in <stack> fsenrd\n",
	"\tExample:		4rel4in 0 fsenrd 2; Read Status of Failsafe Enable #2 \n"};

static int doFailsafeEnRead(int argc, char *argv[])
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
			printf("Relay channel number value out of range!\n");
			return ERROR;
		}

		if (OK != failsafeEnChGet(dev, pin, &state))
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
		if (OK != failsafeEnGet(dev, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}

//  ================= failsafe Value functions ===========================
int failsafeValChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1);
		break;
	default:
		printf("Invalid relay state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int failsafeValChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > RELAY_CH_NO))
	{
		printf("Invalid relay nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1))
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

int failsafeValSet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1);
}

int failsafeValGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_MEM_RELAY_FAILSAFE_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static int doFailsafeValWrite(int argc, char *argv[]);
const CliCmdType CMD_FAILSAFE_VAL_WRITE =
{
	"fsvwr",
	2,
	&doFailsafeValWrite,
	"\tfsvwr:		Set failsafe values On/Off\n",
	"\tUsage:		4rel4in <stack> fsvwr <channel[1..4]> <0/1>\n",
	"\tUsage:		4rel4in <stack> fsvwr <value[0..15]>\n",
	"\tExample:		4rel4in 0 fsvwr 2 1; Turn failsafe value #2 On\n"};
static int doFailsafeValWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

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
		if ( (pin < CHANNEL_NR_MIN) || (pin > RELAY_CH_NO))
		{
			printf("Relay channel number value out of range\n");
			return ARG_RANGE_ERROR;
		}

		/**/if ( (strcasecmp(argv[4], "up") == 0)
			|| (strcasecmp(argv[4], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[4], "down") == 0)
			|| (strcasecmp(argv[4], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
			{
				printf("Invalid relay state!\n");
				return ARG_RANGE_ERROR;
			}
			state = (OutStateEnumType)atoi(argv[4]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != failsafeValChSet(dev, pin, state))
			{
				printf("Fail to write failsafe value\n");
				return IO_ERROR;
			}
			if (OK != failsafeValChGet(dev, pin, &stateR))
			{
				printf("Fail to read failsafe value\n");
				return IO_ERROR;
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write failsafe value\n");
			return IO_ERROR;
		}
		return OK;
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid failsafe value\n");
			return ARG_RANGE_ERROR;
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != failsafeValSet(dev, val))
			{
				printf("Fail to write failsafe value!\n");
				return IO_ERROR;
			}
			if (OK != failsafeValGet(dev, &valR))
			{
				printf("Fail to read failsafe value!\n");
				return IO_ERROR;
			}
		}
		if (retry == 0)
		{
			printf("Fail to write relay!\n");
			return IO_ERROR;
		}
		return OK;
	}
	return ARG_CNT_ERR;
}

static int doFailsafeValRead(int argc, char *argv[]);
const CliCmdType CMD_FAILSAFE_VAL_READ =
{
	"fsvrd",
	2,
	&doFailsafeValRead,
	"\tfsvrd:		Read failsafe values status\n",
	"\tUsage:		4rel4in <stack> fsvrd <channel[1..4]>\n",
	"\tUsage:		4rel4in <stack> fsvrd\n",
	"\tExample:		4rel4in 0 fsvrd 2; Read Status of Failsafe Value #2 \n"};
static int doFailsafeValRead(int argc, char *argv[])
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
			printf("Relay channel number value out of range!\n");
			return ERROR;
		}

		if (OK != failsafeValChGet(dev, pin, &state))
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
		if (OK != failsafeValGet(dev, &val))
		{
			printf("Fail to read!\n");
			return IO_ERROR;
		}
		printf("%d\n", val);
		return OK;
	}
	return ARG_CNT_ERR;
}