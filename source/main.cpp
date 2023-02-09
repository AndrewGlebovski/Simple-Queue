#include <stdio.h>
#include "queue.hpp"


int main() {
    Queue queue = {};

    queue_constructor(&queue, 1);

    for(int i = 0; i < 16; i++)
        queue_push(&queue, i + 1);
    
    for(int i = 1; i <= 16; i++) {
        int value = 0;
        queue_pop(&queue, &value);
    }

    queue_dump(&queue, queue_verificator(&queue), stdout);

    queue_destructor(&queue);

    printf("Queue!\n");

    return 0;
}
