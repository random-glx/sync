#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_READERS 5
#define NUM_WRITERS 2
sem_t rw_mutex, mutex;
int read_count = 0, data = 0;
void *reader(void *arg)
{
    while (1)
    {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);
        printf("Reader %ld reads: %d\n", (long)arg, data);
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&rw_mutex);
        sem_post(&mutex);
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg)
{
    while (1)
    {
        sem_wait(&rw_mutex);
        data++;
        printf("Writer %ld writes: %d\n", (long)arg, data);
        sem_post(&rw_mutex);
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    for (long i = 0; i < NUM_READERS; i++)
        pthread_create(&readers[i], NULL, reader, (void *)i);
    for (long i = 0; i < NUM_WRITERS; i++)
        pthread_create(&writers[i], NULL, writer, (void *)i);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    return 0;
}