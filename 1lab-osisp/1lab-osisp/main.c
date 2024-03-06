#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

struct flags
{
    int l;
    int d;
    int f;
    int s;
    int name;
};

struct flags find_flags_and_path(int argc, char* argv[], char* path, char* name)
{
    struct flags flag = { 0, 0, 0, 0, 0};
    for(int j = 1; j < argc; j++)
    {
        if(argv[j][0] == '-')
        {
            
            for(size_t i = 0; i < strlen(argv[j]); i++)
            {
                if(argv[j][i] == 'n') flag.name = 1;
                if(argv[j][i] == 'l') flag.l = 1;
                if(argv[j][i] == 'd') flag.d = 1;
                if(argv[j][i] == 'f') flag.f = 1;
                if(argv[j][i] == 's') flag.s = 1;
            }
        }
        if(argv[j][0] == '/' || argv[j][0] == '.') strcpy(path, argv[j]);
        if (argv[j][0] == '%' && argv[j][strlen(argv[j]) - 1] == '%') {
            strcpy(name, argv[j]);
           // printf("1\n");
        }
    }
    if(!flag.l && !flag.d && !flag.f)
    {
        flag.l = 1;
        flag.d = 1;
        flag.f = 1;
    }
    return flag;
}

int compare(const struct dirent **a, const struct dirent **b) {
    setlocale(LC_COLLATE, "");
    return strcoll((*a)->d_name, (*b)->d_name);
}

void free_file_list(struct dirent** file_list, int file_count) {
    for (int i = 0; i < file_count; i++) {
        free(file_list[i]);
    }
    free(file_list);
}
void check_flag_name(int flag, char file[256], char name[256], char path[1024])
{
    if((flag > 1 && !strcmp(file, name)) || flag < 2)
    {
        printf("%s/%s\n", path, file);
    }
}
void dirwalk(char path[1024], struct flags flag, char* name)
{
    struct dirent** file_list = NULL;

    int count = scandir(path, &file_list, NULL, NULL);
    
    if(strlen(name) != 0 && flag.name)
    {
        flag.name++;
    }
    if(count == -1)
    {
        printf("Try again!\n");
        return;
    }
    if(flag.s) count = scandir(path, &file_list, NULL, compare);
    for (int i = 0; i < count; i++)
    {
        if (strcmp(file_list[i]->d_name, ".") == 0 || strcmp(file_list[i]->d_name, "..") == 0) continue;
        
        if(flag.d && file_list[i]->d_type == DT_DIR)
        {
            check_flag_name(flag.name, file_list[i]->d_name, name, path);
        }
        
        if(file_list[i]->d_type == DT_DIR)
        {
            char next_path[1024];
            strcpy(next_path, path);
            strcat(next_path, "/");
            strcat(next_path, file_list[i]->d_name);
            dirwalk(next_path ,flag, name);
        }
        if(flag.f && file_list[i]->d_type == DT_REG) check_flag_name(flag.name, file_list[i]->d_name, name, path);
        if(flag.l && file_list[i]->d_type == DT_LNK) check_flag_name(flag.name, file_list[i]->d_name, name, path);
    }
}

int main(int argc, char* argv[])
{
    char path[1024] = "./";
    char name[256] = "";
    struct flags flag = find_flags_and_path(argc, argv, path, name);
    if (strlen(name) >= 2) {
        memmove(name, name + 1, strlen(name) - 2);
        name[strlen(name) - 2] = '\0';
    }
    dirwalk(path, flag, name);
    return 0;
}
