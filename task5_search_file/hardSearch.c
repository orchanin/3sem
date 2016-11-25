#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PATH_LENG 10000
#define MAX_DIR_NAME_LENGTH 2000
int deep = 0;
int isFound = 0;

char directory[MAX_DIR_NAME_LENGTH];


void find(char *path, int deep, char *filename);

int main(int argc, char *argv[])

{

    find(argv[1], atoi(argv[2]), argv[3]);	
    if (!isFound) printf("File wasn't found\n");
	return 0;
}

void find(char *path, int deep,char *filename){
    
    DIR *dirp = opendir(path);
    /* 
    * ок, но это же была просто дополнительная проверка
    * на случай, если при открытии директории произойдет ошибка
    * программа завершалась с нужным кодом ошибки
    */
    struct dirent *current;           
    struct stat buf;

    while((current = readdir(dirp)) != NULL)
    {
        stat(current -> d_name, &buf);
        
        if (S_ISREG(buf.st_mode))
        {
            if(strcmp(filename, current -> d_name) == 0)
            {
                isFound = 1;
                printf("File was found in directory %s\n", directory);
                
            }
        }
    }    
    rewinddir(dirp);

    while((current = readdir(dirp)) != NULL)
    {
            
            char address[MAX_PATH_LENG];
            strcpy(address, path);
            strcat(address, "/");
            strcat(address, current -> d_name);
            stat(address, &buf);
            strcpy(directory, address);

            if (strcmp(current -> d_name, "..") == 0)
            {
                continue;
            }
            if (strcmp(current -> d_name, ".") == 0)
            {
                continue;
            }
            if (S_ISDIR(buf.st_mode)) {
                if (deep > 0){
                    find(address, deep - 1, filename);
                }
            }
    }     
    (void)closedir(dirp);
}

