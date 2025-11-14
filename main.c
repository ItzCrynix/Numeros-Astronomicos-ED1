#include <stdio.h>
#include <string.h>
#include "bigInt.h"

int main() {
    //const char inputFile[10] = "1.in";
    //FILE* file = fopen(inputFile, "r"); //----> .in files that can be used to test; reminder to change input from stdin to file
    /*
    fprintf(stdout, "---------\n"
        "Steps on how to run:\n"
        "\t1. Type the number of operations that will be done.\n"
        "\t2. Input the command and numbers in the following format: command number1 number2\n"
        "\t3. Repeat step two until you do the desired number of operations inputted in step 1.\n"
        "Commands:\n"
        "\t soma - Adds the two integers;\n"
        "\t igual - Checks if the two integers are equal to each other;\n"
        "\t maior - Checks if the first integer is greater than the second;\n"
        "\t menor - Checks if the first integer is less than the second.\n"
        "---------\n"
    );
    */

    int iterations = 0;
    char buffer[256];

    if (fgets(buffer, 256, stdin)) {
        if (sscanf(buffer, "%d", &iterations) == 1) {
            for (int i = 0; i < iterations; i++) {
                BigInt_t* num1, *num2;
                if (fgets(buffer, 256, stdin)) {
                    buffer[strcspn(buffer, "\n")] = 0; // removing \n from buffer
                    char* input = strtok(buffer, " ");
                    int steps = 0, operation = 0;
                    while (input) {
                        switch (steps) {
                            case 0:
                                if (!strcmp(input, "igual")) operation = EQUAL;
                                else if (!strcmp(input, "maior")) operation = GREATER;
                                else if (!strcmp(input, "menor")) operation = LESS;
                                else operation = 3;
                                break;
                            case 1:
                                define_new_bigInt(&num1, input);
                                break;
                            case 2:
                                define_new_bigInt(&num2, input);
                        }

                        steps++;
                        input = strtok(NULL, " ");
                    }
      
                    fprintf(stdout, "Resultado :: ");
                    if (operation == 3) {
                        BigInt_t* sum;
                        operate_bigInt(&sum, num1, num2);
                        print_bigInt(sum);
                        destroy_bigInt(&sum);
                    } else {
                        if (compare_bigInt(num1, num2) == operation) fprintf(stdout, "True\n");
                        else fprintf(stdout, "False\n");
                    }
                }

                destroy_bigInt(&num1);
                destroy_bigInt(&num2);
            }
        }
    }

    //fclose(file);
    
    return 0;
}
