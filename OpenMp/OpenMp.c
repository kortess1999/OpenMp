#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

int getFileSize(const char* file_name) {

    int _file_size = 0;
    FILE* fd;
    fd = fopen(file_name, "rb");
    if (fd == NULL) {
        _file_size = -1;
    }
    else {
        _file_size = fseek(fd, 0, SEEK_END);
        fclose(fd);
    }
    return _file_size;
}

int main()
{
    FILE* fileIn;
    FILE* fileOut;
    char inputFile[1024];
    char outputFile[1024];
    int data;
    int arraySize, stepCount = 0;
    float summa = 0;
    int startTime, endTime;
    long fileLen;

    printf("Input fileName\n");
    scanf("%s", inputFile);
    fileIn = fopen(inputFile, "rb");

    if (!fileIn) {
        printf("Missing file!\n");
        return;
    }

    fileLen = getFileSize(inputFile);
    printf("FileSize: %d\n", fileLen);

    printf("Output fileName\n");
    scanf("%s", outputFile);
    fileOut = fopen(outputFile, "w");

    printf("Input N:\n");
    scanf("%d", &arraySize);

    int numbersCount = fileLen / sizeof(int);
    int* inputData = (int*)malloc(fileLen);
    double* outputData = (double*)malloc((numbersCount / arraySize + 1) * sizeof(double));

    if (inputData == NULL || outputData == NULL) {
        printf("Ошибка выделения памяти");
    }
    int i = 0;
    while (fread(&data, sizeof(int), 1, fileIn)) {
        inputData[i++] = data;
    }

    startTime = clock();
    int thread = 0;
    #pragma omp parallel for
    for (thread = 0; thread < numbersCount / arraySize; thread++) {
        summa = 0;
        for (i = 0; i < arraySize; i++) {
            summa += inputData[thread * arraySize + i];
        }
        outputData[thread] = (summa / arraySize);
    }
        
    summa = 0;
    for (int i = thread * arraySize; i < numbersCount; i++)
        summa += inputData[i];

    outputData[i / arraySize] = (summa / arraySize);
    
    endTime = clock();

    for (int i = 0; i < numbersCount / arraySize + 1; i++) {
        fprintf(fileOut, "%f\n", outputData[i]);
    }
    printf("TIME: %f BYTES_SIZE: %d", (float)(endTime - startTime / CLOCKS_PER_SEC), fileLen);

    free(inputData);
    free(outputData);

    fclose(fileIn);
    fclose(fileOut);

    return 0;
}