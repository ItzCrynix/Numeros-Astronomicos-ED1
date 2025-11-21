#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigInt.h"

struct BigInt {
    char* digits;
    int len;
    int isNegative;
    struct BigInt* next;
};

//
// Initialization functions
//

/*
@brief Creates a new, empty BigInt number
@return A pointer to the new BigInt
*/
BigInt_t* create_bigInt() {
    BigInt_t* bigInt = (BigInt_t*) malloc(sizeof(BigInt_t));
    if (!bigInt) return NULL;

    bigInt->digits = (char*) calloc(DIGIT_SIZE, sizeof(char)); 
    if (!(bigInt->digits)) return NULL;

    bigInt->next = NULL;
    bigInt->isNegative = 0;

    return bigInt;
}

/*
@brief Free the memory of a BigInt
@param bigInt The BigInt you want to free
*/
void destroy_bigInt(BigInt_t** bigInt) {
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

/*
@brief Assign a new number to a new BigInt (pointer).
@param bigInt The address to the BigInt you want to define.
@param digits The string containing the desired number.
@return BIGINT_ERROR or BIGINT_SUCCESS
*/
int define_new_bigInt(BigInt_t** bigInt, char* digits) {
    if (!digits) return BIGINT_ERROR;
    
    BigInt_t* tmp = create_bigInt();
    *(bigInt) = tmp;

    if (digits[0] == '-') 
        tmp->isNegative = 1;

    int len = strlen(digits);
    int idx = 0, aux = 0, tmpLen = len - 1;

    while (digits[aux] == '0' || digits[aux] == 0 || digits[aux] == '+' || digits[aux] == '-') { 
        if (aux == tmpLen) break;
        aux++; 
        len--;
    }

    tmp->len = len;

    while (digits[aux] != '\0') {
        if (idx == DIGIT_SIZE) {
            tmp->next = create_bigInt();
            tmp = tmp->next;
            idx = 0;
        }
   
        tmp->digits[idx] = digits[aux];
        idx++; aux++;
    }

    return BIGINT_SUCCESS;
}

//
// Utils
//

/*
@brief Prints a bigInt type on the console
@param bigInt The number you want to print
@return BIGINT_ERROR or BIGINT_SUCCESS
*/
int print_bigInt(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    if (bigInt->isNegative) fprintf(stdout, "-");

    BigInt_t* tmp = bigInt;
    int idx = 0;
    while (tmp) {
        if (tmp->digits[idx] != 0)
            fprintf(stdout, "%c", tmp->digits[idx]);
        
        idx++;
        
        if (idx == DIGIT_SIZE) {
            idx = 0;
            tmp = tmp->next;
        }
    }

    fprintf(stdout, "\n");

    return BIGINT_SUCCESS;
}

//
// Arithmetic and Logic Operations
//

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
                if (result == EQUAL) result = GREATER;
                break;
            }
            else if (tempNum1->digits[i] < tempNum2->digits[i]) {
                if (result == EQUAL) result = LESS;
                break;
            }
        }

        tempNum1 = tempNum1->next;
        tempNum2 = tempNum2->next;
    }

    if ((tempNum1 != NULL && number1->isNegative) || ((number2->len > number1->len) && !number2->isNegative)) 
        result = LESS;
     else if ((tempNum2 != NULL && number2->isNegative) || ((number1->len > number2->len) && !number1->isNegative)) 
        result = GREATER;

    return result;
}

/*
All three functions before "operate_bigInt" are private functions, which can only be called through here; the user doesn't have access to it directly.
We only call them when we are sure none of the "BigInts" are NULL.
*/

/*
@brief Adds two "BigInts", is only called through "operate_bigInt" when both numbers have the same sign.
@param number1 First "BigInt" in the operation.
@param number2 Second "BigInt" in the operation.
@return A string containing the sum.
*/
char* add_bigInt(BigInt_t* number1, BigInt_t* number2) {
    int max_len = (number1->len > number2->len) ? number1->len : number2->len;
    max_len += 3; // Taking the sign ('+' or '-'), the null terminator and the max length of a sum (largest numLength + 1) into account.

    char* result = calloc(max_len, sizeof(char));
    int resultIdx = max_len - 2;

    int num1Itr = (number1->len + (DIGIT_SIZE - 1)) / DIGIT_SIZE;
    int num2Itr = (number2->len + (DIGIT_SIZE - 1)) / DIGIT_SIZE;   
    int idx1 = DIGIT_SIZE - 1, idx2 = DIGIT_SIZE - 1;

    int carry = 0;

    while (resultIdx > 0) {
        BigInt_t* tmp1 = (num1Itr > 0) ? number1 : NULL;
        BigInt_t* tmp2 = (num2Itr > 0) ? number2 : NULL;

        for (int i = 0; i < num1Itr - 1; i++)
            tmp1 = tmp1->next;

        for (int i = 0; i < num2Itr - 1; i++) 
            tmp2 = tmp2->next;

        while (idx1 >= 0 && idx2 >= 0) {
            if (resultIdx <= 0) break;
         
            if (tmp1) while (tmp1->digits[idx1] == 0) idx1--;

            if (tmp2) while (tmp2->digits[idx2] == 0) idx2--;

            int digit1 = (tmp1 && tmp1->digits[idx1]) ? (tmp1->digits[idx1] - '0') : 0;
            int digit2 = (tmp2 && tmp2->digits[idx2]) ? (tmp2->digits[idx2] - '0') : 0;
            idx1--; idx2--; 

            int sum = digit1 + digit2 + carry;

            carry = sum / 10;

            char resultDigit = ((sum % 10) + '0');
            result[resultIdx] = resultDigit;
            resultIdx--;
        }

        if (idx1 < 0) {
            num1Itr--;
            idx1 = DIGIT_SIZE - 1;
        }
        
        if (idx2 < 0) { 
            num2Itr--;
            idx2 = DIGIT_SIZE - 1;
        }
    }

    if (!number1->isNegative)
        result[0] = '+';
    else
        result[0] = '-';

    return result;
}

