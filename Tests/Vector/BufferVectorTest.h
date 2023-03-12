#pragma once

#include "BaseTestTemplate.h"
#include "BufferVector.h"

typedef struct User {   // Some test custom type
    char *name;
    int age;
} User;

int userAgeComparator(User one, User two) {    // users will be sorted by 'age'
    if (one.age == two.age) {
        return 0;
    } else if (one.age > two.age) {
        return 1;
    }
    return -1;
}

CREATE_NUMBER_COMPARATOR(i, int);
CREATE_NUMBER_COMPARATOR(l, long);
CREATE_NUMBER_COMPARATOR(fl, float);
CREATE_NUMBER_COMPARATOR(i8, int8_t);
CREATE_NUMBER_COMPARATOR(u32, uint32_t);
CREATE_CHAR_COMPARATOR(char);
CREATE_STRING_COMPARATOR(cStr);
CREATE_CUSTOM_COMPARATOR(userName, User, one, two, strcmp(one.name, two.name));

CREATE_VECTOR_TYPE(uint32_t, u32, u32Comparator);
CREATE_VECTOR_TYPE(int8_t, i8, i8Comparator);
CREATE_VECTOR_TYPE(char*, cStr, cStrComparator);
CREATE_VECTOR_TYPE(float, flComparator);
CREATE_VECTOR_TYPE(char, charComparator);
CREATE_VECTOR_TYPE(int, iComparator);
CREATE_VECTOR_TYPE(User, user, userAgeComparator);


void assertIntVec(intVector *intVec, int size, int capacity) {
    assert_uint32(intVec->size, ==, size);
    assert_uint32(intVec->capacity, ==, capacity);
    assert_not_null(intVec->items);
}

static MunitResult testBuffVecCreation(const MunitParameter params[], void *data) {
    assertIntVec(NEW_VECTOR(int, 1), 0, 1);

    assertIntVec(NEW_VECTOR_4(int), 0, 4);
    assertIntVec(NEW_VECTOR_8(int), 0, 8);
    assertIntVec(NEW_VECTOR_16(int), 0, 16);
    assertIntVec(NEW_VECTOR_32(int), 0, 32);
    assertIntVec(NEW_VECTOR_64(int), 0, 64);
    assertIntVec(NEW_VECTOR_128(int), 0, 128);
    assertIntVec(NEW_VECTOR_256(int), 0, 256);
    assertIntVec(NEW_VECTOR_512(int), 0, 512);
    assertIntVec(NEW_VECTOR_1024(int), 0, 1024);

    assertIntVec(VECTOR(int), 0, 1);
    assertIntVec(VECTOR(int, 1), 1, 1);
    assertIntVec(VECTOR(int, 1, 2), 2, 2);
    assertIntVec(VECTOR(int, 1, 2, 3, 4, 5), 5, 5);
    assertIntVec(VECTOR(int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20), 20, 20);

    assertIntVec(NEW_VECTOR_OF(32, int, int), 0, 32);
    assertIntVec(NEW_VECTOR_OF(16, int, int, 1), 1, 16);

    u32Vector *u32Vec = NEW_VECTOR_OF(12, u32, uint32_t, 1, 2, 3, 4, 5);
    assert_uint32(u32Vec->size, ==, 5);
    assert_uint32(u32Vec->capacity, ==, 12);

    i8Vector *i8Vec = VECTOR_OF(i8, int8_t, 1, -2, -110, 4, 5);
    assert_uint32(i8Vec->size, ==, 5);
    assert_uint32(i8Vec->capacity, ==, 5);

    floatVector *flVec = VECTOR(float, 1.1, 2.2, 3.3, 4.456);
    assert_uint32(flVec->size, ==, 4);
    assert_uint32(flVec->capacity, ==, 4);

    charVector *chVec = NEW_VECTOR(char, 12);
    assert_uint32(chVec->size, ==, 0);
    assert_uint32(chVec->capacity, ==, 12);

    cStrVector *strVec = VECTOR_OF(cStr, char*, "aaa", "bbb", "ccc");
    assert_uint32(strVec->size, ==, 3);
    assert_uint32(strVec->capacity, ==, 3);

    userVector *uVec = VECTOR_OF(user, User, { .name = "name_1", .age = 23 }, { .name = "name_2", .age = 56 });
    assert_uint32(uVec->size, ==, 2);
    assert_uint32(uVec->capacity, ==, 4);
    return MUNIT_OK;
}

