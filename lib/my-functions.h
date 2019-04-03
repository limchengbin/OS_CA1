#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS
#define PAGE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define DISK_MEMORY_SIZE 20480
#define MAX 20480
#define MIN 2048
#define INITIAL_VALUE 0x7e
#define NUMBER_OF_FRAMES 256

void assignMemoryValue(unsigned char *);
void assignDiskMemoryValue(unsigned char *);
void addTwoEntry(unsigned char *);
void getUserInput(unsigned char *,unsigned char *);
void swapping(unsigned char *, unsigned char *,unsigned int,unsigned int);

#endif
