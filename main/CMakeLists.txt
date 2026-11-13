idf_component_register(SRCS "main.c" "sensor.c" "display.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES dht esp_ssd1306)
                    
