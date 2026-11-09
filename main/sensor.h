
#ifndef SENSOR_H
#define SENSOR_H

#include "esp_err.h"
#include "sdkconfig.h"

typedef struct {
    float humidity;
    float temperature;
} sensor_data_t;

esp_err_t read_sensor_data(sensor_data_t *sensor_data);

#endif // SENSOR_H