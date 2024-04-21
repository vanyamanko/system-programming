#include "func.h"

int main(void) {
    MessageQueue* queue = initSharedMemory();

    int arrProducer[512];
    static int producerCount = 0;

    int arrConsumer[512];
    static int consumerCount = 0; 

    while(1) {
        char c = getchar();
        if(c == '1') {

        pid_t pid = fork();
    
         if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
            } else if (pid == 0) {
                messageProducer(queue);
            } else {
                arrProducer[producerCount] = pid;
                producerCount++;
                continue;
            }
        }
        if(c == '2') {
            for (int i = 0; i < producerCount; i++) {
                kill(arrProducer[i], SIGTERM); 
            } 
        }
        if(c == '3') {

        pid_t pid = fork();
    
         if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
            } else if (pid == 0) {
                messageConsumer(queue);
            } else {
                arrConsumer[consumerCount] = pid;
                consumerCount++;
                continue;
            }
        }
        if(c == '4') {
           for (int i = 0; i < consumerCount; i++) {
                kill(arrConsumer[i], SIGTERM); 
            }  
        }
        if(c == 's') {
            printf("Count of producter process: %d\n", producerCount);
            printf("Count of consumer process: %d\n", consumerCount);
        }
        if(c == 'q') {
            for (int i = 0; i < consumerCount; i++) {
                kill(arrConsumer[i], SIGTERM); 
            }  
             for (int i = 0; i < producerCount; i++) {
                kill(arrProducer[i], SIGTERM); 
            } 
            break;
        }
    }
    cleanupSharedMemory(queue);
    return 0;
}