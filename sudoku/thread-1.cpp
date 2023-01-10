// Basic threading example
// See also example in book, p. 161
//
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
using namespace std;

void *doChildThread(void *parm);
void delay();

// for illustration
//int i;


int main (int argc, char * const argv[]) {

	pthread_t tid;
	int i;

	cout << "output from parent thread" << endl;
	pthread_create(&tid, NULL, doChildThread, NULL);

	for(i = 0; i < 20; i++)
	{
		delay();
		char buf[32];
		sprintf(buf, "parent %d\n", i);
		write(1, buf, strlen(buf));
	}

	pthread_join(tid, NULL);

    return 0;
}

void *doChildThread(void *parm)
{
	int i;

	for(i = 0; i < 20; i++)
	{
		delay();
		char buf[32];
		sprintf(buf, "child %d\n", i);
		write(1, buf, strlen(buf));
	}

	return NULL;

}

// just to slow things down so that we can see interweaving
// of threads
void delay()
{
	for (float x = 0; x < 999999; x++) {
		;
	}
}
