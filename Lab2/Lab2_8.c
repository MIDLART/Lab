#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

typedef enum {
    ok,
    invalid_arguments,
    memory_not_allocated
} status_code;

char char_sum (int *next_digit, int system, char num_1, char num_2) {
    char res;
    if (num_1 >= 'a') {
        num_1 -= 39;
    } else if (num_1 >= 'A') {
        num_1 -= 7;
    }
    num_1 -= '0';

    if (num_2 >= 'a') {
        num_2 -= 39;
    } else if (num_2 >= 'A') {
        num_2 -= 7;
    }
    num_2 -= '0';

    res = num_1 + num_2 + *next_digit;
    *next_digit = 0;
    if (res >= system) {
        *next_digit = 1;
        res -= system;
    }
    
    res += '0';
    if (res > '9') {
        res += 7;
    }

    return res;
}

status_code column_addition(char **res, int system, char* num_1, char* num_2) {
    int size;
    int len_1 = strlen(num_1), len_2 = strlen(num_2);
    if (len_1 > len_2) {
        size = len_1;
    } else {
        size = len_2;
    }
    size += 1;

    char* sum = (char*)malloc(sizeof(char) * size);
    if (sum == NULL) {
        return memory_not_allocated;
    }

    sum[size - 1] = '\0';

    int next_digit = 0;
    len_1--;
    len_2--;
    if (len_1 > len_2) {
        strcpy(sum, num_1);
        for (; len_2 >= 0; len_2--) {
            sum[len_1] = char_sum(&next_digit, system, sum[len_1], num_2[len_2]);
            len_1--;
        }

        while (next_digit == 1 && len_1 >= 0) {
            sum[len_1] = char_sum(&next_digit, system, sum[len_1], '0');
            len_1 --;
        }
    } else {
        strcpy(sum, num_2);
        for (; len_1 >= 0; len_1--) {
            sum[len_2] = char_sum(&next_digit, system, sum[len_2], num_1[len_1]);
            len_2--;
        }

        while (next_digit == 1 && len_2 >= 0) {
            sum[len_2] = char_sum(&next_digit, system, sum[len_2], '0');
            len_2 --;
        }
    }

    if (next_digit == 1) {
        char* sum_1 = (char*)malloc(sizeof(char) * (size + 1));
        if (sum_1 == NULL) {
            free(sum);
            return memory_not_allocated;
        }
            
        sum_1[0] = '1';
        memcpy(sum_1 + 1, sum, sizeof(char) * size);
        free(sum);

        *res = sum_1;
        return ok;
    }

    *res = sum;
    return ok;
}

status_code str_sum (char **res, int system, int n, ...) {
    if (system < 2 || system > 36) {
        return invalid_arguments;
    }

    va_list ptr;
    va_start(ptr, n);

    if (n == 0) {
        *res = "0";
        return ok;
    } else if (n == 1) {
        *res = va_arg(ptr, char*);
        return ok;
    }

    char* sum_res;
    if (column_addition (res, system, va_arg(ptr, char*), va_arg(ptr, char*)) == memory_not_allocated) {
        return memory_not_allocated;
    }
    sum_res = *res;

    for(int i = 2; i < n; i++) {
        if (column_addition (res, system, sum_res, va_arg(ptr, char*)) == memory_not_allocated) {
            return memory_not_allocated;
        }
        free(sum_res);
        sum_res = *res;
    }

    va_end(ptr);
    return ok;
}

int main () {
    char *res;
    if (str_sum(&res, 10, 5, "999", "1", "1", "9999", "10") == ok) {
        printf("%s\n", res);
        free(res);
    }

    if (str_sum(&res, 16, 3, "1", "b", "F") == ok) {
        printf("%s\n", res);
        free(res);
    }

    if (str_sum(&res, 36, 3, "PZ", "1", "F") == ok) {
        printf("%s\n", res);
        free(res);
    }

    return ok;
}
