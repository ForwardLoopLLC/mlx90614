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
    printf("PWM? %d\n", mlx90614_0.powerManagementControl());
    printf("PWM? %d\n", mlx90614_1.powerManagementControl());
    printf("Set object min max? %d\n", mlx90614_0.setObjectTemperatureMinMax(0.0, 150.0));
    printf("Set object min max? %d\n", mlx90614_1.setObjectTemperatureMinMax(0.0, 150.0));
    printf("Object min? %f\n", mlx90614_0.objectTemperatureMin());
    printf("Object min? %f\n", mlx90614_1.objectTemperatureMin());
    printf("Object max? %f\n", mlx90614_0.objectTemperatureMax());
    printf("Object max? %f\n", mlx90614_1.objectTemperatureMax());
    printf("Set emiss? %d\n", mlx90614_0.setObjectEmissivityCoefficient(1.0));
    printf("Set emiss? %d\n", mlx90614_1.setObjectEmissivityCoefficient(1.0));
    printf("Set emiss? %f\n", mlx90614_0.objectEmissivityCoefficient());
    printf("Set emiss? %f\n", mlx90614_1.objectEmissivityCoefficient());
    mlx90614_0.wait(500000);
    for(int i = 0; i < 100; i++){
        double objTemp1 = mlx90614_0.objectTemperature1();
        printf("[0, %d] Object Temperature: %f\n", i, objTemp1);
        mlx90614_0.wait(50000);
        objTemp1 = mlx90614_1.objectTemperature1();
        printf("[1, %d] Object Temperature: %f\n", i, objTemp1);
        mlx90614_1.wait(50000);
    }
    return 0;
}
