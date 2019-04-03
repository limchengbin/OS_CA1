#include "my-functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void addTwoEntry(unsigned char *mainMemory)
{
  for (int i = 0; i < PAGE_SIZE ; ++i)
  {
    if (mainMemory[i] == INITIAL_VALUE)
    {
      mainMemory[i] = 0x00;
      mainMemory[i + PAGE_SIZE] = 0x00;
      mainMemory[i + 1] = 0x01;
      mainMemory[i + 1 + PAGE_SIZE] = 0x00;
      printf("\n");
      printf("\tThe virtual address that saved in the disk is from %02x00 to %02xff\n", i, i + 1);
      printf("\n");
      i = PAGE_SIZE;
    }
  }
}
void assignDiskMemoryValue(unsigned char *diskMemory)
{
  FILE *outputfileDiskMemory;
  outputfileDiskMemory = fopen("/home/howdididie/solution/data/disk_memory.txt", "w");
  fprintf(outputfileDiskMemory, "Address\t|Framet\t|Content\n");
  fprintf(outputfileDiskMemory, "__________________________\n");

  for (int i = 0; i < DISK_MEMORY_SIZE; ++i)
  {
    if (i < 512)
    {
      char randomChar = (rand() % (125 - 33)) + 33;
      diskMemory[i] = randomChar;
    }
    else
    {
      diskMemory[i] = '~';
    }
    fprintf(outputfileDiskMemory, "0x%04x\t|%03d\t|%c\n", i, i / PAGE_SIZE, diskMemory[i]);
  }
}
void assignMemoryValue(unsigned char *mainMemory)
{
  srand(time(0));
  int initialFrame = (rand() % (176 - 2)) + 2;
  int noOfData = (rand() % (MAX - MIN)) + MIN;

  for (int i = 0; i < PHYSICAL_MEMORY_SIZE; ++i)
  {
    if (i >= initialFrame * PAGE_SIZE && i < (initialFrame * PAGE_SIZE) + noOfData)
    {
      char randomChar = (rand() % (125 - 33)) + 33;
      mainMemory[i] = randomChar;
    }
    else
    {
      mainMemory[i] = '~';
    }
  }
  int pageRequired = noOfData % PAGE_SIZE == 0 ? noOfData / PAGE_SIZE : (noOfData / PAGE_SIZE + 1);
  for (int i = 0; i < NUMBER_OF_FRAMES; i++)
  {
    if (i < pageRequired)
    {
      mainMemory[i] = initialFrame;
      initialFrame++;
      mainMemory[i + PAGE_SIZE] = 0x01;
    }
    else
    {
      mainMemory[i + PAGE_SIZE] = 0x00;
    }
  }
  FILE *outputfileMaineMemory, *outputfilepPageTable;
  outputfileMaineMemory = fopen("/home/howdididie/solution/data/disk_memory.txt", "w");
  outputfilepPageTable = fopen("/home/howdididie/solution/data/page_table.txt", "w");
  fprintf(outputfileMaineMemory, "Address\t|Frame\t|Content\n");
  fprintf(outputfileMaineMemory, "_________________________\n");

  fprintf(outputfilepPageTable, "Page\t|Frame\t|Present\n");
  fprintf(outputfilepPageTable, "______________________\n");

  for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++)
  {
    if (i < NUMBER_OF_FRAMES)
    {
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|0x%02x\n", i, i / PAGE_SIZE, mainMemory[i]);
      if (mainMemory[mainMemory[i] * PAGE_SIZE] != INITIAL_VALUE)
      {
        fprintf(outputfilepPageTable, "0x%02x\t|0x%02x\t|true\n", i, mainMemory[i]);
      }
      else
      {
        fprintf(outputfilepPageTable, "0x%02x\t|0x%02x\t|false\n", i, mainMemory[i]);
      }
    }
    else if (i < 512)
    {
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|0x%02x\n", i, i / PAGE_SIZE, mainMemory[i]);
    }
    else
    {
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|%c\n", i, i / PAGE_SIZE, mainMemory[i]);
    }
  }
  fclose(outputfilepPageTable);
  fclose(outputfileMaineMemory);
}
void swapping(unsigned char *mainMemory, unsigned char *diskMemory, unsigned int pfn, unsigned int vpn)
{
  printf("Swapping\n");
  int savedFrame = -1;
  for (int i = 2; i < NUMBER_OF_FRAMES; ++i)
  {
    if (mainMemory[i * PAGE_SIZE] == INITIAL_VALUE)
    {
      for (int j = 0; j < PAGE_SIZE; ++j)
      {
        mainMemory[(i * PAGE_SIZE) + j] = diskMemory[(pfn * PAGE_SIZE) + j];
      }
      savedFrame = i;
      i = NUMBER_OF_FRAMES;
    }
  }
  mainMemory[vpn] = savedFrame;
  mainMemory[vpn + PAGE_SIZE] = 0x01;
}
void getUserInput(unsigned char *mainMemory, unsigned char *diskMemory)
{
  int answer = 1;
  while (answer != 2)
  {
    printf("Please select the options from the menu\n");
    printf("\t1. Get the physical address\n");
    printf("\t2. Exit\n");
    printf("Option selected: ");
    scanf("%d", &answer);
    if (answer == 1)
    {
      unsigned int userInput;
      printf("Enter the virtual memory address in hexadecimal form (e.g. ffff):");
      scanf("%04X", &userInput);
      printf("\n");

//https://2019-moodle.dkit.ie/pluginfile.php/567340/mod_resource/content/2/32bit-address-parts.c
      unsigned int offset = userInput & 0x00FF;
      unsigned int vpn = userInput >> 8;
      unsigned int pfn = mainMemory[vpn];
      unsigned int address = pfn << 8;
      address |= offset;
      if (pfn != INITIAL_VALUE)
      {
        if (mainMemory[vpn + PAGE_SIZE] == 0x01)
        {
          printf("\tThe data save in the address 0x%x is %c\n", address, mainMemory[address]);
        }
        else
        {
          //swapping
          swapping(mainMemory, diskMemory, pfn, vpn);
        }
      }
      else
      {
        printf("\tThe address: %x is not storing any data\n", address);
      }
    }
    else if (answer != 2)
    {
      printf("\tPlease enter valid option\n");
    }
    printf("\n");
  }
}
