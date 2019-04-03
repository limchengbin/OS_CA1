## Start the Programme
When the program is start, user has 2 option. 1 is to enter the virtual address, another 1 is to exit the programme. 
The user is also allow to use CTRL+C in order to exit the programme. 

## Pseudo-randomise
Using ASCII to create the content of the physical memory. This is to create a random character using the range 33 - 125 in the ASCII table. 
The initial value of the memory is 0x7e,~.
Initial frame number that use to store the content is also randomised between 2 ~ 176 as there will be only 80 pages to store 20480 
data. Storing the data will be continue until the system reach the number of data needed.

## Page Table Entry
The page table entry is storing the frame number, persent bit and valid bit.
Frame number: 
Present Bit: Indicate what pages are currently present in physical memory or are on disk, and can indicate how to 
treat these different pages. For example, to load a page from disk if the present bit is 0, means the data is stored ont the disk.

##Swapping
When the user enter a value which is store in disk memory, the system will raise a page fault 
and perform swapping, which put the memory in disk back to physical memory. When the user enter the same virtual 
address again, the system will return the data that is store in the address.
