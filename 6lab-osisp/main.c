#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

void fillFileWithRandomChars(const char* filename, int numChars) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("error.\n");
        return;
    }

    srand(time(NULL));

    for (int i = 0; i < numChars; i++) {
        char randomChar = 'A' + rand() % 26;
        fputc(randomChar, file);
    }

    fclose(file);
}

typedef struct {
    int fileSize;
    int blockAmount;
    int threadAmount;
    int threadNum;
} ThreadArgs;

void* threadFunc(void* args) {
    //some
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    const char* filename = argv[4];
    int numChars = atoi(argv[1]);

    fillFileWithRandomChars(filename, numChars);

    pthread_t threads[atoi(argv[3])];
    for(int i = 0; i < atoi(argv[3]); i++) {

        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->fileSize = atoi(argv[1]);
        args->blockAmount = atoi(argv[2]);
        args->threadAmount = atoi(argv[3]);
        args->threadNum = i;

        pthread_t thread;
        int result = pthread_create(&thread, NULL, threadFunc, (void*)args);

        if (result != 0) {
         perror("pthread_create");
         exit(EXIT_FAILURE);
        }
        threads[i] = thread;
    }



    for(int i = 0; i < atoi(argv[3]); i++) {
        pthread_join(threads[i], NULL);
    }



    return 0;
}
 //   0     1  2 3     4 
// ./prog 1024 4 2 ./data.txt

// размер блоков потоков 
