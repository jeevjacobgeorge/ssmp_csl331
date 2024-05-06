#include <stdio.h>
#include <string.h>

int main() {
    char operands[] = "No1,No2,op1";
    const char delimiter[] = ",";
    char *token;
    char *arguments[10]; // Assuming a maximum of 10 arguments

    // Tokenizing the operands string and storing each token in the arguments array
    token = strtok(operands, delimiter);
    int arg_count = 0;
    while (token != NULL && arg_count < 10) {
        arguments[arg_count] = token;
        arg_count++;
        token = strtok(NULL, delimiter);
    }

    // Printing each argument from the array
    printf("Arguments:\n");
    for (int i = 0; i < arg_count; i++) {
        printf("Argument %d: %s\n", i + 1, arguments[i]);
    }

    return 0;
}
