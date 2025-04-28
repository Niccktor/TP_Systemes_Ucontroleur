/*
 * MCP23S17.h
 *
 *  Created on: Apr 28, 2025
 *      Author: BEGUIN
 */

#ifndef INC_MCP23S17_H_
#define INC_MCP23S17_H_

#include "spi.h"
#include "stm32l4xx_hal.h"

#define MCP23S17_ADDR 0x40

#define IODIRA 	0x00
#define IODIRB	0x01
#define OLATA	0x14
#define OLATB	0x15

void MCP23S17_Init(SPI_HandleTypeDef *hspi);
void MCP23S17_SetPin(uint8_t port, uint8_t pin, uint8_t state);
void MCP23S17_SetMultiplePins(uint8_t port, uint8_t mask, uint8_t state);

#endif /* INC_MCP23S17_H_ */
