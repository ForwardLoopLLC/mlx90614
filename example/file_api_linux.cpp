#include "mlx90614/mlx90614.h"
#include <stdlib.h> //getenv
#include <stdio.h>
#include <sys/time.h>

double time() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (double) tp.tv_sec+ tp.tv_usec/1000000.0;
}

int main() {
    uint8_t bus = getenv("MLX90614_BUS") - '0';
    printf("Starting MLX90614 on bus: %d\n", bus);
    MLX90614 mlx90614(bus);
    if (mlx90614.error()) {
        printf("Error: MLX90614 did not initialize\n");
        return 1;
    }
    printf("MLX90614 initialized\n");
    printf("Set object min max? %d\n", mlx90614.setObjectTemperatureMinMax(0.0, 150.0));
    printf("Object min? %f\n", mlx90614.objectTemperatureMin());
    printf("Object max? %f\n", mlx90614.objectTemperatureMax());
    printf("Set emiss? %d\n", mlx90614.setObjectEmissivityCoefficient(1.0));
    printf("Emiss? %f\n", mlx90614.objectEmissivityCoefficient());
    printf("Temperature? %f\n", mlx90614.objectTemperature1());
    mlx90614.wait(500000);
    while(true) {
        FILE* fid = fopen(getenv("MLX90614_DATA"), "w");
        if (fid != NULL) {
            fprintf(fid,
                    "%.1f,%3.3f,%3.3f\n",
                    time(),
                    mlx90614.objectTemperature1(),
                    mlx90614.ambientTemperature());
            fflush(fid);
            fclose(fid);
        }
        mlx90614.wait(getenv("MLX90614_DELAY") - '0');
    }
    return 0;
}
