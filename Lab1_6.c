#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

int check_parameters (int argc, char* argv[]) {
    if (argc != 3 || (argv[2][0] != '-')) {
        return -1;
    }
    if (argv[2][2] || !argv[2][1]) {
        return -2;
    }

    double EPS = strtod(argv[1], NULL);
    if (EPS < 1E-7 || EPS >= 1) {
        return -3;
    }
}

double function_a (double x) {
    if (x == 0) {
        return 1;
    }
    return (log(1 + x) / x);
}

double function_b (double x) {
    return exp(-0.5 * x * x);
}

double function_c (double x) {
    if (x == 1) {
        return 99; 
    }
    return log(1 / (1 - x));
}

double function_d (double x) {
    return pow(x, x);
}

double f (double x, char flag) {
    switch (flag) {
        case 'a':
            return function_a(x);

        case 'b':
            return function_b(x);

        case 'c':
            return function_c(x);

        case 'd':
            return function_d(x);
    }
}

double integral (double a, double b, double EPS, char flag) {
    int n = 2;
    double h, sum;
    do {
        sum = f(a, flag) + f(b, flag);
        h = (b - a) / n;
        for (int i = 1; i <= n - 1; i++) {
            sum += 2 * f(a + i * h, flag);
        }
        n *= 2;
    } while(fabs(h) > EPS && n < INT_MAX / 2);
    sum *= h / 2;
    return sum;
}

int main (int argc, char* argv[]) {
    switch (check_parameters(argc, argv)){
        case -1:
            printf("Неверный ввод аргументов!\nВведите EPS -флаг\n");
            return -1;

        case -2:
            printf("Неверный ввод флага!\n");
            return -1;

        case -3:
            printf("Неверный ввод EPS\nВведите EPS от 1E-7 до 1\n");
            return -1;
    }
    double EPS = strtod(argv[1], NULL);
    double a = 0, b = 1;
    
    if (argv[2][1] == 'a' || argv[2][1] == 'b' || 
        argv[2][1] == 'c' || argv[2][1] == 'd') {
        printf("%c) %.13f\n", argv[2][1], integral(a, b, EPS, argv[2][1]));
    } else {
        printf("Неверный ввод флага!\n");
        return -1;
    }

    return 0;
}