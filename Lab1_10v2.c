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
    wrong_system,
    max_is_zero
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
    int i = 64, otr = 0;
    char character;
    if (num < 0) {
        num *= -1;
        otr = 1;
    }
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
    if (otr) {
        str_num[i] = '-';
    }
}

status_code check_and_find_max (int *system, char *max_mod) {
    char end_of_input[] = "Stop", str_num[66];
    str_num[65] = '\0';
    int i = 0, otr, flag_no_num = 1;

    if (scanf("%64s", &str_num) != 1) {
        invalid_arguments;
    }
    
    while (strcmp(str_num, end_of_input) != 0) {
        flag_no_num = 0;

        if (str_num[65] != '\0') {
            return overflow;
        }
        if (strcmp(str_num, "0") == 0){
            strcpy(max_mod, "0");
        }

        errno = 0;
        otr = 0;
        i = 0;

        if (str_num[0] == '-') {
            otr = 1;
        }

        for(int j = otr; j < 65; j++) {
            if (isdigit(str_num[j]) != 0 && isalpha(str_num[j]) != 0) {
                return invalid_arguments;
            }
        }

        if (fabs(strtoll(str_num, NULL, *system)) > fabs(strtoll(max_mod, NULL, *system))) {
            strcpy(max_mod, str_num);
        } 

        if (scanf("%64s", &str_num) != 1) {
            invalid_arguments;
        }
    }

    if (errno == ERANGE) {
        return overflow;
    } 

    if (flag_no_num) {
        return invalid_arguments;
    }

    return ok;
}

int main () {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    int system = 0;
    char max_mod[66];
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

        case max_is_zero:
            max_mod[0] = '0';
            max_mod[1] = '\0';
            break;;
        
        default:
            break;
    }
    printf("Максимальное по модулю: %s\n", max_mod);

    if (conv_to_dec(max_mod, system, &decimal) == overflow) {
        printf("Переполнение\nПереполнение\n");
        return overflow;
    }
    printf("Максимальное по модулю: %s\n", max_mod);
    printf("В десятичной системе: %lld\n", decimal);

    char res_arry[66];
    for (int i = 0; i < 65; i++) {
        res_arry[i] = '0'; 
    }
    res_arry[65] = '\0';

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
