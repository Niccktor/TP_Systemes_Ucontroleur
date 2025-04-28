/*
 * MCP23S17.c
 *
 *  Created on: Apr 28, 2025
 *      Author: BEGUIN
 */
#include "MCP23S17.h"

static 	uint8_t a_state = 0xff;
static 	uint8_t b_state = 0xff;
static	SPI_HandleTypeDef *spiHandle;

void MCP23S17_WriteReg(uint8_t reg, uint8_t data)
{
	uint8_t send[3] = { MCP23S17_ADDR, reg, data };

	HAL_GPIO_WritePin(nCS_MCP23S17_GPIO_Port, nCS_MCP23S17_Pin, 0);
	HAL_SPI_Transmit(spiHandle, send, 3, 0x400);
	HAL_GPIO_WritePin(nCS_MCP23S17_GPIO_Port, nCS_MCP23S17_Pin, 1);
}

uint8_t MCP23S17_ReadReg(uint8_t reg)
{
	uint8_t data[2] = {MCP23S17_ADDR | 0x01, reg};
	uint8_t	RX_data = 0x00;

	HAL_GPIO_WritePin(nCS_MCP23S17_GPIO_Port, nCS_MCP23S17_Pin, 0);
	HAL_SPI_Transmit(spiHandle, data, 2, 0x400);
	HAL_SPI_Receive(spiHandle, &RX_data, 1, 0x400);
	HAL_GPIO_WritePin(nCS_MCP23S17_GPIO_Port, nCS_MCP23S17_Pin, 1);

	return (RX_data);
}

void MCP23S17_Init(SPI_HandleTypeDef *hspi)
{
	spiHandle = hspi;
	HAL_GPIO_WritePin(nCS_MCP23S17_GPIO_Port, nCS_MCP23S17_Pin, 1);

	MCP23S17_WriteReg(IODIRA, 0x00);
	MCP23S17_WriteReg(IODIRB, 0x00);

	MCP23S17_WriteReg(OLATA, 0xff);
	MCP23S17_WriteReg(OLATB, 0xff);

	MCP23S17_WriteReg(OLATB, 0xAA);
}
