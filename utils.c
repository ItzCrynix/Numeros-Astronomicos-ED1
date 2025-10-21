#include "utils.h"

int getLength(char *string) {
    if (!string) return -1;

    int length = 0;
    while (string[length] != '\0') length++;

    return length;
}
