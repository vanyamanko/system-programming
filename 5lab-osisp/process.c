#include "func.h"

void messageProducer(MessageQueue *queue, dispatch_semaphore_t *sem) {
    
    while (1) {
        dispatch_semaphore_wait(*sem, DISPATCH_TIME_FOREVER);
        Message *new_message = (Message *)malloc(sizeof(Message));
        if (new_message == NULL) {
            perror("Error allocating memory for message");
            exit(EXIT_FAILURE);
        }
        generateRandomMessage(new_message);
        push(queue, new_message);
        sleep(1);
        dispatch_semaphore_signal(*sem);
        if(!threadProdFlag) {
            break;
        }
        if(waitFlag) {
            pthread_cond_init(&condvar, NULL);
            pthread_mutex_t mutex; 
            pthread_mutex_lock(&mutex);
            pthread_cond_wait(&condvar, &mutex); 
            printf("HELO\n");
            pthread_mutex_unlock(&mutex); 
        }
    }
}

void messageConsumer(MessageQueue *queue, dispatch_semaphore_t *sem) {
    while (1) {
        dispatch_semaphore_wait(*sem, DISPATCH_TIME_FOREVER);
        pop(queue);
        sleep(1);
        dispatch_semaphore_signal(*sem);
        if(!threadConFlag) {
            break;
        }
    }
}
