#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t agentSem, tobaccoSem, paperSem, matchSem, mutex, tobaccoProduced, matchProduced, paperProduced;
int isPaper = 0, isMatch = 0, isTobbaco = 0;

// Agent
void *agentA(void *args)
{
    while (1)
    {
        sem_wait(&agentSem);
        sem_post(&tobaccoProduced);
        sem_post(&matchProduced);
    }
}
void *agentB(void *args)
{
    while (1)
    {
        sem_wait(&agentSem);
        sem_post(&paperProduced);
        sem_post(&matchProduced);
    }
}
void *agentC(void *args)
{
    while (1)
    {
        sem_wait(&agentSem);
        sem_post(&tobaccoProduced);
        sem_post(&paperProduced);
    }
}

// Pushers

void *tobbaco(void *args)
{
    while (1)
    {
        sem_wait(&tobaccoProduced);
        sem_wait(&mutex);
        if (isPaper)
        {
            sem_post(&matchSem);
        }
        else if (isMatch)
        {
            sem_post(&paperSem);
        }
        else
        {
            isTobbaco = 1;
        }
        sem_post(&mutex);
    }
}
void *paper(void *args)
{
    while (1)
    {
        sem_wait(&paperProduced);
        sem_wait(&mutex);
        if (isTobbaco)
        {
            sem_post(&matchSem);
        }
        else if (isMatch)
        {
            sem_post(&tobaccoSem);
        }
        else
        {
            isPaper = 1;
        }
        sem_post(&mutex);
    }
}
void *match(void *args)
{
    while (1)
    {
        sem_wait(&matchProduced);
        sem_wait(&mutex);
        if (isTobbaco)
        {
            sem_post(&paperSem);
        }
        else if (isPaper)
        {
            sem_post(&tobaccoSem);
        }
        else
        {
            isMatch = 1;
        }
        sem_post(&mutex);
    }
}

// Smokers

void *tobbacoSmoker(void *args)
{
    while (1)
    {
        sem_wait(&tobaccoSem);
        // make a ciggerate
        sem_post(&agentSem);
        // get high
    }
}
void *paperSmoker(void *args)
{
    while (1)
    {
        sem_wait(&paperSem);
        // make a ciggerate
        sem_post(&agentSem);
        // get high
    }
}
void *matchSmoker(void *args)
{
    while (1)
    {
        sem_wait(&matchSem);
        // make a ciggerate
        sem_post(&agentSem);
        // get high
    }
}

int main()
{
    // initialize sems
    sem_init(&agentSem, 0, 1);
    sem_init(&paperProduced, 0, 0);
    sem_init(&matchProduced, 0, 0);
    sem_init(&tobaccoProduced, 0, 0);
    sem_init(&tobaccoSem, 0, 0);
    sem_init(&paperSem, 0, 0);
    sem_init(&matchSem, 0, 0);

    // create agent (comprised of three threads)
    pthread_t agent1, agent2, agent3;
    pthread_create(&agent1, NULL, agentA, NULL);
    pthread_create(&agent2, NULL, agentB, NULL);
    pthread_create(&agent3, NULL, agentC, NULL);

    // create pushers
    pthread_t pusher1, pusher2, pusher3;
    pthread_create(&pusher1, NULL, tobbaco, NULL);
    pthread_create(&pusher2, NULL, paper, NULL);
    pthread_create(&pusher3, NULL, match, NULL);

    // create smokers;
    pthread_t smoker1, smoker2, smoker3;
    pthread_create(&smoker1, NULL, tobbacoSmoker, NULL);
    pthread_create(&smoker2, NULL, paperSmoker, NULL);
    pthread_create(&smoker3, NULL, matchSmoker, NULL);
}