static MunitResult testBuffVecAdd(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_4(cStr, char*);
    assert_true(cStrVecAdd(strVec, "aaa"));
    assert_true(cStrVecAdd(strVec, "bbb"));
    assert_true(cStrVecAdd(strVec, "ccc"));
    assert_true(cStrVecAdd(strVec, "ddd"));

    assert_false(cStrVecAdd(strVec, "eee")); // vector is full, return false
    assert_uint32(strVec->size, ==, 4);
    assert_uint32(strVec->capacity, ==, 4);
    assert_false(cStrVecAdd(NULL, "fff"));  // null check
    return MUNIT_OK;
}

static MunitResult testBuffVecGet(const MunitParameter params[], void *data) {
    intVector *intVec = VECTOR(int, 1, 2, 3, 4);
    assert_int(intVecGet(intVec, 0), ==, 1);
    assert_int(intVecGet(intVec, 1), ==, 2);
    assert_int(intVecGet(intVec, 2), ==, 3);
    assert_int(intVecGet(intVec, 3), ==, 4);

    assert_int(intVecGet(intVec, 4), ==, 0);
    assert_int(intVecGetOrDefault(intVec, 2, 12345), ==, 3);
    assert_int(intVecGetOrDefault(intVec, 5, 12345), ==, 12345);
    return MUNIT_OK;
}

static MunitResult testBuffVecPut(const MunitParameter params[], void *data) {
    intVector *intVec = VECTOR(int, 1, 2, 3, 4);
    assert_uint32(intVecSize(intVec), ==, 4);
    assert_int(intVecGet(intVec, 0), ==, 1);
    assert_int(intVecGet(intVec, 1), ==, 2);
    assert_int(intVecGet(intVec, 2), ==, 3);
    assert_int(intVecGet(intVec, 3), ==, 4);

    assert_true(intVecPut(intVec, 1, 99));   // [1], [99], [3], [4]
    assert_uint32(intVecSize(intVec), ==, 4);
    assert_int(intVecGet(intVec, 0), ==, 1);
    assert_int(intVecGet(intVec, 1), ==, 99);
    assert_int(intVecGet(intVec, 2), ==, 3);
    assert_int(intVecGet(intVec, 3), ==, 4);

    assert_false(intVecPut(intVec, 6, -1));
    assert_false(intVecPut(NULL, 1, -1));
    return MUNIT_OK;
}

static MunitResult testBuffVecAddAt(const MunitParameter params[], void *data) {
    intVector *intVec = NEW_VECTOR_OF(8, int, int, 1, 2, 3, 4);
    assert_uint32(intVecSize(intVec), ==, 4);

    assert_true(intVecAddAt(intVec, 0, 21)); // [21], [1], [2], [3], [4]
    assert_uint32(intVecSize(intVec), ==, 5);
    assert_int(intVecGet(intVec, 0), ==, 21);
    assert_int(intVecGet(intVec, 1), ==, 1);
    assert_int(intVecGet(intVec, 2), ==, 2);
    assert_int(intVecGet(intVec, 3), ==, 3);
    assert_int(intVecGet(intVec, 4), ==, 4);

    assert_true(intVecAddAt(intVec, 5, 22)); // [21], [1], [2], [3], [4], [22]
    assert_uint32(intVecSize(intVec), ==, 6);
    assert_int(intVecGet(intVec, 0), ==, 21);
    assert_int(intVecGet(intVec, 1), ==, 1);
    assert_int(intVecGet(intVec, 2), ==, 2);
    assert_int(intVecGet(intVec, 3), ==, 3);
    assert_int(intVecGet(intVec, 4), ==, 4);
    assert_int(intVecGet(intVec, 5), ==, 22);

    assert_true(intVecAddAt(intVec, 2, 23)); // [21], [1], [23], [2], [3], [4], [22]
    assert_uint32(intVecSize(intVec), ==, 7);
    assert_int(intVecGet(intVec, 0), ==, 21);
    assert_int(intVecGet(intVec, 1), ==, 1);
    assert_int(intVecGet(intVec, 2), ==, 23);
    assert_int(intVecGet(intVec, 3), ==, 2);
    assert_int(intVecGet(intVec, 4), ==, 3);
    assert_int(intVecGet(intVec, 5), ==, 4);
    assert_int(intVecGet(intVec, 6), ==, 22);

    assert_false(intVecAddAt(intVec, 10, -1));  // out of bounds
    assert_true(intVecAddAt(intVec, 0, 24));    // now vector is full
    assert_false(intVecAddAt(intVec, 0, -1));   // can't add new value
    assert_false(intVecAddAt(NULL, 0, -1));     // null check
    return MUNIT_OK;
}

