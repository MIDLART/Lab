#include<stdio.h>
#include<stdbool.h>

int const INT_MAX = 2147483647;

int string_length(char* argv){
    char character = 't'; //чтобы не была равна \0
    int length = 0;
    while(character != '\0'){
        character = argv[length];
        length++;
    }
    return length-1;
}

int conversion_to_int(char* argv, int str_len){
    int num=0;
    for(int i = 0; i < str_len; i++){
        num = num*10 + argv[i] - 48;
    }
    return num;
}

void multiples(int num, int res[]){
    int i = 1;
    if (num != 0){
        while (1) {
            if(num*i <= 100) {
                res[i-1] = num*i;
                i++;
            } else {
                break;
            }
        }
    }
    res[i-1] = '\0';
}

bool check_for_composite(int num){
    if (num > 3){
        for (int i = 2; i <= num/2; i++){
            if (num % i == 0) return true;
        }
    }
    return false;
}

void split_num(char* argv, char res[], int str_len){
    int zeros = 0, j = 0;
    while (argv[zeros] == '0'){
        zeros++;
    }
    if(str_len == zeros){
        res[j] = '0';
    } else {
        for(int i = zeros; i < str_len; i++){
            res[j] = argv[i];
            res[j+1] = ' ';
            j+=2; 
        }
        j-=2;
    }
    res[j+1] = '\0';
}

void exponentiation(int num, long long tab[][10]){
    for (int i = 1; i <= num; i++){
        for (int j = 1; j <= 10; j ++){
            long long res_num = j;
            for (int k = 1; k <= i-1; k++){
                res_num = res_num * j;
            }
            tab[i-1][j-1] = res_num;
        }
    }
}

unsigned long long amount(int num){
    unsigned long long sum = 0;
    for (unsigned long long i = 1; i <= num; i++){
        sum = sum + i;
    }
    return sum;
}

unsigned long long factorial(int num){
    unsigned long long res = 1;
    for (int i = num; i >= 1; i--){
        res = res * i;
    }
    return res;
}

int main(int argc, char* argv[]){
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");
    if (argc != 3 || (argv[2][0] != '-' && argv[2][0] != '/')){
        printf("Неверный ввод аргументов!\nВведите: число -флаг\n");
        return 1;
    } 
    if (argv[2][2] != '\0'){
        printf("Неверный ввод флага!\n");
        return 1;
    }

    int str_len = string_length(argv[1]);
    if (str_len > 10){
        printf("Неверный ввод аргументов!\nСлишком длинный аргумент\n");
        return 1;
    } else if (str_len < 10){
        for(int i = 0; i < str_len; i++){
            if (argv[1][i] < 48 || argv[1][i] > 57){
                printf("Неверный ввод аргументов!\nВведите целое неотрицательное число\n");
                return 1;
            }
        }
    } else {
        long long long_num = conversion_to_int(argv[1], str_len-1)*10 + argv[1][str_len] - 48;
        if(long_num > INT_MAX){
            printf("Неверный ввод аргументов!\nВведённое число превышает INT_MAX\n");
            return 1;
        }
    }

    int num = conversion_to_int(argv[1], str_len);

    switch (argv[2][1]){
        case 'h':
            if (num > 100){
                printf("Данная операция выполняется для чисел <= 100\n");
            } else {
                int res[101], i = 0;
                multiples(num, res);
                while(res[i] != '\0' && i < 100){
                    printf("%d ", res[i]);
                    i++;
                }
                if (res[0] == '\0'){
                    printf("Кратных нет\n");
                }
            }
            break;
        case 'p':
            if (check_for_composite(num)){
                printf("%d является составным числом\n", num);
            } else {
                printf("%d является простым числом\n", num);
            }
            break;
        case 's': {
            char res[2*str_len];
            split_num(argv[1], res, str_len);
            printf("%s\n", res);
            break;
        }
        case 'e':
            if (num > 10 || num < 1){
                printf("Данная операция выполняется для чисел от 1 до 10\n");
            } else {
                long long tab[num][10];
                exponentiation(num, tab);
                printf("Степень\n");
                for (int i = 0; i < num; i++){
                    printf("|%5d|", i+1);
                    for (int j = 0; j < 10; j++){
                        printf("%-12lld", tab[i][j]);
                    }
                    printf("\n");
                }
            }
            break;
        case 'a':
            printf("%llu\n", amount(num));
            break;
        case 'f':
            if (num > 20){
                printf("Данная опрерация выполняется для чисел >= 20\n");
            } else {
                printf("%llu\n", factorial(num));
            }
            break;
        default:
            printf("Неверный ввод флага!\n");
            break;
    }

    return 0;
}