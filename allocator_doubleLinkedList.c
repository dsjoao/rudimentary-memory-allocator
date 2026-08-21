#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
// #include <windows.h> explained later

#define HEAP_MAX_SIZE (64 * 1024 * 1024) // suposed 64mb capacity 
#define HEAP_DATA_SIZE sizeof(heap_data) // size of everything heap_data carries (two pointers, 8 bit flag and 32 bit value)

// this function got the actual heap capacity the OS allocates but due to constraints with the
// algorithms in the code it was scrapped for a suposed capacity. A Unix based implementation could use 
// the sbrk() syscall for easier use of the heap. Might be implemented soon for current windows OS.

/*
 unsigned long long get_max_heap() {
   MEMORYSTATUSEX current;
   current.dwLength = sizeof(current);    
                                          
   if (GlobalMemoryStatusEx (&current))
      return current.ullAvailVirtual;
   return -1;
}
*/

// data held with each "node" present in the heap
typedef struct heap_data 
{
   uint32_t size;
   bool used;
   heap_data *next;
   heap_data *previous;

} heap_data;

// the entire heap in a array of bytes and a pointer to it in the structure of a heap "node" presented above
static unsigned char heap_area[HEAP_MAX_SIZE]; 
static heap_data *heap_init = (heap_data *)heap_area;

// initialize the pointer to the beginning of the heap
void init()
{
   static int counter = 0;
   heap_init->used = false;
   heap_init->size = HEAP_MAX_SIZE - HEAP_DATA_SIZE;
   heap_init->next = NULL;
   heap_init->previous = NULL;
}

void* new_malloc(size_t size)
{


   heap_data *current_heap_loc = heap_init;
   heap_data *new_heap_loc;

   
   while(current_heap_loc)
   {
      
      
      
   
   
   
   }



   return NULL;
}


int main (int argc, char *argv[]) 
{

  

       return EXIT_SUCCESS;
}