idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES board_config sensor_base sensor_i2c sensor_manager
                             sensor_bme280 sensor_pm2009 sensor_cm1107
                             sensor_cb_hcho sensor_battery lcd_i2c)
