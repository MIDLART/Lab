#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <malloc.h>

typedef struct node{
    double data;
    struct node *next;
} node;

typedef struct {
    node *head, *tail;
} List;

void push_back(List *list, double d) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node -> data = d;
    new_node -> next = NULL;
    if (list -> tail != NULL) {
        list -> tail -> next = new_node;
    }
    if (list -> head == NULL) {
        list -> head = new_node;
    }
    list -> tail = new_node;
}

double pop_front(List *list) {
    double elem;
    if (list -> head == list -> tail) {
        elem = list -> tail -> data;
        free(list -> tail);
        list -> head = list -> tail = NULL;
        return elem;
    }

    elem = list -> head -> data;
    node *t_node = list -> head;
    list -> head = t_node -> next;
    free(t_node);
    return elem;
}

int check_parameters(int argc, char* argv[]) {
    if (argc < 4 || (argv[1][0] != '-' && argv[1][0] != '/')) {
        return -1;
    }
    if (argv[1][2]) {
        return -2;
    }
}

int conversion_to_int(char* argv){
    int num = 0;
    for(int i = 0; i < strlen(argv); i++) {
        if (i == 9) {
            if (INT_MAX / 10 < num) {
                return -1;
            } else if (INT_MAX / 10 == num && argv[i] - '0' > 7) {
                return -1;
            }
        }
        num = num * 10 + argv[i] - '0';
    }
    return num;
}

typedef enum {
    ok,
    overflow,
    invalid_parameter,
    incorrect_EPS
} status;

status flag_m (char* argv[], int *res_1, int *res_2){
    for(int i = 0; i < strlen(argv[2]); i++) {
        if (argv[2][i] < '0' || argv[2][i] > '9') {
            return invalid_parameter;
        }
    }

    for(int i = 0; i < strlen(argv[3]); i++) {
        if (argv[3][i] < '0' || argv[3][i] > '9') {
            return invalid_parameter;
        }
    }

    if (strlen(argv[2]) > 10 || strlen(argv[3]) > 10) {
        return overflow;
    } else {
        int num_1 = conversion_to_int(argv[2]);
        int num_2 = conversion_to_int(argv[3]);
        if (num_1 == -1 || num_2 == -1) {
            return overflow;
        }
        *res_1 = num_1;
        *res_2 = num_2;
        return ok;
    }
}

bool multiplicity(int num_1, int num_2){
    return num_1 % num_2 == 0;
}

double conversion_to_double(char* argv){
    double num = 0;

    int negative = 0;
    if (argv[0] == '-') {
        negative = 1;
    }

    int point = 0;
    for(int i = negative; i < strlen(argv); i++) {
        if (argv[i] == '.' || argv[i] == ',') {
            point = i;
        } else {
            num = num * 10 + argv[i] - '0';
        }
    }
    
    if (point != 0) {
        num /= pow(10, (strlen(argv) - 1 - point));
    }
    
    if (negative == 1) {
        num *= -1;
    }
    return num;
}

status flag_q_t (int argc, char* argv[], double *EPS,
                 double *num_1, double *num_2, double *num_3) {
    
    if (argc != 6) {
        return invalid_parameter;
    }

    for (int j = 2; j <= 5; j++){
        int negative = 0;
        if (argv[j][0] == '-') {
            negative = 1;
        }

        if (strlen(argv[j]) > 30){
            return overflow;
        }

        int point = 0;
        for(int i = negative; i < strlen(argv[j]); i++) {
            if (argv[j][i] < '0' || argv[j][i] > '9') {
                if (argv[j][i] == '.' || argv[j][i] == ',' && 
                    point == 0 && i != 0 && i != strlen(argv[j]) - 1) {
                    point = i;
                } else {
                    return invalid_parameter;
                }
            }
        }
    }

    if (conversion_to_double(argv[2]) >= 1 || conversion_to_double(argv[2]) < 1.0E-15 || argv[2][1] == '-') {
        return incorrect_EPS;
    } else {
        *EPS = conversion_to_double(argv[2]);
        *num_1 = conversion_to_double(argv[3]);
        *num_2 = conversion_to_double(argv[4]);
        *num_3 = conversion_to_double(argv[5]);
        return ok;
    }
}

