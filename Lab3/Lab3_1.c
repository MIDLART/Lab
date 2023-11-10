#include <stdio.h>
#include <malloc.h>

typedef enum {
    OK,
    INVALID_ARGUMENTS,
    MEM_NOT_ALLOC
} status_code;

status_code bit_sum (long long int num_1, long long int num_2, long long int *sum) {
    long long int res, remain;
    res = num_1 ^ num_2;
    remain = (num_1 & num_2) << 1;

    while (remain != 0) {
        num_1 = res;
        num_2 = remain;

        res ^= remain;
        remain = (num_1 & num_2) << 1;
    }
    *sum = res;

    return OK;
}

status_code conversion (unsigned int num, unsigned int degree_of_two, char** res) { 
    if (degree_of_two < 1 || degree_of_two > 5) {
        return INVALID_ARGUMENTS;
    }

    char* conv = (char*)calloc(33, sizeof(char));
    if (conv == NULL) {
        return MEM_NOT_ALLOC;
    }

    conv[32] = '\0';
    long long int i = 31;
    long long int sum, system, shift;

    bit_sum (degree_of_two, -1, &shift);
    bit_sum (2 << shift, -1, &system);

    while (num > 0) {
        bit_sum(num & system, '0', &sum);
        if (sum > '9') {
            bit_sum(sum, 7, &sum);
        }
        conv[i] = sum;

        num = num >> degree_of_two;
        bit_sum(i, -1, &i);
    }

    *res = conv;
    return OK;
}

int main () {
    char *res;
    int otr = 0;
    unsigned int degree;
    long long int n = -123;

    if (n < 0) {
        bit_sum(~n, 1, &n);
        otr = 1;
    }

    for (degree = 1; degree < 6; degree++) {
        if (conversion(n, degree, &res) == OK) {
            if (otr) {
                printf("-");
            }
            for(int i = 0; i < 33; i++) {
                if (res[i] != 0) {
                    printf("%c", res[i]);
                }
            }
            printf("\n");

            free(res);
        }
    }

    return OK;
}
