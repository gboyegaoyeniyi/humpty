#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "hal/i2c_types.h"
#include "sdkconfig.h"
#include "sensor.h"
#include "soc/clk_tree_defs.h"
#include <stdio.h>

void i2cInit(i2c_master_bus_handle_t *i2c_bus_handle);

static sensor_data_t g_sensor_data;
static SemaphoreHandle_t g_data_mutex = NULL;

i2c_master_bus_handle_t i2c_bus_handle;


void dht_task(void *pvParameters) {
  while (1) {
    sensor_data_t sensor_values;
    read_sensor_data(&sensor_values);
    if (xSemaphoreTake(g_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      g_sensor_data = sensor_values;
      xSemaphoreGive(g_data_mutex);
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void app_main(void) {
  g_data_mutex = xSemaphoreCreateMutex();
  i2cInit(&i2c_bus_handle);

  xTaskCreatePinnedToCore(dht_task, "DHT Sensor", 2048, NULL, 2, NULL, 1);
}


void i2cInit(i2c_master_bus_handle_t *i2c_bus_handle) {
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .sda_io_num = CONFIG_OLED_SDA_PIN,
        .scl_io_num = CONFIG_OLED_SCL_PIN,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_new_master_bus(&i2c_bus_config, i2c_bus_handle);
}