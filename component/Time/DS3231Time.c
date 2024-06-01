#include "DS3231Time.h"

// %d:%d:%d,%d-%d-%d
    __attribute__((unused)) static char *timeFormat = "%.2d:%.2d:%.2d,%.2d-%.2d-%d";

// %d %d %d
__attribute__((unused)) static const char *timeFormat2 = "%d %d %d";

// %d-%d-%d
static const char *timeFormat3 = "%.4d%.2d%.2d%.2d%.2d%.2d";
static const int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

int currentDay;

esp_err_t ds3231_initialize(i2c_dev_t *dev, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    esp_err_t error;
    error = ds3231_init_desc(dev, port, sda_gpio, scl_gpio);
    if (error != ESP_OK)
    {
        ESP_LOGE(__func__, "DS3231 module initialize fail.");
        return error;
    }
    ESP_LOGI(__func__, "DS3231 module initialize success.");
    struct tm currentTime;
    ds3231_get_time(dev, &currentTime);
    currentDay = currentTime.tm_yday;
    return error;
}

esp_err_t ds3231_convertTimeToString(i2c_dev_t *dev, char* timeString, const unsigned int lenghtString)
{
    struct tm time;
    ds3231_get_time(dev, &time);
#ifdef CONFIG_RTC_TIME_SYNC
    time.tm_mon += 1;
#endif
    memset(timeString, 0, lenghtString);
    int lenght = 0;
    lenght = sprintf(timeString, timeFormat3, time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
    if (lenght)
    {
        ESP_LOGI(__func__, "Convert time to string success.");
        return ESP_OK;
    }

    ESP_LOGE(__func__, "Convert time to string fail.");
    return ESP_FAIL;
}

esp_err_t ds3231_setTime(i2c_dev_t *dev, struct tm *time)
{
#ifdef CONFIG_RTC_TIME_SYNC
    time->tm_year += 1900;
#endif
    return ds3231_set_time(dev, time);
}

esp_err_t ds3231_getTimeString(i2c_dev_t *dev)
{
    char timeString[64];
    struct tm time;
    ds3231_get_time(dev, &time);
#ifdef CONFIG_RTC_TIME_SYNC
    time.tm_mon += 1;
#endif
    memset(timeString, 0, 64);
    int lenght = 0;
    lenght = sprintf(timeString, timeFormat, time.tm_hour, time.tm_min, time.tm_sec, time.tm_mday, time.tm_mon, time.tm_year);
    if (lenght)
    {
        ESP_LOGI(__func__, "Current time: %s , %ld", timeString, (long)mktime(&time));
        return ESP_OK;
    }
    return ESP_FAIL;
}

esp_err_t ds3231_getEpochTime(i2c_dev_t *dev, int64_t *epochTime)
{
    struct tm currentTime = {0};
    esp_err_t err_code = ESP_OK;
    err_code = ds3231_get_time(dev, &currentTime);

    if (err_code != ESP_OK)
    {
        ESP_LOGE(__func__, "DS3231 get UnixTime fail(%s).", esp_err_to_name(err_code));
        return ESP_FAIL;
    }

#ifdef CONFIG_RTC_TIME_SYNC
    currentTime.tm_mon += 1;
#endif

    for (size_t i = 0; i < (currentTime.tm_mon - 1); i++)       // Calculate the total number of days from Jan to curent month
    {
        currentTime.tm_yday += month[i];
        if (i == 1) currentTime.tm_yday += ((currentTime.tm_year % 4) == 0);        // Check leap year
    }
    currentTime.tm_yday += currentTime.tm_mday - 1;

    *epochTime  = SECONDS_FROM_1970_TO_2024;
    *epochTime += (currentTime.tm_year - 2024) * SECONDS_PER_YEAR;
    *epochTime += currentTime.tm_yday * SECONDS_PER_DAY;
    *epochTime += currentTime.tm_hour * SECONDS_PER_HOUR;
    *epochTime += currentTime.tm_min  * SECONDS_PER_MIN;
    *epochTime += currentTime.tm_sec;
    ESP_LOGI(__func__, "DS3231 get EpochTime success.");

    return ESP_OK;
}

// esp_err_t ds3231_getEpochTime(i2c_dev_t *dev, uint64_t *epochTime)
// {
//     struct tm currentTime = {0};
//     esp_err_t err_code = ESP_OK;
//     err_code = ds3231_get_time(dev, &currentTime);

//     if (err_code != ESP_OK)
//     {
//         ESP_LOGE(__func__, "DS3231 get EpochTime fail(%s).", esp_err_to_name(err_code));
//         return ESP_FAIL;
//     }

//     *epochTime = 0;
//     *epochTime |= mktime(&currentTime);
//     ESP_LOGI(__func__, "DS3231 get EpochTime success. %ld", (long)mktime(&currentTime));
//     return ESP_OK;
// }

bool ds3231_isNewDay(i2c_dev_t *dev)
{
    struct tm currentTime;
    ds3231_get_time(dev, &currentTime);
    if (currentDay == currentTime.tm_yday)
    {
        return false;
    } else {
        ESP_LOGI(__func__, "New day come.");
        currentDay = currentTime.tm_yday;
        return true;
    }
}
