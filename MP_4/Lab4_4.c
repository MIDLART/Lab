#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    NOT_INIT,
    OVERFLOW_
} status_code;

typedef struct
{
    char name;
    char value[65];
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

    return e1->name - e2->name;
}

status_code add (Vector* vec, char name, char val[]) 
{
    MemoryCell *elem = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (elem == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    elem->name = name;
    strcpy(elem->value, val);

    if (vec->size >= vec->capacity) 
    {
        vec->capacity *= 2;
        MemoryCell** tmp;
        
        tmp = realloc(vec->elems, sizeof(MemoryCell*) * vec->capacity);
        if (tmp == NULL) 
        {
            free(elem);
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
        free(vector->elems[i]);
    }

    free(vector->elems);
    return OK;
}

status_code find (Vector* vec, char name, int* res)
{
    if (vec->size == 0)
    {
        return INVALID_ARG;
    }
    
    int a = 0, b = vec->size - 1, c, cmp;
    c = b / 2;
    while (b > a)
    {
        cmp = name - vec->elems[c]->name;
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
    if (name == vec->elems[c]->name)
    {
        *res = c;
        return OK;
    }

    return INVALID_ARG;
}


status_code conv_to_dec (char num[], int num_system, int *decimal) 
{
    int res = 0;
    int i, cur, otr = 0;
    if (num[0] == '-') 
    {
        otr = 1;
    }
    i = otr;
    while (num[i] != '\0') 
    {
        cur = num[i];
        if (num[i] >= 'a') 
        {
            cur -= 39;
        } else if (num[i] >= 'A') 
        {
            cur -= 7;
        }
        cur -= '0';

        if ((INT_MAX - cur) / num_system < res) 
        {
            return OVERFLOW_;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) 
    {
        res *= -1;
    }
    *decimal = res;
    return OK;
}

void conversion_10_to_system (int num, int system, char str_num[]) 
{
    int i = 64, otr = 0;
    char character;
    if (num < 0) 
    {
        num *= -1;
        otr = 1;
    }
    while (num != 0) 
    {
        character = num % system;
        if (character > 9) 
        {
            character += 7;
        }
        character += '0';

        str_num[i] = character;
        num /= system;
        i--;
    }

    if (otr) 
    {
        str_num[i] = '-';
    }
}


int is_operation (char c, char c_2)
{
    if (c == '-' && c_2 == '>' || c == '+' && c_2 == '>' || c == '<' && (c_2 == '-' || c_2 == '>'))
    {
        return 2;
    }
    else if (c == '&' || c == '+' || c == '~' || c == '!' || c == '?')
    {
        return 1;
    }

    return 0;
}

bool operation (bool a, bool b, char c)
{
    switch (c)
    {
        case '&':
            return a && b;
        
        case '|':
            return a || b;

        case '~':
            return a == b;

        case '-':
            return !a || b;

        case '+':
            return a && !b;

        case '^':
            return a ^ b;

        case '!':
            return !(a && b);

        case '?':
            return !(a || b);

        case '<':
            return !b || a;
    }
}

status_code bin_operation (char a[], char b[], char c, char res[])
{
    for (int i = 0; i < 64; i++)
    {
        if (operation(a[i], b[i], c))
        {
            res[i] = '1';
        }
        else
        {
            res[i] = '0';
        }
    }

    return OK;
}


status_code bit_otr (const char val[], char res[])
{
    for (int i = 0; i < 64; i++)
    {
        if (val[i] == '0')
        {
            res[i] = '1';
        }
        else
        {
            res[i] = '0';
        }
    }

    return OK;
}


status_code write_op (char num[], int base)
{
    if (base < 2 || base > 36)
    {
        return INVALID_ARG;
    }

    int dec;
    if (conv_to_dec(num, 2, &dec) == OVERFLOW_)
    {
        return OVERFLOW_;
    }

    char res_arry[66];
    for (int i = 0; i < 65; i++) {
        res_arry[i] = '0'; 
    }
    res_arry[65] = '\0';

    conversion_10_to_system(dec, base, res_arry);

    if (dec == 0) 
    {
        printf("0\n");
    } 
    else 
    {
        printf("%s\n", res_arry + strspn(res_arry, "0"));
    }

    return OK;
}


status_code read_operation (char* str, Vector* vec)
{
    char name, var_1, var_2, op;
    char buf[6];

    char res[65];
    res[64] = '\0';

    int ind, ind_2, t, base;

    status_code status;
    
    int i = 0;
    while (isalpha(str[i]))
    {
        buf[i] = str[i];
        i++;
    }
    buf[i] = '\0';

    if (i == 0 || i > 5)
    {
        return INVALID_ARG;
    }

    if (!strcmp("read", buf) || !strcmp("write", buf))
    {   
        if (str[i] != '(')
        {
            return INVALID_ARG;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (isalpha(str[i]))
        {
            var_1 = str[i];

            if (var_1 >= 'a')
            {
                var_1 -= 32;
            }
        }
        else
        {
            return INVALID_ARG;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (str[i] != ',')
        {
            return INVALID_ARG;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if(isdigit(str[i]))
        {
            base = str[i] - '0';
            i++;

            if (isdigit(str[i]))
            {
                base = base * 10 + str[i] - '0';
                i++;
            }
        }
        else
        {
            return INVALID_ARG;
        }

        if (find(vec, var_1, &ind) == INVALID_ARG)
        {
            return NOT_INIT;
        }

        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (str[i] != ')')
        {
            return INVALID_ARG;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (str[i] != ';')
        {
            return INVALID_ARG;
        }

        if (buf[0] == 'r')
        {
            ////
        }
        else
        {
            status = write_op(vec->elems[ind]->value, base);
            if (status == INVALID_ARG)
            {
                return INVALID_ARG;
            }
            else if (status == OVERFLOW_)
            {
                return OVERFLOW_;
            }
        }

        return OK;
    }

    if (i != 1)
    {
        return INVALID_ARG;
    }

    name = buf[0];
    if (name >= 'a')
    {
        name -= 32;
    }

    while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }

    if (str[i] != ':' || str[i + 1] != '=')
    {
        return INVALID_ARG;
    }
    i+=2;
    
    while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }

    if (str[i] == '\\')
    {
        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (isalpha(str[i]))
        {
            var_1 = str[i];

            if (var_1 >= 'a')
            {
                var_1 -= 32;
            }
        }
        else
        {
            return INVALID_ARG;
        }

        if (find(vec, var_1, &ind) == INVALID_ARG)
        {
            return NOT_INIT;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        bit_otr(vec->elems[ind]->value, res);
    }
    else if (isalpha(str[i]))
    {
        var_1 = str[i];

        if (var_1 >= 'a')
        {
            var_1 -= 32;
        }

        if (find(vec, var_1, &ind) == INVALID_ARG)
        {
            return NOT_INIT;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        t = is_operation(str[i], str[i + 1]);
        if (t == 0)
        {
            return INVALID_ARG;
        }
        else if (t == 1)
        {
            if (str[i] == '+')
            {
                op = '|';
            }
            else
            {
                op = str[i];
            }
        }
        else
        {
            if (str[i + 1] == '>' && str[i] == '<')
            {
                op = '^';
            }
            else
            {
                op = str[i];
            }
        }

        i += t;

        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        if (isalpha(str[i]))
        {
            var_2 = str[i];

            if (var_2 >= 'a')
            {
                var_2 -= 32;
            }
        }
        else
        {
            return INVALID_ARG;
        }

        if (find(vec, var_2, &ind_2) == INVALID_ARG)
        {
            return NOT_INIT;
        }

        i++;
        while (str[i] == '\n' || str[i] == ' ' || str[i] == '\t')
        {
            i++;
        }

        bin_operation(vec->elems[ind]->value, vec->elems[ind_2]->value, op, res);
    }
    else
    {
        return INVALID_ARG;
    }

    if (str[i] != ';')
    {
        return INVALID_ARG;
    }
    
    add(vec, name, res);
    
    return OK;
}

status_code read (FILE* file, Vector* vec, FILE* out)
{
    size_t size = 4;
    char* buf = (char*)malloc(sizeof(char) * size);
    if (buf == NULL) 
    {
        return MEM_NOT_ALLOC;
    }

    status_code status;
    char c;
    int i, flag = 1, staples;

    while (flag && (c = getc(file)) != EOF)
    {
        staples = 0;

        while (c == '\n' || c == ' ' || c == '\t')
        {
            c = getc(file);
        }

        if (c == '%')
        {
            while (c != '\n' && c != EOF)
            {
                c = getc(file);
            }
        } 
        else if (c == '{')
        {
            staples++;
            c = getc(file);

            while (c != EOF && staples != 0)
            {
                if (c == '{')
                {
                    staples++;
                }
                else if (c == '}')
                {
                    staples--;
                }

                c = getc(file);
            }
        }
        

        if (c == EOF)
        {
            flag = 0;
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
    }

    free(buf);
    return OK;
}


int main (int argc, char* argv[])
{
    if (argc < 2)
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

    Vector vector;
    if (create_vector(&vector) == MEM_NOT_ALLOC)
    {
        printf("Memory was not allocated\n");
        fclose(file);
        return OK;
    }


    status_code status;

    //
    char a[65], b[65];
    a[64] = '\0';
    b[64] = '\0';
    for (int i = 0; i < 64; i++)
    {
        a[i] = '1';
        b[i] = '0';
    }

    add(&vector, 'A', a);
    add(&vector, 'B', b);
    //

    status = read(file, &vector, NULL);

    //
    if (status == INVALID_ARG)
    {
        printf("I\n");
    }
    else if (status == MEM_NOT_ALLOC)
    {
        printf("M\n");
    }
    else if (status == NOT_INIT)
    {
        printf("not\n");
    }
    else if (status == OK)
    {
        printf("OK\n");
    }
    
    printf("1) %s\n", (&vector)->elems[2]->value);
    printf("2) %s\n", (&vector)->elems[3]->value);
    //


    fclose(file);
    del_vec(&vector);
    return OK;
}