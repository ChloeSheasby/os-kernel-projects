#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main(void)
{
	char write_msg[BUFFER_SIZE] = "Greetings";
	char read_msg[BUFFER_SIZE];
	int fd[2];


	/* create the pipe NOTES: fd[0] is set up for reading, fd[1] is set up for writing*/
	if (pipe(fd) == -1 )
	{
		fprintf(stderr,"Pipe failed");
		return 1;
	}


	pid_t pid = fork();

	if (pid == -1)
	{
      perror("fork failed");
      return 1;
	}
	if (pid >0 ) /* parent process */
	{
		close(fd[READ_END]);
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		close(fd[WRITE_END]);
	}

	else /* child process */
	{
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		printf("read from pipe: %s\n", read_msg);
		close(fd[READ_END]);
	}
   return 0;
}
