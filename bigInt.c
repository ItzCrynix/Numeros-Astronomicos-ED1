#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    bigInt->isNegative = 1;

    return bigInt;
}

/*
@brief Define a "BigInt" struct based on a string of digits.
@param digits The string containing the desired number.
@return Pointer to a "BigInt".
*/
int define(BigInt_t** bigInt, char* digits) {
    if (!digits) return BIGINT_ERROR;
    
    BigInt_t* tmp = create();
    *(bigInt) = tmp;

    if (digits[0] == '-') 
        tmp->isNegative = -1;

    int len = getLength(digits) - 1;
    int idx = 0, aux = 1;

    while (digits[aux] == '0' || digits[aux] == 0) { 
        aux++; 
        len--; 
    }

    tmp->len = len;

    while (digits[aux] != '\0') {
        if (idx == DIGIT_SIZE) {
            tmp->next = create();
            tmp = tmp->next;
            idx = 0;
        }

        
        tmp->digits[idx] = digits[aux];
        idx++; aux++;
    }

    return BIGINT_SUCCESS;
}

/*
@brief Compare two numbers; greater, less or equal.
@param number1 Number that is going to be compared.
@param number2 Number that the first parameter is going to be compared to.
@return GREATER, LESS, or EQUAL.
*/
int compare_bigInt(BigInt_t* number1, BigInt_t* number2) {
    if (!number1 || !number2) return BIGINT_ERROR;

    if (number1->isNegative == -1 && number2->isNegative == 1) 
        return LESS;
    else if (number1->isNegative == 1 && number2->isNegative == -1)
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

    if (tempNum1 != NULL && tempNum1->isNegative == -1) 
        result = LESS;
    else if (tempNum2 != NULL && tempNum2->isNegative == -1)
        result = GREATER;

    return result;
}

int print_bigInt(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    if (bigInt->isNegative == -1) printf("-");

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

    return BIGINT_SUCCESS;
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

int add_bigInts(BigInt_t* result, BigInt_t* number1, BigInt_t* number2) {
    if (number1 == NULL || number2 == NULL) return BIGINT_ERROR;

    char* addition_result;
    addition_result = make_sum(number1, number2);

    define(&result, addition_result);

    free(addition_result);

    return BIGINT_SUCCESS;
}

char* make_sum(BigInt_t* number1, BigInt_t* number2) {
    int max_size = (number1->len > number2->len) ? number1->len : number2->len;
    max_size += 3;

    char* result = calloc(max_size, sizeof(char));
    int resultIndex = max_size - 2;
    
    char* temp1 = to_string(number1);
    printf("TEMP 1: %s\n", temp1);
    char* temp2 = to_string(number2);
    printf("TEMP 2: %s\n", temp2);

    int carry = 0;
    int indexNum1 = number1->len - 1, indexNum2 = number2->len - 1;

    /* 
    negativo + negativo
        carry -1 se soma <= -10
        carry 0 se soma entre -9 e -1
    negativo + positivo
        carry -1 se soma < 0
        carry 0 se soma >= 0
    positivo + positivo
        carry 0 se soma < 10
        carry 1 se soma >= 10
    */
    for (; resultIndex > 1; resultIndex--) {
        printf("IDX: (1) %d, (2) %d\n", indexNum1, indexNum2);
        int digit1 = (indexNum1 >= 0) ? (temp1[indexNum1] - '0') : 0;
        int digit2 = (indexNum2 >= 0) ? (temp2[indexNum2] - '0') : 0;
        
        indexNum1--; indexNum2--;

        printf("DIGITS: (1) %d, (2) %d\n", digit1, digit2);
        int sum = digit1 * number1->isNegative + digit2 * number2->isNegative + carry;
        
        if (number1->isNegative == -1 && number2->isNegative == -1) {
            if (sum <= -10) 
                carry = -1;
            else 
                carry = 0;
        } else if (number1->isNegative == 1 && number2->isNegative == 1) {
            if (sum >= 10) 
                carry = 1;
            else 
                carry = 0;
        } else {
            if (sum < 0) 
                carry = -1;
            else 
                carry = 0;
        }

        sum = abs((sum + 10) % 10);
        printf("SUM: (int) %d, (char) %c\n", sum, sum + '0');
        result[resultIndex] = sum + '0';
    }

    /* Adiciona os digitos restantes do numero
    while (indexNum1 < number1->len) {
        result[resultIndex] = temp1[indexNum1];
        resultIndex--; indexNum1--;
    }

    while (indexNum2 < number2->len) {
        result[resultIndex] = temp2[indexNum2];
        resultIndex--; indexNum2--;
    }
    */

    if (carry < 0) {
        result[resultIndex] = '1';
        result[resultIndex - 1] = '-';
    }
    else {
        result[resultIndex] = carry;
        result[resultIndex - 1] = '+';
    }

    for (int i = 0; i < 10; i++) {
        printf("%c", result[i]);    
    }

    printf("\n");
    
    return result;
}

char* to_string(BigInt_t* bigInt) {
    if (!bigInt) return NULL;
    
    char* number = calloc(bigInt->len, sizeof(char));
    int aux = 0;
    BigInt_t* temp = bigInt;

    while (temp != NULL) {
        for (int i = 0; i < DIGIT_SIZE; i++) {
            if (aux == bigInt->len) break;
            number[aux] = temp->digits[i];
            aux++;
        }

        temp = temp->next;
    }

    return number;
}
