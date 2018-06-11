#include "mlx90614/mlx90614.h"

int main() {
    MLX90614 mlx90614_0(0);
    MLX90614 mlx90614_1(1);
    if (mlx90614_0.error()) {
        printf("Error: MLX90614 0 did not initialize\n");
        return 1;
    }
    if (mlx90614_1.error()) {
        printf("Error: MLX90614 1 did not initialize\n");
        return 1;
    }
    printf("MLX90614 initialized\n");
    printf("Set object min max? %d\n", mlx90614_0.setObjectTemperatureMinMax(0.0, 100.0));
    printf("Set object min max? %d\n", mlx90614_1.setObjectTemperatureMinMax(0.0, 100.0));
    printf("Set emiss? %d\n", mlx90614_0.setObjectEmissivityCoefficient(1.0));
    printf("Set emiss? %d\n", mlx90614_1.setObjectEmissivityCoefficient(1.0));
    for(int i = 0; i < 5; i++){
        double objTemp1 = mlx90614_0.objectTemperature1();
        printf("[0] Object Temperature: %f\n", objTemp1);
        mlx90614_0.wait(1000000);
        objTemp1 = mlx90614_1.objectTemperature1();
        printf("[1] Object Temperature: %f\n", objTemp1);
        mlx90614_1.wait(1000000);
    }
    return 0;
}