/*
@brief Subtracts two "BigInts", is only called through "operate_bigInt" when the numbers have different signs.
@param number1 First "BigInt" in the operation.
@param number2 Second "BigInt" in the operation.
@return A string containing the subtraction.
*/
char* subt_bigInt(BigInt_t* number1, BigInt_t* number2) {
    int max_len = number1->len; // First number is always greater than the second in this case.
    max_len += 2; // Taking the sign ('+' or '-') and the null terminator into account.

    char* result = calloc(max_len, sizeof(char));
    int resultIdx = max_len - 2; 

    int num1Itr = (number1->len + (DIGIT_SIZE - 1)) / DIGIT_SIZE;
    int num2Itr = (number2->len + (DIGIT_SIZE - 1)) / DIGIT_SIZE;   
    int idx1 = DIGIT_SIZE - 1, idx2 = DIGIT_SIZE - 1;

    int borrow = 0;

    while (resultIdx > 0) { 
        BigInt_t* tmp1 = (num1Itr > 0) ? number1 : NULL;
        BigInt_t* tmp2 = (num2Itr > 0) ? number2 : NULL;

        for (int i = 0; i < num1Itr - 1; i++)
            tmp1 = tmp1->next;

        for (int i = 0; i < num2Itr - 1; i++)
            tmp2 = tmp2->next;

        while (idx1 >= 0 && idx2 >= 0) {
            if (resultIdx <= 0) break;

            if (tmp1) while (tmp1->digits[idx1] == 0) idx1--;
            if (tmp2) while (tmp2->digits[idx2] == 0) idx2--;

            int digit1 = (tmp1 && tmp1->digits[idx1]) ? (tmp1->digits[idx1] - '0') : 0;
            int digit2 = (tmp2 && tmp2->digits[idx2]) ? (tmp2->digits[idx2] - '0') : 0;
            idx1--; idx2--;

            int subtraction = digit1 - digit2 - borrow;
            
            if (subtraction < 0) {
                subtraction += 10;
                borrow = 1;
            } else 
                borrow = 0;

            char resultDigit = (subtraction + '0');
            result[resultIdx] = resultDigit;
            resultIdx--;
        }

        if (idx1 < 0) { 
            num1Itr--;
            idx1 = DIGIT_SIZE - 1;
        } 

        if (idx2 < 0) {
            num2Itr--;
            idx2 = DIGIT_SIZE - 1;
        }
    }

    if (!number1->isNegative)
        result[0] = '+';
    else
        result[0] = '-';

    return result;
}

/*
@brief Compares the absolute value of both "BigInts". Called in "operate_bigInt" to know which number should go first in the subtraction.
@param number1 First number.
@param number2 Number that the first one will be compared to.
@return GREATER, EQUAL or LESS.
*/
int compare_abs_bigInt(BigInt_t* number1, BigInt_t* number2) {
    if (number1->len > number2->len) return GREATER;
    else if (number1->len < number2->len) return LESS;
    else {
        BigInt_t* tmp1 = malloc(sizeof(BigInt_t)); 
        BigInt_t* tmp2 = malloc(sizeof(BigInt_t));

        memcpy(tmp1, number1, sizeof(BigInt_t));
        memcpy(tmp2, number2, sizeof(BigInt_t));

        if (tmp1->isNegative) tmp1->isNegative = 0;
        if (tmp2->isNegative) tmp2->isNegative = 0;

        int condition = compare_bigInt(tmp1, tmp2);

        free(tmp1);
        free(tmp2);

        return condition;
    }
}

/*
@brief Does an operation (sum or subtraction) on two "BigInts" based on their sign (negative or positive), the result of which will be in the "result" parameter.
@param result Pointer to a pointer to a "BigInt" where the sum will be stored.
@param number1 Pointer to the first number.
@param number2 Pointer to the second number.
@return BIGINT_SUCCESS or BIGINT_ERROR.
*/
int operate_bigInt(BigInt_t** result, BigInt_t* number1, BigInt_t* number2) {
    if (number1 == NULL || number2 == NULL) return BIGINT_ERROR;

    char* operation_result = NULL;

    // If both numbers have the same sign, do a normal addition
    if ((number1->isNegative && number2->isNegative) || (!number1->isNegative && !number2->isNegative))
        operation_result = add_bigInt(number1, number2);
    else {
        // If they have different signs, compare to find which one is greater.
        int condition = compare_abs_bigInt(number1, number2);
        // Do a subtraction based on the condition (if |A| > |B| : A-B, else: B - A). Subtraction isn't needed if both are equal.
        switch (condition) {
            case GREATER:
                operation_result = subt_bigInt(number1, number2);
                break;
            case LESS:
                operation_result = subt_bigInt(number2, number1);
                break;
            case EQUAL:
                operation_result = calloc(2, sizeof(char));
                operation_result[0] = '0';
            }
    }

    define_new_bigInt(result, operation_result);

    free(operation_result);

    return BIGINT_SUCCESS;
}
