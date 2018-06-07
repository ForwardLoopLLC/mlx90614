#ifndef MLX90614_H
#define MLX90614_H
#include <math.h>
#include "i2c_linux.h"
#define MLX90614_I2CADDR 0x5A

// RAM registers
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08

// EEPROM registers
#define MLX90614_ID1 0x1C
#define MLX90614_ID2 0x1D
#define MLX90614_ID3 0x1E
#define MLX90614_ID4 0x1F
//// these registers are the same
//// as the EEPROM in the datasheet + 20
#define MLX90614_TOMAX 0x20
#define MLX90614_TOMIN 0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x2E

class MLX90614 {
    public:
        MLX90614();
        ~MLX90614();
        double objectTemperature();
        bool setObjectTemperatureMinMax(const double TO_min, const double TO_max);
        bool setAmbientTemperatureMinMax(const double TA_min, const double TA_max);
        bool setObjectEmissivityCoefficient(const double epsilon); 
    private:
        I2C i2c;
        bool err;
        bool write_word(const uint8_t reg, const uint16_t data);
};

MLX90614::MLX90614() : i2c(1, MLX90614_I2CADDR){};

MLX90614::~MLX90614() {}

bool MLX90614::write_word(const uint8_t reg, const uint16_t data) {
    // erase data before write
    if(!i2c.write_word(reg, 0)) {
        return false;
    }
    // writes need to be slow to communicate correctly
    i2c.wait(5000);
    if(!i2c.write_word(reg, data)) {
        return false;
    }
    return true;
}

bool MLX90614::setObjectTemperatureMinMax(const double TO_min, const double TO_max) {
    if (TO_max - TO_min < 1) {
        return false;
    }
    uint16_t min_data = (uint16_t)round((TO_min + 273.15)*100);
    uint16_t max_data = (uint16_t)round((TO_max + 273.15)*100);
    if (!write_word(MLX90614_TOMIN, min_data)) {
        return false;
    }
    if (!write_word(MLX90614_TOMAX, max_data)) {
        return false;
    }
    return true;
}

bool MLX90614::setAmbientTemperatureMinMax(const double TA_min, const double TA_max) {
    if (TA_max - TA_min < 1) {
        return false;
    }
    uint16_t min_byte = (uint16_t)round((TA_min+38.2)*(100/64));
    uint16_t max_byte = (uint16_t)round((TA_max+38.2)*(100/64));
    uint16_t TA_range = (max_byte << 8) + min_byte ;
    if (!write_word(MLX90614_TARANGE, TA_range)) {
        return false;
    }
    return true;
}

double MLX90614::objectTemperature() {
    double temp = (double)i2c.read_word(MLX90614_TOBJ2);
    // convert to Kelvin
    temp *= 0.02;
    // convert to Celsius
    temp -= 273.15;
    return temp;
}
#endif
