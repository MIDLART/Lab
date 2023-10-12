#include <stdio.h>
#include <string.h>

typedef enum {
    ok,
    invalid_arguments,
    file_error
} status_code;

status_code check_parameters (int argc, char* argv[]) {
    if (argc != 2) {
        return invalid_arguments;
    }
    return ok;
}

status_code file_recording (char* file_name, char* str) {
    FILE *input_file = fopen(file_name, "w");
    if (input_file == NULL) {
        printf("Не удалось открыть файл\n");
        return file_error;
    }
    size_t len = strlen(str);
    if (fwrite(str, sizeof(char), len, input_file) != len) {
        fclose(input_file);
        return file_error;
    }
 
    fclose(input_file);
    return ok;
}

int main (int argc, char *argv[]) {
    printf("М8О-213Б-22 Одинцов Артём Максимович\n");

    if (check_parameters(argc, argv) == invalid_arguments) {
        printf("Введите путь к файлу\n");
        return invalid_arguments;
    }

    char str[] = "31415926535";
    if (file_recording(argv[1], str) != ok) {
        return file_error;
    }
 
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("Не удалось открыть файл\n");
        return file_error;
    }

    char character;
    while (fread(&character, sizeof(char), 1, input_file)) {
        printf("%x\t%p\n", character, input_file);
    }

    return 0;
}