#include "dht.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include <stdint.h>
#include <stdio.h>

#define DHT_SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO_PIN GPIO_NUM_4
#define OLED_SDA_PIN GPIO_NUM_21
#define OLED_SCL_PIN GPIO_NUM_22

static const char *DHT_TAG = "DHT11";
static const char *OLED_TAG = "SSD1306 DISPLAY";

void app_main(void) {
  int16_t humidity = 0;
  int16_t temperature = 0;

  i2c_master_bus_handle_t i2c0_bus_handle;
  i2c_master_bus_config_t i2c_bus_config = {
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .i2c_port = I2C_NUM_0,
      .sda_io_num = OLED_SDA_PIN,
      .scl_io_num = OLED_SCL_PIN,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = true,
  };

  i2c_new_master_bus(&i2c_bus_config, &i2c0_bus_handle);

  ssd1306_config_t ssd1306_dev_config = I2C_SSD1306_128x32_CONFIG_DEFAULT;
  ssd1306_handle_t ssd1306_dev_handle;

  ssd1306_init(i2c0_bus_handle, &ssd1306_dev_config, &ssd1306_dev_handle);
  if (ssd1306_dev_handle == NULL) {
    ESP_LOGE(OLED_TAG, "Failed to initialize the ssd1306 display!");
  }

  ESP_LOGI(
      OLED_TAG,
      "######################## SSD1306 - START #########################");

  while (1) {
    if (dht_read_data(DHT_SENSOR_TYPE, DHT_GPIO_PIN, &humidity, &temperature) ==
        ESP_OK) {
      float temp_val = temperature / 10.0;
      float hum_val = humidity / 10.0;

      char line1_buf[17];
      char line2_buf[17];

      snprintf(line1_buf, sizeof(line1_buf), "Temp: %.1f C", temp_val);
      snprintf(line2_buf, sizeof(line2_buf), "Hum:  %.1f%%", hum_val);

      ssd1306_clear_display(ssd1306_dev_handle, false);
      ssd1306_set_contrast(ssd1306_dev_handle, 0xff);

      ssd1306_display_text(ssd1306_dev_handle, 0, "Humpty", false);
      ssd1306_display_text(ssd1306_dev_handle, 1, line1_buf, false);
      ssd1306_display_text(ssd1306_dev_handle, 2, line2_buf, false);

    } else {
      ESP_LOGE(DHT_TAG, "Could not read data from sensor!");
    }
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
