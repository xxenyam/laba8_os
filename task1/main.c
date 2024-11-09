#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5 
#define MAX_NUM 100    
#define TIMEOUT 10     

// Буфер для зберігання згенерованих чисел
int buffer[BUFFER_SIZE];
int in = 0;  // Вказівник на місце вставки
int out = 0; // Вказівник на місце витягування

// Семофори для синхронізації
sem_t empty;  
sem_t full;   
sem_t mutex;  

// Функція для генерації випадкових чисел
void* producer(void* arg) {
    while (1) {
        int num = rand() % MAX_NUM;  
        sem_wait(&empty);            
        sem_wait(&mutex);           

        buffer[in] = num;
        printf("Виробник: вставлено число %d\n", num);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);            
        sem_post(&full);             

        sleep(1);  
    }
    return NULL;
}

// Функція для споживання чисел
void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);             
        sem_wait(&mutex);            

        int num = buffer[out];
        printf("Споживач: забрано число %d\n", num);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);            
        sem_post(&empty);            

        sleep(2);  
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    sem_init(&empty, 0, BUFFER_SIZE);  // Порожні місця в буфері
    sem_init(&full, 0, 0);             // Заповнені місця в буфері
    sem_init(&mutex, 0, 1);            // М'ютекс для захисту доступу до буфера

    // Створення потоків
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    sleep(TIMEOUT);
    
    // Завершення роботи потоків
    pthread_cancel(prod_thread);
    pthread_cancel(cons_thread);

    // Очистка ресурсів
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    
    printf("Програма завершена\n");
    return 0;
}