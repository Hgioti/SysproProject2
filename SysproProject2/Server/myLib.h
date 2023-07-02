#ifndef myLib_h
#define myLib_h


// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 
// A structure to represent a queue
typedef struct  {
    int front, rear, size;
    unsigned capacity;
    char **array;
}Queue;

//extern Queue queue;
 
// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned);
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue);
 
// Queue is empty when size is 0
int isEmpty(struct Queue*);
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue*, char *);
 
// Function to remove an item from queue.
// It changes front and size
char *dequeue(struct Queue*);
 
// Function to get front of queue
char *front(struct Queue*);
 
// Function to get rear of queue
char *rear(struct Queue);


pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cvar;
pthread_cond_t cvar2;
void perror_exit(char *message);
void *communication_thread(void *); 
void *worker_thread(void *);
void ReadFiles(char *path);

#endif

