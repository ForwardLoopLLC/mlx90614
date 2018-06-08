#!/bin/bash
mkdir -p /drivers/i2c/
mkdir -p /drivers/mlx90614/
cp /floop/include/i2c/linux/*.h /drivers/i2c/
cp /floop/mlx90614/*.h /drivers/mlx90614/
g++ /floop/example/main.cpp -I/drivers -o /floop/mlx90614/mlx90614 && /floop/mlx90614/mlx90614
