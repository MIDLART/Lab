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

typedef struct _student {
    int id;
    char name[Name_size];
    char surname[Name_size];
    char group[Name_size];
    unsigned char grades[5];
} Student;

typedef struct {
    Student** students;
    size_t size;
    size_t capacity;
} Vector;

status_code vector_init (Vector* vec) {
    vec->size = 0;
    vec->capacity = 8;
    vec->students = malloc(sizeof(Student) * vec->capacity);
    if (vec->students == NULL) {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code add (Vector* vec, Student* emp) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        Student** tmp;
        
        tmp = realloc(vec->students, sizeof(Student) * vec->capacity);
        if (tmp == NULL) {
            free(vec->students);
            return MEM_NOT_ALLOC;
        }
        vec->students = tmp;
    }

    vec->students[vec->size++] = emp;

    return OK;
}

status_code del_vector (Vector* vec) {
    for (int i = 0; i < vec->size; i++) {
        Student* tmp = vec->students[i];
        free(tmp);
    }

    Student** tv = vec->students;
    free(tv);
    vec->capacity = 0;
    vec->size = 0;

    return OK;
}

status_code read_students (FILE* file, Vector* vec, char* str) {
    Student* student = (Student*)malloc(sizeof(Student));
    if (student == NULL) {
        return MEM_NOT_ALLOC;
    }

    if (sscanf(str, "%d %s %s %s %c %c %c %c %c", &student->id, &student->name, &student->surname, &student->group,
        &student->grades[0], &student->grades[1], &student->grades[2], &student->grades[3], &student->grades[4]) != 9) {
        return INVALID_ARGUMENTS;
    }
    if (add (vec, student) == MEM_NOT_ALLOC) {
        return MEM_NOT_ALLOC;
    }

    return OK;
}

status_code read_info (FILE* file, Vector* vec) {
    char *buf = (char*)malloc(sizeof(Student) + 4);
    if (buf == NULL) {
        return MEM_NOT_ALLOC;
    }
    
    int er_check, flag = 0;
    size_t read_line, len = 0;
    while ((read_line = getline(&buf, &len, file)) != -1) {
        if (er_check = read_students (file, vec, buf) == MEM_NOT_ALLOC) {
            return MEM_NOT_ALLOC;
        } else if (er_check == INVALID_ARGUMENTS) {
            break;
        } else {
            flag = 1;
        }
    }
    free(buf);

    if (!flag) {
        return INVALID_ARGUMENTS;
    }

    return OK;
}

status_code arithmetic_mean (unsigned char* grades, double* res) {
    *res = 0.0;
    for (int i = 0; i < 5; i++) {
        if (grades[i] < '1' || grades[i] > '5') {
            return INVALID_ARGUMENTS;
        }
        *res += grades[i] - '0';
    }

    *res /= 5.0;

    return OK;
}

status_code write_res (char* file_name, Vector* vec) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return NOT_OPEN_FILE;
    }
    
    for (int i = 0; i < vec->size; i++) {
        Student* tmp = vec->students[i];
        fprintf(file, "%d %s %s %s\n", tmp->id, tmp->name, tmp->surname, tmp->group,
                tmp->grades[0], tmp->grades[1], tmp->grades[2], tmp->grades[3], tmp->grades[4]);
    }

    fclose(file);
    return OK;
}

int comparator_name (const void* stud_1, const void* stud_2) {
    Student* s1 = *(Student**) stud_1;
    Student* s2 = *(Student**) stud_2;

    return strcmp(s1->name, s2->name);
}

int comparator_surname (const void* stud_1, const void* stud_2) {
    Student* s1 = *(Student**) stud_1;
    Student* s2 = *(Student**) stud_2;

    return strcmp(s1->surname, s2->surname);
}

int comparator_group (const void* stud_1, const void* stud_2) {
    Student* s1 = *(Student**) stud_1;
    Student* s2 = *(Student**) stud_2;

    return strcmp(s1->group, s2->group);
}

int comparator_id (const void* stud_1, const void* stud_2) {
    Student* s1 = *(Student**) stud_1;
    Student* s2 = *(Student**) stud_2;

    return s1->id - s2->id;
}

status_code sort (Vector* vec, char flag) {
    if (flag == 'n') {
        qsort(vec->students, vec->size, sizeof(Student*), comparator_name);
    } else if (flag == 's') {
        qsort(vec->students, vec->size, sizeof(Student*), comparator_surname);
    } else if (flag == 'g') {
        qsort(vec->students, vec->size, sizeof(Student*), comparator_group);
    } else if (flag == 'i') {
        qsort(vec->students, vec->size, sizeof(Student*), comparator_id);
    } else {
        return INVALID_ARGUMENTS;
    }

    return OK;
}

