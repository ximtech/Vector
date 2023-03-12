# Vector

[![tests](https://github.com/ximtech/Vector/actions/workflows/cmake-ci.yml/badge.svg)](https://github.com/ximtech/Vector/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/Vector/branch/main/graph/badge.svg?token=YKK6M0TWGO)](https://codecov.io/gh/ximtech/Vector)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/99e9fb7e754f4ffa95ae89b02eb75b08)](https://www.codacy.com/gh/ximtech/Vector/dashboard)

**STM32** lightweight implementation of Vector array.\
Vectors are sequence containers representing arrays that can change in size.

## Difference between vector and array

An array follows static approach, means its size cannot be changed during run time.
While vector implements dynamic array, means it automatically resizes itself when appending elements.

### Features

- Specifically designed for embedded applications
- Fixed or auto array size reduction/enlargement
- Easy to use and add to the project
- Performance optimized
- Heapless version
- Low memory consumption

### Trade-offs

- Mostly static Vector data type, can't be changed at runtime
- Not tested in multi-threaded applications

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME Vector
        GITHUB_REPOSITORY ximtech/Vector
        GIT_TAG origin/main)

target_link_libraries(${PROJECT_NAME} Vector)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf Vector)
```

### Stack version

Fixed size of generic type `Vector`. This implementation not allocate heap memory, but need compile time known capacity

#### Single header include

```c
#include "BufferVector.h"
```

#### Setup vector element compare function

- On the top level provide function to compare Vector elements, this function will be used for elements ordering.
- Default type macros can be used.
- Example:

```c
CREATE_NUMBER_COMPARATOR(i, int);   // provide comparator prefix name and type. This creates iComparator() function
CREATE_NUMBER_COMPARATOR(long, long);       // longComparator(long one, long two)
CREATE_NUMBER_COMPARATOR(u8, uint8_t);      // u8Comparator(uint8_t one, uint8_t two)
CREATE_NUMBER_COMPARATOR(u32, uint32_t);    // u32Comparator(uint32_t one, uint32_t two)
CREATE_CHAR_COMPARATOR(char);               // charComparator(char one, char two)
CREATE_STRING_COMPARATOR(cStr);             // cStrComparator(const char *one, const char *two)
```

##### Custom comparator creation

For custom types provide comparator function as in example:

```c
typedef struct User {   // Some custom type
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

int userNameComparator(User one, User two) {    // users will be sorted by 'name'
    return strcmp(one.name, two.name);
}
```

For inline function generation `CREATE_CUSTOM_COMPARATOR` macro can be used \
Provide:

1. Comparator function prefix name, this creates function like: <prefix>Comparator()
2. Your type
3. First and second function parameter name
4. Comparator logic

```c
CREATE_CUSTOM_COMPARATOR(userName, User, one, two, strcmp(one.name, two.name)); // creates `int userNameComparator(User one, User two)`
```

#### Define Vector type

Provide: 
1. Vector elements stored type
2. Optionally: Vector alias, if not provided then type will be used as prefix name
3. Elements comparator function pointer
Example:
```c
CREATE_VECTOR_TYPE(int, iComparator);   // creates `intVector` type that holds int elements
CREATE_VECTOR_TYPE(long, longComparator);   // `longVector` type
CREATE_VECTOR_TYPE(char, charComparator);   // `charVector` type
CREATE_VECTOR_TYPE(User, user, userAgeComparator); // alias will be used for type prefix: userVector
CREATE_VECTOR_TYPE(char*, cStr, cStrComparator); // `cStrVector`
```

#### Method and type naming convention

1. When only type provided, then methods will be generated like: `<type>Vec...()` and `is<type>Vec...()`
2. When type alias(name) provided, then methods will be generated like: `<name>Vec...()` and `is<name>Vec...()`
3. Same for Vector typedef naming


#### Base vector creation
When comparator and all needed types provided, then `Vector` can be instantiated
```c
intVector *intVec1 = NEW_VECTOR(int, 4); // empty vector with max length of 4
intVector *intVec2 = NEW_VECTOR_8(int); // shorter version from predefined macro
intVector *intVec3 = NEW_VECTOR_128(int); // bigger, check available defines in `BufferVector.h'

intVector *intVec4 = VECTOR(int, 1, 2, 3, 4, 5); // auto size resolving for 5 elements
intVector *intVec5 = NEW_VECTOR_OF(32, int, int, 1, 2, 3, 4, 5); // vector with capacity of 32 and 5 elements of `int` type

userVector *uVec = VECTOR_OF(user, User, {.name = "name_1", .age = 23}); // custom type init
```

#### Add elements

***NOTE***: method `<type_name>VecAdd()` return `true` if element has been added or `false` when vector is full

```c
cStrVector *strVec = NEW_VECTOR_4(cStr, char*);  // empty vector
cStrVecAdd(strVec, "aaa");
cStrVecAdd(strVec, "bbb");
cStrVecAdd(strVec, "ccc");
cStrVecAdd(strVec, "ddd");

assert(cStrVecAdd(strVec, "eee") == false); // vector is full, return false
```

#### Get elements
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4);
intVecGet(intVec, 0);   // 1
intVecGet(intVec, 1);   // 2
intVecGet(intVec, 2);   // 3
intVecGet(intVec, 3);   // 4

intVecGet(intVec, 4);   // accessing index that is out of bounds, returns default for type '0'
intVecGetOrDefault(intVec, 5, 12345);   // return '12345'
```

#### Put element at specific index
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4);
intVecPut(intVec, 1, 99);   // [1], [99], [3], [4]
```

#### Add element at index
```c
intVector *intVec = NEW_VECTOR_OF(8, int, int, 1, 2, 3, 4);
intVecAddAt(intVec, 0, 21); // [21], [1], [2], [3], [4]
intVecAddAt(intVec, 5, 22); // [21], [1], [2], [3], [4], [22]
intVecAddAt(intVec, 2, 23); // [21], [1], [23], [2], [3], [4], [22]
```
#### Remove element at index
```c
intVector *intVec = VECTOR(int, 1, 2, 3, 4, 5);
intVecRemoveAt(intVec, 0); // [2], [3], [4], [5]
intVecRemoveAt(intVec, 1); // [2], [4], [5]
intVecRemoveAt(intVec, 2); // [2], [4]
```

#### Check that vector is empty or not
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
assert(ischarVecEmpty(cVec) == false);  // not empty
assert(ischarVecNotEmpty(cVec) == true);

charVecClear(cVec);
assert(ischarVecEmpty(cVec) == true);  // empty
assert(ischarVecNotEmpty(cVec) == false);
```

