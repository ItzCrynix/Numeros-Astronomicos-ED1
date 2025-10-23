#include <stdio.h>
#include "bigInt.h"

int main() {
    BigInt_t* bigInt1, *bigInt2;
    define(&bigInt1, "+10000");
    define(&bigInt2, "-00003");

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

    destroy(&bigInt1);
    destroy(&bigInt2);

    return 0;
}
