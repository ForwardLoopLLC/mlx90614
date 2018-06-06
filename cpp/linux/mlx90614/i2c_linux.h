#ifndef I2C_LINUX_H
#define I2C_LINUX_H
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <linux/i2c-dev.h>

typedef union i2c_smbus_data i2c_data;

class I2C {
    public:
        I2C(const int bus, const int addr);
        ~I2C();
        uint8_t read_byte(const char reg);
        bool write_byte(const char reg, uint8_t data);
        uint16_t read_word(const char reg);
        bool write_word(const char reg, uint16_t data);
    private:
        int file;
        bool err;
        int open(const int bus);
        bool communicate(const unsigned char read_or_write, const char reg, const int size, i2c_data* data);
};

I2C::I2C(const int bus, const int addr) {
    file = open(bus);
    if (err) {
        printf("Error opening i2c bus %d\n", bus);
    }
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        err = true;
    }
    if (err) {
        printf("Error setting slave address %d on i2c bus %d\n", addr, bus);
    }
    if (ioctl(file, I2C_PEC, 1) < 0) {
        err = true;
    }
    if (err) {
        printf("Error setting packet error checking on i2c bus %d\n", bus);
    }
    printf("%d\n", err);
}

I2C::~I2C() {
    close(file);
}

int I2C::open(const int bus) {
    char filename[20];
    snprintf(filename, 19,"/dev/i2c-%d", bus);
    // unistd open
    int file = ::open(filename, O_RDWR);
    if (file < 0) {
       err = true; 
    }
    return file;
}

bool I2C::communicate(const unsigned char read_or_write, const char reg, const int size, i2c_data* data) {
    struct i2c_smbus_ioctl_data bus_data = {
        .read_write = ((read_or_write == 'r') ? I2C_SMBUS_READ : I2C_SMBUS_WRITE),
        .command = reg,
        .size = ((size == 8) ? I2C_SMBUS_BYTE_DATA : I2C_SMBUS_WORD_DATA),
        .data = data
    };
    if (ioctl(file, I2C_SMBUS, &bus_data) < 0) {
        return false;
    }
    return true;
}

uint8_t I2C::read_byte(const char reg) {
    I2C::communicate('r', reg, 8, data);
    return &data;
}

bool I2C::write_byte(const char reg, uint8_t data) {
    return I2C::communicate('w', reg, 8, data);
}

uint16_t I2C::read_word(const char reg) {
    I2C::communicate('r', reg, 16, data);
    return &data;
}

bool I2C::write_word(const char reg, uint16_t data) {
    return I2C::communicate('w', reg, 16, data);
}

#endif
