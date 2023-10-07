#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <malloc.h>

int const arr_size = 20;

typedef enum {
    ok,
    invalid_arguments,
    overflow
} status_code;

status_code check_parameters (int argc, char* argv[], int res[]) {
    if (argc != 3) {
        return invalid_arguments;
    }
    int otr, num;
    for (int j = 1; j <= 2; j++) {
        otr = 0;
        num = 0;
        if (argv[j][0] == '-') {
            otr = 1;
        }
        if (strlen(argv[j]) > (10 + otr)) {
            return overflow;
        }
        for(int i = otr; i < strlen(argv[j]); i++) {
            if (argv[j][i] < '0' || argv[j][i] > '9') {
                return invalid_arguments;
            }
            if (i == (9 + otr)) {
                if (INT_MAX / 10 < num) {
                    return overflow;
                } else if (INT_MAX / 10 == num && argv[j][i] - '0' > 7) {
                    return overflow;
                }
            }
            num = num * 10 + argv[j][i] - '0';
        }
        if (otr) {
            num *= -1;
        }
        res[j - 1] = num;
    }
    return ok;
}

void rand_arr (int arr[], int a, int b, int seed) {
    srand(time(NULL) % seed);
    for (int i = 0; i < arr_size; i++) {
        arr[i] = a + rand() % (b - a + 1);
    }
}

void swap (int arr[]) {
    int min = 0, max = 0;
    for (int i = 0; i < arr_size; i++) {
        if (arr[i] > arr[max]) {
            max = i;
        } else if (arr [i] < arr[min]) {
            min = i;
        }
    }

    int tmp = arr[max];
    arr[max] = arr[min];
    arr[min] = tmp;
}

int rand_size (int a, int b, int seed) {
    srand(time(NULL) % seed);
    int size = a + rand() % (b - a + 1);
    
    return size;
}

int nearest_value (int value, int *arr, int size) {
    int res = 20000;
    for (int i = 0; i < size; i++) {
        if (fabs(value - arr[i]) < fabs(value - res)) {
            res = arr[i];
        }
        if (value - res == 0) {
            break;
        }
    }

    return res;
}

void filling_array (int *arr_A, int *arr_B, int *arr_C, int size, int size_2) {
    for (int i = 0; i < size; i++) {
        arr_C[i] = arr_A[i] + nearest_value(arr_A[i], arr_B, size_2);
    }
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    int res[2];
    switch (check_parameters(argc, argv, res)){
        case invalid_arguments:
            printf("Неверный ввод аргументов!\nВведите 2 целых числа\n");
            return -1;

        case overflow:
            printf("Переполнение\n");
            return -1;
    }

    int arr[arr_size];
    rand_arr(arr, res[0], res[1], 1);

    printf("1. Было:\n");
    for (int i = 0; i < arr_size; i++) {
        printf("%d ", arr[i]);
    }

    swap(arr);
    printf("\n1. Стало:\n");
    for (int i = 0; i < arr_size; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n2. ");
    int size = rand_size(10, 10000, 1);
    int* d_array_A = (int*)malloc(sizeof(int) * size);
    int* d_array_C = (int*)malloc(sizeof(int) * size);

    int size_2 = rand_size(10, 10000, 123);
    int* d_array_B = (int*)malloc(sizeof(int) * size_2);

    if (!d_array_A || !d_array_B || !d_array_C) {
        printf("Ошибка! Не удалось выделить память\n");
        return -1;
    } else {
        rand_arr(d_array_A, -1000, 1000, 1);
        rand_arr(d_array_B, -1000, 1000, 24);

        filling_array(d_array_A, d_array_B, d_array_C, size, size_2);

        printf("Выполнение успешно\n");
    }

    printf("Первые 10 элементов динамичеких массивов\n");
    printf("A\tB\tC\n");
    for (int i = 0; i < 10; i++) {
        printf("%d\t", d_array_A[i]);
        printf("%d\t", d_array_B[i]);
        printf("%d", d_array_C[i]);
        printf("\n");
    }

    free(d_array_A);
    free(d_array_B);
    free(d_array_C);

    return 0;
}
