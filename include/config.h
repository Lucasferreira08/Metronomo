#ifndef CONFIG_H
#define CONFIG_H

#include "display.h"

#define I2C_PORT i2c1       // Controlador I²C0 do RP2040
#define SDA_PIN 14          // GPIO14 para SDA
#define SCL_PIN 15          // GPIO15 para SCL
#define SSD1306_ADDR 0x3C   // Endereço I²C do display (alternativo: 0x3C)

void setup_i2c();

void setup_display(ssd1306_t *ssd);

void display_init(ssd1306_t *ssd);

#endif 