#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fd[2], nbytes;
    pid_t childpid;
    char string[] = "Hello, world!\n";
    char readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    
    if(childpid == 0)
    {
        /* child process closes up input side of pipe */
        close(fd[0]);

        /* send string through the output side of pipe */
        write(fd[1], string, (strlen(string) + 1));
        exit(0);
    }
    else
    {
        /* parent process closes up output side of pipe */
        close(fd[1]);

        /* read in a string from the pipe */
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
    }
    
    return(0);
}