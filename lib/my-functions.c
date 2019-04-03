#include "my-functions.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void addTwoEntry(unsigned char *mainMemory)
{
  for (int i = 0; i < 256; ++i)
  {
    if (mainMemory[i] == 0x7e)
    {
      mainMemory[i] = 0x00;
      mainMemory[i + 256] = 0x00;
      mainMemory[i + 1] = 0x01;
      mainMemory[i + 1 + 256] = 0x00;
    }
  }
}
void assignDiskMemoryValue(unsigned char *diskMemory)
{
  FILE *outputfileDiskMemory;
  outputfileDiskMemory = fopen("/home/howdididie/solution/data/physical_memory.txt", "w");

  for (int i = 0; i < 20480; ++i)
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
    fprintf(outputfileDiskMemory, "0x%04x\t|%03d\t|%c\n", i, i / 256, diskMemory[i]);
  }
}
void assignMemoryValue(unsigned char *mainMemory)
{
  srand(time(0));
  int initialFrame = (rand() % (176 - 2)) + 2;
  int noOfData = (rand() % (20480 - 2048)) + 2048;

  for (int i = 0; i < 65536; ++i)
  {
    if (i >= initialFrame * 256 && i < (initialFrame * 256) + noOfData)
    {
      char randomChar = (rand() % (125 - 33)) + 33;
      mainMemory[i] = randomChar;
    }
    else
    {
      mainMemory[i] = '~';
    }
  }
  int pageRequired = noOfData % 256 == 0 ? noOfData / 256 : (noOfData / 256 + 1);
  for (int i = 0; i < 256; i++)
  {
    if (i < pageRequired)
    {
      mainMemory[i] = initialFrame;
      initialFrame++;
      mainMemory[i + 256] = 0x01;
    }
    else
    {
      mainMemory[i + 256] = 0x00;
    }
  }
  FILE *outputfileMaineMemory, *outputfilepPageTable;
  outputfileMaineMemory = fopen("/home/howdididie/solution/data/disk_memory.txt", "w");
  outputfilepPageTable = fopen("/home/howdididie/solution/data/page_table.txt", "w");

  for (int i = 0; i < 65536; i++)
  {
    if (i < 256)
    {
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|0x%02x\n", i, i / 256, mainMemory[i]);
      printf("%d: %x \n", i, mainMemory[mainMemory[i] * 256]);
      if (mainMemory[mainMemory[i] * 256] != 0x7e)
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
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|0x%02x\n", i, i / 256, mainMemory[i]);
    }
    else
    {
      fprintf(outputfileMaineMemory, "0x%04x\t|%03d\t|%c\n", i, i / 256, mainMemory[i]);
    }
  }
  fclose(outputfilepPageTable);
  fclose(outputfileMaineMemory);
}
void swapping(unsigned char *mainMemory, unsigned char *diskMemory, unsigned int pfn, unsigned int vpn)
{
  printf("Swapping is performing.\n");
  int savedFrame = -1;
  for (int i = 2; i < 256; ++i)
  {
    if (mainMemory[i * 256] == 0x7e)
    {
      for (int j = 0; j < 256; ++j)
      {
        mainMemory[(i * 256) + j] = diskMemory[(pfn * 256) + j];
      }
      savedFrame = i;
      i = 256;
    }
  }
  mainMemory[vpn] = savedFrame;
  mainMemory[vpn + 256] = 0x01;
}
void getUserInput(unsigned char *mainMemory, unsigned char *diskMemory)
{
  int answer = 1;
  while (answer != 2)
  {
    printf("Please select the options from the menu\n");
    printf("1. Get the physical address\n");
    printf("2. Exit\n");
    scanf("%d", &answer);
    if (answer == 1)
    {
      unsigned int userInput;
      printf("Please enter virtual memory address in hexadecimal form (without 0x):");
      scanf("%04X", &userInput);
      printf("\n");
      unsigned int offset = userInput & 0x00FF;
      unsigned int vpn = userInput >> 8;
      unsigned int pfn = mainMemory[vpn];
      unsigned int address = pfn << 8;
      address |= offset;
      if (pfn != 0x7e)
      {
        if (mainMemory[vpn + 256] == 0x01)
        {
          printf("The data save in the address %x is %c\n", address, mainMemory[address]);
        }
        else
        {
          //swapping
          swapping(mainMemory, diskMemory, pfn, vpn);
        }
      }
      else
      {
        printf("The address: %x is not storing any data\n", address);
      }

      printf("vpn: %x, pfn: %x, offset: %x, address: %x \n", vpn, pfn, offset, address);
    }
    else if (answer != 2)
    {
      printf("Please enter valid option\n");
    }
  }
}