#### Holding element count
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
printf("[%d]", charVecSize(cVec)); // [3]
```

#### Remove all elements from vector
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
charVecClear(cVec);
printf("[%d]", charVecSize(cVec)); // [0]
```

#### Add all elements from one vector to other
**NOTE:** Ensure that destination vector have enough space for all elements from source vector
```c
charVector *cVec = NEW_VECTOR_OF(8, char, char, 'a', 'b', 'c');
charVector *cVec2 = VECTOR(char, 'd', 'e', 'f');
assert(charVecAddAll(cVec, cVec2) == true); // cVec -> [a], [b], [c], [d], [e], [f], if lack of capacity returns false
```

#### Add elements to vector from array
```c
int array[] = {1, 2, 3, 4, 5};
intVector *intVec = NEW_VECTOR_8(int);
intVecFromArray(intVec, array, 5);  // intVec -> [1], [2], [3], [4], [5]
assert(intVecSize(intVec) == 5);
```

#### Find element index in vector
```c
cStrVector *strVec = NEW_VECTOR_4(cStr, char*);
cStrVecAdd(strVec, "aaa");
cStrVecAdd(strVec, "bbb");
cStrVecAdd(strVec, "ccc");
cStrVecAdd(strVec, "ddd");

assert(cStrVecIndexOf(strVec, "bbb") == 1);
assert(cStrVecIndexOf(strVec, "eee") == -1); // not found 
```

