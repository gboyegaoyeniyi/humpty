#include "sensor.h"
#include "dht.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define DHT_GPIO_PIN CONFIG_DHT_GPIO_PIN
#define SENSOR_TYPE DHT_TYPE_DHT11

static const char *DHT_TAG = "DHT11 SENSOR";

esp_err_t read_sensor_data(sensor_data_t *sensor_data) {
  if (sensor_data == NULL) {
    return ESP_ERR_INVALID_ARG;
  }

  esp_err_t err =
      dht_read_float_data(SENSOR_TYPE, DHT_GPIO_PIN, &sensor_data->humidity,
                          &sensor_data->temperature);

  if (err != ESP_OK) {
    ESP_LOGE(DHT_TAG,
             "Error reading sensor data from DHT11 sensor, GPIO %d (ERR: %s)",
             DHT_GPIO_PIN, esp_err_to_name(err));
  }

  return err;
};
