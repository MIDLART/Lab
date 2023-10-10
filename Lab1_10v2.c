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
    wrong_system
} status_code;

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

status_code check_and_find_max (int *system, char max_mod[]) {
    char end_of_input[] = "Stop", str_num[65];
    str_num[64] = '\0';

    scanf("%65s", str_num);
    while (strcmp(str_num, end_of_input) != 0) {
        if (str_num[64] != '\0') {
            return overflow;
        }

        int otr;
        errno = 0;

        if (str_num[0] == '-') {
            otr = 1;
        }

        for(int j = otr; j < 64; j++) {
            if (isdigit(str_num[j]) != 0 && isalpha(str_num[j]) != 0) {
                return invalid_arguments;
            }
        }

        if (strtoll(str_num, NULL, *system) > strtoll(max_mod, NULL, *system)) {
            strcpy(max_mod, str_num);
        } 

        scanf("%65s", str_num);
    }

    if (errno == ERANGE) {
        return overflow;
    } 

    return ok;
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    int system = 0;
    char max_mod[65];
    long long decimal;

    printf("Введите основание системы\n");
    if (scanf("%d", &system) != 1) {
        printf("Неверный ввод системы\n");
        return wrong_system;
    } else if (system < 2 || system > 36) {
        printf("Неверный ввод системы\n");
        return wrong_system;
    } 
    
    printf("Введите числа\n");
    switch (check_and_find_max(&system, max_mod)) {
        case invalid_arguments:
            printf("Неверный ввод аргументов\n");
            return invalid_arguments;

        case overflow:
            printf("Переполнение\n");
            return overflow;
        
        default:
            printf("Максимальное по модулю: %s\n", max_mod);
            break;
    }

    if (conv_to_dec(max_mod, system, &decimal) == overflow) {
        printf("Переполнение\n");
        return overflow;
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
