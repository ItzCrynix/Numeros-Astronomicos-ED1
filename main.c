#include <stdio.h>
#include "bigInt.h"

int main() {
    BigInt_t* bigInt1, *bigInt2, *sum;
    define_new_bigInt(&bigInt1, "2500");
    define_new_bigInt(&bigInt2, "113567");

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
    
    destroy_bigInt(&bigInt1);
    destroy_bigInt(&bigInt2);
    destroy_bigInt(&sum);  

    return 0;
}
