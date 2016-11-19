#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>


int deep = 0;
int isFound = 0;
char directory[2000];


void find(char *path, int deep, char *filename);

int main(int argc, char *argv[])

{

    find(argv[1], atoi(argv[2]), argv[3]);	
    if (!isFound) printf("File wasn't found\n");
	return 0;
}

void find(char *path, int deep,char *filename){
    
    DIR *dirp = opendir(path);
    if (dirp == NULL)
    {
        exit(2);
    }
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
            /* FIXIT: magic number */
            char addres[100000];
            strcpy(addres, path);
            strcat(addres, "/");
            strcat(addres, current -> d_name);
            stat(addres, &buf);
            strcpy(directory, addres);

            /* FIXIT: у вас имя не может быть одновременно .. и . */
            if (!(strcmp(current -> d_name, "..") && strcmp(current -> d_name, ".")))
            {
                continue;
            }
            if (S_ISDIR(buf.st_mode)) {
                if (deep > 0){
                    find(addres, deep - 1, filename);
                }
            }
    }     
    (void)closedir(dirp);
}

