#include <stdio.h>
#include "bigInt.h"

int main() {
    BigInt_t* bigInt1 = define("-1000");
    BigInt_t* bigInt2 = define("-1000");

    print_bigInt(bigInt1);
    print_bigInt(bigInt2);

    int resultado = compare_bigint(bigInt1, bigInt2);
    
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
