#pragma once

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#define COMPARATOR_FOR_TYPE(TYPE) TYPE ## Comparator
#define HASH_CODE_FOR_TYPE(TYPE)  TYPE ## HashCode

#define LL_HASH_CODE(value) (int)((value) ^ ((value) >> 32))

#define CREATE_NUMBER_COMPARATOR(NAME, TYPE) \
static inline int NAME ##Comparator(TYPE one, TYPE two) { \
    return (((one) < (two)) ? (-1) : (((one) == (two)) ? 0 : 1));             \
} \

#define CREATE_NUMBER_HASH_CODE(NAME, TYPE) \
static inline int NAME ##HashCode(TYPE value) { \
    return (int) value;             \
} \

#define CREATE_CUSTOM_COMPARATOR(NAME, TYPE, PARAM_NAME_1, PARAM_NAME_2, EXPR) \
static inline int NAME ##Comparator(TYPE PARAM_NAME_1, TYPE PARAM_NAME_2) { \
    return (EXPR);             \
}

CREATE_NUMBER_COMPARATOR(int, int);                 // intComparator()
CREATE_NUMBER_COMPARATOR(long, long);               // longComparator()
CREATE_NUMBER_COMPARATOR(char, char);               // charComparator()
CREATE_NUMBER_COMPARATOR(int8_t, int8_t);           // int8_tComparator()
CREATE_NUMBER_COMPARATOR(uint8_t, uint8_t);         // uint8_tComparator()
CREATE_NUMBER_COMPARATOR(int16_t, int16_t);         // int16_tComparator()
CREATE_NUMBER_COMPARATOR(uint16_t, uint16_t);       // uint16_tComparator()
CREATE_NUMBER_COMPARATOR(int32_t, int32_t);         // int32_tComparator()
CREATE_NUMBER_COMPARATOR(uint32_t, uint32_t);       // uint32_tComparator()
CREATE_NUMBER_COMPARATOR(int64_t, int64_t);         // int64_tComparator()
CREATE_NUMBER_COMPARATOR(uint64_t, uint64_t);       // uint64_tComparator()

CREATE_NUMBER_HASH_CODE(int, int);                 // intHashCode()
CREATE_NUMBER_HASH_CODE(long, long);               // longHashCode()
CREATE_NUMBER_HASH_CODE(char, char);               // charHashCode()
CREATE_NUMBER_HASH_CODE(float, float);             // floatHashCode()
CREATE_NUMBER_HASH_CODE(int8_t, int8_t);           // int8_tHashCode()
CREATE_NUMBER_HASH_CODE(uint8_t, uint8_t);         // uint8_tHashCode()
CREATE_NUMBER_HASH_CODE(int16_t, int16_t);         // int16_tHashCode()
CREATE_NUMBER_HASH_CODE(uint16_t, uint16_t);       // uint16_tHashCode()
CREATE_NUMBER_HASH_CODE(int32_t, int32_t);         // int32_tHashCode()
CREATE_NUMBER_HASH_CODE(uint32_t, uint32_t);       // uint32_tHashCode()


static inline int charIgnoreCaseComparator(char one, char two) {
    return (int)((((char)tolower(one)) < ((char)tolower(two))) ? (-1) : ((((char)tolower(one)) == ((char)tolower(two))) ? 0 : 1));
}

static inline int strComparator(const char *one, const char *two) {
    return strcmp(one, two);
}

static inline int int64_tHashCode(int64_t value) {
    return LL_HASH_CODE(value);
}

static inline int uint64_tHashCode(uint64_t value) {
    return LL_HASH_CODE(value);
}

static inline int doubleHashCode(double value) {
    int64_t bits = (int64_t) value;
    return LL_HASH_CODE(bits);
}

int doubleComparator(double one, double two);
int floatComparator(float one, float two);
int strNaturalSortComparator(const char *one, const char *two);