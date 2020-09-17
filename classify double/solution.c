#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>



/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    return 0;
}


/**
 * Checkers here:
 */

bool checkForDenormal (uint64_t number) {
    return (number & 0x7FF0000000000001) == 0x1;
}

bool checkForNormal (uint64_t number) {
    uint64_t exponent = number & 0x7FF0000000000000;
    return (exponent != 0x0) && (exponent != 0x7FF0000000000000);
}

bool checkForPositive (uint64_t number) {
    return (number & 0x8000000000000000) == 0x0;
}



bool checkForPlusZero (uint64_t number) {
    return number == 0x0000000000000000;
}

bool checkForMinusZero (uint64_t number) {
    return number == 0x8000000000000000;
}

bool checkForPlusInf (uint64_t number) {
    return number == 0x7FF0000000000000;
}

bool checkForMinusInf (uint64_t number) {
    return number == 0xFFF0000000000000;
}

bool checkForPlusNormal (uint64_t number) {
    return checkForNormal(number) && checkForPositive(number);
}

bool checkForMinusNormal (uint64_t number) {
    return checkForNormal(number) && !checkForPositive(number);
}

bool checkForPlusDenormal (uint64_t number) {
    return checkForDenormal(number) && checkForPositive(number);
}

bool checkForMinusDenormal (uint64_t number) {
    return checkForDenormal(number) && !checkForPositive(number);
}

bool checkForSignalingNan (uint64_t number) {
    return (number & 0x7FF8000000000001) == 0x7FF0000000000001;
}

bool checkForQuietNan (uint64_t number) {
    return (number & 0x7FF8000000000000) == 0x7FF8000000000000;
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus regular\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus regular\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}

int main(int argc, char* argv[])
{
    classify(strtod(argv[1], NULL));
    return 0;
}
