#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXSIZE 1024

static const char *utilities[] = {"pwd","lsf","cat","wc","bunedu"};
static const char *standart[] = {"help","exit","cd","!"};

int counter = 0;

char historyArr[MAXSIZE][MAXSIZE];
char execDir[MAXSIZE];

char *readLine();
char **split_input(char *line);
int isPipedCommand(char *line);
int isRedirectCommand(char *line);

void signalHandling(int sig);
int help();
int changeDirectory(char **path);
int exitShell();
int pastCommandRun(int historyNo);
int pastCommand(char *past);
int lsf(char **splitArray);
int pwd(char **splitArray);    
int wc(char **splitArray);
int bunedu(char **splitArray);    
int cat(char **splitArray); 

int standartOrUtilities (char * command);
int findingCommands(char **splitArray);
void pipedCommands(char **pipedArr,int pipeIndex);
void redirectedCommand(char **splitArray,int inOut);

// pipedCommands fonksyonu The Linux Programming Interface kitabının 900. sayfasından alinmistir.
int main(int argc, char* argv[]){

	printf("2019 / CSE 344 / SHELL HOMEWORK \n");

	signal(SIGTERM,signalHandling);
	signal(SIGINT,signalHandling);
	signal(SIGTSTP,signalHandling);

	getcwd(execDir, sizeof(execDir));

	char *inputLine;
	char **splitArray;
	int pipeIndex = 0;
	int isRedirect = 0;
	int terminate = 1;
	int i = 0;
	while(terminate)
	{
		isRedirect = 0;
		pipeIndex = 0;
		printf(">$ ");
		inputLine = readLine();
		if ( (strcmp(inputLine,"\n") == 0) || (strcmp(inputLine," \n") == 0) || (strcmp(inputLine,"\t\n") == 0) )
		{
			free(inputLine);
		}
		else
		{
			pipeIndex = isPipedCommand(inputLine);
			isRedirect = isRedirectCommand(inputLine);
			strcpy(historyArr[counter],inputLine);
			splitArray = split_input(inputLine);
			if ( standartOrUtilities(splitArray[0]) == 0)
	   		{
	   			printf("%s -> command not found. You can use help for information.\n",splitArray[0]);
	   		}
	   		else if(pipeIndex > 0 && (standartOrUtilities(splitArray[pipeIndex+1]) == 0))
   			{
   				printf("%s -> command not found. You can use help for information.\n",splitArray[pipeIndex+1]);
   			}
	   		else{
		   		if(pipeIndex== 0)
		   		{
		   			if(isRedirect == 0)
		   			{
						terminate = findingCommands(splitArray);
		   			}
		   			else
		   			{
		   				redirectedCommand(splitArray,isRedirect);
		   			}
		   		}
		   		else 
		   		{
		   			pipedCommands(splitArray,pipeIndex);
		   		}
				counter++;

				free(inputLine);
				free(splitArray);
	   		}
		}
	}
	return 0;
}

char *readLine()
{

	char *inputline = malloc(sizeof(char)*MAXSIZE);
	fgets(inputline,MAXSIZE,stdin);
	return inputline;

}

char **split_input(char *line)
{
	char **split_arr = malloc(MAXSIZE * sizeof(char*));
	char *tokenize;
	int i = 0;

	tokenize = strtok(line," \t\n");
	while(tokenize != NULL)
	{	
		split_arr[i++] = tokenize;
		tokenize = strtok(NULL," \t\n"); 
	}
	split_arr[i] = NULL;
	return split_arr;
}

