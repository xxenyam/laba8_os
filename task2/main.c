#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int data = -1;  // змінна для зберігання числа
int done = 0;  // завершення програми

// Функція для генерації випадкового числа 
void* producer(void* arg) {
    while (!done) {
        pthread_mutex_lock(&mutex);  
        data = rand() % 100;  
        printf("Виробник: згенеровано число = %d\n", data);
        pthread_cond_signal(&cond);  
        pthread_mutex_unlock(&mutex);  
        sleep(1);  
    }
    return NULL;
}

// Функція для споживання числа 
void* consumer(void* arg) {
    while (!done) {
        pthread_mutex_lock(&mutex);  
        while (data == -1) {  
            pthread_cond_wait(&cond, &mutex);  
        }
        printf("Споживач: спожито число = %d\n", data);
        data = -1;  
        pthread_mutex_unlock(&mutex);  
        sleep(1);  
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t prod, cons;

    // Створення потоків
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    sleep(10);
    done = 1;  

    // завершення потоків
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}