static MunitResult testBuffVecRemoveAt(const MunitParameter params[], void *data) {
    intVector *intVec = VECTOR(int, 1, 2, 3, 4, 5);
    assert_uint32(intVecSize(intVec), ==, 5);

    assert_int(intVecRemoveAt(intVec, 0), ==, 1); // [2], [3], [4], [5]
    assert_uint32(intVecSize(intVec), ==, 4);

    assert_int(intVecRemoveAt(intVec, 1), ==, 3); // [2], [4], [5]
    assert_uint32(intVecSize(intVec), ==, 3);

    assert_int(intVecRemoveAt(intVec, 2), ==, 5); // [2], [4]
    assert_uint32(intVecSize(intVec), ==, 2);

    assert_int(intVecRemoveAt(intVec, 6), ==, 0);  // out of bounds
    assert_int(intVecRemoveAt(NULL, 0), ==, 0);   // null check
    return MUNIT_OK;
}

static MunitResult testBuffVecEmptyFunctions(const MunitParameter params[], void *data) {
    charVector *cVec = VECTOR(char, 'a', 'b', 'c');
    assert_false(ischarVecEmpty(cVec));  // not empty
    assert_true(ischarVecNotEmpty(cVec));

    charVecClear(cVec);
    assert_true(ischarVecEmpty(cVec));  // empty
    assert_false(ischarVecNotEmpty(cVec));

    assert_true(ischarVecEmpty(NULL));
    assert_false(ischarVecNotEmpty(NULL));
    return MUNIT_OK;
}

static MunitResult testBuffVecSize(const MunitParameter params[], void *data) {
    charVector *cVec = VECTOR(char, 'a', 'b', 'c');
    assert_uint32(charVecSize(cVec), ==, 3);
    assert_uint32(charVecSize(NULL), ==, 0);

    charVecClear(cVec);
    assert_uint32(charVecSize(cVec), ==, 0);
    return MUNIT_OK;
}

static MunitResult testBuffVecAddAll(const MunitParameter params[], void *data) {
    charVector *cVec = NEW_VECTOR_OF(6, char, char, 'a', 'b', 'c');
    charVector *cVec2 = VECTOR(char, 'd', 'e', 'f');
    assert_true(charVecAddAll(cVec, cVec2)); // cVec -> [a], [b], [c], [d], [e], [f]
    assert_uint32(charVecSize(cVec), ==, 6);
    assert_char(charVecGet(cVec, 0), ==, 'a');
    assert_char(charVecGet(cVec, 1), ==, 'b');
    assert_char(charVecGet(cVec, 2), ==, 'c');
    assert_char(charVecGet(cVec, 3), ==, 'd');
    assert_char(charVecGet(cVec, 4), ==, 'e');
    assert_char(charVecGet(cVec, 5), ==, 'f');

    charVector *cVec3 = VECTOR(char, 'g');
    assert_false(charVecAddAll(cVec, cVec3));
    assert_uint32(charVecSize(cVec), ==, 6);

    assert_false(charVecAddAll(NULL, cVec3));
    assert_false(charVecAddAll(cVec, NULL));
    return MUNIT_OK;
}

static MunitResult testBuffVecFromArray(const MunitParameter params[], void *data) {
    int array[] = {1, 2, 3, 4, 5};
    intVector *intVec = intVecFromArray(NEW_VECTOR_8(int), array, 5);  // intVec -> [1], [2], [3], [4], [5]
    assert_not_null(intVec);
    assert_uint32(intVecSize(intVec), ==, 5);
    assert_int(intVecGet(intVec, 0), ==, 1);
    assert_int(intVecGet(intVec, 1), ==, 2);
    assert_int(intVecGet(intVec, 2), ==, 3);
    assert_int(intVecGet(intVec, 3), ==, 4);
    assert_int(intVecGet(intVec, 4), ==, 5);
    return MUNIT_OK;
}

static MunitResult testBuffVecIndexOf(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_4(cStr, char*);
    cStrVecAdd(strVec, "aaa");
    cStrVecAdd(strVec, "bbb");
    cStrVecAdd(strVec, "ccc");
    cStrVecAdd(strVec, "ddd");
    assert_uint32(cStrVecSize(strVec), ==, 4);

    assert_int32(cStrVecIndexOf(strVec, "aaa"), ==, 0);
    assert_int32(cStrVecIndexOf(strVec, "bbb"), ==, 1);
    assert_int32(cStrVecIndexOf(strVec, "eee"), ==, -1); // not found
    assert_int32(cStrVecIndexOf(NULL, "eee"), ==, -1); // not found
    return MUNIT_OK;
}

