#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>


int main(int argc,char* argv[])
{   
    DIR *poDir;
    
    struct dirent *dirPtr;
    struct stat fileStat;

    char buffer[512];
    char curDir[1024];
    int i = 0;
    int flag = 0;

    char fileName[1024];
    for(i=0; argv[i] != NULL; ++i)
    {
        if(strcmp(argv[i],">") == 0)
        {
            flag = 1;
        }
        strcpy(fileName,argv[i]);
    }
    if(flag == 0)
    {
        if (getcwd(curDir, sizeof(curDir)) != NULL){
            poDir = opendir(curDir);
        }
        else{
            return 1;
        }   
        printf("TYPES\tACCESS RIGHTS\tSIZE(Bytes)\tFILE NAME\n");

        while((dirPtr = readdir(poDir)) != NULL) 
        {   
            sprintf(buffer, "%s/%s",curDir, dirPtr->d_name);
            stat(buffer, &fileStat);

            if( !(S_ISDIR(fileStat.st_mode))){

                if(S_ISLNK(fileStat.st_mode) && !(S_ISREG(fileStat.st_mode)))
                {
                    printf("S\t");
                }
                else
                {
                    printf("R\t");
                }
                printf( (fileStat.st_mode & S_IRUSR) ? "r" : " -");
                printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
                printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (fileStat.st_mode & S_IXOTH) ? "x\t" : "-\t");
                
                printf("%d\t",(int) fileStat.st_size);
                printf("\t%s\n", dirPtr->d_name);
            }
        }

        closedir(poDir);
    }
    else if(flag == 1)
    {
        FILE *fp = fopen(fileName,"w");

        if (getcwd(curDir, sizeof(curDir)) != NULL){
            poDir = opendir(curDir);
        }
        else{
            return 1;
        }   

        fprintf(fp,"TYPES\tACCESS RIGHTS\tSIZE(Bytes)\tFILE NAME\n");

        while((dirPtr = readdir(poDir)) != NULL) 
        {   
            sprintf(buffer, "%s/%s",curDir, dirPtr->d_name);
            stat(buffer, &fileStat);

            if( !(S_ISDIR(fileStat.st_mode))){
                if(S_ISLNK(fileStat.st_mode) && !(S_ISREG(fileStat.st_mode)))
                {
                    fprintf(fp,"S\t");
                }
                else
                {
                    fprintf(fp,"R\t");
                }
                fprintf(fp, "%s",((fileStat.st_mode & S_IRUSR) ? "r" : " -"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IWUSR) ? "w" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IXUSR) ? "x" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IRGRP) ? "r" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IWGRP) ? "w" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IXGRP) ? "x" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IROTH) ? "r" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IWOTH) ? "w" : "-"));
                fprintf(fp, "%s",((fileStat.st_mode & S_IXOTH) ? "x\t" : "-\t"));
                
                fprintf(fp,"%d\t",(int) fileStat.st_size);
                fprintf(fp,"\t%s\n", dirPtr->d_name);
            }
        }
        fclose(fp);
        closedir(poDir);
    }

    return 1;
}