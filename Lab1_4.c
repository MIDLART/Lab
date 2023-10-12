#include <stdio.h>
#include <string.h>
#include <malloc.h>

int check_parameters (int argc, char* argv[]) {
    if (argc < 3) {
        return -1;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/') {
        return -1;
    }

    if (argv[1][1] == 'n') {
        if (argc != 4) {
            return -1;
        }
        if (argv[1][3]) {
            return -2;
        }
    } else {
        if (argc != 3) {
            return -1;
        }
        if (argv[1][2]) {
            return -2;
        }
    }

    size_t len = strlen(argv[2]) - 1;

    if (len < 5) {
        return -1;
    } 

    if (argv[2][len] != 't' || argv[2][len - 1] != 'x' || 
        argv[2][len - 2] != 't' || argv[2][len - 3] != '.') {
        return -3;
    }

    if (argv[1][1] == 'n') {
        size_t len = strlen(argv[3]) - 1;

        if (len < 5) {
            return -1;
        } 

    }
}

void delete_numbers (FILE *input_file, FILE *output_file) {
    char character;
    while ((character = fgetc(input_file)) != EOF) {
        if (character < '0' || character > '9') {
            fputc(character, output_file);
        }
    }
}

void latin_letters_in_line (FILE *input_file, FILE *output_file) {
    char character;
    int letters = 0;
    while ((character = fgetc(input_file)) != EOF) {
        if ((character >= 'A' && character <= 'Z') || 
            (character >= 'a' && character <= 'z')) {
                letters++;
        } 

        if (character == '\n') {
            fprintf(output_file, "%d\n", letters);
            letters = 0;
        }
    }
}

void other_characters_in_line (FILE *input_file, FILE *output_file) {
    char character;
    float symbols = 0;
    while ((character = fgetwc(input_file)) != EOF) {
        if (((character < '0' && character != ' ') || (character > '9' && character < 'A') ||
            (character > 'Z' && character < 'a') || character > 'z') && 
            character != '\n' && character != '\t') {
                if (character < 0) {
                    symbols += 0.5;
                } else {
                    symbols++;
                }
        } 

        if (character == '\n') {
            fprintf(output_file, "%.0f\n", symbols);
            symbols = 0;
        }
    }
}

void ascii_code_16 (FILE *input_file, FILE *output_file) {
    char character;
    while ((character = fgetc(input_file)) != EOF) {
        if ((character < '0' || character > '9') && character != '\n') {
            fprintf(output_file, "%X", character);
        } else {
            fputc(character, output_file);
        }
    }
}

int main (int argc, char* argv[]){
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    switch (check_parameters(argc, argv)){
        case -1:
            printf("Неверный ввод аргументов!\n");
            return -1;

        case -2:
            printf("Неверный ввод флага!\n");
            return -1;

    }

    FILE *input_file = fopen(argv[2], "r");

    if (input_file == NULL) {
        printf("Не удалось открыть входной файл!\n");
        return -1;
    }

    FILE *output_file;
    if (argv[1][1] == 'n') {
        output_file = fopen(argv[3], "w");
    } else {
        size_t file_name_size = strlen(argv[2]);
        char* file_name = (char*)malloc(sizeof(char)*(file_name_size + 5));
        if(file_name == NULL) {
            printf("Ошибка выделения памяти\n");
            return -1;
        }

        size_t index_current = strlen(argv[2]) - 1;
       
        memcpy(file_name, argv[2], sizeof(char)*index_current);
        file_name[file_name_size + 5] = '\0';
        while (argv[2][index_current] != 92 && index_current >= 0) { // 92 = '\' 
            file_name[index_current + 4] = argv[2][index_current];
            index_current--;
        }
        memcpy(file_name + strlen(file_name) + 2 - index_current, "out_", sizeof(char) * 4);

        printf("%s\n", file_name);
        
        output_file = fopen(file_name, "w");
        free(file_name);
    }

    if (output_file == NULL) {
        printf("Не удалось открыть выходной файл!\n");
        return -1;
    }


    char flag;
    if (argv[1][1] == 'n') {
        flag = argv[1][2];
    } else {
        flag = argv[1][1];
    }

    switch (flag) {
    case 'd':
        delete_numbers(input_file, output_file);
        printf("В выходной файл был записан входной файл с исключением арабский цифр\n");
        break;

    case 'i':
        latin_letters_in_line(input_file, output_file);
        printf("В выходной файл были записано количество латинских букв из входного файла по строкам\n");
        break;

    case 's':
            other_characters_in_line(input_file, output_file);
            printf("В выходной файл были записано количество символов отличных от");
            printf("латинских букв и арабских цифр из входного файла по строкам\n");
        break;

    case 'a':
        ascii_code_16(input_file, output_file);
            printf("В выходной файл были записаны символы вхоного файла в шестнадцатиричном виде\n");
        break;

    default:
        printf("Неверный ввод флага!\n");
        break;
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