static MunitResult testBuffVecContains(const MunitParameter params[], void *data) {
    cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "d");
    assert_true(cStrVecContains(strVec, "c"));
    assert_false(cStrVecContains(strVec, "e"));
    return MUNIT_OK;
}

static MunitResult testBuffVecReverse(const MunitParameter params[], void *data) {
    cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "d");
    assert_string_equal(cStrVecGet(strVec, 0), "a");
    assert_string_equal(cStrVecGet(strVec, 1), "b");
    assert_string_equal(cStrVecGet(strVec, 2), "c");
    assert_string_equal(cStrVecGet(strVec, 3), "d");

    cStrVecReverse(strVec); // [d], [c], [b], [a]
    assert_string_equal(cStrVecGet(strVec, 0), "d");
    assert_string_equal(cStrVecGet(strVec, 1), "c");
    assert_string_equal(cStrVecGet(strVec, 2), "b");
    assert_string_equal(cStrVecGet(strVec, 3), "a");
    return MUNIT_OK;
}

static MunitResult testBuffVecSort(const MunitParameter params[], void *data) {
    cStrVector *strVec = VECTOR_OF(cStr, char*, "b", "a", "d", "c");
    assert_string_equal(cStrVecGet(strVec, 0), "b");
    assert_string_equal(cStrVecGet(strVec, 1), "a");
    assert_string_equal(cStrVecGet(strVec, 2), "d");
    assert_string_equal(cStrVecGet(strVec, 3), "c");

    cStrVecSort(strVec); // [a], [b], [c], [d]
    assert_string_equal(cStrVecGet(strVec, 0), "a");
    assert_string_equal(cStrVecGet(strVec, 1), "b");
    assert_string_equal(cStrVecGet(strVec, 2), "c");
    assert_string_equal(cStrVecGet(strVec, 3), "d");
    return MUNIT_OK;
}

static MunitResult testBuffVecEquals(const MunitParameter params[], void *data) {
    charVector *cVec = VECTOR(char, 'a', 'b', 'c');
    charVector *cVec2 = VECTOR(char, 'a', 'b', 'c');
    assert_true(ischarVecEquals(cVec, cVec2));
    assert_true(ischarVecEquals(cVec, cVec));

    assert_false(ischarVecEquals(NULL, cVec2));
    assert_false(ischarVecEquals(cVec, NULL));

    charVector *cVec3 = VECTOR(char, 'a', 'b');
    assert_false(ischarVecEquals(cVec, cVec3));

    charVector *cVec4 = VECTOR(char, 'a', 'b', 'd');
    assert_false(ischarVecEquals(cVec, cVec4));
    return MUNIT_OK;
}

static MunitResult testBuffVecRemoveDup(const MunitParameter params[], void *data) {
    cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "a", "a", "a", "c");
    cStrVecRemoveDup(strVec); // [a], [b], [c]
    assert_uint32(cStrVecSize(strVec), ==, 3);
    assert_string_equal(cStrVecGet(strVec, 0), "a");
    assert_string_equal(cStrVecGet(strVec, 1), "b");
    assert_string_equal(cStrVecGet(strVec, 2), "c");
    return MUNIT_OK;
}

static MunitResult testBuffVecUnion(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
    cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
    cStrVecUnion(strVec, strVec2);  // strVec -> [1], [2], [3], [4], [5], [6], [7], [8], [9]
    assert_uint32(cStrVecSize(strVec), ==, 9);
    assert_string_equal(cStrVecGet(strVec, 0), "1");
    assert_string_equal(cStrVecGet(strVec, 1), "2");
    assert_string_equal(cStrVecGet(strVec, 2), "3");
    assert_string_equal(cStrVecGet(strVec, 3), "4");
    assert_string_equal(cStrVecGet(strVec, 4), "5");
    assert_string_equal(cStrVecGet(strVec, 5), "6");
    assert_string_equal(cStrVecGet(strVec, 6), "7");
    assert_string_equal(cStrVecGet(strVec, 7), "8");
    assert_string_equal(cStrVecGet(strVec, 8), "9");

    cStrVector *strVec3 = cStrVecUnion(NEW_VECTOR_16(cStr, char*), strVec2);
    assert_uint32(cStrVecSize(strVec3), ==, 6);
    assert_string_equal(cStrVecGet(strVec3, 0), "2");
    assert_string_equal(cStrVecGet(strVec3, 1), "3");
    assert_string_equal(cStrVecGet(strVec3, 2), "6");
    assert_string_equal(cStrVecGet(strVec3, 3), "7");
    assert_string_equal(cStrVecGet(strVec3, 4), "8");
    assert_string_equal(cStrVecGet(strVec3, 5), "9");

    assert_null(cStrVecUnion(NULL, strVec2));
    assert_null(cStrVecUnion(NEW_VECTOR_16(cStr, char*), NULL));
    return MUNIT_OK;
}

