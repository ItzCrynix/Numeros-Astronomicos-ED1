#ifndef BIGINT_H
#define BIGINT_H

#define BIGINT_ERROR -1
#define DIGIT_SIZE 4
#define EQUAL 1
#define GREATER 1
#define LESS 1

typedef struct BigInt BigInt_t;

BigInt_t* create();
BigInt_t* define(char* digits);

// int equal(BigInt_t* bigIntThis, BigInt_t* bigIntThat);
// int greater(BigInt_t* bigIntThis, BigInt_t* bigIntThat);
// int less(BigInt_t* bigIntThis, BigInt_t* bigIntThat);

int compare_bigint(BigInt_t* number1, BigInt_t* number2);

int print_bigInt(BigInt_t* bigInt);

void destroy(BigInt_t** bigInt);

#endif
