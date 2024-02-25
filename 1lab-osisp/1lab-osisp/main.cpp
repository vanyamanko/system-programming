#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

struct flags
{
    bool l = false;
    bool d = false;
    bool f = false;
    bool s = false;
};

flags find_flags_and_path(int argc, char* argv[], char* path)
{
    flags flag;
    for(int j = 1; j<argc; j++)
    {
        if(argv[j][0] == '-')
        {
            
            for(size_t i = 0; i < strlen(argv[j]); i++)
            {
                if(argv[j][i] == 'l') flag.l = true;
                if(argv[j][i] == 'd') flag.d = true;
                if(argv[j][i] == 'f') flag.f = true;
                if(argv[j][i] == 's') flag.s = true;
            }
        }
        if(argv[j][0] == '/' || argv[j][0] == '.') strcpy(path, argv[j]);
    }
    if(!flag.l && !flag.d && !flag.f)
    {
        flag.l = true;
        flag.d = true;
        flag.f = true;
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
    flags flag = find_flags_and_path(argc, argv, path);
    
    struct dirent** file_list;
    
    int n = scandir(path, &file_list, NULL, NULL);
    
    if(flag.s) n = scandir(path, &file_list, NULL, compare);
    
    for (int i = 0; i < n; i++)
    {
        if(flag.d && file_list[i]->d_type == DT_DIR) printf("%s\n",file_list[i]->d_name);
        if(flag.f && file_list[i]->d_type == DT_REG) printf("%s\n",file_list[i]->d_name);
        if(flag.l && file_list[i]->d_type == DT_LNK) printf("%s\n", file_list[i]->d_name);
    }
    if(n == -1) printf("Try again!\n");
    return 0;
}
