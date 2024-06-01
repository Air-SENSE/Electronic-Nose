#include "button.h"

esp_err_t button_init(button_config_st *config, button_callback callback, void *parameters)
{
    gpio_config(&(config->io_config));

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(config->gpio_num, callback, (void*)parameters);

    return ESP_OK;
}

esp_err_t button_getStatus(button_config_st *config, uint8_t *state)
{
    *state = gpio_get_level(config->gpio_num);
    return ESP_OK;
}

esp_err_t button_disable(button_config_st *config)
{
    return gpio_intr_disable(config->gpio_num);
}

esp_err_t button_enable(button_config_st *config)
{
    return gpio_intr_enable(config->gpio_num);
}

