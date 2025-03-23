#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "4rel4in.h"
#include "comm.h"

int rs485Get(int dev)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (OK != i2cMem8Read(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to read RS485 settings!\n");
		return ERROR;
	}
	memcpy(&settings, buff, sizeof(ModbusSetingsType));
	printf("<mode> <baudrate> <stopbits> <parity> <add> %d %d %d %d %d\n",
		(int)settings.mbType, (int)settings.mbBaud, (int)settings.mbStopB,
		(int)settings.mbParity, (int)settings.add);
	return OK;
}

int doRs485Read(int argc, char *argv[]);
const CliCmdType CMD_RS485_READ =
{
	"cfg485rd",
	2,
	&doRs485Read,
	"\tcfg485rd:	Read the RS485 communication settings\n",
	"\tUsage:		4rel4in <stack> cfg485rd\n",
	"",
	"\tExample:		4rel4in 0 cfg485rd; Read the RS485 settings on Board #0\n"};

int doRs485Read(int argc, char *argv[])
{
	int dev = 0;
	UNUSED(argv);

	if (argc == 3)
	{
		dev = doBoardInit(atoi(argv[1]));
		if (dev <= 0)
		{
			return ERROR;
		}
		if (OK != rs485Get(dev))
		{
			printf("Fail to read RS485 settings!\n");
			return ERROR;
		}
	}
	else
	{
		return ARG_CNT_ERR;
	}
	return OK;
}

int doRs485Write(int argc, char *argv[]);
const CliCmdType CMD_RS485_WRITE =
	{
		"cfg485wr",
		2,
		&doRs485Write,
		"\tcfg485wr:	Write the RS485 communication settings\n",
		"\tUsage:		4rel4in <stack> cfg485wr <mode> <baudrate> <stopBits> <parity> <slaveAddr>\n"
		"\t      		4rel4in <stack> cfg485wr 0 (disable modbus)\n",
		"",
		"\tExample:		4rel4in 0 cfg485wr 1 9600 1 0 1; Write the RS485 settings on Board #0 \n\t\t\t(mode = Modbus RTU; baudrate = 9600 bps; stop bits one; parity none; modbus slave address = 1)\n"
		"\tExample:		4rel4in 0 cfg485wr 0; Disable modbus\n"
};

int doRs485Write(int argc, char *argv[])
{
	if (argc < 4)
	{
		return ARG_CNT_ERR;
	}

	ModbusSetingsType settings;
	int aux = atoi(argv[3]); // Mode
	if (aux == 0) // Disable modbus and free the RS485 for Raspberry usage
	{
		settings.mbType = 0;
		settings.mbBaud = 38400;
		settings.mbStopB = 1;
		settings.mbParity = 0;
		settings.add = 1;
	}
	else //  enable the modbus and we need all the parameters
	{
		if (argc != 8)
		{
			return ARG_CNT_ERR;
		}
		if (aux != 1)
		{
			printf("Mode must be [0/1]\n");
			return ERROR;
		}
		settings.mbType = 1;
		aux = atoi(argv[4]); // Baudrate
		if (aux < 1200 || aux > 920600)
		{
			printf("Baudrate must be [1200..920600]\n");
			return ERROR;
		}
		settings.mbBaud = aux;
		aux = atoi(argv[5]); // Stop bits
		if (aux < 1 || aux > 2)
		{
			printf("Stop bits must be [1/2]\n");
			return ERROR;
		}
		settings.mbStopB = aux;
		aux = atoi(argv[6]); // Parity
		if (aux < 0 || aux > 2)
		{
			printf("Parity must be [0/1/2]\n");
			return ERROR;
		}
		settings.mbParity = aux;
		aux = atoi(argv[7]); // Modbus ID
		if (aux < 1 || aux > 254)
		{
			printf("Modbus ID must be [1..254]\n");
			return ERROR;
		}
		settings.add = aux;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		return ERROR;
	}
	if (OK != i2cMem8Write(dev, I2C_MODBUS_SETINGS_ADD, (uint8_t*)&settings, sizeof(ModbusSetingsType)))
	{
		printf("Fail to write RS485 settings!\n");
		return ERROR;
	}
	return OK;
}