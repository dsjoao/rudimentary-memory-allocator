#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <windows.h> explained later

#define HEAP_MAX_SIZE (64 * 1024 * 1024) // suposed 64mb capacity 
#define HEAP_DATA_SIZE sizeof(heap_data) // size of everything heap_data carries (two pointers, 8 bit flag and 32 bit value)

/* unsigned long long get_max_heap() {
   MEMORYSTATUSEX current;
   current.dwLength = sizeof(current);    this function got the actual heap capacity the os allocates but due to constraints with the
                                          algorithms in the code it was scrapped for a suposed capacity. Might be implemented soon
   if (GlobalMemoryStatusEx (&current))
      return current.ullAvailVirtual;
   return -1;
}
*/

typedef struct heap_data // data held with each "node" present in the heap
{
   uint32_t size;
   uint8_t used;
   heap_data* next;
   heap_data* previous;

} heap_data;

// the entire heap in a array of bytes and a pointer to it in the structure of a heap "node" presented above
static unsigned char heap_area[HEAP_MAX_SIZE]; 
static heap_data *heapinit = (heap_data *)heap_area;





int main (int argc, char *argv[]) {

  

       return EXIT_SUCCESS;
}