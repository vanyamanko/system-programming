#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define FILENAME "data.txt"
#define NUMBER_OF_RECORDS 10

struct flock lock;
int fd;

void readInput(char* buffer, int size)
{
	char *nl = NULL;
	memset(buffer, 0, size);

	if (fgets(buffer, size, stdin) != NULL) {
		nl = strchr(buffer, '\n');
		if (nl) *nl = '\0';
	}
}

void modification(int num) {    
    if(num > NUMBER_OF_RECORDS || num < 1) {
        printf("Incorrect input. Try again!\n");
        return;
    }

    off_t startPosition = (num - 1) * 2;
    lock.l_start = startPosition;
    lock.l_len = 2;
    lock.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        printf("This record already in modification\n");
        return;
    }
    
    printf("Enter new record: ");
    char buff[2];
    readInput(buff, sizeof buff);
    lseek(fd, startPosition, SEEK_SET);

    printf("Enter '1' to PUT this modification: ");
    int choice;
    scanf("%d", &choice);

    if(choice == 1) {
        write(fd, buff, strlen(buff));
        printf("File modified\n");
    } else {
        printf("File not modified\n"); 
    }

    fcntl(fd, F_UNLCK, &lock);
}

void list(void) {
    for(int i = 0; i < NUMBER_OF_RECORDS; i++) {
        off_t startPosition = i * 2; 
        lock.l_type = F_WRLCK;
        lock.l_start = startPosition;
        fcntl(fd, F_GETLK, &lock);

        printf("\n%d. ", i + 1);
        if(startPosition == lock.l_start && lock.l_type != F_UNLCK) {
            printf("Sorry this record is being modified\n");
        } else {
            lseek(fd, startPosition, SEEK_SET);
            char record[2];
            read(fd, record, 1);
            printf("%s\n", record);
        }
    }
}

void get(int num) {
   if(num > NUMBER_OF_RECORDS || num < 1) {
        printf("Incorrect input. Try again!\n");
        return;
    }
    off_t startPosition = (num - 1) * 2; 
    lock.l_type = F_WRLCK; 
    lock.l_start = startPosition;
    fcntl(fd, F_GETLK, &lock);
    
    printf("\n%d. ", num);
    if(startPosition == lock.l_start && lock.l_type != F_UNLCK) {
        printf("Sorry this record is being modified\n");
    } else {
        lseek(fd, startPosition, SEEK_SET);
        char record[2];
        read(fd, record, 1);
        printf("%s\n", record);
    }
}

int main(void) {

    fd = open(FILENAME, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (lseek(fd, 0, SEEK_END) == 0) { 
        char* strings[] = {"A\n", "B\n", "C\n", "D\n", "E\n", 
                        "F\n", "G\n", "H\n", "I\n", "J\n"};
        for (int i = 0; i < NUMBER_OF_RECORDS; i++) {
            write(fd, strings[i], strlen(strings[i]));
        }
    }
    lseek(fd, 0, SEEK_SET);

    char buff[16];

    printf("Enter choice: ");
    readInput(buff, sizeof buff);

    char *number = NULL;
	number = strtok (buff," ");
	number = strtok (NULL, " ");

    if (strcmp(buff, "LST") == 0) {
       list();
	}
    if (strcmp(buff, "GET") == 0) {
        int num = atoi(number);
        get(num);
    }
    if (strcmp(buff, "MOD") == 0) {
        int num = atoi(number);
        modification(num);
    }

    return 0;
}
