#include <stdio.h>
#include <string.h>

int check_parameters (int argc, char* argv[]) {
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

    int len = strlen(argv[2]) - 1;

    if (len < 5) {
        return -1;
    } 

    if (argv[2][len] != 't' || argv[2][len - 1] != 'x' || 
        argv[2][len - 2] != 't' || argv[2][len - 3] != '.') {
        return -3;
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
    int symbols = 0;
    while ((character = fgetwc(input_file)) != EOF) {
        if (((character < '0' && character != ' ') || (character > '9' && character < 'A') ||
            (character > 'Z' && character < 'a') || character > 'z') && character != '\n') {
                if (character > 127) {

                } else {
                symbols++;
                printf("%c\n", character);
                }
        } 

        if (character == '\n') {
            fprintf(output_file, "%d\n", symbols);
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

        case -3:
            printf("Файл должен иметь разрешение .txt\n");
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
        int i = strlen(argv[2]) - 1;
        char file_name[i + 6];
        file_name[i + 5] = '\0';

        while (argv[2][i] != 92 && i >= 0) { // 92 = '\' 
            file_name[i + 4] = argv[2][i];
            i--;
        }
        
        file_name[i + 4] = '_';
        file_name[i + 3] = 't';
        file_name[i + 2] = 'u';
        file_name[i + 1] = 'o';

        while (i >= 0) {
            file_name[i] = argv[2][i];
            i--;
        }
        
        output_file = fopen(file_name, "w");
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