bool right_triangle(double EPS, double num_1, double num_2, double num_3) {
    num_1 = pow(num_1, 2);
    num_2 = pow(num_2, 2);
    num_3 = pow(num_3, 2);
    if (fabs(num_1 + num_2 - num_3) < EPS || 
        fabs(num_1 + num_3 - num_2) < EPS || fabs(num_3 + num_2 - num_1) < EPS) {
        return true;
    } else {
        return false;
    }
}

void quadratic_equation (double EPS, double a, double b, double c, List *list){
    double D = pow(b, 2) - 4 * a * c;
    double x_1, x_2;

    if (fabs(D) <= EPS) {
        x_1 = (-b) / (2 * a);
        if (x_1 == -0) {
            x_1 = 0;
        }
        push_back(list, x_1);
    } else if (D > EPS) {
        x_1 = (-b - sqrt(D)) / (2 * a);
        if (x_1 == -0) {
            x_1 = 0;
        }
        push_back(list, x_1);
        x_2 = (-b + sqrt(D)) / (2 * a);
        if (x_2 == -0) {
            x_2 = 0;
        }
        push_back(list, x_2);
    } else {
        return;
    }
}

void permutation(double EPS, double a, double b, double c, List *list) {
    double tmp;
    for (int i = 0; i < 3; i++) {
        tmp = b;
        b = c;
        c = tmp;
        if (fabs(a) > EPS) {
            quadratic_equation (EPS, a, b, c, list);
        }
        tmp = a;
        a = b;
        b = tmp;
        if (fabs(a) > EPS) {
            quadratic_equation (EPS, a, b, c, list);
        }
    }
}

int main(int argc, char* argv[]){
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    switch (check_parameters(argc, argv)){
        case -1:
            printf("Неверный ввод аргументов!\n");
            return 1;
        case -2:
            printf("Неверный ввод флага!\n");
            return 1;
    }

    if (argv[1][1] == 'm') {
        int num_1, num_2;
        if (argc != 4) {
            printf("Неверный ввод аргументов!\n");
            return 1;
        }
        
        switch (flag_m(argv, &num_1, &num_2)) {
        case invalid_parameter:
            printf("Неверный ввод аргументов!\nВведите 2 целых неотрицательных числа\n");
            break;
        case overflow:
            printf("Неверный ввод аргументов!\nВведённое число превышает INT_MAX\n");
            break;
        case ok: 
            if (multiplicity(num_1, num_2)){
                printf("%d кратно %d\n", num_1, num_2);
            } else {
                printf("%d не кратно %d\n", num_1, num_2);
            }
            break;
        }
    } else if (argv[1][1] == 'q' || argv[1][1] == 't') {
        double EPS, num_1, num_2, num_3;

        switch (flag_q_t(argc, argv, &EPS, &num_1, &num_2, &num_3)) {
        case invalid_parameter:
            printf("Неверный ввод аргументов!\nВведите 4 числа\n");
            break;
        case overflow:
            printf("Неверный ввод аргументов!\nВведённое число слишком длинное\n");
            break;
        case incorrect_EPS:
            printf("Неверный ввод аргументов!\nВведите эпсилон от 1E-15 до 1 в виде десятичной дроби\n");
            break;
        case ok: 
            if (argv[1][1] == 't'){
                if (right_triangle(EPS, num_1, num_2, num_3)){
                    printf("%.15f, %.15f, %.15f - являются сторонами прямоугольного треугольника", 
                            num_1, num_2, num_3);
                } else {
                    printf("%.15f, %.15f, %.15f - не являются сторонами прямоугольного треугольника", 
                            num_1, num_2, num_3);
                }
            }

            if (argv[1][1] == 'q'){
                int root = 0;

                List *list = (List*)malloc(sizeof(List));
                if (list){
                    list -> head = NULL;
                    list -> tail = NULL;

                    permutation(EPS, num_1, num_2, num_3, list);

                    printf("Все вещественные решения уравнения для всех возможных перестановок:\n");
                    while (list -> head != NULL) {
                        printf("%.15f\n", pop_front(list));
                        root++;
                    }

                    if (root == 0) {
                        printf("Корней не существет\n");
                    }

                    free(list);
                } else {
                    printf("Ошибка! Не выделилась память\n");
                    return 1;
                }
            }
            break;
        }
    } else {
        printf("Неверный ввод флага!\n");
        return 1;
    }

    return 0;
}
