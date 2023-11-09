#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <math.h>

#define EPS 1E-5

typedef enum {
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC
} status_code;

typedef struct {
    double* elems;
    unsigned int n;
} vector;

status_code create_vector (unsigned int n, vector* vec, ...) {
    vec->n = n;
    vec->elems = (double*)malloc(sizeof(double) * n);
    if (vec->elems == NULL) {
        return MEM_NOT_ALLOC;
    }

    va_list ptr;
    va_start(ptr, vec);
    for (int i = 0; i < n; i++) {
        vec->elems[i] = va_arg(ptr, double);
    }
    va_end(ptr);

    return OK;
}

void print_vector(vector* vec) {
    for (int i = 0; i < vec->n; i++) {
        printf("%.3f ", vec->elems[i]);
    }
    printf("\t");
}

status_code del_vector (vector* vec) {
    if (vec == NULL) {
        return INVALID_ARG;
    }

    free(vec->elems);
    vec->elems = NULL;
    vec->n = 0;

    return OK;
}

status_code norm (vector* vec, double* nrm) {
    double mx = 0.0;
    for (unsigned int i = 0; i < vec->n; i++) {
        mx = fmax(mx, fabs(vec->elems[i]));
    }
    *nrm = mx;

    return OK;
}

status_code norm_p (vector* vec, unsigned int p, double* nrm) {
    if (p < 1) {
        return INVALID_ARG;
    }

    double sum = 0.0;
    for (unsigned int i = 0; i < vec->n; i++) {
        sum += pow(fabs(vec->elems[i]), p);
    }
    *nrm = pow(sum, 1.0 / p);

    return OK;
}

status_code matrix_multipl (double** A, double* x, double** res, int n) {
    (*res) = (double*)malloc(sizeof(double) * n);
    if (*res == NULL) {
        return MEM_NOT_ALLOC;
    }

    for (int i = 0; i < n; i++){
        (*res)[i] = 0.0;
        for (int j = 0; j < n; j++) {
            (*res)[i] += A[i][j] * x[j];
        }
    }

    return OK;
}

status_code norm_m (vector* vec, double* nrm) {
    double** A = (double**)malloc(sizeof(double*) * vec->n);
    if (A == NULL) {
        return MEM_NOT_ALLOC;
    }

    for (int i = 0; i < vec->n; i++) {
        A[i] = (double*)malloc(sizeof(double) * vec->n);
        if (A[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(A[j]);
            }
            free(A);
            return MEM_NOT_ALLOC;
        }

        for (int j = 0; j < vec->n; ++j) {
            if (i == j) {
                A[i][j] = 1;
            }
            else {
                A[i][j] = 0;
            }
        }
    }

    double* mult_res = NULL;
    if (matrix_multipl(A, vec->elems, &mult_res, vec->n) == MEM_NOT_ALLOC) {
        for (int i = 0; i < vec->n; ++i) {
            free(A[i]);
        }
        free(A);
        return MEM_NOT_ALLOC;
    }

    *nrm = 0;
    for (int i = 0; i < vec->n; ++i) {
        *nrm += mult_res[i] * vec->elems[i];
    }

    for (int i = 0; i < vec->n; ++i) {
        free(A[i]);
    }
    free(A);
    free(mult_res);

    *nrm = sqrt(*nrm);

    return OK;
}

