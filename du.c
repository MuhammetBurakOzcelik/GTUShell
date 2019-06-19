
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
/*
    * Muhammet Burak Ozcelik - 151044050 
    *           CSE344 - HW1 
*/

//https://github.com/bias/usp_exercises/blob/master/ch05/traversing-directories/mydu.c 
//Fikir olarak yardim aldigim kaynak buradadir.

int depthFirstApply(char *path, int pathfun(char *pathl), int flag,FILE *fp);

int sizepathfun(char *path);

#define parameter "-z"
int control = 0;

int main(int argc, char *argv[]) 
{ 
    FILE *fp = NULL;
    DIR *testDir;
    char name[1024];

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
    if(flag == 1)
    {
        fp = fopen(fileName,"w");
        if(strcmp(argv[1],"-z") == 0)
        {
            if( (strcmp(argv[1],parameter) != 0) )
            {
                fprintf(stderr, "Usage : %s\n","Invalid parameter. For Ex: buNeDu -z rootpath " );
                return 1;
            }
            control = 1;
            fprintf(fp,"Output of buNeDu -z %s\n",argv[2]);
            strcpy(name,argv[2]);
        }
        else
        {
            fprintf(fp,"Output of buNeDu %s\n",argv[1]);
            strcpy(name,argv[1]);
        }
        testDir = opendir(name);
        if( (testDir == NULL) )
        {
            fprintf(stderr, "Usage du: %s\n","Can't open directory." );
        }
        else
        {
            depthFirstApply(name, sizepathfun,flag,fp);
        }    
        closedir(testDir);
        fclose(fp);
        return 1; 
    }
     if(flag == 0)
    {
        if (argc == 1 ) 
        {
            scanf("%s",name);
            printf("Output of buNeDu %s don't add subdirectory sizes\n",name);
            testDir = opendir(name);
            if( (testDir == NULL) )
            {
                fprintf(stderr, "Usage : %s\n","Can't open directory." );
            }
            else
            {
                depthFirstApply(name, sizepathfun,flag,fp);
            }
            closedir(testDir);
            return 1;
        }
        else if(argc == 2 )
        {
            if(strcmp(argv[1],"-z") == 0)
            {
                if( (strcmp(argv[1],parameter) != 0) )
                {
                    fprintf(stderr, "Usage : %s\n","Invalid parameter. For Ex: buNeDu -z rootpath " );
                    return 1;
                }
                control = 1;
                scanf("%s",name);
                printf("Output of buNeDu -z %s\n",name);
            }
            else
            {
                printf("Output of buNeDu %s\n",argv[1]);
                strcpy(name,argv[1]);
            }
            testDir = opendir(name);
            if( (testDir == NULL) )
            {
                fprintf(stderr, "Usage : %s\n","Can't open directory." );
            }
            
            else
            {
                depthFirstApply(name, sizepathfun,flag,fp);
            }    
            closedir(testDir);
            return 1;
        }
        else if(argc == 3)
        {
            printf("Output of buNeDu -z %s\n",argv[2]);
            testDir = opendir(argv[2]);
            if( (testDir == NULL) )
            {
                fprintf(stderr, "Usage : %s\n","Can't open directory." );
            }
            else if( (strcmp(argv[1],parameter) != 0) )
            {
                fprintf(stderr, "Usage : %s\n","Invalid parameter. For Ex: buNeDu -z rootpath " );
            }
            else
            {
                control = 1; //-z parametresinin geldigini dogrular.
                printf("%s\n",argv[2] );
                depthFirstApply(argv[2], sizepathfun,flag,fp);
            }  
            closedir(testDir);
            return 1;
        }
    }
}

int depthFirstApply(char *path, int (*pathfun)(char *),int flag,FILE *fp) 
{

    struct stat fileStat;
    struct dirent *dirPtr;
    
    DIR *poDir;
    int folderSize = 0;
    char fullPath[1024];
    int temp = 0;

    //Dosya acilabiliyor mu kontrolu
    poDir = opendir(path);
    if(poDir == NULL)
    {
        fprintf(stderr, "Usage : %s\t%s\n","Can't open directory.",path );
        return -1;
    }
    /*
        *Bir dizindeki tum icerikleri ele alana kadar donerek ve gizli olan ".", ".." ele almadan parametresine gore
    tum dosyalarin boyutunu sizepathfun fonksiyonuna gondererek hesaplamaya yarar.
        *Eger -z parametresi ile komut cagrildiysa control degiskenine bakarak altdizindeki dizinlerin de hesaplamaya katilmasi saglanir.
        *Eger dosya acilamiyorsa donen deger -1 olacagindan (temp > 0 ) kontroluyle dosya boyutu ancak o zaman hesaba katilir.
        *Link veya regular olmayan bir dosya tipi geldiginde ise error not edilir.
        *Regular olan dosyalar icinse boyutu hesaplanarak sizepathfun fonksiyonundan alınır, eklenir.
    */

    while ((dirPtr = readdir(poDir)) != NULL) 
    {
        //bulundugu ya da parent dosyalarini tekrar hesaplamamasi icin
        if ( strcmp(dirPtr->d_name, ".") != 0 && strcmp(dirPtr->d_name, "..") != 0 ) 
        {
            
            sprintf(fullPath,"%s%s%s", path,"/",dirPtr->d_name);

            if ( lstat(fullPath, &fileStat) == -1) 
            {
                perror("Couldn't get file status");
                fprintf(stderr, "\t%s %s\n", path, dirPtr->d_name);
                return -1;
            }

            if ( S_ISDIR(fileStat.st_mode) )
            {
                // -z parametresi dogru geldigi takdirde control dogru demektir ve subdirectory boyularini da katmalidir.
                if(control==1)
                {
                    temp=depthFirstApply(fullPath, pathfun,flag,fp);
                    if(temp > 0)
                    {
                        folderSize += temp;
                    }
                }
                else 
                {
                    depthFirstApply(fullPath, pathfun,flag,fp);
                }
                
            }
            else
            {   
                //Link ya da regular olmayan dosyalar icin print
                if(S_ISLNK(fileStat.st_mode) && !(S_ISREG(fileStat.st_mode)))
                {
                    if(flag == 0)
                    {
                        printf("Special file %s\n", dirPtr->d_name);
                    }
                    else if(flag == 1)
                    {
                        fprintf(fp,"Special file %s\n", dirPtr->d_name);
                    }
                }
                else
                {
                    folderSize += pathfun(fullPath);
                }
            }
        }
    }

    closedir(poDir);
    
    if(flag == 0)
    {
        printf("%d \t%s\n", (folderSize/1024), path); 
    }
    else if(flag == 1)
    {
        fprintf(fp,"%d \t%s\n", (folderSize/1024), path); 
    }
    return folderSize;   
}

// Dosyanin boyutunu dondurmeye yarayan fonksiyon.
int sizepathfun(char *path) 
{
    struct stat fileStat;
    
    if ( lstat(path, &fileStat) == -1) 
    {
        perror("Couldn't get file status");
        fprintf(stderr, "\t%s\n", path);
        return -1;
    }
    
    return fileStat.st_size;
}
