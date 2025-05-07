#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
 
#define N 5 
 
sem_t forks[N];
pthread_t philosophers[N];
 
void *philosopher(void *num) {
    long id = (long)num;
    while (1) {
        printf("Philosopher %ld is thinking.\n", id);
        usleep(500000); 
 
        sem_wait(&forks[id]); 
        sem_wait(&forks[(id + 1) % N]); 
 
        printf("Philosopher %ld is eating.\n", id);
        usleep(500000); 
 
        sem_post(&forks[id]); 
        sem_post(&forks[(id + 1) % N]);
    }
}
 
int main() {
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }
 
    for (long i = 0; i < N; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)i);
    }
 
    getchar(); 
 
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }
 
    return 0;
}

