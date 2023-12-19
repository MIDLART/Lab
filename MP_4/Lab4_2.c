#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define SIZE 8

typedef enum 
{
    OK,
    INVALID_ARG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    OVERFLOW_
} status_code;


status_code valid_name (char c, int* name)
{
    *name = c;
    if (c >= 'a')
    {
        (*name) -= 'a';
    }
    else
    {
        (*name) -= 'A';
    }

    if (*name < 0 || *name >= 26)
    {
        return INVALID_ARG;
    }

    return OK;
}


status_code Load_read (FILE* file, int** arrays, size_t* sizes, int name)
{
    char c;
    int flag = 1;
    int num, i, otr, n = 0;
    size_t size = SIZE;

    while (flag)
    {
        num = 0;
        i = 0;
        otr = 0;

        while ((c = getc(file)) != EOF && c != ' ' && c != '\n' && c != '\t')
        {
            if (i == 0 && c == '-')
            {
                otr = 1;
            }
            else
            {
                if (!isdigit(c))
                {
                    return INVALID_ARG;
                }

                if (i >= 9 && (INT_MAX - c + '0') / 10 < num)
                {
                    return OVERFLOW_;
                }

                num = num * 10 + c - '0';
            }

            i++;
        }

        if (i != 0)
        {
            if (otr)
            {
                num *= -1;
            }

            if (n - 2 >= size)
            {
                size *= 2;

                int* tmp = (int*)realloc(arrays[name], sizeof(int) * size);
                if (tmp == NULL)
                {
                    return MEM_NOT_ALLOC;
                }

                arrays[name] = tmp;
            }

            sizes[name] += 1;

            arrays[name][n] = num;
            n++;
        }
        
        if (c == EOF)
        {
            flag = 0;
        }
    }

    return OK;
}

status_code Load (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    while (str[i] != ';')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (j == 0)
    {
        return INVALID_ARG;
    }

    FILE* file = fopen(buf, "r");
    if (file == NULL)
    {
        return NOT_OPEN_FILE;
    }

    if (Load_read(file, arrays, sizes, name) != OK)
    {
        fclose(file);
        return INVALID_ARG;
    }

    fclose(file);
    return OK;
}


status_code Save (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    while (str[i] != ';')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (j == 0)
    {
        return INVALID_ARG;
    }

    FILE* file = fopen(buf, "w");
    if (file == NULL)
    {
        return NOT_OPEN_FILE;
    }

    for (int i = 0; i < sizes[name]; i++)
    {
        fprintf(file, "%d\n", arrays[name][i]);
    }

    fclose(file);
    return OK;
}

