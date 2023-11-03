#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define Name_size 15

typedef enum {
    OK,
    INVALID_ARGUMENTS,
    INVALID_FLAG,
    MEM_NOT_ALLOC,
    NOT_OPEN_FILE,
    FILE_ERROR
} status_code;

typedef struct _employee {
    int id;
    char name[Name_size];
    char surname[Name_size];
    double salary;
} Employee;

typedef struct {
    Employee** empl;
    size_t size;
    size_t capacity;
} Vector;

status_code vector_init (Vector* vec) {
    vec->size = 0;
    vec->capacity = 8;
    vec->empl = malloc(sizeof(Employee) * vec->capacity);
    if (vec->empl == NULL) {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code add (Vector* vec, Employee* emp) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        Employee** tmp;
        
        tmp = realloc(vec->empl, sizeof(Employee) * vec->capacity);
        if (tmp == NULL) {
            free(vec->empl);
            return MEM_NOT_ALLOC;
        }
        vec->empl = tmp;
    }

    vec->empl[vec->size++] = emp;

    return OK;
}

status_code del_vector (Vector* vec) {
    for (int i = 0; i < vec->size; i++) {
        Employee* tmp = vec->empl[i];
        free(tmp);
    }

    Employee** tv = vec->empl;
    free(tv);
    vec->capacity = 0;
    vec->size = 0;

    return OK;
}

status_code print_vector (Vector* vec) {
    for (int i = 0; i < vec->size; i++) {
        Employee* tmp = vec->empl[i];
        printf("%d %s %s %f\n", tmp->id, tmp->name, tmp->surname, tmp->salary);
    }

    return OK;
}

status_code check_arguments (int argc, char* argv[]) {
    if (argc != 4) {
        return INVALID_ARGUMENTS;
    }

    if (argv[2][0] != '-' && argv[2][0] != '/') {
        return INVALID_ARGUMENTS;
    }

    if (argv[2][1] != 'a' && argv[2][1] != 'd') {
        return INVALID_FLAG;
    }
}

status_code read_employee (FILE* file, Vector* vec, char* str) {
    Employee* employee = (Employee*)malloc(sizeof(Employee));
    if (employee == NULL) {
        return MEM_NOT_ALLOC;
    }

    if (sscanf(str, "%d %s %s %lf", &employee->id, &employee->name, &employee->surname, &employee->salary) != 4) {
        return INVALID_ARGUMENTS;
    }
    if (add (vec, employee) == MEM_NOT_ALLOC) {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code read_info (FILE* file, Vector* vec) {
    char *buf = (char*)malloc(sizeof(Employee) + 4);
    if (buf == NULL) {
        return MEM_NOT_ALLOC;
    }
    
    int er_check;
    size_t read_line, len = 0;
    while ((read_line = getline(&buf, &len, file)) != -1) {
        if (er_check = read_employee (file, vec, buf) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (er_check == INVALID_ARGUMENTS) {
            return INVALID_ARGUMENTS;
        }
    }
    free(buf);

    return OK;
}

status_code write_res (FILE* file, Vector* vec) {
    for (int i = 0; i < vec->size; i++) {
        Employee* tmp = vec->empl[i];
        fprintf(file, "%d %s %s %f\n", tmp->id, tmp->name, tmp->surname, tmp->salary);
    }

    return OK;
}

int comparator (const void* emp_1, const void* emp_2) {
    Employee* e1 = *(Employee**) emp_1;
    Employee* e2 = *(Employee**) emp_2;

    const double EPS = 1E-8;
    double razn = e1->salary - e2->salary;
    if (fabs(razn) > EPS) {
        if (razn > 0.0) {
            return 1;
        } else {
            return -1;
        }
    }

    int res_strcmp;
    if ((res_strcmp = strcmp(e1->surname, e2->surname)) != 0) {
        return res_strcmp;
    }
    if ((res_strcmp = strcmp(e1->name, e2->name)) != 0) {
        return res_strcmp;
    }

    return e1->id - e2->id;
}

int rev_comparator (const void* emp_1, const void* emp_2) {
    return comparator (emp_2, emp_1);
}

status_code sort (Vector* vec, char flag) {
    if (flag == 'a') {
        qsort(vec->empl, vec->size, sizeof(Employee*), comparator);
    } else if (flag == 'd') {
        qsort(vec->empl, vec->size, sizeof(Employee*), rev_comparator);
    }

    return OK;
}

int main (int argc, char* argv[]) {
    switch (check_arguments(argc, argv)) {
        case INVALID_ARGUMENTS:
            printf("Invalid input\nEnter: Input file -flag Output file\n");
            return INVALID_ARGUMENTS;
        case INVALID_FLAG:
            printf("Invalid flag\nFlags: a, d\n");
            return INVALID_FLAG;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Not open input file\n");
        return NOT_OPEN_FILE;
    }

    FILE* output_file = fopen(argv[3], "w");
    if (output_file == NULL) {
        fclose(input_file);
        printf("Not open output file\n");
        return NOT_OPEN_FILE;
    }

    Vector vector;
    if (vector_init(&vector) == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        fclose(input_file);
        fclose(output_file);
        return MEM_NOT_ALLOC;
    }

    int er_check;
    if (er_check = read_info(input_file, &vector) == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        del_vector(&vector);
        fclose(input_file);
        fclose(output_file);
        return MEM_NOT_ALLOC;
    } else if (er_check == INVALID_ARGUMENTS) {
        printf("Invalid input\n");
        del_vector(&vector);
        fclose(input_file);
        fclose(output_file);
        return INVALID_ARGUMENTS;
    }

    //print_vector(&vector);

    sort(&vector, argv[2][1]);

    //print_vector(&vector);

    write_res(output_file, &vector);

    del_vector(&vector);
    fclose(input_file);
    fclose(output_file);

    return OK;
}
