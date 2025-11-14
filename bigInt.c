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
    bigInt->isNegative = 1;

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
@brief Assign a new number to an already existent BigInt.
@param bigInt The address to the BigInt you want to define.
@param digits The string containing the desired number.
@return BIGINT_ERROR or BIGINT_SUCCESS
*/
int define_new_bigInt(BigInt_t** bigInt, char* digits) {
    if (!digits) return BIGINT_ERROR;
    
    BigInt_t* tmp = create_bigInt();
    *(bigInt) = tmp;

    if (digits[0] == '-') 
        tmp->isNegative = -1;

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
@brief Turns a "BigInt" into a string.
@param bigInt "BigInt" which will be turned into a string.
@return A string containing the value of "BigInt"
*/
char* to_string(BigInt_t* bigInt) { 
    /*
    This is a private function, so it can only be called in this file.
    We only call it when we are sure none of the "BigInts" are NULL.
    */

    char* number = calloc(bigInt->len + 1, sizeof(char));

    BigInt_t* temp = bigInt;
    int aux = 0;
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

/*
@brief Prints a bigInt type on the console
@param bigInt The number you want to print
@return BIGINT_ERROR or BIGINT_SUCCESS
*/
int print_bigInt(BigInt_t* bigInt) {
    if (!bigInt) return BIGINT_ERROR;

    if (bigInt->isNegative == -1) fprintf(stdout, "-");

    char* str = to_string(bigInt);
    fprintf(stdout, "%s\n", str);

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

    if ((tempNum1 != NULL && number1->isNegative == -1) || ((number2->len > number1->len) && number2->isNegative == 1)) 
        result = LESS;
     else if ((tempNum2 != NULL && number2->isNegative == -1) || ((number1->len > number2->len) && number1->isNegative == 1)) 
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

    // Since "BigInt" is a linked list and starts from the most significant digit, turning them into strings beforehand sounded like the best approach. 
    char* temp1 = to_string(number1);
    char* temp2 = to_string(number2);
    int num1Idx = number1->len - 1;
    int num2Idx = number2->len - 1;

    int carry = 0;

    while (resultIdx > 0) {
        int digit1 = (num1Idx >= 0) ? (temp1[num1Idx] - '0') : 0;
        int digit2 = (num2Idx >= 0) ? (temp2[num2Idx] - '0') : 0;
        num1Idx--; num2Idx--;

        int sum = digit1 + digit2 + carry;
        carry = sum / 10;

        char resultDigit = ((sum % 10) + '0');
        result[resultIdx] = resultDigit;

        resultIdx--;
    }

    if (number1->isNegative == 1)
        result[0] = '+';
    else
        result[0] = '-';

    free(temp1);
    free(temp2);

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

    // Since "BigInt" is a linked list and starts from the most significant digit, turning them into strings beforehand sounded like the best approach. 
    char* temp1 = to_string(number1);
    char* temp2 = to_string(number2);
    int num1Idx = number1->len - 1;
    int num2Idx = number2->len - 1;

    int borrow = 0;
    
    while (resultIdx > 0) { 
        int digit1 = (num1Idx >= 0) ? (temp1[num1Idx] - '0') : 0;
        int digit2 = (num2Idx >= 0) ? (temp2[num2Idx] - '0') : 0;
        num1Idx--; num2Idx--;

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

    if (number1->isNegative == 1)
        result[0] = '+';
    else
        result[0] = '-';

    free(temp1);
    free(temp2);

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
        char* num1 = to_string(number1), *num2 = to_string(number2);
        int condition = strcmp(num1, num2);
        free(num1); 
        free(num2);
        
        if (condition > 0)
            return GREATER;
        else if (condition < 0)
            return LESS;
        else
            return EQUAL;

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

    char* operation_result;

    // If both numbers have the same sign, do a normal addition
    if ((number1->isNegative == -1 && number2->isNegative == -1) || (number1->isNegative == 1 && number2->isNegative == 1))
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
