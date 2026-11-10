#include "display.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "hal/i2c_types.h"
#include "sdkconfig.h"
#include "sensor.h"
#include "ssd1306.h"
#include <stdio.h>
#include <string.h>

void i2cInit(i2c_master_bus_handle_t *i2c_bus_handle);

static sensor_data_t g_sensor_data;
static SemaphoreHandle_t g_data_mutex = NULL;

i2c_master_bus_handle_t i2c_bus_handle;
ssd1306_handle_t display_handle;

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

void display_task(void *pvParameters) {
  sensor_data_t sensor_data;
  sensor_data_t prev_sensor_data;
  ssd1306_clear_display(display_handle, false);
  ssd1306_set_contrast(display_handle, 0xff);
  while (1) {
    if (xSemaphoreTake(g_data_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
      prev_sensor_data = sensor_data;
      sensor_data = g_sensor_data;
      xSemaphoreGive(g_data_mutex);
    }

    displaySensorReading(display_handle, sensor_data, prev_sensor_data);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void) {
  g_data_mutex = xSemaphoreCreateMutex();
  i2cInit(&i2c_bus_handle);
  displayInit(i2c_bus_handle, &display_handle);
  if (display_handle == NULL) {
    ESP_LOGE("DISPLAY", "Failed to initialize the ssd1306 oled display");
  }

  xTaskCreatePinnedToCore(dht_task, "DHT Sensor", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(display_task, "Oled Display", 3072, NULL, 2, NULL, 1);
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
