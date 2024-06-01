/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ruslan V. Uss <unclerus@gmail.com>
 * Copyright (c) 2024 Nguyen Nhu Hai Long <long27032002@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file pcf8575.h
 * @defgroup pcf8575 pcf8575
 * @{
 *
 * ESP-IDF driver for PCF8575 remote 16-bit I/O expander for I2C-bus
 *
 * Copyright (c) 2019 Ruslan V. Uss <unclerus@gmail.com>
 *
 * MIT Licensed as described in the file LICENSE
 */
#ifndef __PCF8575_H__
#define __PCF8575_H__

#include <stddef.h>
#include <i2cdev.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PCF8575_I2C_ADDR_BASE 0x20

typedef enum 
{
    PCF8575_GPIO_PIN_00 = 0,
    PCF8575_GPIO_PIN_01 = 1,
    PCF8575_GPIO_PIN_02 = 2,
    PCF8575_GPIO_PIN_03 = 3,
    PCF8575_GPIO_PIN_04 = 4,
    PCF8575_GPIO_PIN_05 = 5,
    PCF8575_GPIO_PIN_06 = 6,
    PCF8575_GPIO_PIN_07 = 7,
    PCF8575_GPIO_PIN_10 = 8,
    PCF8575_GPIO_PIN_11 = 9,
    PCF8575_GPIO_PIN_12 = 10,
    PCF8575_GPIO_PIN_13 = 11,
    PCF8575_GPIO_PIN_14 = 12,
    PCF8575_GPIO_PIN_15 = 13,
    PCF8575_GPIO_PIN_16 = 14,
    PCF8575_GPIO_PIN_17 = 15,
    PCF8575_GPIO_PIN_MAX
} pcf8575_pinMap_et;

typedef void (* Interrupt_handle)(void *);

/**
 * @brief Initialize device descriptor
 *
 * Default SCL frequency is 400kHz
 *
 * @param dev Pointer to I2C device descriptor
 * @param port I2C port number
 * @param addr I2C address (`0b0100<A2><A1><A0>` for PCF8575)
 * @param sda_gpio SDA GPIO
 * @param scl_gpio SCL GPIO
 * @return `ESP_OK` on success
 */
esp_err_t pcf8575_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio, gpio_num_t interrupt_gpio, Interrupt_handle interrupt_handle);

/**
 * @brief Free device descriptor
 * @param dev Pointer to I2C device descriptor
 * @return `ESP_OK` on success
 */
esp_err_t pcf8575_free_desc(i2c_dev_t *dev);

/**
 * @brief Read GPIO port value
 * @param dev Pointer to I2C device descriptor
 * @param val 8-bit GPIO port value
 * @return `ESP_OK` on success
 */
esp_err_t pcf8575_port_read(i2c_dev_t *dev, uint16_t *val);

/**
 * @brief Write value to GPIO port
 * @param dev Pointer to I2C device descriptor
 * @param value GPIO port value
 * @return ESP_OK on success
 */
esp_err_t pcf8575_port_write(i2c_dev_t *dev, uint16_t *value);

/**
 * @brief Write value to GPIO pin
 * 
 * @param dev Pointer to I2C device descriptor
 * @param pin GPIO pin
 * @param value GPIO pin value
 * @return esp_err_t 
 */
esp_err_t pcf8575_pin_write(i2c_dev_t *dev, pcf8575_pinMap_et pin, uint8_t value);

/**
 * @brief Enable interrupt on GPIO pin
 * 
 * @param dev Pointer to I2C device descriptor
 *  * @param interrupt_gpio Interrupt pin
 * @return `ESP_OK` on success
 */
esp_err_t pcd8575_enableInterruptGPIO(i2c_dev_t *dev, gpio_num_t interrupt_gpio);

/**
 * Enable interrupt on GPIO pin
 * 
 * @param dev Pointer to I2C device descriptor
 * @param interrupt_gpio Interrupt pin
 * @return `ESP_OK` on success
 */
esp_err_t pcd8575_disableInterruptGPIO(i2c_dev_t *dev, gpio_num_t interrupt_gpio);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __PCF8575_H__ */
