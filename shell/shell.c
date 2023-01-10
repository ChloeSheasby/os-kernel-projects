#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

int tokenize(char *temp, char *args[])
{
    int i = 0;
    char *token = strtok(temp, " ");
    while(token != NULL)
    {
        //printf("%s\n", token);
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    
    return i;
}

void parse_command(char *temp, char *args[], bool *amp)
{
    temp[strcspn(temp, "\n")] = 0;
    temp[strlen(temp)] = '\0';

    int count = tokenize(temp, args);
    if(count == 0)
    {
        return;
    }

    if(count > 0 && strcmp(args[count - 1], "&") == 0)
    {
        *amp = true;
        args[count - 1] = 0;
    }
}

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char temp[MAX_LINE];
    int index = 0;
    bool amp; /* flag for if the input has an ampersand */
    int should_run = 1; /* flag to determine when to exit program */
    pid_t pid;

    setvbuf(stdout, NULL, _IONBF, 0);

    while(should_run) {
        printf(">>> ");
        fflush(stdout);

        fgets(temp, MAX_LINE, stdin);
        parse_command(temp, args, &amp);
        fflush(stdout);

        //printf("Amp flag is %d\n", amp);

        pid = fork();

        if(pid < 0)
        {
            printf("Fork failed.\n");
            exit(1);
        }
        else if(pid == 0)
        {
            if(execvp(args[0], args) == -1)
            {
                printf("Error executing command.\n");
            }
            exit(0);
        }
        else
        {
            if(amp)
            {
                wait(NULL);
                amp = false;
            }
        }
    }

    return 0;
}