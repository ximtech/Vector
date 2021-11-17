# Vector
**STM32** lightweight implementation of Vector array.\
The Vector is a resizable array. It is like an array, but there is no size limit.

## Difference between vector and array
An array follows static approach, means its size cannot be changed during run time while vector implements dynamic array means it automatically resizes itself when appending elements.

### Features 
- Specially designed for embedded applications
- Automatic array size reduction / enlargement
- Easy to user and add to project
- Fast element get and add
- Element insert in the middle
- Low memory consumption

### Trade-offs
- Mostly static Vector data type, can't be changed at runtime
- Not tested in multi thread applications

### Usage
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
```