status_code find (Vector* vec, char flag, char* file_name, char* str, int id) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return NOT_OPEN_FILE;
    }

    double res;
    if (flag == 'n') {
        for (int i = 0; i < vec->size; i++) {
            Student* tmp = vec->students[i];
            if (strcmp(str, tmp->name) == 0) {
                if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
                    return INVALID_ARGUMENTS;
                }
                fprintf(file, "%d %s %s %s %.2f\n", tmp->id, tmp->name, tmp->surname, tmp->group, res);
            }
        }

    } else if (flag == 's') {
        for (int i = 0; i < vec->size; i++) {
            Student* tmp = vec->students[i];
            if (strcmp(str, tmp->surname) == 0) {
                if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
                    return INVALID_ARGUMENTS;
                }
                fprintf(file, "%d %s %s %s %.2f\n", tmp->id, tmp->name, tmp->surname, tmp->group, res);
            }
        }
        
    } else if (flag == 'g') {
        for (int i = 0; i < vec->size; i++) {
            Student* tmp = vec->students[i];
            if (strcmp(str, tmp->group) == 0) {
                if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
                    return INVALID_ARGUMENTS;
                }
                fprintf(file, "%d %s %s %s %.2f\n", tmp->id, tmp->name, tmp->surname, tmp->group, res);
            }
        }
        
    } else if (flag == 'i') {
        for (int i = 0; i < vec->size; i++) {
            Student* tmp = vec->students[i];
            if (tmp->id == id) {
                if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
                    return INVALID_ARGUMENTS;
                }
                fprintf(file, "%d %s %s %s %.2f\n", tmp->id, tmp->name, tmp->surname, tmp->group, res);
            }
        }
        
    } else {
        fclose(file);
        return INVALID_ARGUMENTS;
    }

    fclose(file);
    return OK;
}

status_code average_score (Vector* vec, double* aver) {
    double res = 0.0;
    *aver = res;
    for (int i = 0; i < vec->size; i++) {
        Student* tmp = vec->students[i];
        if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
            return INVALID_ARGUMENTS;
        }
        *aver += res;
    }

    *aver /= 1.0 * vec->size;

    return OK;
}

status_code higher_aver_score (Vector* vec, char* file_name) {
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return NOT_OPEN_FILE;
    }

    double aver, res, EPS = 1E-10;
    if (average_score(vec, &aver) == INVALID_ARGUMENTS) {
        return INVALID_ARGUMENTS;
    }

    for (int i = 0; i < vec->size; i++) {
        Student* tmp = vec->students[i];
        if (arithmetic_mean(tmp->grades, &res) == INVALID_ARGUMENTS) {
            return INVALID_ARGUMENTS;
        }

        if (res - aver > EPS) {
            fprintf(file, "%d %s %s %s %.2f\n", tmp->id, tmp->name, tmp->surname, tmp->group, res);
        }
    }

    fclose(file);

    return OK;
}

int main (int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalid input\n");
        return INVALID_ARGUMENTS;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Not open input file\n");
        return NOT_OPEN_FILE;
    }

    Vector vector;
    if (vector_init(&vector) == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        fclose(input_file);
        return MEM_NOT_ALLOC;
    }

    int er_check;
    if (er_check = read_info(input_file, &vector) == MEM_NOT_ALLOC) {
        printf("Memory was not allocated\n");
        del_vector(&vector);
        fclose(input_file);
        return MEM_NOT_ALLOC;
    } else if (er_check == INVALID_ARGUMENTS) {
        printf("Invalid input\n");
        del_vector(&vector);
        fclose(input_file);
        return INVALID_ARGUMENTS;
    }

    int e = 1, id = 0;
    status_code status;
    char ent, ent_2;
    char buf[256];
    while (e) {
        printf("Enter:\ne - exit\nf - find student\ns - sort students\na - students with a score above average\n");
        scanf(" %c", &ent);
        getchar();
        switch (ent) {
            case 'e':
                e = 0;
                break;

            case 'f':
                printf("Find:\nn - name\ns - surname\ng - group\ni - id\n");
                scanf("%c", &ent_2);
                getchar();

                printf("Find: ");
                if (ent_2 == 'i') {
                    if (scanf("%d", &id) != 1) {
                        printf("Invalid input1\n");
                        getchar();
                        break;
                    }
                } else {
                    if (scanf("%s", &buf) != 1) {
                        printf("Invalid input2\n");
                        getchar();
                        break;
                    }
                }
                getchar();

                status = find(&vector, ent_2, argv[2], buf, id);
                if (status == INVALID_ARGUMENTS) {
                    printf("Invalid input3\n");
                    break;
                } else if (status == NOT_OPEN_FILE) {
                    fclose(input_file);
                    printf("Not open output file\n");
                    return NOT_OPEN_FILE;
                }
                break;

            case 's':
                printf("Sort:\nn - name\ns - surname\ng - group\ni - id\n");
                scanf("%c", &ent_2);
                getchar();

                if (sort(&vector, ent_2) == INVALID_ARGUMENTS) {
                    printf("Invalid input4\n");
                    break;
                }

                if (write_res(argv[2], &vector) == NOT_OPEN_FILE) {
                    fclose(input_file);
                    printf("Not open output file\n");
                    return NOT_OPEN_FILE;
                }
                break;

            case 'a':
                status = higher_aver_score(&vector, argv[2]);
                if (status == INVALID_ARGUMENTS) {
                    printf("Invalid input3\n");
                    break;
                } else if (status == NOT_OPEN_FILE) {
                    fclose(input_file);
                    printf("Not open output file\n");
                    return NOT_OPEN_FILE;
                }
                break;
            
            default:
                printf("Invalid input5\n");
                break;
        }
    }

    del_vector(&vector);
    fclose(input_file);

    return OK;
}