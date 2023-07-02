#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Queue {

    int mpros, piso, size;
    unsigned xwritikotita;
    char **filenames;
};



struct Queue* createQueue(unsigned xwritikotita)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));

    queue->xwritikotita = xwritikotita;
    queue->filenames = malloc(queue->xwritikotita * sizeof(char *));

    queue->mpros = queue->size = 0;   
    queue->piso = xwritikotita - 1;

    return queue;
}



void enqueue(struct Queue* queue, char *file)
{
    int length=100;


    queue->piso = (queue->piso + 1)% queue->xwritikotita;

    queue->filenames[queue->piso]=malloc(length*sizeof(char));

    strcpy(queue->filenames[queue->piso],file);

    queue->size = queue->size + 1;
}
 


char *dequeue(struct Queue* queue)
{

    char *file=malloc(strlen(queue->filenames[queue->mpros])*sizeof(char));
    strcpy(file,queue->filenames[queue->mpros]);

    queue->mpros = (queue->mpros + 1)% queue->xwritikotita;
    queue->size = queue->size - 1;
    
    return file;
}



int Full(struct Queue* queue)
{
    return (queue->size == queue->xwritikotita);
}
 

int Empty(struct Queue* queue)
{
    return (queue->size == 0);
}