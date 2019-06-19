#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
   
   char myfile[1024];
   char fileName[1024];
   char ch;
   int lines = 0;
   int i = 0;
   int flag = 0;
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
      strcpy(myfile,argv[1]);

      lines = 0;

      FILE* file = fopen(myfile,"r");
      FILE* fp = fopen(fileName,"w");
      if(file == NULL)
      {
      }
      else
      {
         while ((fgets(myfile,1024,file)) )
         {
            if(file != NULL)
            {
               lines++;
            }
         }        
         fprintf(fp,"%d\n",lines);   
         fclose(fp); 
         fclose(file);

      }    
   }
   else if(flag == 0)
   {
      if(argc == 1)
      {
         while(scanf("%c",&ch) != EOF )
         {
            if(ch == '\n')
            {
               lines++;
            }
         }
         printf("%d\n",lines );
         return 1;
      }
      if(argc == 2)
      {
         strcpy(myfile,argv[1]);

         lines = 0;

         FILE* file = fopen(myfile,"r");

         if(file == NULL)
         {
         }
         else
         {
            while ((fgets(myfile,1024,file)) )
            {
               if(file != NULL)
               {
                  lines++;
               }
            }        
            printf("%d\n",lines);    
         }    
         fclose(file);

      }
   }
   
   
   return 1;
}