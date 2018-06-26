#include "mlx90614/mlx90614.h" //drivers
#include <stdlib.h> //getenv

int main() {
    // Initialize I2C resources using BUS in run.sh.linux
    MLX90614 mlx90614(getenv("MLX90614_BUS")[0] - '0');

    // After initialization, check that I2C started correctly
    if (mlx90614.error()) {
        printf("Error: MLX90614 did not initialize\n");
        return 1;
    }

    printf("MLX90614 initialized\n");

    // Set emissivity
    if(!mlx90614.setObjectEmissivityCoefficient(1.0)) {
        printf("Error: Set object emissivity coefficient failed\n");
        return 1;
    }
    printf("Object emissivity coefficient: %f\n", mlx90614.objectEmissivityCoefficient());

    // Set object temperature range
    if(!mlx90614.setObjectTemperatureMinMax(0.0, 380.0)) {
        printf("Error: Set object minimum and maximum temperature failed\n");
        return 1;
    }
    printf("Object minimum temperature: %f\n", mlx90614.objectTemperatureMin());
    printf("Object maximum temperature: %f\n", mlx90614.objectTemperatureMax());

    // Set ambient temperature range
    if(!mlx90614.setAmbientTemperatureMinMax(0.0, 50.0)) {
        printf("Error: Set ambient minimum and maximum temperature failed\n");
        return 1;
    }

    // Read temperature values in °C
    double ambTemp = mlx90614.ambientTemperature();
    printf("Ambient Temperature: %f\n", ambTemp);

    // For single zone sensors, only this value is accurate
    double objTemp1 = mlx90614.objectTemperature1();
    printf("Object Temperature 1: %f\n", objTemp1);

    // For single zone sensors, this value is not accurate
    double objTemp2 = mlx90614.objectTemperature2();
    printf("Object Temperature 2: %f\n", objTemp2);

    printf("MLX90614 install succeeded\n");
    return 0;
}
