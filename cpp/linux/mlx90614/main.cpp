#include "mlx90614.h"

int main() {
    MLX90614 mlx90614(0);
    if (mlx90614.error()) {
        return 1;
    }
    return 0;
}
