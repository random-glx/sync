#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BUFFER 10

int buffer[MAX_BUFFER];
int count = 0;
int in = 0;
int out = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

int produce_item() {
    return rand() % 10;
}

void insert_item(int item) {
    buffer[in] = item;
    in = (in + 1) % MAX_BUFFER;
    count++;
    printf("Produced: %d\n", item);
}

int remove_item() {
    int item = buffer[out];
    out = (out + 1) % MAX_BUFFER;
    count--;
    printf("Consumed: %d\n", item);
    return item;
}

void *producer() {
    while(1) {
        pthread_mutex_lock(&mutex);

        while (count == MAX_BUFFER) {
            pthread_cond_wait(&empty,&mutex);
        }   

        int item = produce_item();
        insert_item(item);

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);

        usleep(1500000);

    }

    return NULL;
}

void *consumer() {
    while (1) {
        pthread_mutex_lock(&mutex);

        while(count == 0) {
            pthread_cond_wait(&full,&mutex);
        }

        int item = remove_item();

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);

        usleep(1500000);
    }

    return NULL;
    
}

int main() {

    pthread_t prod,cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
