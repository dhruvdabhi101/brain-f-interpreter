#include <_ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAPE_SIZE 30000

void test();

int ischar(int c) {
    return isprint(c) || isspace(c);
}

void interpret(char *code, int *ptr, int *tape) {
    char *code_ptr = code;
    int *data_ptr = ptr;

    while (*code_ptr != '\0') {
        switch (*code_ptr) {
            case '>':
                if (data_ptr < tape + TAPE_SIZE - 1) data_ptr++;
                break;
            case '<':
                if (data_ptr > tape) data_ptr--;
                break;
            case '+': (*data_ptr)++; break;
            case '-': (*data_ptr)--; break;
            case '.':
                if (ischar(*data_ptr)) {
                    printf("%c", *data_ptr);
                } else {
                    printf("%d", *data_ptr);
                }
                fflush(stdout);
                break;
            case ',': *data_ptr = getchar(); break;
            case '[':
                if (*data_ptr == 0) {
                    int loop_depth = 1;
                    while (loop_depth > 0) {
                        code_ptr++;
                        if (*code_ptr == '[') loop_depth++;
                        if (*code_ptr == ']') loop_depth--;
                    }
                }
                break;
            case ']':
                if (*data_ptr != 0) {
                    int loop_depth = 1;
                    while (loop_depth > 0) {
                        code_ptr--;
                        if (*code_ptr == '[') loop_depth--;
                        if (*code_ptr == ']') loop_depth++;
                    }
                    code_ptr--;
                }
                break;
        }
        code_ptr++;
    }
}

int main(int argc, char **argv) {
    int tape[TAPE_SIZE] = {0};
    int *ptr = tape;

    if (argc > 1) {
        if (strcmp(argv[1], "-t") == 0) {
            test();
            return 0;
        } else if (strcmp(argv[1], "-h") == 0) {
            printf("Usage: %s [-t] [-h] [filename]\n", argv[0]);
            printf("Options:\n");
            printf("  -t: Run tests\n");
            printf("  -h: Display this help message\n");
            printf("  filename: The filename of the source code to interpret\n");
            interpret(argv[1], ptr, tape);
        } else {
            FILE *file = fopen(argv[1], "r");
            if (file == NULL) {
                fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
                return 1;
            }

            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            char *code = malloc(file_size + 1);
            fread(code, 1, file_size, file);
            code[file_size] = '\0';

            interpret(code, ptr, tape);

            fclose(file);
        }

    } else {
        test();
    }

    return 0;
}

void test() {
    int tape[TAPE_SIZE] = {0};
    int *ptr = tape;


    // Test 1: Basic increment and print
    char test1[] = "+++++++++++.";
    printf("Test 1 output: ");
    interpret(test1, ptr, tape);
    printf("\n");

    // Test 2: Decrement
    memset(tape, 0, sizeof(tape));
    ptr = tape;
    char test2[] = "+++++++++++++++.--.";
    printf("Test 2 output: ");
    interpret(test2, ptr, tape);
    printf("\n");

    // Test 3: Simple loop
    memset(tape, 0, sizeof(tape));
    ptr = tape;
    char test3[] = "+++++[>+++++<-]>.";
    printf("Test 3 output: ");
    interpret(test3, ptr, tape);
    printf("\n");

    // Test 4: Nested loops
    memset(tape, 0, sizeof(tape));
    ptr = tape;
    char test4[] = "+++++[>+++++[>+++++<-]<-]>>.";
    printf("Test 4 output: ");
    interpret(test4, ptr, tape);
    printf("\n");
    // what will this print ?

    memset(tape, 0, sizeof(tape));
    ptr = tape;
    char test5[] = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.";
    printf("Test 5 output: ");
    interpret(test5, ptr, tape);
    printf("\n");

}
