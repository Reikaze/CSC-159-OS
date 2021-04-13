/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Queue Utilities
 */

#include "queue.h"
#include "spede.h"

/**
 * Adds an item to the end of a queue
 * @param  queue - pointer to the queue
 * @param  item  - the item to add
 * @return -1 on error; 0 on success
 */
int enqueue(queue_t *queue, int item) {
    // Return an error if the queue is full
    // Add the item to the tail of the queue
    // Move the tail forward
    // If we are at the end of the array, move the tail to the beginning
    // Increment size (since we just added an item to the queue)

    /* !!! CODE NEEDED !!! */
    //We Get the queue from the pointer and store the contents of the pointer into the same variable
   // breakpoint();


    //We will check to see if the queue is full
    if(queue->size >= QUEUE_SIZE )
    {

        return -1;
    }
    else
    {
        //the item will be placed in the items array
        queue->items[queue->tail]=item;


        queue->size= queue->size+1;

         //We move the tail forward
        queue->tail = (queue->tail+1);
        if( queue->tail == (QUEUE_SIZE) )
	           queue->tail=0;
        //We increase the size to know how many items are in the items array
        //queue->size=(queue->size+1);
    }

    //queue = nexto;
    //cons_printf("Item %d, tail %d, size %d \n", nexto.items[nexto.tail], queue->tail, queue->size);

    return 0;
}

/**
 * Pulls an item from the specified queue
 * @param  queue - pointer to the queue
 * @return -1 on error; 0 on success
 */
int dequeue(queue_t *queue, int *item) {
    // Return an error if queue is empty
    // Get the item from the head of the queue
    // Move the head forward
    // If we are at the end of the array, move the head to the beginning
    // Decrement size (since we just removed an item from the queue)

    /* !!! CODE NEEDED !!! */
    //queue_t next;

    if (queue->size == 0)
    {
        //panic("Queue empty");
        return -1;
    }
    else
    {
        //We will collect the iteam that is dequeued
        //*item = next.items[next.head];
        *item = queue->items[queue->head];
        //We are checking to see if we are at the end of the queue 
        //If we are not at the end of the array just move the head forward
        queue->head= (queue->head+1) ;


        if(  queue->head == QUEUE_SIZE  )

        {
            queue->head=0;
        }

        //decrease the size of the iteams queued
        queue->size= (queue->size-1);
    }
    //*queue= next;
    //cons_printf("Item %d, head %d, size %d \n", *item, queue->head, queue->size);

    return 0;
}
