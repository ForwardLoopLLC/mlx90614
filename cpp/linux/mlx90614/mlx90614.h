#ifndef MLX90614_H
#define MLX90614_H
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
    private:
        I2C i2c;
        int file;
        bool err;
};

MLX90614::MLX90614(void) {
}

MLX90614::~MLX90614() {
    int result = close(file);
    err = result;
    printf("%d\n", err);
}
#endif
