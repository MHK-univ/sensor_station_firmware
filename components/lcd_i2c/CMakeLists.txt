idf_component_register(SRCS "lcd_i2c.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES sensor_i2c
                    PRIV_REQUIRES board_config esp_rom)
