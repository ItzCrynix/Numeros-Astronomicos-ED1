#ifndef BIGINT_H
#define BIGINT_H

#define BIGINT_SUCCESS 0
#define BIGINT_ERROR -1

#define DIGIT_SIZE 4
#define EQUAL 0
#define GREATER 1
#define LESS 2

typedef struct BigInt BigInt_t;

BigInt_t* create_bigInt();
void destroy_bigInt(BigInt_t** bigInt);
int define_new_bigInt(BigInt_t** bigInt, char* digits);

int print_bigInt(BigInt_t* bigInt);

int compare_bigInt(BigInt_t* number1, BigInt_t* number2);
int add_bigInts(BigInt_t** result, BigInt_t* number1, BigInt_t* number2);

#endif
