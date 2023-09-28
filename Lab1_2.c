#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

//2,71 82 81 82 84 59 04 52 35 36

//3,14 15 92 65 35 89 79 32 38 46

//0,69 31 47 18 05 59 94 53 09 41

//1,41 42 13 56 23 73 09 50 48 80

//0,57 72 15 66 49 01 53 28 60 60

int check_parameters(int argc, char* argv[]) {
    if (argc != 3 || (argv[2][0] != '-')) {
        return -1;
    }
    if (argv[2][2]) {
        return -2;
    }
    if (argv[1][5]) {
        return -3;
    }
    if (argv[1][0] != '1' || (argv[1][1] != 'E' && argv[1][1] != 'e') 
        || argv[1][2] != '-' || argv[1][3] < '0' || argv[1][3] > '9'
        || (argv[1][4] && (argv[1][4] < '0' || argv[1][4] > '9'))) {
        return -4;
    }
}

int check_degree(char* argv[]) {
    int degree = argv[1][3] - '0';
    if (argv[1][4] != 0) {
        degree = degree * 10 + argv[1][4] - '0';
    }
    if (degree > 16) {
        return -5;
    }
    return degree;
}

double e_limit(double EPS) {
    double lim, prev_lim = 0;
    unsigned long long n = 1;
    do {
        prev_lim = lim;
        lim = pow(1 + 1.0 / n, n);
        n*=2;
    } while (fabs(lim - prev_lim) >= EPS);
    return lim;
}

double e_row(double EPS) {
    double sum = 0, prev_sum = -1;
    unsigned long long factorial_n = 1;
    for (int n = 0; n <= 20; n++) {
        factorial_n *= n;
        if (n == 0) {
            factorial_n = 1;
        }
        sum = sum + 1.0 / (factorial_n);
        if (sum - prev_sum < EPS) {
            break;
        }
        prev_sum = sum;
    }
    return sum;
}

double e_equation(double EPS) {
    double left_e = 2.7;
    double right_e = 2.8;
    double x;
    do {
        x = (left_e + right_e) / 2;
        if (log(x) > 1) {
            right_e = x;
        } else {
            left_e = x;
        }
    } while (fabs(left_e - right_e) >= EPS);
    return x;
}

double pi_limit(double EPS) {
    double lim = 4.0, prev_lim = 0;
    unsigned long long n = 1;
    do {
        prev_lim = lim;
        lim *= ((double) (4 * n * (n+1)) / pow((2*n + 1), 2));
        n++;
    } while (fabs(lim - prev_lim) >= EPS);
    return lim;
}

double pi_row(double EPS) {
    double sum = 1.0, prev_sum;
    int n = 1;
    do{
        prev_sum = sum;
        n++;
        sum -= 1.0 / (2 * n - 1);
        n++;
        sum += 1.0 / (2 * n - 1);
    } while ((fabs(prev_sum - sum)) >= EPS);
    return 4 * sum;
}

double pi_equation(double EPS) {
    double left_e = 3.1;
    double right_e = 3.2;
    double x;
    do {
        x = (left_e + right_e) / 2;
        if (sin(x) < 0) {
            right_e = x;
        } else {
            left_e = x;
        }
    } while (fabs(left_e - right_e) >= EPS);
    return x;
}

double ln_2_limit(double EPS) {
    double lim, prev_lim = 0, inverse_n = 1;
    unsigned long long n = 1;
    do {
        prev_lim = lim;
        lim = n * (pow(2, inverse_n) - 1);
        n *= 2;
        inverse_n/=2;
    } while (fabs(lim - prev_lim) >= EPS);
    return lim;
}

double ln_2_row(double EPS) {
    double sum = 1.0, prev_sum;
    int n = 1;
    do {
        prev_sum = sum;
        n++;
        sum -= 1.0 / n;
        n++;
        sum += 1.0 / n;
    } while ((fabs(prev_sum - sum)) >= EPS);
    return sum;
}

double ln_2_equation(double EPS) {
    double left_ln = 0.6;
    double right_ln = 0.7;
    double x;
    do {
        x = (left_ln + right_ln)/2;
        if (exp(x) > 2) {
            right_ln = x;
        } else {
            left_ln = x;
        }
    } while (fabs(left_ln - right_ln) >= EPS);
    return x;
}

double root_2_limit(double EPS) {
    double x = -0.5, prev_x = 0;
    do {
        prev_x = x;
        x = x * (1 - x / 2) + 1;
    } while (fabs(x - prev_x) >= EPS);
    return x;
}

double root_2_composition(double EPS) {
    double prev_composition, composition, two_degree = pow(2, 0.25);
    composition = two_degree;
    do {
        prev_composition = composition;
        two_degree = sqrt(two_degree);
        composition *= two_degree; 
    } while (composition - prev_composition >= EPS);
    return composition;
}

double root_2_equation(double EPS) {
    double left = 1.4;
    double right = 1.5;
    double x;
    do {
        x = (left + right) / 2;
        if (pow(x, 2) > 2) {
            right = x;
        } else {
            left = x;
        }
    } while (right - left >= EPS);
    return x;
}

double euler_const_limit(double EPS) {
    double lim = 1, prev_lim;
    int n = 2;
    do {
        prev_lim = lim;
        lim += 1.0 / n;
        n++;
    } while (fabs(lim - prev_lim) >= EPS);
    return lim - log(n - 1);
}

