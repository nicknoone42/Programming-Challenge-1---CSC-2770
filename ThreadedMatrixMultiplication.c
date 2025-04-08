#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 3
#define NUM_THREADS 3

int matrixA[MATRIX_SIZE][MATRIX_SIZE];
int matrixB[MATRIX_SIZE][MATRIX_SIZE];
int resultMatrix[MATRIX_SIZE][MATRIX_SIZE];

int calculationCount = 0; 
pthread_mutex_t calculationMutex;

typedef struct {
    int row;
    int col;
} thread_data_t;

void *multiply(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        pthread_mutex_lock(&calculationMutex);
        resultMatrix[data->row][data->col] += matrixA[data->row][i] * matrixB[i][data->col];
        calculationCount++;
        pthread_mutex_unlock(&calculationMutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MATRIX_SIZE * MATRIX_SIZE];
    thread_data_t thread_data[MATRIX_SIZE * MATRIX_SIZE];

    pthread_mutex_init(&calculationMutex, NULL);

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrixA[i][j] = rand() % 10;
            matrixB[i][j] = rand() % 10;
            resultMatrix[i][j] = 0; 
        }
    }

    int thread_count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            thread_data[thread_count].row = i;
            thread_data[thread_count].col = j;
            pthread_create(&threads[thread_count], NULL, multiply, (void *)&thread_data[thread_count]);
            thread_count++;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&calculationMutex);

    printf("Result Matrix:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    printf("Total number of calculations: %d\n", calculationCount);

    return 0;
}
