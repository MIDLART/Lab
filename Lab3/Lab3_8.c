#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef enum {
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE
} status_code;

typedef struct _polynom {
    int k;
    int n;
    struct _polynom* next;
} polynom;

typedef struct {
    polynom *head, *tail;
} List;


status_code push_back(List *list, int coef, int degree) {
    polynom *new_polynom = (polynom*)malloc(sizeof(polynom));
    if (new_polynom == NULL) {
        return MEM_NOT_ALLOC;
    }
    new_polynom -> k = coef;
    new_polynom -> n = degree;
    new_polynom -> next = NULL;
    if (list -> tail != NULL) {
        list -> tail -> next = new_polynom;
    }
    if (list -> head == NULL) {
        list -> head = new_polynom;
    }
    list -> tail = new_polynom;

    return OK;
}

status_code push_front(List *list, int coef, int degree) {
    polynom *new_polynom = (polynom*)malloc(sizeof(polynom));
    if (new_polynom == NULL) {
        return MEM_NOT_ALLOC;
    }
    new_polynom -> k = coef;
    new_polynom -> n = degree;
    if (list -> head != NULL) {
        new_polynom -> next = list -> head;
    }
    if (list -> tail == NULL) {
        list -> tail = new_polynom;
    }
    list -> head = new_polynom;

    return OK;
}

