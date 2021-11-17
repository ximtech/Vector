#include "Vector.h"

#define MIN(x, y) (((x)<(y))?(x):(y))

static bool doubleVectorCapacity(Vector vector);
static bool halfVectorCapacity(Vector vector);

struct Vector {
    ItemType *itemArray;
    uint32_t initialCapacity;
    uint32_t capacity;
    uint32_t size;
};

Vector getVectorInstance(uint32_t capacity) {
    if (capacity < 1) return NULL;

    Vector vector = malloc(sizeof(struct Vector));
    if (vector == NULL) return NULL;
    vector->size = 0;
    vector->capacity = capacity;
    vector->initialCapacity = capacity;
    vector->itemArray = calloc(vector->capacity, sizeof(ItemType));

    if (vector->itemArray == NULL) {
        vectorDelete(vector);
        return NULL;
    }
    return vector;
}

void vectorAdd(Vector vector, ItemType item) {
    if (vector != NULL) {
        if (vector->size >= vector->capacity) {
            if (!doubleVectorCapacity(vector)) return;
        }
        vector->itemArray[vector->size++] = item;
    }
}

ItemType vectorGet(Vector vector, uint32_t index) {
    return (vector != NULL && index < vector->size) ? vector->itemArray[index] : (ItemType) NULL;
}

void vectorPut(Vector vector, uint32_t index, ItemType item) {
    if (vector != NULL && index < vector->size) {
        vector->itemArray[index] = item;
    }
}

void vectorAddAt(Vector vector, uint32_t index, ItemType item) {
    if (vector != NULL && index < vector->size) {
        if (vector->size >= vector->capacity) {
            if (!doubleVectorCapacity(vector)) return;
        }
        for (uint32_t i = vector->size; i > index; i--) {
            vector->itemArray[i] = vector->itemArray[i - 1];
        }
        vector->itemArray[index] = item;
        vector->size++;
    }
}

ItemType vectorRemoveAt(Vector vector, uint32_t index) {
    if (vector != NULL && index < vector->size) {
        ItemType item = vector->itemArray[index];
        for (uint32_t i = index + 1; i < vector->size; i++) {
            vector->itemArray[i - 1] = vector->itemArray[i];
        }
        vector->size--;

        if ((vector->size * 4) < vector->capacity) {
            if (!halfVectorCapacity(vector)) return (ItemType) NULL;
        }
        return item;
    }
    return (ItemType) NULL;
}

bool isVectorEmpty(Vector vector) {
    return vector != NULL && vector->size == 0;
}

bool isVectorNotEmpty(Vector vector) {
    return !isVectorEmpty(vector);
}

uint32_t getVectorSize(Vector vector) {
    return vector != NULL ? vector->size : 0;
}

void vectorClear(Vector vector) {
    if (vector != NULL) {
        vector->size = 0;
        while (vector->capacity > vector->initialCapacity) {
            if (!halfVectorCapacity(vector)) break;
        }
    }
}

void vectorDelete(Vector vector) {
    if (vector != NULL) {
        free(vector->itemArray);
        free(vector);
    }
}

void initSingletonVector(Vector *vector, uint32_t capacity) {
    if (*vector == NULL) {
        *vector = getVectorInstance(capacity);
    }
}

static bool doubleVectorCapacity(Vector vector) {
    uint32_t newCapacity = vector->capacity * 2;
    if (newCapacity < vector->capacity) return false;   // overflow (capacity would be too big)

    ItemType *newItemArray = malloc(sizeof(ItemType) * newCapacity);
    if (newItemArray == NULL) return false;

    for (uint32_t i = 0; i < vector->size; i++) {
        newItemArray[i] = vector->itemArray[i];
    }
    free(vector->itemArray);
    vector->itemArray = newItemArray;
    vector->capacity = newCapacity;
    return true;
}

static bool halfVectorCapacity(Vector vector) {
    if (vector->capacity <= vector->initialCapacity) return false;
    uint32_t newCapacity = vector->capacity / 2;
    ItemType *newItemArray = malloc(sizeof(ItemType) * newCapacity);
    if (newItemArray == NULL) return false;

    for (uint32_t i = 0; i < MIN(vector->size, newCapacity); i++) {
        newItemArray[i] = vector->itemArray[i];
    }
    free(vector->itemArray);
    vector->itemArray = newItemArray;
    vector->capacity = newCapacity;
    vector->size = MIN(vector->size, newCapacity);
    return true;
}