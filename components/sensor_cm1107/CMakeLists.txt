idf_component_register(SRCS "cm1107.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES sensor_base sensor_i2c
                    PRIV_REQUIRES board_config)
