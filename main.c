#include "bigInt.h"

int main() {
    BigInt_t* bigInt = define("999999999999999999999999999999999");

    print(bigInt);

    destroy(&bigInt);

    return 0;
}
