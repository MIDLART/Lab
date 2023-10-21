#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef enum {
    ok,
    no,
    invalid_arguments
} status_code;

status_code convex_polygon (int dimension, ...) {
    double turn = 0, next_x, next_y, cur_x, cur_y, ab_x, ab_y, bc_y, bc_x, first_x, first_y;
    int bypass_side = 0;

    if (dimension < 3) {
        return invalid_arguments;
    }

    va_list ptr;
    va_start(ptr, dimension);

    first_x = va_arg(ptr, double);
    first_y = va_arg(ptr, double);

    cur_x = va_arg(ptr, double);
    cur_y = va_arg(ptr, double);
    ab_x = cur_x - first_x;
    ab_y = cur_y - first_y;

    for (int i = 2; i < dimension; i++) {
        next_x = va_arg(ptr, double); 
        next_y = va_arg(ptr, double);
        bc_x = next_x - cur_x;
        bc_y = next_y - cur_y;

        turn = ab_x * bc_y - ab_y * bc_x;

        if (bypass_side == 0 && turn != 0) {
            if (turn < 0) {
                bypass_side = -1;
            } else {
                bypass_side = 1;
            }
        }

        if (bypass_side * turn < 0) {
            return no;
        }

        ab_x = bc_x;
        ab_y = bc_y;
        cur_x = next_x;
        cur_y = next_y;
    }

    turn = ab_x * (first_y - cur_y) - ab_y * (first_x - cur_x);
    if (bypass_side * turn < 0) {
        return no;
    }

    if (bypass_side == 0) {
        return invalid_arguments;
    }

    va_end(ptr);
    return ok;
}

status_code polynom (double *res, double x, int n, ...) {
    va_list ptr;
    va_start(ptr, n);

    for (; n > 0; n--) {
        *res += va_arg(ptr, double) * pow(x, n);
    }
    *res += va_arg(ptr, double);

    va_end(ptr);
    return ok;
}

int main () {
    int res;
    if ((res = convex_polygon(5, 0.0,0.0, 0.0,1.0, 0.5,0.5, 1.0,1.0, 1.0,0.0)) == invalid_arguments) {
        printf("Invalid arguments\n");
    } else if (res == ok) {
        printf("convex\n");
    } else {
        printf("not convex\n");
    }

    double res_2;
    polynom(&res_2, 2.0, 2, 1.0, 1.0);
    printf("%f\n", res_2);

    return ok;
}