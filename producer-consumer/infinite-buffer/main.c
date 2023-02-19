#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

int count;
sem_t mutex, empty;

void *producer(void *args)
{
    int i = 10;
    while (i--)
    {
        sem_wait(&mutex);
        count++;
        sem_post(&empty);
        sem_post(&mutex);
    }
    return NULL;
}

void *consumer(void *args)
{
    int i = 10;
    while (i--)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        count--;
        sem_post(&mutex);
    }
    return NULL;
}

int main()
{
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, 0);

    pthread_t producers[20], consumers[20];

    for (int i = 0; i < 20; i++)
    {
        pthread_create(&producers[i], NULL, producer, NULL);
        pthread_create(&consumers[i], NULL, producer, NULL);
    }
    for (int i = 0; i < 20; i++)
    {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }
}