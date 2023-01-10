#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int status;

    pid_t pid = fork();

   if (pid == -1) {
      perror("fork failed");
      return 1;
   }
   else if (pid == 0) {
      printf("Hello from the child process!\n");
      return 0;
   }
   else {
      pid_t child=wait(&status);
      printf("After wait in parent process for child = %d Status = %d\n",child,status);
   }
   return 0;
}
