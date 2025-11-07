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
    int idx = 0, aux = 0;

    while (digits[aux] == '0' || digits[aux] == 0 || digits[aux] == '+' || digits[aux] == '-') { 
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
    Como essa função não foi definida no header, se trata de uma função privada. 
    Só chamamos ela quando temos certeza que nenhum dos bigint é NULL
    */

    char* number = calloc(bigInt->len, sizeof(char));

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
@brief Given two "BigInts", turns them into strings, adds each digit and places that in a new string.
@param number1 The first number in the operation.
@param number2 The second number in the operation.
@return String which contains the sum.
*/
char* make_sum(BigInt_t* number1, BigInt_t* number2) {
    /*
    Como essa função não foi definida no header, se trata de uma função privada. 
    Só chamamos ela quando temos certeza que nenhum dos bigint é NULL
    */

    int max_size = (number1->len > number2->len) ? number1->len : number2->len;
    max_size += 3;

    // String containing the sum
    char* result = calloc(max_size, sizeof(char));
    int resultIndex = max_size - 2;
    
    // Temporary strings to do the calculations
    char* temp1 = to_string(number1);
    char* temp2 = to_string(number2);
    int indexNum1 = number1->len - 1;
    int indexNum2 = number2->len - 1;

    int carry = 0, sign = 0; // 0 = positive, 1 = negative  

    for (; resultIndex > 1; resultIndex--) {
        int digit1 = (indexNum1 >= 0) ? (temp1[indexNum1] - '0') : 0;
        int digit2 = (indexNum2 >= 0) ? (temp2[indexNum2] - '0') : 0;
        indexNum1--; 
        indexNum2--;
        int sum = digit1 * number1->isNegative + digit2 * number2->isNegative + carry;
        if (number1->isNegative == -1 && number2->isNegative == -1) {
            sign = 1;
            if (sum <= -10) 
                carry = -1;
            else 
                carry = 0;
        } else if (number1->isNegative == 1 && number2->isNegative == 1) {
            sign = 0;
            if (sum >= 10) 
                carry = 1;
            else 
                carry = 0;
        } else {
            if (sum > 0) {
                sign = 1;
                carry = 1;
            } else if (sum < 0) {
                carry = -1;
                sign = 0;
            } else
                carry = 0;
        }
        
        int result_digit = (sum % 10 + 10) % 10;
        result[resultIndex] = result_digit + '0';
    }

    if (sign)
        result[0] = '-';
    else
        result[0] = '+';

    result[1] = carry + '0';

    free(temp1);
    free(temp2);

    return result;
}

/*
@brief Adds two "BigInts" together, the result of which will be in the "result" parameter.
@param result Pointer to a pointer to a "BigInt" where the sum will be stored.
@param number1 Pointer to the first number.
@param number2 Pointer to the second number.
@return BIGINT_SUCCESS or BIGINT_ERROR.
*/
int add_bigInts(BigInt_t** result, BigInt_t* number1, BigInt_t* number2) {
    if (number1 == NULL || number2 == NULL) return BIGINT_ERROR;

    char* addition_result = make_sum(number1, number2);

    define_new_bigInt(result, addition_result);

    free(addition_result);

    return BIGINT_SUCCESS;
}