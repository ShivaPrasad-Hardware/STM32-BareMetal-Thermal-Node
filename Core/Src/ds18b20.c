#include "ds18b20.h"

// Initialize DWT for microsecond delays
void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

// Microsecond delay using DWT cycle counter
void delay_us(uint32_t us)
{
    uint32_t startTicks = DWT->CYCCNT;
    uint32_t targetTicks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - startTicks) < targetTicks);
}

// 1-Wire Low-Level Bit-Banging (PB4 / D5)
static void OW_SetLow(void)  { GPIOB->BSRR = (1 << 20); } // Reset PB4
static void OW_SetHigh(void) { GPIOB->BSRR = (1 << 4); }  // Set PB4
static uint8_t OW_ReadPin(void) { return (GPIOB->IDR & (1 << 4)) ? 1 : 0; }

static uint8_t DS18B20_Reset(void)
{
    uint8_t presence = 0;
    OW_SetLow();
    delay_us(480);
    OW_SetHigh();
    delay_us(80);
    presence = OW_ReadPin();
    delay_us(400);
    return presence;
}

static void OW_WriteBit(uint8_t bit)
{
    OW_SetLow();
    if (bit) {
        delay_us(5);
        OW_SetHigh();
        delay_us(60);
    } else {
        delay_us(60);
        OW_SetHigh();
        delay_us(10);
    }
}

static uint8_t OW_ReadBit(void)
{
    uint8_t bit = 0;
    OW_SetLow();
    delay_us(2);
    OW_SetHigh();
    delay_us(10);
    bit = OW_ReadPin();
    delay_us(50);
    return bit;
}

static void OW_WriteByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        OW_WriteBit(byte & 0x01);
        byte >>= 1;
    }
}

static uint8_t OW_ReadByte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (OW_ReadBit()) {
            byte |= (1 << i);
        }
    }
    return byte;
}

// High-Level DS18B20 Functions
void DS18B20_StartConversion(void)
{
    DS18B20_Reset();
    OW_WriteByte(0xCC); // Skip ROM
    OW_WriteByte(0x44); // Convert T
}

float DS18B20_ReadTemperature(void)
{
    uint8_t scratchpad[9];
    int16_t raw_temperature = 0;
    float celsius = 0;

    DS18B20_Reset();
    OW_WriteByte(0xCC); // Skip ROM
    OW_WriteByte(0xBE); // Read Scratchpad

    for (uint8_t i = 0; i < 9; i++) {
        scratchpad[i] = OW_ReadByte();
    }

    raw_temperature = ((int16_t)scratchpad[1] << 8) | scratchpad[0];
    celsius = (float)raw_temperature * 0.0625f;

    return celsius;
}
