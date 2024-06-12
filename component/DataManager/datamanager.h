#ifndef __DATAMANAGER_H__
#define __DATAMANAGER_H__

#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <string.h>
#include <inttypes.h>

#define ERROR_VALUE UINT32_MAX

struct dataSensor_st
{
    int64_t timeStamp;
    float temperature;
    float humidity;
    float pressure;
    int16_t ADC_Value[8];
};

const char dataSensor_templateSaveToSDCard[] = "%" PRIi64 ",%.2f,%.2f,%" PRId32 ",%" PRId32 ",%" PRId32 ",%" PRId32 ",%" PRId32 ",%" PRId32 ",%" PRId32 ",%" PRId32 "\n";

#endif