#include <stdio.h>
#include <stdlib.h>
#include "queue.hpp"


/**
 * \brief Does some action in case of condition fail
 * \param [in] condition Condition to check
 * \param [in] action This code will be executed if condition fails
*/
#define CHECK(condition, action)                \
do {                                            \
    if (!(condition)) {                         \
        action;                                 \
    }                                           \
} while(0)


/**
 * \brief If queue is invalid returns an error code
 * \param [in] queue Queue to check
*/
#define RETURN_ON_ERROR(queue)                  \
do {                                            \
    int error = queue_verificator(queue);       \
    if (error) return error;                    \
} while(0)


/**
 * \brief Resizes queue
 * \param [in] queue This queue will be resized automaticaly
 * \return Non zero value means error
*/
static int queue_resize(Queue *queue);




int queue_constructor(Queue *queue, int capacity) {
    CHECK(queue, return EXIT_CODES::INVALID_ARGUMENT);
    CHECK(capacity > 0, return EXIT_CODES::INVALID_ARGUMENT);

    queue -> data = (queue_data_t *) calloc(capacity, sizeof(queue_data_t));
    CHECK(queue -> data, return EXIT_CODES::ALLOCATE_FAIL);

    for(int i = 0; i < capacity ; i++)
        (queue -> data)[i] = POISON_VALUE;
    
    queue -> capacity = capacity;
    queue -> size = 0;
    queue -> head = 0;

    RETURN_ON_ERROR(queue);

    return 0;
}


static int queue_resize(Queue *queue) {
    RETURN_ON_ERROR(queue);

    int prev_capacity = queue -> capacity;

    if (4 * queue -> size <= queue -> capacity)
        queue -> capacity /= 2;
    
    else if (queue -> size == queue -> capacity)
        queue -> capacity *= 2;

    else 
        return 0;

    queue_data_t *prev_data = queue -> data;

    queue -> data = (queue_data_t *) calloc(queue -> capacity, sizeof(queue_data_t));
    CHECK(queue -> data, return EXIT_CODES::ALLOCATE_FAIL);

    for(int i = queue -> size; i < queue -> capacity ; i++)
        (queue -> data)[i] = POISON_VALUE;

    for(int i = 0; i < queue -> size; i++)
        (queue -> data)[i] = (prev_data)[(queue -> head + i) % prev_capacity];
    
    free(prev_data);

    queue -> head = 0;

    RETURN_ON_ERROR(queue);

    return 0;
}


int queue_push(Queue *queue, queue_data_t new_data) {
    RETURN_ON_ERROR(queue);

    (queue -> data)[(queue -> head + queue -> size) % queue -> capacity] = new_data;
    queue -> size++;

    RETURN_ON_ERROR(queue);

    return queue_resize(queue);
}


int queue_pop(Queue *queue, queue_data_t *data) {
    CHECK(data, return EXIT_CODES::INVALID_ARGUMENT);

    RETURN_ON_ERROR(queue);

    CHECK(queue -> size, return EXIT_CODES::EMPTY_QUEUE);

    *data = (queue -> data)[queue -> head];
    (queue -> data)[queue -> head] = POISON_VALUE;
    queue -> head = (queue -> head + 1) % queue -> capacity;
    queue -> size--;

    RETURN_ON_ERROR(queue);

    return queue_resize(queue);
}


int queue_destructor(Queue *queue) {
    RETURN_ON_ERROR(queue);

    free((char *)(queue -> data));

    queue -> data = nullptr;
    
    queue -> capacity = 0;
    queue -> size = 0;

    return 0;
}


int queue_verificator(Queue *queue) {
    CHECK(queue, return EXIT_CODES::INVALID_ARGUMENT);

    CHECK(queue -> data, return EXIT_CODES::INVALID_DATA);

    CHECK(queue -> capacity >= 0 && queue -> capacity <= MAX_CAPACITY_VALUE, return EXIT_CODES::INVALID_CAPACITY);

    CHECK(queue -> size >= 0 && queue -> size <= queue -> capacity, return EXIT_CODES::INVALID_SIZE);

    CHECK(queue -> head >= 0 && queue -> head < queue -> capacity, return EXIT_CODES::INVALID_HEAD);
    
    int count = 0;

    for(int i = 0; i < queue -> capacity; i++) {
        if (i == queue -> head) count = queue -> size;

        if (count) {
            CHECK((queue -> data)[i] != POISON_VALUE, return EXIT_CODES::UNEXP_POISON_VAL);
            count--;
        }
        else
            CHECK((queue -> data)[i] == POISON_VALUE, return EXIT_CODES::UNEXP_NORMAL_VAL);
    }
    
    return 0;
}


void queue_dump(Queue *queue, int error, FILE *stream) {
    CHECK(queue, return);
    
    fprintf(stream, "Queue[%p]:\n", queue);

    fprintf(stream, "\%2s%-8s: %i\n", "", "Capacity", queue -> capacity);
    fprintf(stream, "\%2s%-8s: %i\n", "", "Size", queue -> size);
    fprintf(stream, "\%2s%-8s: %i\n", "", "Head", queue -> head);

    fprintf(stream, "\%2sData[%p]", "", queue -> data);
    
    if (error == EXIT_CODES::INVALID_DATA || error == EXIT_CODES::INVALID_CAPACITY) {
        fputc('\n', stream);
        return;
    }
    
    fprintf(stream, ":\n");

    for(int i = 0; i < queue -> capacity; i++) {
        fprintf(stream, "%4s[%03i] ", "", i); // queue_data_t index

        fprintf(stream, OBJECT_TO_STR, (queue -> data)[i]); // print value function (possible macros)

        if ((queue -> data)[i] == POISON_VALUE) fprintf(stream, " (POISON VALUE)"); // poison value warning
            
        fputc('\n', stream); // new line
    }

    fputc('\n', stream);
}