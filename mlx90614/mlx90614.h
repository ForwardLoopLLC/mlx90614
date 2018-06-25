#ifndef MLX90614_H
#define MLX90614_H
#include <math.h>
#include "i2c/i2c.h"
#define MLX90614_I2CADDR 0x5A

// RAM registers
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
//// Ambient temperature
#define MLX90614_TA 0x06
//// Object temperature (dual zone 1)
#define MLX90614_TOBJ1 0x07
//// Object temperature (dual zone 2)
#define MLX90614_TOBJ2 0x08

// EEPROM registers
#define MLX90614_SMBUS_ADDR 0x2E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F
//// these write registers are the same
//// as the EEPROM in the datasheet + 20
//// Object maximum temperature (<= 380)
#define MLX90614_TOMAX 0x20
//// Object minimum temperature
#define MLX90614_TOMIN 0x21
//// Power management control 
#define MLX90614_PWMCTRL 0x22
//// Ambient temperature range
#define MLX90614_TARANGE 0x23
//// Target emissivity coefficient
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x2E

class MLX90614 {
    public:
        //! Constructor initializes I2C resources
        /*!
            \param bus the I2C bus to which the MLX90614 sensor is connected 
        */
        MLX90614(const uint8_t bus);
        //! Destructor cleans up I2C resources
        ~MLX90614();
        //! Pause main thread execution
        /*!
            \param delay time in milliseconds to pause 
        */
        void wait(const int delay);
        //! Check if there was an error during object construction
        /*!
            \return True, if constructor failed. False, if succeeded.
        */ 
        bool error();
        //! Read object temperature from zone 1. For single zone MLX90614 models, this temperature is **more** accurate than `objectTemperature2()`.
        /*!
            \return Object temperature in units of °C
        */
        double objectTemperature1();
        //! Read object temperature from zone 2. For single zone MLX90614 models, this temperature is **less** accurate than `objectTemperature1()`.
        /*!
            \return Object temperature in units of °C
        */
        double objectTemperature2();
        //! Read ambient temperature of the sensor. 
        /*!
            \return Object temperature in units of °C
        */
        double ambientTemperature();
        //! Set object emissivity coefficient. The object emissivity coefficient depends on <a href="https://www.engineeringtoolbox.com/emissivity-coefficients-d_447.html">the material of the object.</a>
        /*!
            \param epsilon the object emissivity coefficient, a number between 0.1 and 1.0. If `epsilon` fails outside of this range, then the method does **not** try to set the emissivity coefficient.
            \return False, if set failed or `epsilon` not between 0.1 and 1.0. True, if set succeeded.
        */
        bool setObjectEmissivityCoefficient(const double epsilon); 
        //! Read current object emissivity coefficient.
        /*!
            \return Object emissivity coefficient, a number between 0.1 and 1.0. 
        */
        double objectEmissivityCoefficient();
        //! Set object temperature minimum and maximum range. The lowest possible minimum temperature is -70 °C. The highest maximum temperature is 380 °C. The difference between the maximum and minimum temperature must be greater than 1 °C.
        /*!
            \param T0_min minimum temperature in units of °C, must be between -70 °C and 380 °C and at least 1 °C less than `T0_max`
            \param T0_max maximum temperature in units of °C, must be between -70 °C and 380 °C and at least 1 °C more than `T0_min`
            \return False, if set failed. True, if set succeeded.
        */
        bool setObjectTemperatureMinMax(const double TO_min, const double TO_max);
        //! Read current minimum temperature setting.
        /*!
            \return Object minimum temperature setting in units of °C, between -70 °C and 380 °C.
        */
        double objectTemperatureMin();
        //! Read current maximum temperature setting.
        /*!
            \return Object maximum temperature setting in units of °C, between -70 °C and 380 °C.
        */
        double objectTemperatureMax();
        //! Set ambient temperature minimum and maximum range. The lowest possible minimum temperature is -40 °C. The highest maximum temperature is 125 °C. The difference between the maximum and minimum temperature must be greater than 1 °C.
        /*!
            \param TA_min minimum temperature in units of °C, must be between -40 °C and 125 °C and at least 1 °C less than `TA_max`
            \param TA_max maximum temperature in units of °C, must be between -40 °C and 125 °C and at least 1 °C more than `TA_min`
            \return False, if set failed. True, if set succeeded.
        */
        bool setAmbientTemperatureMinMax(const double TA_min, const double TA_max);
        //! Enable SMBus packet error checking (PEC). Most models of MLX90614 need PEC in order to communicate over SMBus.
        /*!
            \return False, if enable failed. True, if enable succeeded.
        */
        bool enablePacketErrorChecking(); 
        //! Disable SMBus packet error checking (PEC). Most models of MLX90614 need PEC in order to communicate over SMBus.
        /*!
            \return False, if disable failed. True, if disable succeeded.
        */
        bool disablePacketErrorChecking(); 
    private:
        I2C i2c;
        bool err = false;
        bool write_word(const uint8_t reg, const uint16_t data);
        double temperature(const uint8_t reg);
        uint16_t powerManagementControl();
        uint16_t whoAmI();
};

MLX90614::MLX90614(const uint8_t bus) : i2c(bus, MLX90614_I2CADDR) {
    if (i2c.error()) {
        err = true;
    }
    if(!i2c.enablePacketErrorChecking()) {
        err = true;
    }
}

MLX90614::~MLX90614() {}

uint16_t MLX90614::whoAmI() {
    return i2c.read_word(MLX90614_SMBUS_ADDR) >> 8;
} 

uint16_t MLX90614::powerManagementControl() {
    return i2c.read_word(MLX90614_PWMCTRL);
}

void MLX90614::wait(const int delay) {
    i2c.wait(delay);
}

bool MLX90614::error() {
    return err;
}

bool MLX90614::write_word(const uint8_t reg, const uint16_t data) {
    // erase data before write
    if(!i2c.write_word(reg, 0)) {
        return false;
  }
    // writes need to be slow to communicate correctly
    i2c.wait(500000); // 0.5 seconds
    if(!i2c.write_word(reg, data)) {
        return false;
    }
    i2c.wait(500000); // 0.5 seconds
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

double MLX90614::objectTemperatureMin() {
    double objTempMin = (double)i2c.read_word(MLX90614_TOMIN);
    return objTempMin/100.0 - 273.15;
}

double MLX90614::objectTemperatureMax() {
    double objTempMax = (double)i2c.read_word(MLX90614_TOMAX);
    return objTempMax/100.0 - 273.15;
}
#endif