int isPipedCommand(char *line)
{
	int i;
	int counter = 0;
	for(i = 0; line[i] != '\0'; ++i)
	{
		if(line[i] == ' ')
		{
			counter++;
		}
		if(line[i] == '|')
		{
			return counter;
		}
	}
	return 0;
}
int isRedirectCommand(char *line)
{
	int i;
	for(i = 0; line[i] != '\0'; ++i)
	{
		if(line[i] == '<')
		{
			return 1;
		}
		else if(line[i] == '>')
		{
			return 2;
		}
	}
	return 0;
}
int help()
{
    printf("CSE 344 System Programming Shell Homework\n");

    printf("cd : Changes directory \n");
    printf("Usage : cd [rootpath]  \n\n");

    printf("pwd : Display current working directory \n");
    printf("called like : pwd\n\n");

    printf("cat : display given argument on default output\n");
    printf("example usage : cat rapor2.txt = will display rapor2.txt to screen if not changed.\n\n");

    printf("lsf: Lists just file's -> File Types (Regular(R) or Special (S) ) / Access Rights (Ex : rwxr-x-wx) / File Size (Bytes) pf current directory. \n");
    printf("Usage : lsf \n\n");

    printf("wc : which will print on standard output the number of lines in the file provided to it as argument or the string coming from standard input until EOF character\n");
    printf("Usage :  wc file.txt --> Output : 55 \n\n");

    printf("bunedu : This command finds the size of the given directory and subdirectories.\n");
    printf("Usage : bunedu rootpath or bunedu -z rootpath (first does command not add subdirtectories sizes)\n\n");

    printf("> : Redirects to given file name. \n");
    printf("Usage : lsf > example (example contains lsf's output.) \n\n");

    printf("< : Redirects to command as an argument given file's context \n");
    printf("Usage : bunedu -z < example.txt (example.txt contains ../CSE344. It will be print ../CSE344 and subdirtectories sizes. )\n\n");

    printf("| : Pipe. This will redirect output of the first command to input of second command \n");
    printf("Usage : cat file.txt | bunedu -z, this will call bunedu -z on the path written inside file.txt\n\n");

    printf("help: Which will print the list of supported commands. \n");
    printf("just type help\n\n");

    printf("exit : which will exit the shell\n");
    printf("\n");


    return 1;

}

void signalHandling(int sig)
{
    if (sig == SIGINT) 
    {
        printf("\tUsage : Ctrl+C signal has received.\n");
        exit(0);
    }
    if (sig == SIGTERM) 
    {
        printf("\tUsage : Ctrl+C signal has received.\n");
        exit(0);
    }
    if (sig == SIGTSTP)
    {
    	printf("\tUsage : Ctrl+Z signal has received\n");
    	exit(0);
    }
}

int changeDirectory(char **path)
{
    if (path[1] == NULL) {
        fprintf(stderr, "cd must have argument path. Usage : cd path ");
    } 
    else 
    {
        if (chdir(path[1]) != 0)
        {
            fprintf(stderr, "cd, Error on path : %s\n",path[1] );
        }
        else
        {
        	printf("current location has changed\n" );
        }
    }
    return 1;
}

int exitShell()
{
	return 0;
}


int pastCommandRun(int historyNo)
{
	char **splitArray;
	int pipeIndex = 0;
	int isRedirect = 0;
	if ( (strcmp(historyArr[historyNo],"\n") == 0) || (strcmp(historyArr[historyNo]," \n") == 0) || (strcmp(historyArr[historyNo],"\t\n") == 0) )
		{
			
		}
		else
		{
			pipeIndex = isPipedCommand(historyArr[historyNo]);
			isRedirect = isRedirectCommand(historyArr[historyNo]);
			
			splitArray = split_input(historyArr[historyNo]);
			if ( standartOrUtilities(splitArray[0]) == 0)
	   		{
	   			printf("%s -> command not found. You can use help for information.\n",splitArray[0]);
	   		}
	   		if(pipeIndex== 0)
	   		{
	   			if(isRedirect == 0)
	   			{
					findingCommands(splitArray);
	   			}
	   			else
	   			{
	   				redirectedCommand(splitArray,isRedirect);
	   			}
	   		}
	   		else 
	   		{
	   			pipedCommands(splitArray,pipeIndex);
	   		}
			counter++;

			free(splitArray);
		}
	return 1;
}


int pastCommand(char *past)
{	
	int i = 1;
	char number[MAXSIZE];
	int historyNo = 0;

	while(past[i] != '\0')
	{
		number[i-1] = past[i];
		i += 1;
	}
	number[i-1] = '\0';
	historyNo = atoi(number);

	if(counter - historyNo < 0)
	{
		printf("Usage : The number of input greate than history size. \n");
	}
	else
	{
		pastCommandRun(counter - historyNo);
	}
	return 1;
}

