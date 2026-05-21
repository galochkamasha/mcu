#include "bme280-driver.h"
#include <stdio.h>

// Определения регистров BME280
#define BME280_REG_id        0xD0
#define BME280_REG_ctrl_hum  0xF2
#define BME280_REG_config    0xF5
#define BME280_REG_ctrl_meas 0xF4
#define BME280_REG_temp_msb  0xFA
#define BME280_REG_press_msb 0xF7
#define BME280_REG_hum_msb   0xFD

static bme280_ctx_t bme280_ctx = {0};

void bme280_init(bme280_i2c_read i2c_read, bme280_i2c_write i2c_write)
{
    bme280_ctx.i2c_read = i2c_read;
    bme280_ctx.i2c_write = i2c_write;

    uint8_t id_reg_buf[1] = {0};
    bme280_read_regs(BME280_REG_id, id_reg_buf, sizeof(id_reg_buf));
    if (id_reg_buf[0] != 0x60) 
    {
        printf("Invalid BME280 ID: 0x%X (expected 0x60)\n", id_reg_buf[0]);
    }
    else
    {
        printf("BME280 detected OK\n");
    }

    uint8_t ctrl_hum_reg_value = 0;
    ctrl_hum_reg_value |= (0b001 << 0);
    bme280_write_reg(BME280_REG_ctrl_hum, ctrl_hum_reg_value);

    uint8_t config_reg_value = 0;
    config_reg_value |= (0b0 << 0);
    config_reg_value |= (0b000 << 2);
    config_reg_value |= (0b001 << 5);
    bme280_write_reg(BME280_REG_config, config_reg_value);

    uint8_t ctrl_meas_reg_value = 0;
    ctrl_meas_reg_value |= (0b001 << 5);
    ctrl_meas_reg_value |= (0b001 << 2);
    ctrl_meas_reg_value |= (0b11 << 0);
    bme280_write_reg(BME280_REG_ctrl_meas, ctrl_meas_reg_value);
}

void bme280_read_regs(uint8_t start_reg_address, uint8_t* buffer, uint8_t length)
{
    uint8_t data[1] = {start_reg_address};
    bme280_ctx.i2c_write(data, sizeof(data));
    bme280_ctx.i2c_read(buffer, length);
}

void bme280_write_reg(uint8_t reg_address, uint8_t value)
{
    uint8_t data[2] = {reg_address, value};
    bme280_ctx.i2c_write(data, sizeof(data));
}

uint16_t bme280_read_temp_raw()
{
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_temp_msb, read, sizeof(read));
    uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
    return value;
}

uint16_t bme280_read_press_raw()
{
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_press_msb, read, sizeof(read));
    uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
    return value;
}

uint16_t bme280_read_hum_raw()
{
    uint8_t read[2] = {0};
    bme280_read_regs(BME280_REG_hum_msb, read, sizeof(read));
    uint16_t value = ((uint16_t)read[0] << 8) | ((uint16_t)read[1]);
    return value;
}