status_code longest_vector (
    unsigned int n, status_code (*norm_1)(vector*, double*), 
    unsigned int p, status_code (*norm_2)(vector*, unsigned int, double*), 
    status_code (*norm_3)(vector*, double*), int *size_1, int *size_2, int *size_3,
    vector** res_1, vector** res_2, vector** res_3, int quantity, ...
    ) {

    (*res_1) = (vector*)malloc(sizeof(vector) * quantity);
    if (res_1 == NULL) {
        return MEM_NOT_ALLOC;
    }
    (*res_2) = (vector*)malloc(sizeof(vector) * quantity);
    if (res_2 == NULL) {
        free(res_1);
        return MEM_NOT_ALLOC;
    }
    (*res_3) = (vector*)malloc(sizeof(vector) * quantity);
    if (res_3 == NULL) {
        free(res_1);
        free(res_2);
        return MEM_NOT_ALLOC;
    }

    va_list ptr;
    va_start(ptr, quantity);
    
    int ind_1 = 0, ind_2 = 0, ind_3 = 0;
    double max_1 = 0, max_2 = 0, max_3 = 0, cur_1, cur_2, cur_3;
    for (int i = 0; i < quantity; i++) {
        vector vec = va_arg(ptr, vector);

        norm_1(&vec, &cur_1);

        if (fabs(cur_1 - max_1) < EPS) {
            ind_1++;
            (*res_1)[ind_1] = vec;
        } else if (cur_1 - max_1 > EPS) {
            max_1 = cur_1;
            ind_1 = 0;
            (*res_1)[0] = vec;
        }

        if (norm_2(&vec, p, &cur_2) == INVALID_ARG) {
            return INVALID_ARG;
        }

        if (fabs(cur_2 - max_2) < EPS) {
            ind_2++;
            (*res_2)[ind_2] = vec;
        } else if (cur_2 - max_2 > EPS) {
            max_2 = cur_2;
            ind_2 = 0;
            (*res_2)[0] = vec;
        }

        if (norm_3(&vec, &cur_3) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        }

        if (fabs(cur_3 - max_3) < EPS) {
            ind_3++;
            (*res_3)[ind_3] = vec;
        } else if (cur_3 - max_3 > EPS) {
            max_3 = cur_3;
            ind_3 = 0;
            (*res_3)[0] = vec;
        }

    }

    va_end(ptr);

    *size_1 = ind_1 + 1;
    *size_2 = ind_2 + 1;
    *size_3 = ind_3 + 1;

    return OK;
}


int main () {
    if (EPS > 1.0) {
        printf("EPS is too big\n");
        return INVALID_ARG;
    }
    unsigned int n = 3;

    vector vec_1, vec_2, vec_3;
    if (create_vector(n, &vec_1, 1.0, 1.0, 1.0) == MEM_NOT_ALLOC) {
        printf("Memory was not allocated1\n");
        return MEM_NOT_ALLOC;
    }
    if (create_vector(n, &vec_2, 1.0, 1.0, 3.5) == MEM_NOT_ALLOC) {
        del_vector(&vec_1);
        printf("Memory was not allocated2\n");
        return MEM_NOT_ALLOC;
    }
    if (create_vector(n, &vec_3, 1.0, 1.0, -3.51) == MEM_NOT_ALLOC) {
        del_vector(&vec_1);
        del_vector(&vec_2);
        printf("Memory was not allocated3\n");
        return MEM_NOT_ALLOC;
    }

    vector *res_1 = NULL, *res_2 = NULL, *res_3 = NULL;
    int size_1, size_2, size_3;
    status_code status;
    status = longest_vector(n, norm, 3, norm_p, norm_m, &size_1, &size_2, &size_3, &res_1, &res_2, &res_3, 3, vec_1, vec_2, vec_3);
    if (status == MEM_NOT_ALLOC) {
        del_vector(&vec_1);
        del_vector(&vec_2);
        del_vector(&vec_3);
        printf("Memory was not allocated11111111\n");
        return MEM_NOT_ALLOC;
    } else if (status == INVALID_ARG) {
        del_vector(&vec_1);
        del_vector(&vec_2);
        del_vector(&vec_3);
        printf("Invalid input\n");
        return INVALID_ARG;
    }

    for (int i = 0; i < size_1; i++) {
        print_vector(&res_1[i]);
    }
    printf("\n");
    for (int i = 0; i < size_2; i++) {
        print_vector(&res_2[i]);
    }
    printf("\n");
    for (int i = 0; i < size_3; i++) {
        print_vector(&res_3[i]);
    }

    del_vector(&vec_1);
    del_vector(&vec_2);
    del_vector(&vec_3);
    del_vector(res_1);
    del_vector(res_2);
    del_vector(res_3);

    return OK;
}