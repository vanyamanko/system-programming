#include "func.h"

void messageProducer(MessageQueue *queue) {
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(MAX_SEM_COUNT); 
    while (1) {
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_signal(semaphore);
        Message *new_message = (Message *)malloc(sizeof(Message));
        if (new_message == NULL) {
            perror("Error allocating memory for message");
            exit(EXIT_FAILURE);
        }
        generateRandomMessage(new_message);
        push(queue, new_message);
        sleep(1);
       
    }
}

void messageConsumer(MessageQueue *queue) {
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(MAX_SEM_COUNT); 
    while (1) {
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_signal(semaphore);
        pop(queue);
        sleep(1);
       
    }
}
