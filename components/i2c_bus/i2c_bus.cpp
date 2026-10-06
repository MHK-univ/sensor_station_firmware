#include "i2c_bus.h"

I2cBus::I2cBus(gpio_num_t sda, gpio_num_t scl, i2c_port_num_t port)
    : sda_(sda), scl_(scl), port_(port) {}

I2cBus::~I2cBus()
{
    if (bus_) i2c_del_master_bus(bus_);
}

esp_err_t I2cBus::init()
{
    i2c_master_bus_config_t cfg = {};
    cfg.i2c_port = port_;
    cfg.sda_io_num = sda_;
    cfg.scl_io_num = scl_;
    cfg.clk_source = I2C_CLK_SRC_DEFAULT;
    cfg.glitch_ignore_cnt = 7;
    cfg.flags.enable_internal_pullup = true;
    return i2c_new_master_bus(&cfg, &bus_);
}

i2c_master_dev_handle_t I2cBus::addDevice(uint8_t addr, uint32_t speed_hz)
{
    i2c_device_config_t cfg = {};
    cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    cfg.device_address = addr;
    cfg.scl_speed_hz = speed_hz;
    i2c_master_dev_handle_t dev = nullptr;
    if (i2c_master_bus_add_device(bus_, &cfg, &dev) != ESP_OK) return nullptr;
    return dev;
}

esp_err_t I2cBus::probe(uint8_t addr, int timeout_ms)
{
    return i2c_master_probe(bus_, addr, timeout_ms);
}
