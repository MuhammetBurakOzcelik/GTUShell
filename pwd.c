#include<stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	char location[1024];
    if(argc == 1)
    {
        if (getcwd(location, sizeof(location)) != NULL)
        {
            printf("%s\n", location);
        }
        else{
            perror("Usage : Error on getcwd."); 
        }
        return 0;
    }
    else if(argc == 3)
    {
        FILE *fp = fopen(argv[2],"w");
        if (getcwd(location, sizeof(location)) != NULL)
        {
            fprintf(fp,"%s\n", location);
        }
        else{
            perror("Usage : Error on getcwd."); 
        }
        fclose(fp);
    }
}