static MunitResult testBuffVecIntersect(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "A", "B", "C", "D", "E", "F");
    cStrVector *strVec2 = VECTOR_OF(cStr, char*, "B", "D", "F", "G", "H", "K");
    cStrVecIntersect(strVec, strVec2);  // strVec -> [B], [D], [F]
    assert_uint32(cStrVecSize(strVec), ==, 3);
    assert_uint32(strVec->capacity, ==, 12);
    assert_string_equal(cStrVecGet(strVec, 0), "B");
    assert_string_equal(cStrVecGet(strVec, 1), "D");
    assert_string_equal(cStrVecGet(strVec, 2), "F");
    return MUNIT_OK;
}

static MunitResult testBuffVecSubtract(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
    cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
    cStrVecSubtract(strVec, strVec2);  // strVec -> [1], [4], [5]
    assert_uint32(cStrVecSize(strVec), ==, 3);
    assert_string_equal(cStrVecGet(strVec, 0), "1");
    assert_string_equal(cStrVecGet(strVec, 1), "4");
    assert_string_equal(cStrVecGet(strVec, 2), "5");
    return MUNIT_OK;
}

static MunitResult testBuffVecDisjunction(const MunitParameter params[], void *data) {
    cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
    cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
    cStrVecDisjunction(strVec, strVec2);  // strVec -> [1], [4], [5], [7], [8], [9]
    assert_uint32(cStrVecSize(strVec), ==, 6);
    assert_string_equal(cStrVecGet(strVec, 0), "1");
    assert_string_equal(cStrVecGet(strVec, 1), "4");
    assert_string_equal(cStrVecGet(strVec, 2), "5");
    assert_string_equal(cStrVecGet(strVec, 3), "7");
    assert_string_equal(cStrVecGet(strVec, 4), "8");
    assert_string_equal(cStrVecGet(strVec, 5), "9");
    return MUNIT_OK;
}


static MunitTest bufferVectorTests[] = {
        {.name =  "Test new Vector - should correctly create and init vector", .test = testBuffVecCreation},
        {.name =  "Test new <type>VecAdd() - should correctly add elements to vector", .test = testBuffVecAdd},
        {.name =  "Test new <type>VecGet() - should correctly return elements from vector", .test = testBuffVecGet},
        {.name =  "Test new <type>VecPut() - should correctly replace element at index", .test = testBuffVecPut},
        {.name =  "Test new <type>VecAddAt() - should correctly shift vector with new element at index", .test = testBuffVecAddAt},
        {.name =  "Test new <type>VecRemoveAt() - should correctly shift vector removing element at index", .test = testBuffVecRemoveAt},
        {.name =  "Test new is<type>VecEmpty/NotEmpty() - should correctly check vector emptiness", .test = testBuffVecEmptyFunctions},
        {.name =  "Test new <type>VecSize() - should correctly return elements count", .test = testBuffVecSize},
        {.name =  "Test new <type>VecAddAll() - should correctly merge two vectors", .test = testBuffVecAddAll},
        {.name =  "Test new <type>VecFromArray() - should correctly copy array to vector", .test = testBuffVecFromArray},
        {.name =  "Test new <type>VecIndexOf() - should correctly find element index", .test = testBuffVecIndexOf},
        {.name =  "Test new <type>VecContains() - should correctly check element existence", .test = testBuffVecContains},
        {.name =  "Test new <type>VecReverse() - should correctly reverse vector", .test = testBuffVecReverse},
        {.name =  "Test new <type>VecSort() - should correctly order vector elements", .test = testBuffVecSort},
        {.name =  "Test new is<type>VecEquals() - should correctly check vector equality", .test = testBuffVecEquals},
        {.name =  "Test new <type>VecRemoveDup() - should correctly remove repeated values", .test = testBuffVecRemoveDup},
        {.name =  "Test new <type>VecUnion() - should correctly union two vectors", .test = testBuffVecUnion},
        {.name =  "Test new <type>VecIntersect() - should correctly intersect two vectors", .test = testBuffVecIntersect},
        {.name =  "Test new <type>VecSubtract() - should correctly subtract two vectors", .test = testBuffVecSubtract},
        {.name =  "Test new <type>VecDisjunction() - should correctly make disjunction of two vectors", .test = testBuffVecDisjunction},

        END_OF_TESTS
};

static const MunitSuite bufferVectorTestSuite = {
        .prefix = "BufferVector: ",
        .tests = bufferVectorTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};