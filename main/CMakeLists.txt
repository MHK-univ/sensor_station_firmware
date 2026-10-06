idf_component_register(SRCS "main.c" "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES board_config sensor_i2c)
