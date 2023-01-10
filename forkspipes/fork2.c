///
//  main.c
//  OS Prog 1
//
//  Created by David North on 1/15/15.
//  Copyright (c) 2015 David North. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>



// This is just a general utility function we call
// to display an error message and die.

void fatal(const char *errorMsg)
{
    printf ("%s",errorMsg);
    exit(1);
}

int main()
{
	char ch, first, last;		// controls which chars to print
	int pid;					// process id of our child

	long i;							// loop counter
	const long BIGNUM = 1000000;	// control delay loop
	int factor = 1;			// make child/parent delay different amounts

	if ((pid = fork()) > 0)
	{
		first = 'A';  // only parent does this section
		last  = 'Z';
		factor = 2;	  // parent will delay twice as long as child
		//wait(NULL);
	}
	else if (pid == 0)
	{
		first = 'a';	  // only child does this section
		last  = 'z';
		factor = 1;
	}
	else
		fatal("fork failed");

	// both parent and child execute code below

	for(ch = first; ch <= last; ch++)
	{
		// dummy delay loop to make things slow down
		for(i=0; i<= BIGNUM*factor; i++)
            ;  // null loop

		write(1, &ch,1);	// write 1 char to stdout (screen by default)

	}
	
	printf("\n");

	return 0;
}
