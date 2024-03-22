#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

#define CHILD_PATH "CHILD_PATH"

extern char **environ;

int compare_env_vars(const void *a, const void *b) {
    const char *var_one = *(const char **)a;
    const char *var_two = *(const char **)b;
    return strcoll(var_one, var_two);
}

int main(int argc, char * argv[], char * envp[]) {
    setlocale(LC_COLLATE, "C");
    
    int env_count = 0;
    char **env_vars = NULL;
    for (char **env = environ; *env != NULL; env++) {
        env_vars = realloc(env_vars, (env_count + 1) * sizeof(char *));
        env_vars[env_count] = *env;
        env_count++;
    }
    qsort(env_vars, env_count, sizeof(char *), compare_env_vars);
    for (int i = 0; i < env_count; i++) {
        printf("%s\n", env_vars[i]);
    }
    
    while (1) {
        int c = getchar();
        
        if(c == '+') {
            char *value = getenv(CHILD_PATH);
            if (value != NULL) {
                printf("%s=%s\n", CHILD_PATH, value);
            } else {
                printf("%s - add value\n", CHILD_PATH);
            }
            
        }
        
        if(c == '%') {
            for (int i = 0; envp[i] != NULL; i++) {
                
                char *name = malloc(256);
                strcpy(name, envp[i]);
                char *new_name = strchr(name, '=');
                *new_name = '\0';
                
                if (strcmp(CHILD_PATH, name) == 0) {
                    printf("%s\n", envp[i]);
                }
                free(name);
            }
        }
        
        if(c == '@') {
            for (char **env = environ; *env != NULL; env++) {
                
                char *name = malloc(256);
                strcpy(name, *env);
                char *new_name = strchr(name, '=');
                *new_name = '\0';
                
                if (strcmp(CHILD_PATH, name) == 0) {
                    printf("%s\n", *env);
                }
                
                free(name);
            }
        }
        
        if (c == 'q') {
            break;
        }
    }
    
    return 0;
}
