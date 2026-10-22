#ifndef DS18B20_H
#define DS18B20_H

#include "main.h"

// Public Function Prototypes
void DWT_Init(void);
void delay_us(uint32_t us);
void DS18B20_StartConversion(void);
float DS18B20_ReadTemperature(void);

#endif /* DS18B20_H */
