#ifndef MLX90614_H
#define MLX90614_H
#include <math.h>
#include "i2c/i2c.h"
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
        MLX90614(const uint8_t bus);
        ~MLX90614();
        bool error();
        double objectTemperature1();
        double objectTemperature2();
        double ambientTemperature();
        double objectEmissivityCoefficient();
        bool setObjectTemperatureMinMax(const double TO_min, const double TO_max);
        bool setAmbientTemperatureMinMax(const double TA_min, const double TA_max);
        bool setObjectEmissivityCoefficient(const double epsilon); 
        bool enablePacketErrorChecking(); 
        bool disablePacketErrorChecking(); 
    private:
        I2C i2c;
        bool err = false;
        bool write_word(const uint8_t reg, const uint16_t data);
        double temperature(const uint8_t reg);
};

MLX90614::MLX90614(const uint8_t bus) : i2c(bus, MLX90614_I2CADDR) {
    if (i2c.error()) {
        err = true;
    }
    if(!i2c.enablePacketErrorChecking()) {
        err = true;
    }
    if(i2c.read_word(MLX90614_ADDR) != MLX90614_I2CADDR) {
        err = true;
    }
}

MLX90614::~MLX90614() {}

bool MLX90614::error() {
    return err;
}

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

double MLX90614::temperature(const uint8_t reg) {
    double temp = (double)i2c.read_word(reg);
    // convert to Kelvin
    temp *= 0.02;
    // convert to Celsius
    temp -= 273.15;
    return temp;
}


double MLX90614::objectTemperature1() {
    return temperature(MLX90614_TOBJ1);
}

double MLX90614::objectTemperature2() {
    return temperature(MLX90614_TOBJ2);
}

double MLX90614::ambientTemperature() {
    return temperature(MLX90614_TA);
}

bool MLX90614::setObjectEmissivityCoefficient(const double epsilon) {
    if (epsilon > 1.0 || epsilon < 0.1) {
        return false;
    }
    uint16_t emissivity = (uint16_t)round(65535*epsilon);
    if (!write_word(MLX90614_EMISS, emissivity)) {
        return false;
    }
    return true;
}

double MLX90614::objectEmissivityCoefficient() {
    double emissivity = (double)i2c.read_word(MLX90614_EMISS);
    return emissivity/65535.0;
}
#endif
