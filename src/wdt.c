#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "4rel4in.h"
#include "comm.h"

//************************************************************* WDT *************************************************

int doWdtReload(int argc, char *argv[]);
const CliCmdType CMD_WDT_RELOAD =
	{"wdtr", 2, &doWdtReload,
		"\twdtr:		Reload the watchdog timer and enable the watchdog if is disabled\n",
		"\tUsage:		4rel4in <stack> wdtr\n", "",
		"\tExample:		4rel4in 0 wdtr; Reload the watchdog timer on Board #0 with the period \n"};

int doWdtReload(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[2] = {0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		buff[0] = WDT_RESET_SIGNATURE;
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_RESET_ADD, buff, 1))
		{
			printf("Fail to write watchdog reset key!\n");
			return IO_ERROR;
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_RELOAD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_PERIOD =
	{"wdtpwr", 2, &doWdtSetPeriod,
		"\twdtpwr:		Set the watchdog period in seconds, reload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		4rel4in <stack> wdtpwr <val> \n", "",
		"\tExample:		4rel4in 0 wdtpwr 10; Set the watchdog timer period on Board #0 at 10 seconds \n"};

int doWdtSetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] = {0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			return ARG_RANGE_ERROR;
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			return IO_ERROR;
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_PERIOD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_PERIOD =
	{"wdtprd", 2, &doWdtGetPeriod,
		"\twdtprd:		Get the watchdog period in seconds, reload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		4rel4in <stack> wdtprd \n", "",
		"\tExample:		4rel4in 0 wdtprd; Get the watchdog timer period on Board #0\n"};

int doWdtGetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			return IO_ERROR;
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_PERIOD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_INIT_PERIOD =
	{"wdtipwr", 2, &doWdtSetInitPeriod,
		"\twdtipwr:	Set the watchdog initial period in seconds, This period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		4rel4in <stack> wdtipwr <val> \n", "",
		"\tExample:		4rel4in 0 wdtipwr 10; Set the watchdog timer initial period on Board #0 at 10 seconds \n"};

int doWdtSetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] = {0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			return ARG_RANGE_ERROR;
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			return IO_ERROR;
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_INIT_PERIOD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_INIT_PERIOD =
	{"wdtiprd", 2, &doWdtGetInitPeriod,
		"\twdtiprd:	Get the watchdog initial period in seconds. This period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		4rel4in <stack> wdtiprd \n", "",
		"\tExample:		4rel4in 0 wdtiprd; Get the watchdog timer initial period on Board #0\n"};

int doWdtGetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			return IO_ERROR;
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_INIT_PERIOD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtSetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_OFF_PERIOD =
	{"wdtopwr", 2, &doWdtSetOffPeriod,
		"\twdtopwr:	Set the watchdog off period in seconds (max 48 days), This is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		4rel4in <stack> wdtopwr <val> \n", "",
		"\tExample:		4rel4in 0 wdtopwr 10; Set the watchdog off interval on Board #0 at 10 seconds \n"};

int doWdtSetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] = {0, 0, 0, 0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 4)
	{
		period = (u32)atoi(argv[3]);
		if ( (0 == period) || (period > WDT_MAX_OFF_INTERVAL_S))
		{
			printf("Invalid period!\n");
			return ARG_RANGE_ERROR;
		}
		memcpy(buff, &period, 4);
		if (OK
			!= i2cMem8Write(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff, 4))
		{
			printf("Fail to write watchdog period!\n");
			return IO_ERROR;
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_OFF_PERIOD.usage1);
		return ARG_CNT_ERR;
	}
	return OK;
}

int doWdtGetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_OFF_PERIOD =
	{"wdtoprd", 2, &doWdtGetOffPeriod,
		"\twdtoprd:	Get the watchdog off period in seconds (max 48 days), This is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		4rel4in <stack> wdtoprd \n", "",
		"\tExample:		4rel4in 0 wdtoprd; Get the watchdog off period on Board #0\n"};

int doWdtGetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4];

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}

	if (argc == 3)
	{
		if (OK
			!= i2cMem8Read(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, buff, 4))
		{
			printf("Fail to read watchdog period!\n");
			return IO_ERROR;
		}
		memcpy(&period, buff, 4);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_OFF_PERIOD.usage1);
		return ARG_CNT_ERR;
	}

	return OK;

}