double euler_const_row(double EPS) {
    double PI = 3.14159265358979323846;
    double sum = -pow(PI, 2) / 6, prev_sum, root;
    int k = 2, whole_part, zero_difference;
    do {
        prev_sum = sum;
        whole_part = sqrt(k);
        if (sqrt(k) == whole_part){
            zero_difference = 1;
        } else {
            sum += 1.0 / pow(whole_part, 2) - 1.0 / k;
            zero_difference = 0;
        }
        k++;
    } while (zero_difference == 1 || (fabs(prev_sum - sum)) >= EPS );
    return sum;
}

bool check_for_composite(int num) {
    if (num > 3){
        if (num % 2 == 0) {
            return true;
        }
        int sqrt_num = sqrt(num);
        for (int i = 3; i <= sqrt_num; i += 2){
            if (num % i == 0) {
                return true;
            }
        }
    }
    return false;
}

double euler_const_equation(double EPS) {
    int prime = 2;
    double equation = log(2) * 0.5, prev_equation, composition = 0.5;
    do {
        prev_equation = equation;
        do {
            prime++;
        } while (check_for_composite(prime));
        composition *= (prime - 1.0) / prime;
        equation = log(prime) * composition;
    } while (fabs(equation - prev_equation) >= EPS);
    return -log(equation);
}

int main(int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    int check_res;
    switch (check_parameters(argc, argv)){
        case -1:
            printf("Неверный ввод аргументов!\nВведите: эпсилон -флаг\n");
            return 1;
        case -2:
            printf("Неверный ввод флага!\n");
            return 1;
        case -3:
            printf("Неверный ввод аргумента!\nСлишком длинный аргумент\n");
            return 1;
        case -4:
            printf("Неверный ввод аргумента!\nВведите эпсилон в виде: 1E-число\n");
            return 1;
        default:
            check_res = check_degree(argv);
            if (check_res == -5){
                printf("Неверный ввод аргумента!\nЭпсилон должен быть не меньше 1E-16\n");
                return 1;
            }
    }
    double EPS = pow(10, -check_res);

    switch (argv[2][1]){
        case 'e':
            if (EPS == 1.0E-16){
                EPS = 1.0E-15;
                printf("Значение EPS было изменено на минимальное для этих функций: 1.0E-15\n");
            }

            double e = e_limit(EPS);
            printf("e = %.16f\n", e);
            e = e_row(EPS);
            printf("e = %.16f\n", e);
            e = e_equation(EPS);
            printf("e = %.16f\n", e);

            break;
        case 'p': {
            int small_EPS = 0;
            double EPS_2 = EPS;
            if (EPS == 1.0E-16) {
                small_EPS = 1;
                EPS_2 = 1.0E-15;
            }

            double pi; 
            pi = pi_limit(EPS_2); //15
            printf("Pi = %.16f", pi);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            pi = pi_row(EPS); //16
            printf("Pi = %.16f\n", pi);

            pi = pi_equation(EPS_2); //15
            printf("Pi = %.16f", pi);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            break;
        }
        case 'l': {
            int small_EPS = 0;
            double EPS_2 = EPS;
            if (EPS == 1.0E-16) {
                small_EPS = 1;
                EPS_2 = 1.0E-15;
            }

            double ln_2;
            ln_2 = ln_2_limit(EPS_2); //15
            printf("ln2 = %.16f", ln_2);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            ln_2 = ln_2_row(EPS); //16
            printf("ln2 = %.16f\n", ln_2);

            ln_2 = ln_2_equation(EPS_2); //15
            printf("ln2 = %.16f", ln_2);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            break;
        }
        case 'r': {
            int small_EPS = 0;
            double EPS_2 = EPS;
            if (EPS == 1.0E-16) {
                small_EPS = 1;
                EPS_2 = 1.0E-15;
            }

            double root_2;
            root_2 = root_2_limit(EPS_2); //15
            printf("2^0.5 = %.16f", root_2);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            root_2 = root_2_composition(EPS); //16
            printf("2^0.5 = %.16f\n", root_2);
            
            root_2 = root_2_equation(EPS_2); //15
            printf("2^0.5 = %.16f", root_2);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            break;
        }
        case 'g': {
            double EPS_2 = EPS;
            int small_EPS = 0;
            if (EPS < 1.0E-8){
                small_EPS = 1;
                EPS_2 = 1.0E-8;
            }

            double euler_const;
            euler_const = euler_const_limit(EPS_2); //8
            printf("γ = %.16f", euler_const);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-8");
            }
            printf("\n");
            small_EPS = 0;

            if (EPS == 1.0E-16){
                small_EPS = 1;
                EPS_2 = 1.0E-15;
            }
            euler_const = euler_const_row(EPS_2); //15
            printf("γ = %.16f", euler_const);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-15");
            }
            printf("\n");

            if (EPS < 1.0E-13){
                small_EPS = 1;
                EPS_2 = 1.0E-13;
            }
            euler_const = euler_const_equation(EPS_2); //13
            printf("γ = %.16f", euler_const);
            if (small_EPS) {
                printf("\tЗначение EPS было изменено на минимальное для этой функции: 1.0E-13");
            }
            printf("\n");

            break;
        }
        default:
            printf("Неверный ввод флага!\nФлаги: -e, -p, -l, -r, -g\n");
            break;
    }

    return 0;
}
