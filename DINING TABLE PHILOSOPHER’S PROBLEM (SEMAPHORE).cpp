#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
sem_t chopsticks[NUM_CHOPSTICKS];

void *philosopher(void *arg) 
{
    int id = *(int *)arg;
    int left_chopstick = id;
    int right_chopstick = (id + 1) % NUM_CHOPSTICKS;

    while (1) 
	{
        // Think for a bit
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 10);

        // Attempt to pick up chopsticks
        printf("Philosopher %d is hungry and attempting to pick up chopsticks %d and %d\n", id, left_chopstick, right_chopstick);
        sem_wait(&chopsticks[left_chopstick]);
        sem_wait(&chopsticks[right_chopstick]);

        // Eat for a bit
        printf("Philosopher %d is eating\n", id);
        sleep(rand() % 10);

        // Put down chopsticks
        printf("Philosopher %d is putting down chopsticks %d and %d\n", id, left_chopstick, right_chopstick);
        sem_post(&chopsticks[left_chopstick]);
        sem_post(&chopsticks[right_chopstick]);
    }
}

int main() 
{
    int i;
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize semaphores
    for (i = 0; i < NUM_CHOPSTICKS; i++) 
	{
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) 
	{
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Wait for philosopher threads to finish
    for (i = 0; i < NUM_PHILOSOPHERS; i++) 
	{
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (i = 0; i < NUM_CHOPSTICKS; i++) 
	{
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
