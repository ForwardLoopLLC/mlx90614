#!/bin/bash
mkdir -p /drivers/mlx90614/
cp /floop/mlx90614/*.h /drivers/mlx90614/
g++ /floop/example/main.cpp -I/drivers -o /floop/mlx90614/mlx90614 && /floop/mlx90614/mlx90614
