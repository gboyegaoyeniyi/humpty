#include "display.h"
#include "sensor.h"
#include "ssd1306.h"
#include <stdio.h>

void displayInit(i2c_master_bus_handle_t i2c_bus_handle,
                 ssd1306_handle_t *ssd1306_handle) {
  ssd1306_config_t ssd1306_config = I2C_SSD1306_128x64_CONFIG_DEFAULT;
  ssd1306_init(i2c_bus_handle, &ssd1306_config, ssd1306_handle);
}

void displaySensorReading(ssd1306_handle_t ssd1306_handle,
                          sensor_data_t sensor_data) {
  char temp_buf[17];
  char humidity_buf[17];

  snprintf(temp_buf, sizeof(temp_buf), "Temp: %.1f C", sensor_data.temperature);
  snprintf(humidity_buf, sizeof(humidity_buf), "Hum: %.1f %%",
           sensor_data.humidity);

  ssd1306_clear_display(ssd1306_handle, false);
  ssd1306_set_contrast(ssd1306_handle, 0xff);

  ssd1306_display_text(ssd1306_handle, 0, "Humpty", false);
  ssd1306_display_text(ssd1306_handle, 1, temp_buf, false);
  ssd1306_display_text(ssd1306_handle, 2, humidity_buf, false);
}
