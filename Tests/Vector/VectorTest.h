#pragma once

#include "BaseTestTemplate.h"
#include "Vector.h"

#define VECTOR_INITIAL_CAPACITY 8

static Vector someVector = NULL;

static void *vectorSetup(const MunitParameter params[], void *userData) {
    Vector vector = getVectorInstance(VECTOR_INITIAL_CAPACITY);
    assert_not_null(vector);
    return vector;
}

static MunitResult testVectorAddElements(const MunitParameter params[], void *vector) {
    (Vector) vector;
    for (int i = 0; i < 10000; i++) {
        vectorAdd(vector, (ItemType) i);
        assert_int(getVectorSize(vector), ==, i + 1);
        assert_int((int) vectorGet(vector, i), ==, i);
    }
    assert_int(getVectorSize(vector), ==, 10000);
    return MUNIT_OK;
}

static MunitResult testVectorPutElements(const MunitParameter params[], void *vector) {
    (Vector) vector;
    vectorAdd(vector, (ItemType) 12);
    vectorAdd(vector, (ItemType) 13);
    vectorAdd(vector, (ItemType) 14);

    vectorPut(vector, 1, (ItemType) 456);
    assert_int(getVectorSize(vector), ==, 3);
    assert_int((int) vectorGet(vector, 1), ==, 456);
    return MUNIT_OK;
}

static MunitResult testVectorAddAtElement(const MunitParameter params[], void *vector) {
    (Vector) vector;
    vectorAdd(vector, (ItemType) 12);
    vectorAdd(vector, (ItemType) 13);
    vectorAdd(vector, (ItemType) 14);
    assert_int(getVectorSize(vector), ==, 3);

    vectorAddAt(vector, 2, (ItemType) 56);  // insert element at 2nd position
    assert_int(getVectorSize(vector), ==, 4);   // size should be increased
    assert_int((int) vectorGet(vector, 2), ==, 56); // check for new value at index
    assert_int((int) vectorGet(vector, 3), ==, 14); // previous element should be shifted right

    vectorAddAt(vector, 0, (ItemType) 33);  // insert element at 0 position
    assert_int(getVectorSize(vector), ==, 5);   // size should be increased
    assert_int((int) vectorGet(vector, 0), ==, 33); // check for new value at index

    // all elements in vector should be shifted right
    assert_int((int) vectorGet(vector, 0), ==, 33);
    assert_int((int) vectorGet(vector, 1), ==, 12);
    assert_int((int) vectorGet(vector, 2), ==, 13);
    assert_int((int) vectorGet(vector, 3), ==, 56);
    assert_int((int) vectorGet(vector, 4), ==, 14);
    return MUNIT_OK;
}

static MunitResult testVectorRemoveAtElement(const MunitParameter params[], void *vector) {
    (Vector) vector;
    vectorAdd(vector, (ItemType) 1);    // index: 0
    vectorAdd(vector, (ItemType) 2);    // index: 1
    vectorAdd(vector, (ItemType) 3);    // index: 2
    vectorAdd(vector, (ItemType) 4);    // index: 3
    vectorAdd(vector, (ItemType) 5);    // index: 3
    assert_int(getVectorSize(vector), ==, 5);

    assert_int((int) vectorRemoveAt(vector, 2), ==, 3); // should return value stored at index
    assert_int(getVectorSize(vector), ==, 4);
    assert_int((int) vectorGet(vector, 2), ==, 4);  // vector should shrink to left, so element from index 3 should be at index 2

    assert_int((int) vectorRemoveAt(vector, 0), ==, 1); // should return value stored at index
    assert_int(getVectorSize(vector), ==, 3);

    // all elements should be shifted left
    assert_int((int) vectorGet(vector, 0), ==, 2);
    assert_int((int) vectorGet(vector, 1), ==, 4);
    assert_int((int) vectorGet(vector, 2), ==, 5);
    return MUNIT_OK;
}

static MunitResult testVectorRemoveElements(const MunitParameter params[], void *vector) {
    (Vector) vector;
    for (int i = 0; i < 10000; i++) {
        vectorAdd(vector, (ItemType) i);
    }
    assert_int(getVectorSize(vector), ==, 10000);

    while (isVectorNotEmpty(vector)) {
        vectorRemoveAt(vector, getVectorSize(vector) - 1);
    }
    assert_true(isVectorEmpty(vector));
    assert_int(getVectorSize(vector), ==, 0);
    return MUNIT_OK;
}

static MunitResult testVectorEmptyChecks(const MunitParameter params[], void *vector) {
    (Vector) vector;
    assert_true(isVectorEmpty(vector));
    assert_false(isVectorNotEmpty(vector));

    vectorAdd(vector, (ItemType) 1);
    assert_false(isVectorEmpty(vector));
    assert_true(isVectorNotEmpty(vector));
    return MUNIT_OK;
}

static MunitResult testVectorClear(const MunitParameter params[], void *vector) {
    (Vector) vector;
    vectorAdd(vector, (ItemType) 1);
    vectorAdd(vector, (ItemType) 2);
    vectorAdd(vector, (ItemType) 3);
    assert_int(getVectorSize(vector), ==, 3);

    vectorClear(vector);
    assert_true(isVectorEmpty(vector));
    return MUNIT_OK;
}

static MunitResult testVectorSingleton(const MunitParameter params[], void *vector) {
    assert_null(someVector);
    initSingletonVector(&someVector, 1);
    assert_not_null(someVector);
    vectorAdd(someVector, (ItemType) 12);
    vectorAdd(someVector, (ItemType) 13);
    assert_true(isVectorNotEmpty(someVector));

    initSingletonVector(&someVector, 1);
    vectorAdd(someVector, (ItemType) 14);
    assert_int(getVectorSize(someVector), ==, 3);
    vectorDelete(someVector);
    someVector = NULL;
    return MUNIT_OK;
}

static void vectorTearDown(void *vector) {
    vectorDelete(vector);
    vector = NULL;
    munit_assert_ptr_null(vector);
}

static MunitTest vectorTests[] = {
        {
                .name =  "Test vectorAdd() - should correctly hold elements",
                .test = testVectorAddElements,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test vectorPut() - should correctly set element by index",
                .test = testVectorPutElements,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test vectorAddAt() - should correctly add element at index",
                .test = testVectorAddAtElement,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test vectorRemoveAt() - should correctly remove element at index",
                .test = testVectorRemoveAtElement,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test vectorRemove() - should correctly remove elements",
                .test = testVectorRemoveElements,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test isVectorEmpty() and not empty - should check vector emptiness",
                .test = testVectorEmptyChecks,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test isVectorClear() - should clear vector",
                .test = testVectorClear,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        {
                .name =  "Test initSingletonVector() - should create vector once",
                .test = testVectorSingleton,
                .setup = vectorSetup,
                .tear_down = vectorTearDown
        },
        END_OF_TESTS
};

static const MunitSuite vectorTestSuite = {
        .prefix = "Vector: ",
        .tests = vectorTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};