int lsf(char **splitArray){    
    pid_t childpid;
    char working[MAXSIZE];

    childpid = fork();

    if(childpid < 0){
        perror("Fork failed.\n");
    }

    else if(childpid == 0){
    	sprintf(working,"%s%s",execDir,"/lsf");
        execvp(working, splitArray);   

        exit(0);
    }

    else{
        wait(NULL);
    } 
    return 1;   
}

int wc(char **splitArray){    
    pid_t childpid;
    char working[MAXSIZE];

    childpid = fork();

    if(childpid < 0){
        perror("Fork failed.\n");
    }

    else if(childpid == 0){
    	sprintf(working,"%s%s",execDir,"/wc");
        execvp(working, splitArray);   

        exit(0);
    }

    else{
        wait(NULL);
    } 
    return 1;   
}

int bunedu(char **splitArray){    
    pid_t childpid;
    char working[MAXSIZE];

    childpid = fork();

    if(childpid < 0){
        perror("Fork failed.\n");
    }

    else if(childpid == 0){
    	sprintf(working,"%s%s",execDir,"/bunedu");
        execvp(working, splitArray);   

        exit(0);
    }

    else{
        wait(NULL);
    } 
    return 1;   
}

int cat(char **splitArray){    
    pid_t childpid;
    char working[MAXSIZE];

    childpid = fork();

    if(childpid < 0){
        perror("Fork failed.\n");
    }

    else if(childpid == 0){
    	sprintf(working,"%s%s",execDir,"/cat");
        execvp(working, splitArray);   

        exit(0);
    }

    else{
        wait(NULL);
    } 
    return 1;   
}

int pwd(char **splitArray){    
    pid_t childpid;
    char working[MAXSIZE];

    childpid = fork();

    if(childpid < 0){
        perror("Fork failed.\n");
    }

    else if(childpid == 0){
    	sprintf(working,"%s%s",execDir,"/pwd");
        execvp(working, splitArray);   

        exit(0);
    }

    else{
        wait(NULL);
    } 
    return 1;   
}
int standartOrUtilities (char * command)
{	
	int i;
	for(i = 0; i<5; ++i)
	{
		if( (strcmp(utilities[i],command)) == 0)
		{
			return 2;
		}
	}
	for(i = 0; i<3; ++i)
	{
		if( (strcmp(standart[i],command)) == 0)
		{
			return 1;
		}
	}
	if(command[0] == '!')
	{
		return 1;
	}
	return 0;
}

