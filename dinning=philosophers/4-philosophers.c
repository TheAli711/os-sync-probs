#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
sem_t forks[5];
sem_t fourphilo;

void *philospoher(void *arg)
{
    int i = *((int *)arg);
    int j = 2;
    while (j--)
    {
        // think
        printf("Philosopher %d thinking\n", i);
        sleep(1);
        // get chopsticks
        sem_wait(&fourphilo);
        sem_wait(&forks[i]);
        sem_wait(&forks[(i + 1) % 5]);
        // eat
        printf("Philosopher %d eating\n", i);
        sleep(1);
        // release chopsticks
        sem_post(&forks[i]);
        sem_post(&forks[(i + 1) % 5]);
        sem_post(&fourphilo);
        printf("Philosopher %d finished eating\n", i);
    }
}

int main()
{
    int thread_nums[5] = {0, 1, 2, 3, 4};
    pthread_t threads[5];
    for (int i = 0; i < 5; i++)
    {
        sem_init(&forks[i], 0, 1);
    }
    sem_init(&fourphilo, 0, 4);

    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, philospoher, (void *)&thread_nums[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}