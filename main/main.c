#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "sensor.h"
#include <stdio.h>

static sensor_data_t g_sensor_data;
static SemaphoreHandle_t g_data_mutex = NULL;

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

  xTaskCreatePinnedToCore(dht_task, "DHT Sensor", 2048, NULL, 2, NULL, 1);
}
