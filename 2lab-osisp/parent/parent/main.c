#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

extern char **environ;

int compare_env_vars(const void *a, const void *b) {
    const char *var_one = *(const char **)a;
    const char *var_two = *(const char **)b;
    return strcoll(var_one, var_two);
}

int main(int argc, const char * argv[]) {
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
            //child
        }
        
        if (c == 'q') {
            break;
        }
    }
    
    return 0;
}
