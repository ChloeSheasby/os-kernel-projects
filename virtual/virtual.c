#include <stdio.h>

#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define NUM_FRAMES 256
#define TLB_SIZE 16

int main(int argc, char* argv[])
{
    // open backing store
    FILE *backingStore = fopen("BACKING_STORE.bin", "r");
    if(!backingStore)
    {
        printf("Error! Could not find BACKING_STORE.bin.\n");
        return 1;
    }

    // create physical memory
    int physicalMem[NUM_FRAMES * FRAME_SIZE];
    int freeAddress = 0;
    int freeFrame = 0;

    // create page table and initialize everything to -1
    int pageTable[PAGE_TABLE_SIZE];
    for(int i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        pageTable[i] = -1;
    }

    // create 2 TLB arrays for pages and frames, and initialize everything to -1
    int tlbPages[TLB_SIZE];
    int tlbFrames[TLB_SIZE];
    for(int i = 0; i < TLB_SIZE; i++)
    {
        tlbPages[i] = -1;
        tlbFrames[i] = -1;
    }
    int freeTLB = 0;

    // variables to keep track of page faults
    double numPageFaults = 0;
    double numFrameFaults = 0;
    double count = 0;

    try
    {
        FILE *addr = fopen("addresses.txt", "r");
        char buffer[256];
        while()

        fclose(addr);
        fclose(backingStore);
    }
}