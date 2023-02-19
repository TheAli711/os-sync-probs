#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t roomEmpty;
pthread_mutex_t reader_lock;
pthread_mutex_t order_lock;

int readers = 0;
void *writer(void *)
{
    while (1)
    {
        pthread_mutex_lock(&order_lock);
        pthread_mutex_lock(&roomEmpty);
        pthread_mutex_unlock(&order_lock);
        sleep(4);
        pthread_mutex_unlock(&roomEmpty);
    }
}

void *reader(void *)
{
    while (1)
    {
        pthread_mutex_lock(&order_lock);
        pthread_mutex_lock(&reader_lock);
        if (!readers)
        {
            pthread_mutex_lock(&roomEmpty);
        }
        else
        {
        }
        readers++;
        pthread_mutex_unlock(&order_lock);
        pthread_mutex_unlock(&reader_lock);

        // sleep(2);

        pthread_mutex_lock(&reader_lock);
        readers--;
        if (!readers)
        {
            pthread_mutex_unlock(&roomEmpty);
        }
        pthread_mutex_unlock(&reader_lock);
    }
}

void main()
{
    pthread_t ptid1, ptid2, ptid3, ptid4;
    pthread_mutex_init(&roomEmpty, NULL);
    pthread_mutex_init(&reader_lock, NULL);
    pthread_create(&ptid1, NULL, writer, NULL);
    pthread_create(&ptid2, NULL, reader, NULL);
    pthread_create(&ptid3, NULL, reader, NULL);
    pthread_create(&ptid4, NULL, reader, NULL);
    pthread_join(ptid1, NULL);
    pthread_join(ptid2, NULL);
    pthread_join(ptid3, NULL);
    pthread_join(ptid4, NULL);
}