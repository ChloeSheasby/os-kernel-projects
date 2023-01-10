#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */
#define MAX_HIST 10 /* History can only go back 10 commands */

char *history[MAX_HIST];
int command_counter = 0;

// free the history array for further use
void free_history()
{
    for(int i = 0; i < MAX_HIST; i++)
    {
        free(history[i]);
        history[i] = NULL;
    }
}

void add_history(char *temp)
{
    char *temp2 = malloc(MAX_LINE);
    strcpy(temp2, temp);

    // need to make sure that array only has 10 items and everything gets moved
    int index = command_counter % MAX_HIST;
    //printf("%d\n", index);
    if(command_counter >= MAX_HIST)
    {
        free(history[index]);
        history[index] = NULL;
    }

    // need to shift everything in history up in the array
    for(int i = 9; i > 0; i--)
    {
        if(history[i-1] != NULL)
        {
            history[i] = strdup(history[i-1]);
        }
    }

    history[0] = strdup(temp2);

    // free allocated memory back up
    free(temp2);
    temp2 = NULL;

    command_counter++;
}

void print_history()
{
    printf("Shell history:\n");
    int temp = command_counter;
    for(int i = 0; i < MAX_HIST && temp != 0; i++)
    {
        printf("%d\t%s\n", temp, history[i]);
        temp--;
    }
}

int tokenize(char *temp, char *args[])
{
    int i = 0;
    char *token = strtok(temp, " ");
    while(token != NULL)
    {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    
    return i;
}

void parse_command(char *temp, char *args[], bool *amp)
{
    *amp = false;

    temp[strcspn(temp, "\n")] = 0;
    temp[strlen(temp)] = '\0';

    // don't add the history command to history
    if(strlen(temp) != 0 && temp[0] != '!' && strcmp(temp, "history") != 0)
    {
        add_history(temp);
    }

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

// checks which command needs to be performed
void evaluate_command(char *args[], bool *amp)
{
    if(strcmp(args[0], "exit") == 0)
    {
        free_history();
        printf("Goodbye.\n");
        exit(0);
    }
    else if(strcmp(args[0], "history") == 0)
    {
        if(command_counter > 0)
        {
            print_history();
        }
        else
        {
            printf("No commands in history.\n");
        }
        return;
    }
    // checks if command is !!
    else if(strcmp(args[0], "!!") == 0)
    {
        if(command_counter == 0)
        {
            printf("No commands in history.\n");
        }
        else
        {
            // do most recent command in history
            char* temp = strdup(history[0]);
            parse_command(temp, args, amp);
            evaluate_command(args, amp);
            //free memory
            free(temp);
            temp = NULL;
        }
        return;
    }
    // checks if command is !n
    else if(args[0][0] == '!')
    {
        if(command_counter == 0)
        {
            printf("No commands in history.\n");
        }
        else
        {
            // do command at n
            char* numTemp = malloc(sizeof(char) * (strlen(args[0])));
            // fills numTemp with number from args
            for(int i = 1, j = 0; i < strlen(args[0]); i++, j++)
            {
                numTemp[j] = args[0][i];
            }
            numTemp[strlen(args[0])] = '\0';
            // changes numTemp to an int
            int n = atoi(numTemp);
            if(n > command_counter)
            {
                printf("No such command in history.\n");
                return;
            }
            char* temp = strdup(history[command_counter - n]);//(n-1) % MAX_HIST]);
            parse_command(temp, args, amp);
            evaluate_command(args, amp);
            //free memory
            free(temp);
            free(numTemp);
            temp = NULL;
            numTemp = NULL;
        }
        return;
    }

    pid_t pid = fork();

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

int main(void)
{
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char temp[MAX_LINE];
    int index = 0;
    bool amp = false; /* flag for if the input has an ampersand */
    int should_run = 1; /* flag to determine when to exit program */
    command_counter = 0;

    free_history();

    while(should_run) {
        printf(">>>>> ");
        fflush(stdout);

        fgets(temp, MAX_LINE, stdin);
        parse_command(temp, args, &amp);

        //printf("Amp flag is %d\n", amp);

        evaluate_command(args, &amp);

        // clearing out temp for next line
        temp[0] = '\n';
        for(int i = 1; i < MAX_LINE; i++)
        {
            temp[i] = '\0';
        }
    }

    return 0;
}