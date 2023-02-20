#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

int buffer[10], in = 0, out = 0;
const int BUFFER_SIZE = 10;
sem_t mutex, items, spaces;

void *producer(void *args)
{
    int i = 10;
    while (i--)
    {
        sem_wait(&spaces);
        sem_wait(&mutex);
        buffer[in] = 1;
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&items);
        sem_post(&mutex);
    }
    return NULL;
}

void *consumer(void *args)
{
    int i = 10;
    while (i--)
    {
        sem_wait(&items);
        sem_wait(&mutex);
        buffer[out] = 0;
        out = (out + 1) % BUFFER_SIZE;
        sem_post(&spaces);
        sem_post(&mutex);
    }
    return NULL;
}

int main()
{
    sem_init(&mutex, 0, 1);
    sem_init(&items, 0, 0);
    sem_init(&spaces, 0, BUFFER_SIZE);

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