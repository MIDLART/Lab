#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {
    ok,
    invalid_arguments,
    invalid_flag,
    memory_not_allocated
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc < 3) {
        return invalid_arguments;
    }
    if (argv[1][0] != '-' || argv[1][2] != '\0') {
        return invalid_flag;
    }

    if (argv[1][1] == 'c' && argc < 4) {
        return invalid_arguments;
    } else if (argv[1][1] != 'c' && argc != 3) {
        return invalid_arguments;
    }
}

int string_length (char* str) {
    char character;
    int length = 0;
    do {
        character = str[length];
        length++;
    } while(character);

    return length - 1;
}

status_code string_revers (char* str, char** revers) {
    int size = string_length(str) + 1;
    *revers = (char*)malloc(sizeof(char) * size);
    if (revers == NULL) {
        return memory_not_allocated;
    }
    (*revers)[size - 1] = '\0';

    for (int i = 0; i < size; i++) {
        (*revers)[i] = str[size - 1 - i];
    }
    return ok;
}

status_code up_register (char* str, char** up) {
    int size = string_length(str) + 1;
    *up = (char*)malloc(sizeof(char) * size);
    if (up == NULL) {
        return memory_not_allocated;
    }
    (*up)[size - 1] = '\0';

    for (int i = 0; i < size; i++) {
        if (i % 2 == 0 && str[i] >= 'a' && str[i] <= 'z') {
            (*up)[i] = str[i] - 32;
        } else {
            (*up)[i] = str[i];
        }
    }

    return ok;
}

status_code num_alph_oth (char* str, char** res) {
    int size = string_length(str) + 1;
    *res = (char*)malloc(sizeof(char) * size);
    if (res == NULL) {
        return memory_not_allocated;
    }

    int j = 0;
    for (int i = 0; i < size; i++) {
        if (isdigit(str[i]) != 0) {
            (*res)[j] = str[i];
            j++;
        } 
    }
    for (int i = 0; i < size; i++) {
        if (isalpha(str[i]) != 0) {
            (*res)[j] = str[i];
            j++;
        } 
    }
    for (int i = 0; i < size; i++) {
        if (isdigit(str[i]) == 0 && isalpha(str[i]) == 0) {
            (*res)[j] = str[i];
            j++;
        } 
    }
    (*res)[j] = '\0';

    return ok;
}

status_code concatenation (int seed, char** str_concat, int argc, char* argv[]) {
    int size = 1;
    for (int i = 3; i < argc; i++) {
        size += string_length(argv[i]);
    }

    *str_concat = (char*)malloc(sizeof(char) * size);
    if (str_concat == NULL) {
        return memory_not_allocated;
    }

    int* used_count = (int*)malloc(sizeof(int) * (argc - 3));
    if (used_count == NULL) {
        return memory_not_allocated;
    }

    srand(seed);
    int concat_len = 0, r, unique;
    for (int j = 0; j < (argc - 3); j++) {
        unique = 0;
        while (unique == 0) {
            r = 3 + rand() % (argc - 3);
            unique = 1;

            for (int k = 0; k < j; k++) {
                if (r == used_count[k]) {
                    unique = 0;
                    break;
                }
            }
            
        }
        used_count[j] = r;

        int len = string_length(argv[r]);
        for (int i = 0; i < len; i++){
            (*str_concat)[concat_len] = argv[r][i];
            concat_len++; 
        }
    }
    (*str_concat)[concat_len] = '\0';

    free(used_count);
    return ok;
}

int main (int argc, char* argv[]) {
    switch (check_parameters(argc, argv)) {
        case invalid_arguments:
            printf("Invalid arguments\n");
            return invalid_arguments;

        case invalid_flag:
            printf("Invalid flag\n");
            return invalid_arguments;
    }

    switch (argv[1][1]) {
        case 'l':
            printf("%d\n", string_length(argv[2]));
            break;
        
        case 'r': {
            char* revers;
            if (string_revers(argv[2], &revers) == memory_not_allocated) {
                printf("Memory was not allocated\n");
                return memory_not_allocated;
            }
            printf("%s\n", revers);

            free(revers);
            break;
        }
        case 'u': {
            char* up;
            if (up_register(argv[2], &up) == memory_not_allocated) {
                printf("Memory was not allocated\n");
                return memory_not_allocated;
            }
            printf("%s\n", up);

            free(up);
            break;
        }
        case 'n': {
            char* res;
            if (num_alph_oth(argv[2], &res) == memory_not_allocated) {
                printf("Memory was not allocated\n");
                return memory_not_allocated;
            }
            printf("%s\n", res);

            free(res);
            break;
        }
        case 'c': {
            char* res; 
            int seed = atoi(argv[2]);
            if (seed <= 0) {
                printf("Invalid flag\n");
                return invalid_arguments;
            }

            if (concatenation(seed, &res, argc, argv) == memory_not_allocated) {
                printf("Memory was not allocated\n");
                return memory_not_allocated;
            }
            printf("%s\n", res);

            free(res);
            break; 
        }

        default:
            printf("Invalid flag\n");
            return invalid_flag;
    }

    return ok;
}
