#include <stdio.h>
#include <stdlib.h>
#include "bigInt.h"
#include "utils.h"

struct BigInt {
    char* digits;
    int len;
    int isNegative;
    struct BigInt* next;
};

BigInt_t* create() {
    BigInt_t* bigInt = (BigInt_t*) malloc(sizeof(BigInt_t));
    if (!bigInt) return NULL;

    bigInt->digits = (char*) calloc(DIGIT_SIZE, sizeof(char)); 
    if (!(bigInt->digits)) return NULL;

    bigInt->next = NULL;
    bigInt->isNegative = 0;

    return bigInt;
}

/*
@brief Define a "BigInt" struct based on a string of digits.
@param digits The string containing the desired number.
@return Pointer to a "BigInt".
*/
BigInt_t* define(char* digits) {
    BigInt_t* bigInt = create();

    if (digits[0] == '-') 
        bigInt->isNegative = 1;

    int len = getLength(digits) - 1;
    bigInt->len = len;
    int idx = 0;

    BigInt_t* tmp = bigInt;
    
    for (int i = 1; i <= len; i++) {
        if (idx == DIGIT_SIZE) {
            tmp->next = create();
            tmp = tmp->next;
            idx = 0;
        }

        tmp->digits[idx] = digits[i];
        idx++;
    }

    return bigInt;
}

/*
@brief Compare two numbers; greater, less or equal.
@param number1 Number that is going to be compared.
@param number2 Number that the first parameter is going to be compared to.
@return GREATER, LESS, or EQUAL.
*/
int compare_bigInt(BigInt_t* number1, BigInt_t* number2) {
    if (!number1 || !number2) return BIGINT_ERROR;

    if (number1->isNegative && !number2->isNegative) 
        return LESS;
    else if (!number1->isNegative && number2->isNegative)
        return GREATER;

    int result = EQUAL;

    BigInt_t* tempNum1 = number1;
    BigInt_t* tempNum2 = number2;

    while (tempNum1 != NULL && tempNum2 != NULL) {
        for (int i = 0; i < DIGIT_SIZE; i++) {
            if (tempNum1->digits[i] > tempNum2->digits[i]) {
                result = GREATER;
                break;
            }
            else if (tempNum1->digits[i] < tempNum2->digits[i]) {
                result = LESS;
                break;
            }
        }

        tempNum1 = tempNum1->next;
        tempNum2 = tempNum2->next;
    }

    if (tempNum1 != NULL && tempNum1->isNegative) 
        result = LESS;
    else if (tempNum2 != NULL && tempNum2->isNegative)
        result = GREATER;

    return result;
}

int print_bigInt(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    if (bigInt->isNegative) printf("-");

    BigInt_t* tmp = bigInt;
    int idx = 0;

    for (int i = 0; i < bigInt->len; i++) {
        if (idx == DIGIT_SIZE) {
            tmp = tmp->next;
            idx = 0;
        }

        printf("%c", tmp->digits[idx]);   
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
