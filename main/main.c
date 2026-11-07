#include "dht.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include <stdio.h>

#define DHT_SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO_PIN GPIO_NUM_4

static const char *DHT_TAG = "DHT11";

void app_main(void) {
  int16_t humidity = 0;
  int16_t temperature = 0;

  while (1) {
    if (dht_read_data(DHT_SENSOR_TYPE, DHT_GPIO_PIN, &humidity, &temperature) ==
        ESP_OK) {
      ESP_LOGI(DHT_TAG, "Humidity: %.1f%%, Temprature: %.1f C\n",
               humidity / 10.0, temperature / 10.0);
    } else {
      ESP_LOGE(DHT_TAG, "Could not read data from sensor!");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
