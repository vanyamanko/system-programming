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
};

struct flags find_flags_and_path(int argc, char* argv[], char* path)
{
    struct flags flag = { 0, 0, 0, 0 };
    for(int j = 1; j < argc; j++)
    {
        if(argv[j][0] == '-')
        {
            
            for(size_t i = 0; i < strlen(argv[j]); i++)
            {
                if(argv[j][i] == 'l') flag.l = 1;
                if(argv[j][i] == 'd') flag.d = 1;
                if(argv[j][i] == 'f') flag.f = 1;
                if(argv[j][i] == 's') flag.s = 1;
            }
        }
        if(argv[j][0] == '/' || argv[j][0] == '.') strcpy(path, argv[j]);
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
int main(int argc, char* argv[])
{
    char path[256] = "./";
    struct flags flag = find_flags_and_path(argc, argv, path);
    
    struct dirent** file_list;
    
    int n = scandir(path, &file_list, NULL, NULL);
    
    if(flag.s) n = scandir(path, &file_list, NULL, compare);
    
    for (int i = 0; i < n; i++)
    {
        if(flag.d && file_list[i]->d_type == DT_DIR) printf("%s/%s\n", path, file_list[i]->d_name);
        if(flag.f && file_list[i]->d_type == DT_REG) printf("%s/%s\n", path, file_list[i]->d_name);
        if(flag.l && file_list[i]->d_type == DT_LNK) printf("%s/%s\n", path, file_list[i]->d_name);
    }
    if(n == -1) printf("Try again!\n");
    
    return 0;
}
