#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef enum {
    ok,
    invalid_arguments
} status_code;

double function_1 (double x) {
    return x - sqrt(2);
}

double function_2 (double x) {
    return sqrt(x) - 3;
}

double function_3 (double x) {
    return 5 * x - 7;
}

status_code dichotomy (double *res, double a, double b, double EPS, double (*function)(double)) {
    if (a >= b) {
        return invalid_arguments;
    }

    if (function(a) * function(b) > 0) {
        return invalid_arguments;
    }

    double c = 0;
    while (fabs(b - a) > EPS) {
        c = (b + a)/2;
        if (function(c) < EPS) {
            a = c;
        } else {
            b = c;
        }
    }
    *res = c;

    return ok;
}

int main () {
    double res;
    dichotomy(&res, -0.1, 2.0, 1E-7, function_1);
    printf("%f\n", res);
    dichotomy(&res, -10.0, 100.0, 1E-7, function_2);
    printf("%f\n", res);
    dichotomy(&res, 0.0, 10.0, 1E-7, function_3);
    printf("%f\n", res);

    return ok;
}