status_code Rand (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;


    long count = -1, lb = 0, rb = 0;
    int fl_1 = 1, fl_2 = 1;

    while (str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (j == 0)
    {
        return INVALID_ARG;
    }

    count = strtol(buf, NULL, 10);

    i++;
    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }

    j = 0;
    while (str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_1 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    lb = strtol(buf, NULL, 10);

    i++;
    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }

    j = 0;
    while (str[i] != ';')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_2 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    rb = strtol(buf, NULL, 10);

    if (count == -1 || (lb == 0 && !fl_1) || (rb == 0 && !fl_2))
    {
        return INVALID_ARG;
    }


    if (count > SIZE && count >= sizes[name])
    {
        int* tmp = (int*)realloc(arrays[name], sizeof(int) * count);
        if (tmp == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        arrays[name] = tmp;
    }

    sizes[name] = count;

    for (int k = 0; k < count; k++)
    {
        arrays[name][k] = lb + rand() % (rb + 1);
    }

    return OK;
}

status_code Concat (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name, name_2;

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    if (valid_name(str[i], &name_2) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ';')
    {
        return INVALID_ARG;
    }


    int count = sizes[name] + sizes[name_2];

    if (count > SIZE && count >= sizes[name])
    {
        int* tmp = (int*)realloc(arrays[name], sizeof(int) * count);
        if (tmp == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        arrays[name] = tmp;
    }

    for (int k = sizes[name]; k < count; k++)
    {
        arrays[name][k] = arrays[name_2][j];
        j++;
    }

    sizes[name] = count;

    return OK;
}

status_code Free (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;

    if (str[i] != '(')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ')' || str[i + 1] != ';')
    {
        return INVALID_ARG;
    }
    
    
    if (sizes[name] > SIZE)
    {
        int* tmp = (int*)realloc(arrays[name], sizeof(int) * SIZE);
        if (tmp == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        arrays[name] = tmp;

        sizes[name] = SIZE;
    }

    for (int k = 0; k < sizes[name]; k++)
    {
        arrays[name][k] = 0;
    }

    sizes[name] = 0;

    return OK;
}

status_code Copy (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name, name_2;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name_2) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    long lb = 0, rb = 0;
    int fl_1 = 1, fl_2 = 1;

    while (str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_1 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    lb = strtol(buf, NULL, 10);

    i++;
    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    j = 0;
    while (str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_2 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    rb = strtol(buf, NULL, 10);

    i++;
    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if ((lb == 0 && !fl_1) || (rb == 0 && !fl_2))
    {
        return INVALID_ARG;
    }

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ';')
    {
        return INVALID_ARG;
    }

    if (lb < 0 || rb < 0 || rb - lb < 0 || rb > sizes[name_2])
    {
        return INVALID_ARG;
    }


    int count = rb - lb;

    if (count > SIZE && count >= sizes[name])
    {
        int* tmp = (int*)realloc(arrays[name], sizeof(int) * count);
        if (tmp == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        arrays[name] = tmp;
    }

    j = lb;
    for (int k = 0; k < count; k++)
    {
        arrays[name][k] = arrays[name_2][j];
        j++;
    }

    sizes[name] = count;

    return OK;
}

status_code Remove (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    long lb = 0, rb = 0;
    int fl_1 = 1, fl_2 = 1;

    while (str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_1 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    lb = strtol(buf, NULL, 10);

    i++;
    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    j = 0;
    while (str[i] != ';')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (buf[0] == '0')
    {
        fl_2 = 0;
    }

    if (j == 0)
    {
        return INVALID_ARG;
    }

    rb = strtol(buf, NULL, 10);

    if (lb < 0 || rb < 0 || rb + lb > sizes[name])
    {
        return INVALID_ARG;
    }


    if (sizes[name] > SIZE)
    {
        int* tmp = (int*)realloc(arrays[name], sizeof(int) * SIZE);
        if (tmp == NULL)
        {
            return MEM_NOT_ALLOC;
        }

        arrays[name] = tmp;
    }

    for (int k = lb; k < lb + rb; k++)
    {
        arrays[name][k] = 0;
    }
    
    if (rb + lb == sizes[name])
    {
        sizes[name] = lb;
    }

    return OK;
}


int comparator(const void* x1, const void* x2)
{
    return (*(int*)x1 - *(int*)x2);
}

int comparator_2(const void* x1, const void* x2)
{
    return (*(int*)x2 - *(int*)x1);
}

int shuffle(const void* x1, const void* x2)
{
    int r = rand() % 2;

    if (r == 0)
    {
        return -1;
    }

    return 1;
}

status_code Sort (char* str, int** arrays, size_t* sizes, int Shuf)
{
    int i = 0;
    int name;

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }

    if (Shuf) 
    {
        qsort(arrays[name], sizes[name], sizeof(int), shuffle);
    }
    else
    {
        i++;
        if (str[i] == '+')
        {
            qsort(arrays[name], sizes[name], sizeof(int), comparator);
        }
        else if (str[i] == '-')
        {
            qsort(arrays[name], sizes[name], sizeof(int), comparator_2);
        }
        else
        {
            return INVALID_ARG;
        }
    }
    i++;

    if (str[i] != ';')
    {
        return INVALID_ARG;
    }

    return OK;
}


status_code Print (char* str, int** arrays, size_t* sizes)
{
    int i = 0, j = 0;
    int name;
    char buf[256];

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ',' || str[i + 1] != ' ')
    {
        return INVALID_ARG;
    }
    i += 2;

    while (str[i] != ';' && str[i] != ',')
    {
        buf[j] = str[i];
        j++;
        i++;
    }
    buf[j] = '\0';

    if (j == 0)
    {
        return INVALID_ARG;
    }


    int lb = 0, rb = 0;

    if (str[i] == ';')
    {
        if (strcmp("all", buf) == 0)
        {
            for (int k = 0; k < sizes[name]; k++)
            {
                printf("%d\n", arrays[name][k]);
            }
        }
        else
        {
            lb = strtol(buf, NULL, 10);

            if ((lb == 0 && buf[0] != '0') || lb >= sizes[name])
            {
                return INVALID_ARG;
            }

            printf("%d\n", arrays[name][lb]);
        }
    } 
    else
    {
        lb = strtol(buf, NULL, 10);

        if ((lb == 0 && buf[0] != '0') || lb >= sizes[name])
        {
            return INVALID_ARG;
        }

        i++;
        if (str[i] != ' ')
        {
            return INVALID_ARG;
        }
        i++;

        j = 0;
        while (str[i] != ';')
        {
            buf[j] = str[i];
            j++;
            i++;
        }
        buf[j] = '\0';

        if (j == 0)
        {
            return INVALID_ARG;
        }

        rb = strtol(buf, NULL, 10);

        if ((rb == 0 && buf[0] != '0') || rb >= sizes[name] || lb > rb)
        {
            return INVALID_ARG;
        }

        for (int k = lb; k < rb + 1; k++)
        {
            printf("%d\n", arrays[name][k]);
        }
    }

    printf("\n");
    
    return OK;
}


status_code find_max (int** arrays, size_t* sizes, int name, int* ind)
{
    *ind = 0;
    int max = arrays[name][0];

    for (int i = 0; i < sizes[name]; i++)
    {
        if (arrays[name][i] > max)
        {
            max = arrays[name][i];
            *ind = i;
        }
    }

    return OK;
}

status_code find_min (int** arrays, size_t* sizes, int name, int* ind)
{
    *ind = 0;
    int min = arrays[name][0];

    for (int i = 0; i < sizes[name]; i++)
    {
        if (arrays[name][i] < min)
        {
            min = arrays[name][i];
            *ind = i;
        }
    }

    return OK;
}

status_code find_average (int** arrays, size_t* sizes, int name, double* aver)
{
    long long comp = 0;

    for (int i = 0; i < sizes[name]; i++)
    {
        comp += arrays[name][i];
    }

    *aver = (1.0 * comp) / sizes[name];

    return OK;
}

status_code most_frequent_and_deviation (
    int** arrays, size_t* sizes, int name, double aver, double* deviation, int* frequent)
{
    int* copy = (int*)malloc(sizeof(int) * sizes[name]);
    if (copy == NULL)
    {
        return MEM_NOT_ALLOC;
    }

    for (int i = 0; i < sizes[name]; i++)
    {
        copy[i] = arrays[name][i];
    }

    qsort(copy, sizes[name], sizeof(int), comparator);

    *deviation = ((fabs(aver - copy[0])) > (fabs(aver - copy[sizes[name] - 1]))) 
                ? fabs(aver - copy[0]) 
                : fabs(aver - copy[sizes[name] - 1]);

    int k = 1, max = 1;
    *frequent = copy[0];

    for (int i = 1; i < sizes[name]; i++)
    {
        if (copy[i] == copy[i - 1])
        {
            k++;

            if (k >= max)
            {
                *frequent = copy[i];
                max = k;
            }
        }
        else
        {
            k = 1;
        }
    }

    if (max == 1)
    {
        *frequent = copy[sizes[name] - 1];
    }

    free(copy);
    return OK;
}

status_code Stats (char* str, int** arrays, size_t* sizes)
{
    int i = 0;
    int name;

    if (str[i] != ' ')
    {
        return INVALID_ARG;
    }
    i++;

    if (valid_name(str[i], &name) == INVALID_ARG)
    {
        return INVALID_ARG;
    }
    i++;

    if (str[i] != ';')
    {
        return INVALID_ARG;
    }

    printf("Size: %d\n", sizes[name]);
    
    if (sizes[name] > 0)
    {
        int ind, m_fr;
        double aver, dev;

        find_max(arrays, sizes, name, &ind);
        printf("Max: %d, ind: %d\n", arrays[name][ind], ind);

        find_min(arrays, sizes, name, &ind);
        printf("Min: %d, ind: %d\n", arrays[name][ind], ind);

        find_average(arrays, sizes, name, &aver);
        printf("Average: %f\n", aver);

        most_frequent_and_deviation(arrays, sizes, name, aver, &dev, &m_fr);
        printf("Max deviation: %f\n", dev);
        printf("Most frequent: %d\n", m_fr);
    }

    printf("\n");
    
    return OK;
}


status_code comand (char* str, FILE* out, int** arrays, size_t* sizes)
{
    char com[10];
    int i = 0;
    int name;
    status_code status;

    srand (time(NULL));

    while (str[i] != ' ' && str[i] != '(')
    {
        com[i] = str[i];
        i++;
    }
    
    if (i > 8)
    {
        return INVALID_ARG;
    }

    com[i] = '\0';


    if (strcmp("Load", com) == 0)
    {
        status = Load(str + i, arrays, sizes);
    }
    else if (strcmp("Save", com) == 0)
    {
        status = Save(str + i, arrays, sizes);
    }
    else if (strcmp("Rand", com) == 0)
    {
        status = Rand(str + i, arrays, sizes);
    }
    else if (strcmp("Concat", com) == 0)
    {
        status = Concat(str + i, arrays, sizes);
    }
    else if (strcmp("Free", com) == 0)
    {
        status = Free(str + i, arrays, sizes);
    }
    else if (strcmp("Copy", com) == 0)
    {
        status = Copy(str + i, arrays, sizes);
    }
    else if (strcmp("Remove", com) == 0)
    {
        status = Remove(str + i, arrays, sizes);
    }
    else if (strcmp("Sort", com) == 0)
    {
        status = Sort(str + i, arrays, sizes, 0);
    }
    else if (strcmp("Shuffle", com) == 0)
    {
        status = Sort(str + i, arrays, sizes, 1);
    }
    else if (strcmp("Print", com) == 0)
    {
        status = Print(str + i, arrays, sizes);
    }
    else if (strcmp("Stats", com) == 0)
    {
        status = Stats(str + i, arrays, sizes);
    }
    else
    {
        status = INVALID_ARG;
    }

    return status;
}

status_code read (FILE* file, FILE* out, int** arrays, size_t* sizes)
{
    char c;
    int i = 0;
    status_code status;

    char buf[256];

    while ((c = getc(file)) != EOF)
    {
        if (c == '\n')
        {
            if (buf[i - 1] != ';')
            {
                return INVALID_ARG;
            }

            buf[i] = '\0';
            i = 0;

            status = comand(buf, out, arrays, sizes);

            if (status != OK)
            {
                return status;
            }
        }
        else
        {
            buf[i] = c;

            i++;
        }
    }

    if (buf[i - 1] != ';')
    {
        return INVALID_ARG;
    }

    buf[i] = '\0';

    status = comand(buf, out, arrays, sizes);

    if (status != OK)
    {
        return status;
    }

    return OK;
}


int main ()
{
    FILE* file = fopen("C:\\VSCode\\Lab\\MP_4\\test_2.txt", "r");
    if (file == NULL)
    {
        printf("The file was not opened\n");
        return NOT_OPEN_FILE;
    }

    FILE* out = fopen("C:\\VSCode\\Lab\\MP_4\\out_2.txt", "w");
    if (out == NULL)
    {
        printf("The output file was not opened\n");
        fclose(file);

        return NOT_OPEN_FILE;
    }


    int** arrays = (int**)malloc(sizeof(int*) * 26);
    if (arrays == NULL)
    {
        printf("Memory was not allocated\n");
        fclose(file);
        fclose(out);

        return MEM_NOT_ALLOC;
    }

    size_t* sizes = (size_t*)calloc(26, sizeof(size_t));
    if (sizes == NULL)
    {
        printf("Memory was not allocated\n");

        free(arrays);
        fclose(file);
        fclose(out);

        return MEM_NOT_ALLOC;
    }

    for (int i = 0; i < 26; i++)
    {
        arrays[i] = (int*)malloc(sizeof(int) * SIZE);
        if (arrays[i] == NULL)
        {
            free(arrays);
            free(sizes);
            fclose(file);
            fclose(out);

            printf("Memory was not allocated\n");

            return MEM_NOT_ALLOC;
        }
    }

    
    status_code status;
    status = read(file, out, arrays, sizes);
    if (status != OK)
    {
        free(arrays);
        free(sizes);
        fclose(file);
        fclose(out);

        printf("Reading error\n");

        return status;
    }


    for (int i = 0; i < 26; i++)
    {
        free(arrays[i]);
    }

    free(arrays);
    free(sizes);

    fclose(file);
    fclose(out);

    return OK;
}