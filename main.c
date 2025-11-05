#include <stdio.h>
#include "bigInt.h"

int main() {
    BigInt_t* bigInt1, *bigInt2, *sum;
    define(&bigInt1, "+10000");
    define(&bigInt2, "-01003");

    print_bigInt(bigInt1);
    print_bigInt(bigInt2);

    int resultado = compare_bigInt(bigInt1, bigInt2);

    switch (resultado) {
        case EQUAL:
            printf("Iguais\n");
            break;
        case LESS:
            printf("Menor\n");
            break;
        case GREATER:
            printf("Maior\n");
    }

    add_bigInts(&sum, bigInt1, bigInt2);
    print_bigInt(sum);

    destroy(&bigInt1);
    destroy(&bigInt2);
    destroy(&sum);  

    return 0;
}
