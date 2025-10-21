#ifndef BIGINT_H
#define BIGINT_H

#define BIGINT_ERROR -1
#define DIGIT_SIZE 4

typedef struct BigInt BigInt_t;

BigInt_t* create();
BigInt_t* define(char* digits);

int print(BigInt_t* bigInt);

void destroy(BigInt_t** bigInt);

#endif
