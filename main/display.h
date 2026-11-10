#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"
#include "sensor.h"

void displayInit(i2c_master_bus_handle_t i2c_bus_handle, ssd1306_handle_t *ssd1306_handle);

void displaySensorReading(ssd1306_handle_t ssd1306_handle, sensor_data_t sensor_data);

#endif
