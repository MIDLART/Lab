#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    DIV_0
} status_code;

typedef struct
{
    char* name;
    long long int value;
} MemoryCell;

typedef struct 
{
    MemoryCell** elems;
    size_t size;
    size_t capacity;
} Vector;

status_code create_vector (Vector* vec) 
{
    vec->size = 0;
    vec->capacity = 8;
    vec->elems = (MemoryCell**)malloc(sizeof(MemoryCell*) * vec->capacity);
    if (vec->elems == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

int comparator (const void* e_1, const void* e_2) {
    MemoryCell* e1 = *(MemoryCell**) e_1;
    MemoryCell* e2 = *(MemoryCell**) e_2;

    return strcmp(e1->name, e2->name);
}

status_code add (Vector* vec, MemoryCell* elem) 
{
    if (vec->size >= vec->capacity) 
    {
        vec->capacity *= 2;
        MemoryCell** tmp;
        
        tmp = realloc(vec->elems, sizeof(MemoryCell*) * vec->capacity);
        if (tmp == NULL) 
        {
            free(vec->elems);
            return MEM_NOT_ALLOC;
        }
        vec->elems = tmp;
    }
    vec->elems[vec->size++] = elem;

    qsort(vec->elems, vec->size, sizeof(MemoryCell*), comparator);

    return OK;
}

status_code del_vec (Vector* vector)
{
    for (int i = 0; i < vector->size; i++) 
    {
        free(vector->elems[i]->name);
        free(vector->elems[i]);
    }
    free(vector->elems);
    return OK;
}

status_code create_var (MemoryCell** var, size_t size)
{
    *var = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (*var == NULL)
    {
        return MEM_NOT_ALLOC;
    }
    (*var)->name = (char*)malloc(sizeof(char) * size);
    if ((*var)->name == NULL) 
    {
        free(*var);
        return MEM_NOT_ALLOC;
    }
    return OK;
}


status_code print_all (Vector* vec)
{
    for (int i = 0; i < vec->size; i++)
    {
        printf("%s = %d\n", vec->elems[i]->name, vec->elems[i]->value);
    }
    return OK;
}

// status_code find (Vector* vec, char* name, MemoryCell** res)
// {
//     int a = 0, b = vec->size - 1, c, cmp;
//     c = b / 2;
//     while (b > a)
//     {
//         cmp = strcmp(name, vec->elems[c]->name);
//         if (cmp == 0)
//         {
//             *res = vec->elems[c];
//             return OK;
//         }
//         else if (cmp > 0)
//         {
//             a = c + 1;
//         }
//         else
//         {
//             b = c;
//         }
//         c = (a + b) / 2;
//     }
//     if (strcmp(name, vec->elems[c]->name) == 0)
//     {
//         *res = vec->elems[c];
//         return OK;
//     }

//     return INVALID_ARG;
// }

status_code find (Vector* vec, char* name, int* res)
{
    int a = 0, b = vec->size - 1, c, cmp;
    c = b / 2;
    while (b > a)
    {
        cmp = strcmp(name, vec->elems[c]->name);
        if (cmp == 0)
        {
            *res = c;
            return OK;
        }
        else if (cmp > 0)
        {
            a = c + 1;
        }
        else
        {
            b = c;
        }
        c = (a + b) / 2;
    }
    if (strcmp(name, vec->elems[c]->name) == 0)
    {
        *res = c;
        return OK;
    }

    return INVALID_ARG;
}

status_code print_var (Vector* vec, char* name)
{
    int res;
    if (find(vec, name, &res) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    printf("%s = %d\n", vec->elems[res]->name, vec->elems[res]->value);

    return OK;
}

status_code read_value (char* str, int* k, long long int* res, Vector* vec)
{
    int otr = 1;
    size_t size = 2;
    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    if (str[*k] == '-')
    {
        otr = -1;
        (*k)++;
    }

    if (isdigit(str[*k]))
    {
        int i = 0;
        while (isdigit(str[*k]))
        {
            if (i == size - 1) 
            {
                size*= 2;
                char* tmp = realloc(buf, sizeof(char) * size);
                if (tmp == NULL)
                {
                    return MEM_NOT_ALLOC;
                }
                buf = tmp;
            }
            buf[i] = str[*k];
            (*k)++;
            i++;
        }
        buf[i] = '\0';

        *res = otr * strtoll(buf, NULL, 10);
        if (errno == ERANGE)
        {
            return INVALID_ARG;
        }

        free(buf);
        return OK;
    }
    else if (isalpha(str[*k]))
    {
        if (vec->size == 0)
        {
            return INVALID_ARG;
        }

        int i = 0;
        while (isalpha(str[*k]))
        {
            if (i == size - 1) 
            {
                size*= 2;
                char* tmp = realloc(buf, sizeof(char) * size);
                if (tmp == NULL)
                {
                    return MEM_NOT_ALLOC;
                }
                buf = tmp;
            }
            buf[i] = str[*k];
            (*k)++;
            i++;
        }
        buf[i] = '\0';

        int res_f;
        if (find(vec, buf, &res_f) == INVALID_ARG)
        {
            return INVALID_ARG;
        }
        *res = otr * vec->elems[res_f]->value;

        free(buf);
        return OK;
    }
    else
    {
        return INVALID_ARG;
    }

    free(buf);
    return INVALID_ARG;
}

status_code operation (char op, long long int *num_1_res, long long int num_2) 
{
    switch (op)
    {
        case '+':
            *num_1_res = *num_1_res + num_2;
            break;
        case '-':
            *num_1_res = *num_1_res - num_2;
            break;
        case '*':
            *num_1_res = *num_1_res * num_2;
            break;
        case '/':
            if (num_2 == 0) return DIV_0;
            *num_1_res = *num_1_res / num_2;
            break;
        case '%':
            if (num_2 == 0) return INVALID_ARG;
            *num_1_res = *num_1_res % num_2;
            break;
    }

    return OK;
}

status_code read_operation (char* str, Vector* vec)
{
    size_t size = 2;
    char* name = (char*)malloc(sizeof(char) * size);
    if (name == NULL) 
    {
        return MEM_NOT_ALLOC;
    }
    
    int i = 0;
    while (isalpha(str[i]))
    {
        if (i == size - 1)
        {
            size*= 2;
            char* tmp = realloc(name, sizeof(char) * size);
            if (tmp == NULL)
            {
                free(name);
                return MEM_NOT_ALLOC;
            }
            name = tmp;
        }
        name[i] = str[i];
        i++;
    }
    name[i] = '\0';

    if (i == 0)
    {
        free(name);
        return INVALID_ARG;
    }

    if (!strcmp("print", name))
    {
        if (str[i] == ';')
        {
            print_all(vec);
            free(name);
            return OK;
        }
        else if (str[i] == ' ')
        {
            size_t size_2 = 2;
            char* buf = (char*)malloc(sizeof(char) * size_2);
            if (buf == NULL) 
            {
                return MEM_NOT_ALLOC;
            }
            i++;
            int j = 0;
            while (isalpha(str[i]))
            {
                if (j == size - 1)
                {
                    size_2*= 2;
                    char* tmp_2 = realloc(buf, sizeof(char) * size_2);
                    if (tmp_2 == NULL)
                    {
                        free(buf);
                        return MEM_NOT_ALLOC;
                    }
                    buf = tmp_2;
                }
                buf[j] = str[i];
                i++;
                j++;
            }
            buf[j] = '\0';
            
            if (j == 0)
            {
                free(buf);
                free(name);
                return INVALID_ARG;
            }

            if (print_var(vec, buf) == INVALID_ARG)
            {
                free(buf);
                free(name);
                return INVALID_ARG;
            }

            free(buf);
            free(name);
            return OK;
        }
        else
        {
            free(name);
            return INVALID_ARG;
        }
    }
    
    if (str[i] != '=')
    {   
        free(name);
        return INVALID_ARG;
    }
    i++;


    MemoryCell* var = NULL;
    if (create_var(&var, size) == MEM_NOT_ALLOC)
    {
        return MEM_NOT_ALLOC;
    }

    long long int res;
    status_code status = read_value(str, &i, &res, vec);
    if (status != OK)
    {
        free(name);
        free(var);
        return status;
    }
    var->value = res;

    if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%')
    {
        char op = str[i];
        i++;
        status_code status = read_value(str, &i, &res, vec);
        if (status != OK)
        {
            free(name);
            free(var);
            return status;
        }

        status_code status_2;
        if ((status_2 = operation(op, &(var->value), res)) != OK)
        {
            free(name);
            free(var);
            return status_2;
        }
    }

    if (str[i] == ';')
    {
        int re;
        if (vec->size != 0 && find(vec, name, &re) == OK)
        {
            vec->elems[re]->value = var->value;
            free(var);
        }
        else 
        {
            strcpy(var->name, name);

            if (add(vec, var) == MEM_NOT_ALLOC)
            {
                free(name);
                free(var);
                return MEM_NOT_ALLOC;
            }
        }
    }
    else
    {
        free(name);
        free(var);
        return INVALID_ARG;
    }

    free(name);
    return OK;
}

status_code read (FILE* file, Vector* vec)
{
    size_t size = 4;
    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    status_code status;
    char c;
    int i;
    while ((c = getc(file)) != EOF)
    {
        while (c == '\n' || c == ' ' || c == '\t')
        {
            c = getc(file);
        }
        if (c == EOF)
        {
            break;
        }

        i = 0;
        while (c != ';')
        {
            if (c == '\n' || c == '\t' || c == EOF)
            {
                free(buf);
                return INVALID_ARG;
            }

            if (i == size - 2) 
            {
                size*= 2;
                char* tmp = realloc(buf, sizeof(char) * size);
                if (tmp == NULL)
                {
                    return MEM_NOT_ALLOC;
                }
                buf = tmp;
            }
            buf[i] = c;
            i++;
            c = getc(file);
        } 
        buf[i] = c;
        buf[i+1] = '\0';

        status = read_operation(buf, vec);
        if (status != OK)
        {
            free(buf);
            return status;
        }

        if ((c = getc(file)) == EOF)
        {
            break;
        }
        else if (c != '\n')
        {
            return INVALID_ARG;
        }
        
    }

    free(buf);
    return OK;
}


int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Invalid input\n");
        return INVALID_ARG;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("The file was not opened\n");
        return NOT_OPEN_FILE;
    }

    Vector* vector;
    if (create_vector(vector) == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
        return OK;
    }

    status_code status;
    status = read(file, vector);
    if (status == INVALID_ARG)
    {
        printf("Invalid input\n");
    }
    else if (status == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
    }
    else if (status == DIV_0)
    {
        printf("Division by 0\n");
    }
    
    
    del_vec(vector);
    free(vector);
    fclose(file);
    return status;
}
