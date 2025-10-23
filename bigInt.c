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

// 2 -> o numero 2 é maior
// 1 -> o numero 1 é maior
// 0 -> são iguais
int compare_bigint(BigInt_t* number1, BigInt_t* number2) {
    if (!number1 || !number2) return BIGINT_ERROR;

    if (number1->isNegative && !number2->isNegative) 
        return 2;
    else if (!number1->isNegative && number2->isNegative)
        return 1;

    int result = 0;

    BigInt_t* tempNum1 = number1;
    BigInt_t* tempNum2 = number2;

    while (tempNum1 != NULL && tempNum2 != NULL) {
        for (int i = 0; i < DIGIT_SIZE; i++) {
            if (tempNum1->digits[i] > tempNum2->digits[i]) {
                result = 1;
                break;
            }
            else if (tempNum1->digits[i] > tempNum2->digits[i]) {
                result = 2;
                break;
            }
        }

        tempNum1 = tempNum1->next;
        tempNum2 = tempNum2->next;
    }

    if (tempNum1 != NULL && tempNum1->isNegative) 
        result = 2;
    else if (tempNum2 != NULL && tempNum2->isNegative)
        result = 1;

    return result;
}

// int equal(BigInt_t* bigIntThis, BigInt_t* bigIntThat) {
//     if (!bigIntThis || !bigIntThat) return BIGINT_ERROR;

//     int lenThis = bigIntThis->len;
//     int lenThat = bigIntThat->len;
//     int idx = 1;

//     BigInt_t* tmpThis = bigIntThis;
//     BigInt_t* tmpThat = bigIntThat;

//     if (lenThis != lenThat) return 0;

//     for (int i = 1; i < lenThis; i++) {
//         if (idx == DIGIT_SIZE) {
//             idx = 0;
//             tmpThis = tmpThis->next;
//             tmpThat = tmpThat->next;
//         }
    
//         if (tmpThis->digits[idx] != tmpThat->digits[idx]) return 0;
//         idx++;
//     }
    
//     return EQUAL;
// }

// int greater(BigInt_t* bigIntThis, BigInt_t* bigIntThat) {
//     if (!bigIntThis || !bigIntThat) return BIGINT_ERROR; 

//     int lenThis = bigIntThis->len, lenThat = bigIntThat->len;
//     int isNegativeThis = (bigIntThis->digits[0] == '-' ? 1 : 0);
//     int isNegativeThat = (bigIntThat->digits[0] == '-' ? 1 : 0);
//     int idx = 1;
//     BigInt_t* tmpThis = bigIntThis;
//     BigInt_t* tmpThat = bigIntThat;
    
//     if (!isNegativeThis) {
//         if (isNegativeThat) return GREATER;

//         if (lenThis > lenThat) return GREATER;
//         else if (lenThis < lenThat) return 0;
//         else {
//             for (int i = 1; i < lenThis; i++) {
//                 if (idx == DIGIT_SIZE) {
//                     idx = 0;
//                     tmpThis = tmpThis->next;
//                     tmpThat = tmpThat->next;
//                 }
  
//                 if (tmpThis->digits[idx] > tmpThat->digits[idx]) return GREATER;
//                 idx++;
//             }
//         }
//     } else {
//         if (!isNegativeThat) return 0;

//         if (lenThis < lenThat) return GREATER;
//         else if (lenThis > lenThat) return 0;
//         else {
//             for (int i = 1; i < lenThis; i++) {
//                 if (idx == DIGIT_SIZE) {
//                     idx = 0;
//                     tmpThis = tmpThis->next;
//                     tmpThat = tmpThat->next;
//                 }

//                 if (tmpThis->digits[idx] < tmpThat->digits[idx]) return GREATER;
//                 idx++;
//             }
//         }
//     }

//     return 0;
// }

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
