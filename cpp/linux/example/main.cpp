#include "mlx90614/mlx90614.h"

int main() {
    MLX90614 mlx90614(0);
    printf("%d\n", mlx90614.error());
    if (mlx90614.error()) {
        printf("Error: MLX90614 did not initialize\n");
        return 1;
    }
    return 0;
}