status_code insert (List *list, int coef, int degree) {
    if (coef == 0) {
        return OK;
    }

    if (list->head == NULL) {
        if (push_back(list, coef, degree) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        return OK;
    }

    if (list->head->n < degree) {
        if (push_front(list, coef, degree) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        return OK;
    }

    polynom* cur = list->head;
    while (cur->next != NULL && cur->next->n >= degree) {
        cur = cur->next;
    }

    if (cur->n == degree) {
        cur->k += coef;
        return OK;
    }

    if (cur->next == NULL) {
        if (push_back(list, coef, degree) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        return OK;
    }

    polynom *new_polynom = (polynom*)malloc(sizeof(polynom));
    if (new_polynom == NULL) {
        return MEM_NOT_ALLOC;
    }
    new_polynom -> k = coef;
    new_polynom -> n = degree;
    new_polynom->next = cur->next;
    cur->next = new_polynom;
    
    return OK;
}

void del_list(List *list) {
    if (list -> head == NULL) {
        return;
    }
    if (list -> head == list -> tail) {
        free(list -> tail);
        list -> head = list -> tail = NULL;
        return;
    }
    polynom *t_polynom = list -> head;
    list -> head = t_polynom -> next;
    free(t_polynom);
    del_list(list);
}

status_code valid_c (char c) {
    if (!isdigit(c) && c != 'x' && c != '+' && c != '-' && c != '^' && c != ';') {
        return INVALID_ARG;
    }

    return OK;
}

status_code free_all (List* polynom_1, List* polynom_2, List* adder) {
    del_list(polynom_1);
    del_list(polynom_2);
    del_list(adder);
    free(polynom_1);
    free(polynom_2);
    free(adder);

    return OK;
}

status_code test_print (List* polynom_1, List* polynom_2, char* op) {
    printf("%s\n", op);
    
    int flag_1 = 1, flag_2 = 1;

    polynom* cur = polynom_1->head;
    while (cur != NULL) {
        if (cur->k != 0) {
            if (cur->n != 0) {
                printf("%dx^%d ", cur->k, cur->n);
            } else {
                printf("%d", cur->k);
            }
            flag_1 = 0;
        }
        cur = cur->next;
    }

    if (flag_1) {
        printf("0");
    }
    printf("\n");
    
    if (polynom_2 == NULL || polynom_2->head == NULL) {
        return OK;
    }
    cur = polynom_2->head;
    while (cur != NULL) {
        if (cur->k != 0) {
            if (cur->n != 0) {
                printf("%dx^%d ", cur->k, cur->n);
            } else {
                printf("%d", cur->k);
            }
            flag_2 = 0;
        }
        cur = cur->next;
    }

    if (flag_1) {
        printf("0");
    }
    printf("\n");

    return OK;
}

status_code add (List* polynom_1, List* polynom_2, List* res) {
    polynom* cur_1 = polynom_1->head;
    while (cur_1 != NULL) {
        if (insert(res, cur_1->k, cur_1->n) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        cur_1 = cur_1->next;
    }

    polynom* cur_2 = polynom_2->head;
    while (cur_2 != NULL) {
        if (insert(res, cur_2->k, cur_2->n) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        cur_2 = cur_2->next;
    }

    return OK;
}

status_code sub (List* polynom_1, List* polynom_2, List* res) {
    polynom* cur_1 = polynom_1->head;
    while (cur_1 != NULL) {
        if (insert(res, cur_1->k, cur_1->n) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        cur_1 = cur_1->next;
    }

    polynom* cur_2 = polynom_2->head;
    while (cur_2 != NULL) {
        if (insert(res, -cur_2->k, cur_2->n) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
        cur_2 = cur_2->next;
    }

    return OK;
}

status_code mult (List* polynom_1, List* polynom_2, List* res) {
    polynom* cur_1 = polynom_1->head;
    while (cur_1 != NULL) {
        polynom* cur_2 = polynom_2->head;
        while(cur_2 != NULL) {
            if (insert(res, cur_2->k * cur_1->k, cur_2->n + cur_1->n) == MEM_NOT_ALLOC) {
                return MEM_NOT_ALLOC;
            }
            cur_2 = cur_2->next;
        }
        cur_1 = cur_1->next;
    }

    return OK;
}

status_code _div (List* polynom_1, List* polynom_2, List* res) { 
    if (polynom_1->head->n < polynom_2->head->n || polynom_2->head->k == 0) {
        return INVALID_ARG;
    }

    List *ost = (List*)malloc(sizeof(List));
    if (ost != NULL){
        ost -> head = NULL;
        ost -> tail = NULL;
    } else {
        return MEM_NOT_ALLOC;
    }

    List *prev_ost = (List*)malloc(sizeof(List));
    if (prev_ost != NULL){
        prev_ost -> head = NULL;
        prev_ost -> tail = NULL;
    } else {
        free(ost);
        return MEM_NOT_ALLOC;
    }

    List *p_sub = (List*)malloc(sizeof(List));
    if (p_sub != NULL){
        p_sub -> head = NULL;
        p_sub -> tail = NULL;
    } else {
        free(ost);
        free(prev_ost);
        return MEM_NOT_ALLOC;
    }

    int coef, degree;

    polynom* cur_1 = polynom_1->head;
    while(cur_1 != NULL) {
        if (insert(prev_ost, cur_1->k, cur_1->n) == MEM_NOT_ALLOC) {
            free(ost);
            free(p_sub);
            free(prev_ost);
            return MEM_NOT_ALLOC;
        }
        cur_1 = cur_1->next;
    }

    while (prev_ost->head != NULL && prev_ost->head->n >= polynom_2->head->n) {
        coef = prev_ost->head->k / polynom_2->head->k;
        degree = prev_ost->head->n - polynom_2->head->n;

        if (push_back(res, coef, degree) == MEM_NOT_ALLOC) {
            free_all(ost, p_sub, prev_ost);
            return MEM_NOT_ALLOC;
        }

        polynom* cur_2 = polynom_2->head;
        del_list(p_sub);
        while(cur_2 != NULL) {
            if (insert(p_sub, cur_2->k * coef, cur_2->n + degree) == MEM_NOT_ALLOC) {
                free_all(ost, p_sub, prev_ost);
                return MEM_NOT_ALLOC;
            }
            cur_2 = cur_2->next;
        }

        del_list(ost);
        if (sub(prev_ost, p_sub, ost) == MEM_NOT_ALLOC) {
            free_all(ost, p_sub, prev_ost);
            return MEM_NOT_ALLOC;
        }

        del_list(prev_ost);
        polynom* cur_3 = ost->head;
        while(cur_3 != NULL) {
            if (insert(prev_ost, cur_3->k, cur_3->n) == MEM_NOT_ALLOC) {
                free_all(ost, p_sub, prev_ost);
                return MEM_NOT_ALLOC;
            }
            cur_3 = cur_3->next;
        }
        
    }
    
    free_all(ost, p_sub, prev_ost);
    return OK;
}

status_code mod (List* polynom_1, List* polynom_2, List* res) { 
    if (polynom_1->head->n < polynom_2->head->n || polynom_2->head->k == 0) {
        return INVALID_ARG;
    }

    List *d_res = (List*)malloc(sizeof(List));
    if (d_res != NULL){
        d_res -> head = NULL;
        d_res -> tail = NULL;
    } else {
        return MEM_NOT_ALLOC;
    }

    List *prev_ost = (List*)malloc(sizeof(List));
    if (prev_ost != NULL){
        prev_ost -> head = NULL;
        prev_ost -> tail = NULL;
    } else {
        free(d_res);
        return MEM_NOT_ALLOC;
    }

    List *p_sub = (List*)malloc(sizeof(List));
    if (p_sub != NULL){
        p_sub -> head = NULL;
        p_sub -> tail = NULL;
    } else {
        free(d_res);
        free(prev_ost);
        return MEM_NOT_ALLOC;
    }

    int coef, degree;

    polynom* cur_1 = polynom_1->head;
    while(cur_1 != NULL) {
        if (insert(prev_ost, cur_1->k, cur_1->n) == MEM_NOT_ALLOC) {
            free(d_res);
            free(p_sub);
            free(prev_ost);
            return MEM_NOT_ALLOC;
        }
        cur_1 = cur_1->next;
    }

    while (prev_ost->head != NULL && prev_ost->head->n >= polynom_2->head->n) {
        coef = prev_ost->head->k / polynom_2->head->k;
        degree = prev_ost->head->n - polynom_2->head->n;

        if (push_back(d_res, coef, degree) == MEM_NOT_ALLOC) {
            free_all(d_res, p_sub, prev_ost);
            return MEM_NOT_ALLOC;
        }

        polynom* cur_2 = polynom_2->head;
        del_list(p_sub);
        while(cur_2 != NULL) {
            if (insert(p_sub, cur_2->k * coef, cur_2->n + degree) == MEM_NOT_ALLOC) {
                free_all(d_res, p_sub, prev_ost);
                return MEM_NOT_ALLOC;
            }
            cur_2 = cur_2->next;
        }

        del_list(res);
        if (sub(prev_ost, p_sub, res) == MEM_NOT_ALLOC) {
            free_all(d_res, p_sub, prev_ost);
            return MEM_NOT_ALLOC;
        }

        del_list(prev_ost);
        polynom* cur_3 = res->head;
        while(cur_3 != NULL) {
            if (insert(prev_ost, cur_3->k, cur_3->n) == MEM_NOT_ALLOC) {
                free_all(d_res, p_sub, prev_ost);
                return MEM_NOT_ALLOC;
            }
            cur_3 = cur_3->next;
        }
        
    }
    
    free_all(d_res, p_sub, prev_ost);
    return OK;
}

status_code diff (List* polynom_1, List* res) {
    polynom* cur = polynom_1->head;
    while (cur != NULL) {
        if (cur->n != 0) {
            if (push_back(res, cur->k * cur->n, cur->n - 1) == MEM_NOT_ALLOC) {
                return MEM_NOT_ALLOC;
            }
        }
        
        cur = cur->next;
    }
    
    return OK;
}

status_code eval (List* polynom_1, List* polynom_2, List* res) {
    if (polynom_1->head->n != 0) {
        return INVALID_ARG;
    }

    int set_point = polynom_1->head->k, res_1 = 0;
    polynom* cur = polynom_2->head;
    while (cur != NULL) {
        res_1 += pow(set_point, cur->n) * cur->k;

        cur = cur->next;
    }

    if (push_back(res, res_1, 0) == MEM_NOT_ALLOC) {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code fast_exp (List* polynom_1, int degree, List* res) {
    List *res_1 = (List*)malloc(sizeof(List));
    if (res_1 != NULL){
        res_1 -> head = NULL;
        res_1 -> tail = NULL;
    } else {
        return MEM_NOT_ALLOC;
    }

    if (push_back(res_1, 1, 0) == MEM_NOT_ALLOC) {
        free(res_1);
        return MEM_NOT_ALLOC;
    }

    List *mult_res = (List*)malloc(sizeof(List));
    if (mult_res != NULL){
        mult_res -> head = NULL;
        mult_res -> tail = NULL;
    } else {
        free(res_1);
        return MEM_NOT_ALLOC;
    }

    polynom* cur_1 = polynom_1->head;
    while (cur_1 != NULL) {
        if (insert(mult_res, cur_1->k, cur_1->n) == MEM_NOT_ALLOC) {
            del_list(mult_res);
            free(mult_res);
            del_list(res_1);
            free(res_1);
            return MEM_NOT_ALLOC;
        }
        cur_1 = cur_1->next;
    }

    List *mult_res_1 = (List*)malloc(sizeof(List));
    if (mult_res_1 != NULL){
        mult_res_1 -> head = NULL;
        mult_res_1 -> tail = NULL;
    } else {
        del_list(mult_res);
        free(mult_res);
        del_list(res_1);
        free(res_1);
        return MEM_NOT_ALLOC;
    }

    while (degree) {
        if (degree & 1 == 0) {
            degree /= 2;
            del_list(mult_res_1);
            if (mult(mult_res, mult_res, mult_res_1) == MEM_NOT_ALLOC) { 
                free_all(mult_res, mult_res_1, res_1);
                return MEM_NOT_ALLOC;
            }

            polynom* cur_m = mult_res_1->head;
            del_list(mult_res);
            while (cur_m != NULL) {
                if (insert(mult_res, cur_m->k, cur_m->n) == MEM_NOT_ALLOC) {
                    free_all(mult_res, mult_res_1, res_1);
                    return MEM_NOT_ALLOC;
                }
                cur_m = cur_m->next;
            }
        } else {
            degree--;
            del_list(res);
            if (mult(res_1, mult_res, res) == MEM_NOT_ALLOC) {
                free_all(mult_res, mult_res_1, res_1);
                return MEM_NOT_ALLOC;
            }

            del_list(res_1);
            polynom* cur = res->head;
            while (cur != NULL) {
                if (insert(res_1, cur->k, cur->n) == MEM_NOT_ALLOC) {
                    free_all(mult_res, mult_res_1, res_1);
                    return MEM_NOT_ALLOC;
                }
                cur = cur->next;
            }
        }
    }

    free_all(mult_res, mult_res_1, res_1);

    return OK;
}

status_code cmps (List* polynom_1, List* polynom_2, List* res) {
    polynom* cur = polynom_1->head;

    List *res_1 = (List*)malloc(sizeof(List));
    if (res_1 != NULL){
        res_1 -> head = NULL;
        res_1 -> tail = NULL;
    } else {
        return MEM_NOT_ALLOC;
    }

    while (cur != NULL) {
        if (cur->n != 0) {
            if (fast_exp(polynom_2, cur->n, res_1) == MEM_NOT_ALLOC) {
                del_list(res_1);
                free(res_1);
                return MEM_NOT_ALLOC;
            }

            polynom* cur_r = res_1->head;
            while (cur_r != NULL) {
                if (insert(res, cur_r->k * cur->k, cur_r->n) == MEM_NOT_ALLOC) {
                    del_list(res_1);
                    free(res_1);
                    return MEM_NOT_ALLOC;
                }

                cur_r = cur_r->next;
            }
        } else {
            if (insert(res, cur->k, 0) == MEM_NOT_ALLOC) {
                del_list(res_1);
                free(res_1);
                return MEM_NOT_ALLOC;
            }
        }

        cur = cur->next;
    }

    del_list(res_1);
    free(res_1);
    return OK;
}

status_code operations (List* polynom_1, List* polynom_2, char* op, List* res) {
    status_code status;

    if (polynom_1->head == NULL) {
        if (push_back(polynom_1, 0, 0) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }
    }

    if (strcmp("Diff", op) == 0) {
        del_list(res);
        if (diff(polynom_1, res) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        return OK;
    }

    if (polynom_2->head == NULL) {
        if (res->head == NULL) {
            if (push_back(polynom_2, 0, 0) == MEM_NOT_ALLOC) {
                return MEM_NOT_ALLOC;
            }
        } else {
            polynom* cur = res->head;
            while (cur != NULL) {
                if (insert(polynom_2, cur->k, cur->n) == MEM_NOT_ALLOC) {
                    return MEM_NOT_ALLOC;
                }

                cur = cur->next;
            }
        }
    }
    del_list(res);

    if (strcmp("Add", op) == 0) {
        if (add(polynom_1, polynom_2, res) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        return OK;
    }

    if (strcmp("Sub", op) == 0) {
        if (sub(polynom_1, polynom_2, res) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        return OK;
    }

    if (strcmp("Mult", op) == 0) {
        if (mult(polynom_1, polynom_2, res) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        return OK;
    }

    if (strcmp("Div", op) == 0) {
        status = _div(polynom_1, polynom_2, res);
        if (status == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (status == INVALID_ARG) {
            return INVALID_ARG;
        }

        return OK;
    }

    if (strcmp("Mod", op) == 0) {
        status = mod(polynom_1, polynom_2, res);
        if (status == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (status == INVALID_ARG) {
            return INVALID_ARG;
        }

        return OK;
    }

    if (strcmp("Eval", op) == 0) {
        status = eval(polynom_1, polynom_2, res);
        if (status == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (status == INVALID_ARG) {
            return INVALID_ARG;
        }

        return OK;
    }

    if (strcmp("Cmps", op) == 0) {
        if (cmps(polynom_1, polynom_2, res) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        return OK;
    }

    return INVALID_ARG;
}

status_code read (FILE* file) {
    char c;
    int i, coef, degree, otr, fl_coef;
    char operation[5];
    status_code status;

    List *adder = (List*)malloc(sizeof(List));
    if (adder != NULL){
        adder -> head = NULL;
        adder -> tail = NULL;
    } else {
        return MEM_NOT_ALLOC;
    }

    List *polynom_1 = (List*)malloc(sizeof(List));
    if (polynom_1 != NULL){
        polynom_1 -> head = NULL;
        polynom_1 -> tail = NULL;
    } else {
        free(adder);
        return MEM_NOT_ALLOC;
    }

    List *polynom_2 = (List*)malloc(sizeof(List));
    if (polynom_2 != NULL){
        polynom_2 -> head = NULL;
        polynom_2 -> tail = NULL;
    } else {
        free(adder);
        free(polynom_1);
        return MEM_NOT_ALLOC;
    }

    while ((c = getc(file)) != EOF) {
        del_list(polynom_1);
        del_list(polynom_2);

        while (c == ' ' || c == '\n') {
            c = getc(file);
        }
        if (c == EOF) {
            break;
        }

        if (c == '[') {
            while (c != ']' && c != EOF) {
                c = getc(file);
            }
            if (c == EOF) {
                free_all (polynom_1, polynom_2, adder);
                return INVALID_ARG;
            }
            c = getc(file);

        } else if (c == '%') {
            while (c != '\n' && c != EOF) {
                c = getc(file);
            }
            if (c == EOF) {
                free_all (polynom_1, polynom_2, adder);
                return OK;
            }
            
        } else if (isalpha(c)) { 
            i = 0;

            while (c != '(') {
                operation[i] = c;
                c = getc(file);
                i++;
            }

            if (i != 3 && i != 4) {
                free_all (polynom_1, polynom_2, adder);
                return INVALID_ARG;
            }
            operation[i] = '\0';
            
            c = getc(file);
            while (c != ')' && c != ',') {
                coef = 0, degree = 0, otr = 0, fl_coef = 0;

                if (c == '-') {
                    otr = 1;
                    c = getc(file);

                    if (!isdigit(c) && c != 'x') {
                        free_all (polynom_1, polynom_2, adder);
                        return INVALID_ARG;
                    }

                } else if (c == '+') {
                    c = getc(file);

                    if (!isdigit(c) && c != 'x') {\
                        free_all (polynom_1, polynom_2, adder);
                        return INVALID_ARG;
                    }
                }

                while (isdigit(c)) {
                    coef = coef * 10 + c - '0';
                    fl_coef = 1;
                    c = getc(file);
                }

                if (!fl_coef) {
                    coef = 1;
                }

                if (otr) {
                    coef *= -1;
                }

                if (c == '-' || c == '+' || c == ',' || c == ')') {
                    if (insert(polynom_1, coef, 0) == MEM_NOT_ALLOC) {
                        free_all (polynom_1, polynom_2, adder);
                        return MEM_NOT_ALLOC;
                    }

                } else if (c == 'x') {
                    c = getc(file);
                    if (c == '^') {
                        c = getc(file);
                        if (!isdigit(c)) {
                            free_all (polynom_1, polynom_2, adder);
                            return INVALID_ARG;
                        }

                        while (isdigit(c)) {
                            degree = degree * 10 + c - '0';
                            c = getc(file);
                        }

                        if (insert(polynom_1, coef, degree) == MEM_NOT_ALLOC) {
                            free_all (polynom_1, polynom_2, adder);
                            return MEM_NOT_ALLOC;
                        }

                    } else if (c == '+' || c == '-' || c == ',' || c == ')') {
                        if (insert(polynom_1, coef, 1) == MEM_NOT_ALLOC) {
                            free_all (polynom_1, polynom_2, adder);
                            return MEM_NOT_ALLOC;
                        }
                    } else {
                        free_all (polynom_1, polynom_2, adder);
                        return INVALID_ARG;
                    }

                } else {
                    free_all (polynom_1, polynom_2, adder);
                    return INVALID_ARG;
                }
            }

            if (c == ',') {
                c = getc(file);

                while (c != ')') {
                    coef = 0, degree = 0, otr = 0, fl_coef = 0;

                    if (c == '-') {
                        otr = 1;
                        c = getc(file);

                        if (!isdigit(c) && c != 'x') {
                            free_all (polynom_1, polynom_2, adder);
                            return INVALID_ARG;
                        }

                    } else if (c == '+') {
                        c = getc(file);

                        if (!isdigit(c) && c != 'x') {
                            free_all (polynom_1, polynom_2, adder);
                            return INVALID_ARG;
                        }
                    }

                    while (isdigit(c)) {
                        coef = coef * 10 + c - '0';
                        fl_coef = 1;
                        c = getc(file);
                    }

                    if (!fl_coef) {
                        coef = 1;
                    }

                    if (otr) {
                        coef *= -1;
                    }

                    if (c == '-' || c == '+' || c == ')') {
                        if (insert(polynom_2, coef, 0) == MEM_NOT_ALLOC) {
                            free_all (polynom_1, polynom_2, adder);
                            return MEM_NOT_ALLOC;
                        }

                    } else if (c == 'x') {
                        c = getc(file);
                        if (c == '^') {
                            c = getc(file);
                            if (!isdigit(c)) {
                                free_all (polynom_1, polynom_2, adder);
                                return INVALID_ARG;
                            }

                            while (isdigit(c)) {
                                degree = degree * 10 + c - '0';
                                c = getc(file);
                            }

                            if (insert(polynom_2, coef, degree) == MEM_NOT_ALLOC) {
                                free_all (polynom_1, polynom_2, adder);
                                return MEM_NOT_ALLOC;
                            }

                        } else if (c == '+' || c == '-' || c == ')') {
                            if (insert(polynom_2, coef, 1) == MEM_NOT_ALLOC) {
                                free_all (polynom_1, polynom_2, adder);
                                return MEM_NOT_ALLOC;
                            }
                        } else {
                            free_all (polynom_1, polynom_2, adder);
                            return INVALID_ARG;
                        }

                    } else {
                        free_all (polynom_1, polynom_2, adder);
                        return INVALID_ARG;
                    }
                }
            }

            c = getc(file);

            if (c == ';') {
                test_print(polynom_1, polynom_2, operation);

                status = operations(polynom_1, polynom_2, operation, adder);
                if (status != OK) {
                    free_all (polynom_1, polynom_2, adder);
                    return status;
                }

                printf("res ");
                test_print(adder, NULL, operation);
                printf("\n");

            } else {
                free_all (polynom_1, polynom_2, adder);
                return INVALID_ARG;
            }

        } else {
            free_all (polynom_1, polynom_2, adder);
            return INVALID_ARG;
        } 
        
    }

    free_all (polynom_1, polynom_2, adder);

    return OK;
}

int main () {
    FILE* file = fopen("C:\\VSCode\\files\\mp_3\\input_8.txt", "r");
    if (file == NULL) {
        printf("The file was not opened\n");
        return NOT_OPEN_FILE;
    }

    status_code status;
    status = read(file);
    if (status == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        fclose(file);
        return MEM_NOT_ALLOC;
    } else if (status == INVALID_ARG) {
        printf("Invalid input\n");
        fclose(file);
        return INVALID_ARG;
    }

    fclose(file);
    return OK;
}
