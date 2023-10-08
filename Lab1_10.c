#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>

typedef enum {
    ok,
    invalid_arguments,
    overflow,
    no_stop,
    wrong_system,
    memory_not_allocated
} status_code;

status_code check_param_and_find_max (int argc, char* argv[], int *num_system, int *max_mod_index) {
    if (argc < 4) {
        return invalid_arguments;
    }

    char end_of_input[] = "Stop";
    if (strcmp(argv[argc - 1], end_of_input) != 0) {
        return no_stop;
    }

    int system = 0;
    for (int k = 0; k < strlen(argv[1]); k++) {
        if (argv[1][k] < '0' || argv[1][k] > '9') {
            return invalid_arguments;
        }
        if (k > 1) {
            return wrong_system;
        }
        system = system * 10 + argv[1][k] - '0';
    }
    
    if (system < 2 || system > 36) {
        return wrong_system;
    }

    *num_system = system;

    int otr;
    errno = 0;
    for (int i = 2; i <= argc - 2; i++) {
        if (argv[i][0] == '-') {
            otr = 1;
        }
        for(int j = otr; j < strlen(argv[i]); j++) {
            if (isdigit(argv[i][j]) != 0 && isalpha(argv[i][j]) != 0) {
                return invalid_arguments;
            }
        }

        if (strtol(argv[i], NULL, system) > strtol(argv[*max_mod_index], NULL, system)) {
            *max_mod_index = i;
        }
    }

    if (errno == ERANGE) {
        return overflow;
    }
    
    return ok;
}

status_code conv_to_dec (char num[], int num_system, long long *decimal) {
    long long res = 0;
    int i, cur, otr = 0;
    if (num[0] == '-') {
        otr = 1;
    }
    i = otr;
    while (num[i] != '\0') {
        cur = num[i];
        if (num[i] >= 'a') {
            cur -= 39;
        } else if (num[i] >= 'A') {
            cur -= 7;
        }
        cur -= '0';

        if ((LLONG_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    *decimal = res;
    return ok;
}

void conversion_10_to_system (long long num, int system, char *str_num) {
    int i = 63;
    char character;
    while (num != 0) {
        character = num % system;
        if (character > 9) {
            character += 7;
        }
        character += '0';

        str_num[i] = character;
        num /= system;
        i--;
    }
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    int max_mod_index = 2, system = 0;
    switch (check_param_and_find_max(argc, argv, &system, &max_mod_index)) {
        case invalid_arguments:
            printf("Неверный ввод аргументов!\n");
            return -1;

        case no_stop:
            printf("Ввод должен оканчиваться словом Stop\n");
            return -1;

        case wrong_system:
            printf("Диапазон системы счисления [2..36]\n");
            return -1;

        case overflow:
            printf("Переполнение\n");
            return -1;

        default:
            printf("Максимальное по модулю: %s\n", argv[max_mod_index]);
    }

    long long decimal;
    if (conv_to_dec(argv[max_mod_index], system, &decimal) == overflow) {
        printf("Переполнение\n");
        return -1;
    }
    printf("В десятичной системе: %lld\n", decimal);

    char res_arry[65];
    for (int i = 0; i < 64; i++) {
        res_arry[i] = '0'; 
    }
    res_arry[64] = '\0';

    printf("Число в системах счисления с основаниями 36, 27, 18, 9\n");
    if (decimal == 0) {
        printf("0\n0\n0\n");
    } else {
        for (int new_system = 36; new_system >= 9; new_system -= 9) {
            conversion_10_to_system(decimal, new_system, res_arry);
            printf("%s\n", res_arry + strspn(res_arry, "0"));
        }
    }

    return 0;
}