#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int main(int argc , char *argv[])
{
  FILE *fp;
  int fd;
  char line[10000];
  char name[1024];
  char ch;
  struct stat statbuf;

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
    if(argc == 1)
    {    
      fp = fopen(name,"r");
      if(fp == NULL){
      }
      stat(name, &statbuf);
      if(S_ISDIR(statbuf.st_mode))
      {
        printf("%s",name );
      }
      fd = open(name,O_RDONLY);
      if( fd != -1)
      {
        while(fscanf(fp,"%[^\n]\n",line)!=EOF)
        {
          printf("%s", line);
        }
      }
      else
      {
        while(scanf("%c",&ch) != EOF)
        {
          printf("%s",name);
          printf("%c",ch );
        }
      }
      fclose(fp);
      close(fd);
      return 1;
    }
    else if(argc == 2)
    { 
      fd = open(argv[1],O_RDONLY);
      if( fd != -1)
      {
        while(read(fd,line,sizeof(line)) > 0)
        {
          printf("%s", line);
        }
      }
      else
      {
        printf("%s\n",argv[1] );
      }
      close(fd);
    }
  }
  else if(flag == 1)
  {
    fd = open(argv[1],O_RDONLY);
    FILE *fp = fopen(fileName,"w");
    if( fd != -1)
    {
      while(read(fd,line,sizeof(line)) > 0)
      {
        fprintf(fp,"%s", line);
      }
    }
    else
    {
      fprintf(fp,"%s\n",argv[1] );
    }
    close(fd);
    fclose(fp);
  }
  
  return 1;
}