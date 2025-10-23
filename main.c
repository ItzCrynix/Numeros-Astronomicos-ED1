#include <stdio.h>
#include "bigInt.h"

int main() {
    BigInt_t* bigInt1 = define("+999");
    BigInt_t* bigInt2 = define("+1000");

    print(bigInt1);
    print(bigInt2);
    
    if (equal(bigInt1, bigInt2) == EQUAL) printf("equal!\n");
    else printf("not equal!\n");
    
    if (greater(bigInt1, bigInt2) == GREATER) printf("greater!\n");
    else printf("not greater!\n");

    destroy(&bigInt1);
    destroy(&bigInt2);

    return 0;
}
