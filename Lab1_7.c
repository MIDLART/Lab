#include <stdio.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    invalid_flag,
    not_open_file
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc < 4) {
        return invalid_arguments;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/' || argv[1][2]) {
        return invalid_arguments;
    }

    int files;
    if (argv[1][1] == 'r') {
        if (argc != 5) {
            return invalid_arguments;
        }
        files = 3;
    } else if (argv[1][1] == 'a') {
        if (argc != 4) {
            return invalid_arguments;
        }
        files = 2;
    } else {
        return invalid_flag;
    }

    for (int i = 2; i <= files + 1; i++) {
        size_t len = strlen(argv[i]) - 1;

        if (len < 5) {
            return invalid_arguments;
        } 
    }
    return ok;
}

status_code flag_r (char argv1[], char argv2[], char argv3[]) {
    FILE *file1 = fopen(argv1, "r");
    if(file1 == NULL) {
        return not_open_file;
    }
    FILE *file2 = fopen(argv2, "r");
    if(file1 == NULL) {
        fclose(file1);
        return not_open_file;
    }
    FILE *file3 = fopen(argv3, "w");
    if(file1 == NULL) {
        fclose(file1);
        fclose(file2);
        return not_open_file;
    }
    
    char character_1, character_2;
    while (character_1 != EOF || character_2 != EOF) {
        if (character_1 != EOF) {
            while ((character_1 = fgetc(file1)) == ' ' ||
                    character_1 == '\t' || character_1 == '\n'); //пропуск пробелов, табуляций и переносов

            while (character_1 != EOF && character_1 != ' ' &&
                    character_1 != '\t' && character_1 != '\n') {
                        fputc(character_1, file3);
                        character_1 = fgetc(file1);
            }
            if (character_1 != EOF) {
                fputc(' ', file3);
            }
        }

        if (character_2 != EOF) {
            while ((character_2 = fgetc(file2)) == ' ' ||
                    character_2 == '\t' || character_2 == '\n'); //пропуск пробелов, табуляций и переносов

            while (character_2 != EOF && character_2 != ' ' &&
                    character_2 != '\t' && character_2 != '\n') {
                        fputc(character_2, file3);
                        character_2 = fgetc(file2);
            }
            if (character_2 != EOF) {
                fputc(' ', file3);
            }
        }
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);
    return ok;
}

int conversion_10_to_4 (int num) {
    int res = 0, i = 1;
    while (num != 0) {
        res = (num % 4) * i + res;
        num /= 4;
        i *= 10;
    }
    return res;
}

status_code flag_a (char argv1[], char argv2[]) {
    FILE *input_file = fopen(argv1, "r");
    if (input_file == NULL) {
        return not_open_file;
    }
    FILE *output_file = fopen(argv2, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return not_open_file;
    }

    char character_1;
    int lexeme = 1, int_character;
    while (character_1 != EOF) {
        while ((character_1 = fgetc(input_file)) == ' ' ||
                character_1 == '\t' || character_1 == '\n'); //пропуск пробелов, табуляций и переносов

        while (character_1 != EOF && character_1 != ' ' &&
                character_1 != '\t' && character_1 != '\n') {
                    if (lexeme % 2 == 0 && character_1 >= 'A' && character_1 <= 'Z') {
                        character_1 += 32;

                        if (lexeme % 10 == 0) {
                            int_character = conversion_10_to_4(character_1);
                            fputc(int_character, output_file);
                        } else {
                            fputc(character_1, output_file);
                        }
                    } else if (lexeme % 5 == 0) {
                        int_character = character_1;
                        fprintf(output_file, "%o", int_character);
                    } else {
                        fputc(character_1, output_file);
                    }

                    character_1 = fgetc(input_file);
        }
        fputc(' ', output_file);
        lexeme++;
    }

    fclose(input_file);
    fclose(output_file);
    return ok;
}

int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    switch (check_parameters(argc, argv)){
        case invalid_arguments:
            printf("Неверный ввод аргументов!\n");
            return invalid_arguments;

        case invalid_flag:
            printf("Неверный ввод флага!\n");
            return invalid_flag;
    }

    switch (argv[1][1]) {
        case 'r':
            if (flag_r(argv[2], argv[3], argv[4]) == not_open_file) {
                printf("Ошибка!\n");
            } else {
                printf("Выполнение успешно\n");
            }
            break;

        case 'a':
            if (flag_a(argv[2], argv[3]) == not_open_file) {
                printf("Ошибка!\n");
            } else {
                printf("Выполнение успешно\n");
            }
            break;
        
        default:
            printf("Неверный ввод флага!\n");
            return invalid_flag;
        }

    return 0;
}
