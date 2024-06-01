#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <sys/time.h>
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define EVENT_INTERRUPT_POSITIVE_EDGE BIT0
#define EVENT_INTERRUPT_NEGATIVE_EDGE BIT1

typedef void (* button_callback)(void *parameters);

typedef struct {
    gpio_config_t io_config;
    gpio_num_t gpio_num;
} button_config_st;

esp_err_t button_init(button_config_st *config, button_callback callback, void *parameters); 

esp_err_t button_getStatus(button_config_st *config, uint8_t *state);

esp_err_t button_disable(button_config_st *config);

esp_err_t button_enable(button_config_st *config);

#endif // !