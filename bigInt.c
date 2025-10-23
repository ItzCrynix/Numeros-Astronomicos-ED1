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

    if (digits[0] == '-') bigInt->digits[0] = '-';
    else bigInt->digits[0] = '+';

    int len = getLength(digits);
    bigInt->len = len;
    int idx = 1;

    BigInt_t* tmp = bigInt;
    
    for (int i = 1; i < len; i++) {
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

int equal(BigInt_t* bigIntThis, BigInt_t* bigIntThat) {
    if (!bigIntThis || !bigIntThat) return BIGINT_ERROR;

    int lenThis = bigIntThis->len;
    int lenThat = bigIntThat->len;
    int idx = 1;

    BigInt_t* tmpThis = bigIntThis;
    BigInt_t* tmpThat = bigIntThat;

    if (lenThis != lenThat) return 0;

    for (int i = 1; i < lenThis; i++) {
        if (idx == DIGIT_SIZE) {
            idx = 0;
            tmpThis = tmpThis->next;
            tmpThat = tmpThat->next;
        }
    
        if (tmpThis->digits[idx] != tmpThat->digits[idx]) return 0;
        idx++;
    }
    
    return EQUAL;
}

int greater(BigInt_t* bigIntThis, BigInt_t* bigIntThat) {
    if (!bigIntThis || !bigIntThat) return BIGINT_ERROR; 

    int lenThis = bigIntThis->len, lenThat = bigIntThat->len;
    int isNegativeThis = (bigIntThis->digits[0] == '-' ? 1 : 0);
    int isNegativeThat = (bigIntThat->digits[0] == '-' ? 1 : 0);
    int idx = 1;
    BigInt_t* tmpThis = bigIntThis;
    BigInt_t* tmpThat = bigIntThat;
    
    if (!isNegativeThis) {
        if (isNegativeThat) return GREATER;

        if (lenThis > lenThat) return GREATER;
        else if (lenThis < lenThat) return 0;
        else {
            for (int i = 1; i < lenThis; i++) {
                if (idx == DIGIT_SIZE) {
                    idx = 0;
                    tmpThis = tmpThis->next;
                    tmpThat = tmpThat->next;
                }
  
                if (tmpThis->digits[idx] > tmpThat->digits[idx]) return GREATER;
                idx++;
            }
        }
    } else {
        if (!isNegativeThat) return 0;

        if (lenThis < lenThat) return GREATER;
        else if (lenThis > lenThat) return 0;
        else {
            for (int i = 1; i < lenThis; i++) {
                if (idx == DIGIT_SIZE) {
                    idx = 0;
                    tmpThis = tmpThis->next;
                    tmpThat = tmpThat->next;
                }

                if (tmpThis->digits[idx] < tmpThat->digits[idx]) return GREATER;
                idx++;
            }
        }
    }

    return 0;
}

int print(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    if (bigInt->digits[0] == '-') printf("%c", bigInt->digits[0]);

    BigInt_t* tmp = bigInt;
    int idx = 1;

    for (int i = 1; i < bigInt->len; i++) {
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
