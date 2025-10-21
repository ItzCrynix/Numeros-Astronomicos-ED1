#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"
#include "utils.h"

struct BigInt {
    char* digits;
    int len;
    struct BigInt* next;
};

BigInt_t* create() {
    BigInt_t* bigInt = malloc(sizeof(BigInt_t));
    if (!bigInt) return NULL;

    bigInt->digits = malloc(sizeof(char) * DIGIT_SIZE); 
    if (!(bigInt->digits)) return NULL;

    bigInt->next = NULL;

    return bigInt;
}

BigInt_t* define(char* digits) {
    BigInt_t* bigInt = create();

    int len = getLength(digits);
    bigInt->len = len;
    int idx = 0;

    BigInt_t* tmp = bigInt;
    
    for (int i = 0; i < len; i++) {
        if (idx == DIGIT_SIZE) {
            tmp->next = create();
            tmp = tmp->next;
            idx = 0;
        }

        tmp->digits[idx] = (digits[i] - '0');
        idx++;
    }

    return bigInt;
}

int print(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    BigInt_t* tmp = bigInt;
    int idx = 0;

    for (int i = 0; i < bigInt->len; i++) {
        if (idx == DIGIT_SIZE) {
            tmp = tmp->next;
            idx = 0;
        }

        printf("%d", tmp->digits[idx]);
        idx++;
    }

    printf("\n");

    return 1;
}

void destroy(BigInt_t** bigInt) {
    if (!bigInt) return;

    BigInt_t* head = (*bigInt);

    while (head != NULL) {
        BigInt_t* tmp = head;
        head = head->next;
        free(tmp->digits);
        free(tmp); 
    }

    *bigInt = NULL;
}
