#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
    ok,
    invalid_arguments,
    invalid_flag
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 2) {
        return invalid_arguments;
    }
    if (argv[1][0] != '-' || argv[1][2] != '\0') {
        return invalid_flag;
    }

    return ok;
}

status_code my_pow (double *res, double num, int degree) {
    if (degree == 0) {
        *res = 1.0;
        return ok;
    }
    if (degree == 1) {
        *res = num;
        return ok;
    }

    double req_step = 1.0;
    if (degree < 0) {
        my_pow(&req_step, num, -degree);
        *res = 1.0 / req_step;
        return ok;
    }

    my_pow(&req_step, num, degree / 2);
    if (degree % 2 == 0) {
        *res = req_step * req_step;
    } else {
        *res = req_step * req_step * num;
    }

    return ok;
}

status_code geometric_mean (double *res, int quantity, ...) { 
    double composition = 1.0;
    va_list ptr;
    va_start(ptr, quantity);
    for (int i = 0; i < quantity; i++) {
        composition *= va_arg(ptr, double);
    }
    va_end(ptr);
    if (composition < 0.0) {
        if ((quantity & 1)) {
            *res = -pow(-composition, 1.0 / quantity);
            return ok;
        } else {
            return invalid_arguments;
        }
    }

    *res = pow(composition, 1.0 / quantity);
    return ok;
}

int main (int argc, char* argv[]) {
    switch (check_parameters(argc, argv)) {
        case invalid_arguments:
            printf("Invalid arguments\n");
            return invalid_arguments;

        case invalid_flag:
            printf("Invalid flag\n");
            return invalid_arguments;
    }

    switch (argv[1][1]) {
        case '1': {
            double geom_mean;
            if (geometric_mean(&geom_mean, 3, -1.0, 1.0, 1.0) == invalid_arguments) {
                printf("Invalid arguments\n");
                return invalid_arguments;
            }
            printf("%f\n", geom_mean);
            break;
        }
        
        case '2': {
            double res = 1.0;
            my_pow(&res, 2.0, 5);
            printf("%f\n", res);
            break;
        }

        default:
            printf("Invalid flag\n");
            return invalid_flag;
    }


    return ok;
}
