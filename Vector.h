#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Vector *Vector;
typedef void* ItemType; // vector can keep any type, change for specific

Vector getVectorInstance(uint32_t capacity);

void vectorAdd(Vector vector, ItemType item);
ItemType vectorGet(Vector vector, uint32_t index);
void vectorPut(Vector vector, uint32_t index, ItemType item);

void vectorAddAt(Vector vector, uint32_t index, ItemType item);
ItemType vectorRemoveAt(Vector vector, uint32_t index);

bool isVectorEmpty(Vector vector);
bool isVectorNotEmpty(Vector vector);
uint32_t getVectorSize(Vector vector);

void vectorClear(Vector vector);
void vectorDelete(Vector vector);

void initSingletonVector(Vector *vector, uint32_t capacity);
