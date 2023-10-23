#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <math.h>

#define EPS 1E-10

typedef enum {
    ok,
    invalid_arguments,
    no, 
    memory_not_allocated
} status_code;

typedef struct _remains {
    double rem;
    struct _remains* next_remains;
} remains;

typedef struct {
    remains *head, *tail;
} List;


void push_back(List *list, double fractional) {
    remains *new_remains = (remains*)malloc(sizeof(remains));
    if (new_remains == NULL) {
        return;
    }
    new_remains -> rem = fractional;
    new_remains -> next_remains = NULL;
    if (list -> tail != NULL) {
        list -> tail -> next_remains = new_remains;
    }
    if (list -> head == NULL) {
        list -> head = new_remains;
    }
    list -> tail = new_remains;
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
    remains *t_remains = list -> head;
    list -> head = t_remains -> next_remains;
    free(t_remains);
    del_list(list);
}

status_code find (List *list, double fractional) {
    if (list -> head == NULL) {
        return ok;
    }

    remains *t_remains = list -> head;
    while (t_remains != NULL) {
        if (fabs(t_remains -> rem - fractional) < EPS) {
            return no;
        }
        t_remains = t_remains -> next_remains;
    }
    return ok;
}

status_code finite_representation (int system, double num) {
    if (num > 1 || num < 0) {
        return invalid_arguments;
    }
    if (num == 1) {
        return ok;
    }

    List *list = (List*)malloc(sizeof(List));
    if (list != NULL){
        list -> head = NULL;
        list -> tail = NULL;
    } else {
        return memory_not_allocated;
    }

    int integer_part;
    double fractional_part = num;

    while (fractional_part > EPS) { 
        fractional_part = fractional_part * system;

        integer_part = fractional_part;
        fractional_part -= integer_part; 

        if (find(list, fractional_part) == no && fractional_part > EPS) {
            del_list(list);
            free(list);
            return no;
        }

        push_back(list, fractional_part);
    }
    
    del_list(list);
    free(list);

    return ok;
}

status_code check (int n, int system, ...) {
    va_list ptr;
    va_start(ptr, system);
    double num;

    for (; n > 0; n--) {
        num = va_arg(ptr, double);
        int res;
        if ((res = finite_representation(system, num)) == ok) {
            printf("The number %f has a finite representation in a %d system\n", num, system);
        } else if (res == no){
            printf("The number %f does not have a finite representation in a %d system\n", num, system);
        } else if (res == invalid_arguments) {
            printf("Invalid arguments\n");
        } else {
            printf("Memory not allocated\n");
        }
    }

    va_end(ptr);
    return ok;
}

int main () {
    check (3, 8, 0.125, 0.2, 0.359375);
    printf("\n");
    check (2, 2, 0.1, 0.75);
    printf("\n");
    check (2, 16, 0.9375, 0.7);
    return ok;
}