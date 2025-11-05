#ifndef BIGINT_H
#define BIGINT_H

#define DIGIT_SIZE 4
#define EQUAL 0
#define GREATER 1
#define LESS 2

typedef enum ReturnTypes {
    BIGINT_SUCCESS,
    BIGINT_ERROR,
};

typedef struct BigInt BigInt_t;

BigInt_t* create();
int define(BigInt_t** bigInt, char* digits);
void destroy(BigInt_t** bigInt);

int print_bigInt(BigInt_t* bigInt);
char* to_string(BigInt_t* BigInt);

int compare_bigInt(BigInt_t* number1, BigInt_t* number2);
int add_bigInts(BigInt_t* result, BigInt_t* number1, BigInt_t* number2);
char* make_sum(BigInt_t* number1, BigInt_t* number2);

#endif