#### Check that element is in vector
```c
cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "d");
assert(cStrVecContains(strVec, "c") == true);
assert(cStrVecContains(strVec, "e") == false);
```

#### Reverse elements in vector
```c
cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "d");
cStrVecReverse(strVec); // [d], [c], [b], [a]
```

#### Sort elements
Vector element sort by provided comparator
```c
cStrVector *strVec = VECTOR_OF(cStr, char*, "b", "a", "d", "c");
cStrVecSort(strVec); // [a], [b], [c], [d]
```

#### Check that two vectors equals
```c
charVector *cVec = VECTOR(char, 'a', 'b', 'c');
charVector *cVec2 = VECTOR(char, 'a', 'b', 'c');
assert(ischarVecEquals(cVec, cVec2) == true);
```

#### Remove duplicated values
```c
cStrVector *strVec = VECTOR_OF(cStr, char*, "a", "b", "c", "a", "a", "a", "c");
cStrVecRemoveDup(strVec); // [a], [b], [c]
```

#### Vector Union
```c
cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
cStrVecUnion(strVec, strVec2);  // strVec -> [1], [2], [3], [4], [5], [6], [7], [8], [9]
```

**Shortcut:** Copy all elements to empty vector from other
```c
cStrVector *strVec = cStrVecUnion(NEW_VECTOR_16(cStr, char*), strVec2);
```

#### Vector Intersection
```c
cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "A", "B", "C", "D", "E", "F");
cStrVector *strVec2 = VECTOR_OF(cStr, char*, "B", "D", "F", "G", "H", "K");
cStrVecIntersect(strVec, strVec2);  // strVec -> [B], [D], [F]
```

#### Vector Subtraction
```c
cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
cStrVecSubtract(strVec, strVec2);  // strVec -> [1], [4], [5]
```

#### Vector Disjunction
```c
cStrVector *strVec = NEW_VECTOR_OF(12, cStr, char*, "1", "2", "3", "4", "5", "6");
cStrVector *strVec2 = VECTOR_OF(cStr, char*, "2", "3", "6", "7", "8", "9");
cStrVecDisjunction(strVec, strVec2);  // strVec -> [1], [4], [5], [7], [8], [9]
```

### Heap version

This version uses generic type of `void*` and uses heap memory allocation increasing inner array size

Example of usage:

```C
Vector vector = getVectorInstance(4);   // create instance
vectorAdd(vector, 1);   // add some elements
vectorAdd(vector, 2);
vectorAdd(vector, 3);

printf("Size: %d\n", getVectorSize(vector)); // 3
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 3
}
printf("\n");

vectorAdd(vector, 5);   // add more elements
vectorAdd(vector, 6);
printf("Added size: %d\n", getVectorSize(vector)); // size is increased to 5

vectorAddAt(vector, 3, 4);  // insert in the middle
printf("New size: %d\n", getVectorSize(vector)); // 6
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 3 4 5 6
}
printf("\n");

vectorRemoveAt(vector, 2);  // remove some element
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 1 2 4 5 6
}
printf("\n");

vectorPut(vector, 0, 0);    // override value
for (int i = 0; i < getVectorSize(vector); i++) {
    printf("%d ", vectorGet(vector, i)); // 0 2 4 5 6
}
printf("\n");

vectorClear(vector);
printf("Is Vector has elements: %s\n", isVectorNotEmpty(vector) ? "Yes" : "No");    // No
printf("Is Vector empty: %s\n", isVectorEmpty(vector) ? "Yes" : "No");              // Yes

vectorDelete(vector);
```