int findingCommands(char **splitArray)
{
    if (splitArray[0] == NULL) {
        return 1;
    }

    if (strcmp(splitArray[0],"cd") == 0)
    {
        return changeDirectory(splitArray);
    }
    else if (strcmp(splitArray[0],"help") == 0)
    {
        return help();
    }
    else if (strcmp(splitArray[0],"exit") == 0)
    {
        return exitShell();
    }
    else if (strcmp(splitArray[0],"lsf") == 0)
    {
    	if(splitArray[1] == NULL)
    	{
        	return lsf(splitArray);
    	}
    	else
    	{
    		printf("Usage : lsf take no argument. lsf [NULL] \n");
    		return 1;
    	}
    }
    else if (strcmp(splitArray[0],"cat") == 0)
    {
        return cat(splitArray);
    }
    else if (strcmp(splitArray[0],"pwd") == 0)
    {
        return pwd(splitArray);
    }
    else if (strcmp(splitArray[0],"wc") == 0)
    {
        return wc(splitArray);
    }
    else if (strcmp(splitArray[0],"bunedu") == 0)
    {
        return bunedu(splitArray);
    }
    else if ((splitArray[0][0] == '!'))
    {
    	return pastCommand(splitArray[0]);
    }
    return 1;
}
// pipedCommands fonksyonu The Linux Programming Interface kitabının 900. sayfasından alinmistir.
void pipedCommands(char **pipedArr,int pipeIndex)
{
	char **word = malloc(MAXSIZE * sizeof(char*));
	char **word2 = malloc(MAXSIZE * sizeof(char*));

	char exe[1024];
	int i = 0;
	int j = 0;
	int k = 0;
	
	for(i = 0; pipedArr[i] != NULL; ++i)
	{
		if(i < pipeIndex)
		{
			word[j] = pipedArr[i];
			j++;
		}
		else if( i > pipeIndex)
		{
			word2[k] = pipedArr[i];
			k++;
		}
	}
	word[j] = NULL;
	word2[k] = NULL;
	int pfd[2]; /* Pipe file descriptors */
	if (pipe(pfd) == -1) /* Create pipe */
		printf("pipe");
	switch (fork()) 
	{
		case -1:
		 	printf("fork");
		case 0: /* First child: exec 'ls' to write to pipe */
		if (close(pfd[0]) == -1) /* Read end is unused */
			printf("close 1");
	 /* Duplicate stdout on write end of pipe; close duplicated descriptor */
		if (pfd[1] != STDOUT_FILENO) 
		{ /* Defensive check */
			if (dup2(pfd[1], STDOUT_FILENO) == -1)
			 	printf("dup2 1");
			if (close(pfd[1]) == -1)
				printf("close 2");
	 	}
		sprintf(exe,"%s%s%s",execDir,"/",word[0]);
		execvp(exe, word); /* Writes to pipe */
		printf("execvp ");
		default: /* Parent falls through to create next child */
			break;
 	}
	switch (fork()) 
	{
		case -1:
		 	printf("fork");
		case 0: /* Second child: exec 'wc' to read from pipe */
		if (close(pfd[1]) == -1) /* Write end is unused */
			printf("close 3");
		 /* Duplicate stdin on read end of pipe; close duplicated descriptor */
		if (pfd[0] != STDIN_FILENO) { /* Defensive check */
			if (dup2(pfd[0], STDIN_FILENO) == -1)
		 		printf("dup2 2");
			if (close(pfd[0]) == -1)
				printf("close 4");
		}
		sprintf(exe,"%s%s%s",execDir,"/",word2[0]);
		execvp(exe, word2); /* Reads from pipe */
		printf("execvp ");
		default: /* Parent falls through */
			break;
	}
	 /* Parent closes unused file descriptors for pipe, and waits for children */
	if (close(pfd[0]) == -1)
	 	printf("close 5");
	if (close(pfd[1]) == -1)
	 	printf("close 6");
	if (wait(NULL) == -1)
	 	printf("wait 1");
	if (wait(NULL) == -1)
	 	printf("wait 2\n");
	free(word);
	free(word2);
}

void redirectedCommand(char **splitArray,int inOut)
{
	char **word = malloc(MAXSIZE * sizeof(char*));
	char **word2 = malloc(MAXSIZE * sizeof(char*));
	char fileName[MAXSIZE];

	char exe[1024];

	int i = 0;
	int j = 0;
	int k = 0;
	
	int control = 0;
	int fd = 0;
	int pid = 0;
	for(i = 0; splitArray[i] != NULL; ++i)
	{
		if((strcmp(splitArray[i],"<") == 0))
		{
			control = 1;
		}
		if((strcmp(splitArray[i],">") == 0) )
		{
			control = 2;
		}
		if( control == 0 )
		{
			word[j] = splitArray[i];
			j++;
		}
		else if( control != 0)
		{
			word2[k] = splitArray[i];
			k++;
		}
	}
	if(control == 1)
	{
		printf("%s\n",word2[k-1] );
		fd = open(word2[k-1],O_RDONLY);
		if(fd != -1)
		{
			read(fd,fileName,sizeof(fileName));
		}
		fileName[strlen(fileName)-1] = '\0';
		word[j] = fileName;
		j++;
		word[j] = NULL;
		close(fd);

		pid = fork();
		if(pid == 0)
		{
			sprintf(exe,"%s%s%s",execDir,"/",word[0]);
			execvp(exe,word);
		}
		else if(pid>0)
		{
			wait(NULL);
		}
	}
	else if(control == 2)
	{
		pid = fork();
		if(pid == 0)
		{
			sprintf(exe,"%s%s%s",execDir,"/",word[0]);
			execvp(exe,splitArray);
		}
		else if(pid>0)
		{
			wait(NULL);
		}
	}
	free(word);
	free(word2);

}