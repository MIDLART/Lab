#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

typedef enum {
    ok,
    invalid_arguments,
    not_open_file,
    overflow
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 3) {
        return invalid_arguments;
    }
    return ok;
}

status_code conversion (char num[], int num_system, long long *decimal) {
    long long res = 0;
    int i, cur, otr = 0;
    if (num[0] == '-') {
        otr = 1;
    }
    i = otr;
    while (num[i] != '\0') {
        cur = num[i];
        if (cur >= 'A') {
            cur -= 7;
        }
        cur -= '0';
        if ((ULLONG_MAX - cur) / num_system < res) {
            return overflow;
        }
        res = res * num_system + cur;
        i++;
    }
    if (otr) {
        res *= -1;
    }
    *decimal = res;
    return ok;
}

status_code number_system (char argv1[], char argv2[]) {
    FILE *input_file = fopen(argv1, "r");
    if (input_file == NULL) {
        return not_open_file;
    }
    FILE *output_file = fopen(argv2, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return not_open_file;
    }

    char character, min_num_system = 0, str[256];
    int int_character, i = 0, flag_no_file = 1;
    long long decimal;
    while (character != EOF) {
        while ((character = fgetc(input_file)) == ' ' ||
                character == '\t' || character == '\n');
                
        if(feof(input_file)) {
            flag_no_file = 0;
        }

        if (character == '-') {
            str[0] = character;
            i++;
            fputc(character, output_file);
            character = fgetc(input_file);
        }
        while (character == '0') {
            character = fgetc(input_file);
        }
        if (character == EOF || character == ' ' ||
            character == '\t' || character == '\n') {
                fputc('0', output_file);
        }

        while (character != EOF && character != ' ' &&
                character != '\t' && character != '\n') {
                    if (isdigit(character) != 0 && isalpha(character) != 0) {
                        return invalid_arguments;
                    }
                    
                    if (character >= 'a' && character <= 'z') {
                        character -= 32;
                    }
                    if (character > min_num_system) {
                        min_num_system = character;
                    }

                    str[i] = character;
                    i++;

                    fputc(character, output_file);
                    character = fgetc(input_file);
        }
        if (i > 256) {
            return overflow;
        }
        str[i] = '\0';

        if (min_num_system >= 'A') {
            min_num_system -= 7;
        }
        if (min_num_system == 0 && flag_no_file != 0) {
            fprintf(output_file, " 2 ");
            fputc('0', output_file);
        } else {
            min_num_system += 1 - '0';
            fprintf(output_file, " %d ", min_num_system);

            if (conversion(str, min_num_system, &decimal) == ok) {
                fprintf(output_file, "%lld", decimal);
            } else {
                fprintf(output_file, "Переполнение");
            }
            
            min_num_system = 0;
        }

        i = 0;
        decimal = 0;

        fputc('\n', output_file);
    }

    fclose(input_file);
    fclose(output_file);
    return ok;
}


int main (int argc, char* argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    if (check_parameters(argc, argv) == invalid_arguments){
        printf("Неверный ввод аргументов!\n");
        return invalid_arguments;
    }

    if (number_system(argv[1], argv[2]) != ok) {
        printf("Ошибка!\n");
        return invalid_arguments;
    } else {
        printf("Выполнение успешно\n");
    }

    return 0;
}
