#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include <windows.h> explained later

#define HEAP_MAX_SIZE (64 * 1024 * 1024) // suposed 64mb capacity 
#define HEAP_DATA_SIZE (uint64_t)sizeof(heap_data) // size of everything heap_data carries ( pointer, and 32 bit value)


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
   uint64_t size;
   struct heap_data *next;

} heap_data;

// the entire heap in a array of bytes and a pointer to it in the structure of a heap "node" presented above with a counter 
static unsigned char heap_area[HEAP_MAX_SIZE]; 
static heap_data *heap_init = (heap_data *)heap_area;


// initialize the pointer to the beginning of the heap
void init()
{
   heap_init->size = HEAP_MAX_SIZE - HEAP_DATA_SIZE;
   heap_init->next = NULL;
}



// allocate new memory by checking the best block available traversing the lits of free regions on the heap from the start with best-fit logic 
// tried some algos before but the one implemented in the article is the currently best for malloc and free in here so in the end it's implementation is almost identical
void* new_malloc(uint64_t size)
{
   
   heap_data *current_heap_loc;
   heap_data *best_heap_fit;
   uint64_t best_block_size;

   
   current_heap_loc = heap_init;
   
   best_heap_fit = (heap_data *)NULL;
   best_block_size = HEAP_MAX_SIZE;
  
   while (current_heap_loc)
   {
      
      if ((current_heap_loc->size >= (size + HEAP_DATA_SIZE)) &&
          (current_heap_loc->size <= best_block_size))
      {
        
         best_heap_fit = current_heap_loc;
         best_block_size = current_heap_loc->size;
      }
      current_heap_loc = current_heap_loc->next;
   }



   if (best_heap_fit != NULL)
   {
      heap_data *heap_new_loc;
     
      best_heap_fit->size = best_heap_fit->size - size - HEAP_DATA_SIZE;

      heap_new_loc = (heap_data *)(((unsigned char *)best_heap_fit) + HEAP_DATA_SIZE + best_heap_fit->size);
      heap_new_loc->size = size;

     
      return (void *)((unsigned char *)heap_new_loc + HEAP_DATA_SIZE);
   }
   puts("erro\n");
   return NULL;
}

// free the memory (it goes to the list of possible "nodes" to allocate future arrays)
void new_free(void* ptr)
{

   heap_data *before; 
   heap_data *after;

   if(ptr == NULL) return;

   heap_data *freeing_block = (heap_data*)((unsigned char*)ptr - HEAP_DATA_SIZE);

   if(freeing_block == NULL) { puts("error"); return;}

   before = NULL; 
   after = heap_init;

   while (after != NULL && after < freeing_block) {
       
      before = after;
      after = after->next;

   }

   freeing_block->next = after;

   if (before != NULL)
   {
      before->next = freeing_block;
   }
   else
   {
      heap_init = freeing_block; 
   }

   if ((after != NULL) &&
       (((unsigned char *)freeing_block + freeing_block->size + HEAP_DATA_SIZE) == (unsigned char *)after))
   {
      freeing_block->size += after->size + HEAP_DATA_SIZE;
      freeing_block->next = after->next;
   }

   if ((before != NULL) &&
       (((unsigned char *)before + before->size + HEAP_DATA_SIZE) == (unsigned char *)freeing_block))
   {
      before->size += freeing_block->size + HEAP_DATA_SIZE;
      before->next = freeing_block->next;
   }


   puts("array freed");

}  

// copy an region of memory to a specific size
void new_memcpy(void *out, void *in, size_t size)
{

   if(out == NULL || in == NULL) return;

   unsigned char *temp_out = (unsigned char *)out;
   unsigned char *temp_in  = (unsigned char *)in;

   for (size_t i = 0; i < size; ++i)
      *(temp_out + i) = *(temp_in + i);
}

// set a region to a value
void new_memset(uint64_t set, void* ptr, size_t size)
{

   if(ptr == NULL) return;

   unsigned char *p = (unsigned char *)ptr;

   for (size_t i = 0; i < size; ++i)
      *(p + i) = set;
   
}

// reallocate an alrealdy existing region, changing it's size and keeping wathever content still fits 
void* new_realloc(void* ptr, size_t size)
{

   if(ptr == NULL) {puts("error");return ptr;}
   
   void *p = NULL;
   
   heap_data *new_loc = (heap_data *)((unsigned char *)ptr - HEAP_DATA_SIZE);
   
   p = new_malloc(size);
   
  
   uint64_t copy_size = size;

   // if original size is smaller the copied content will just be it, if its greater then the copied will be until the demanded new size
   if (new_loc->size < size)  
      copy_size = new_loc->size;

   
   new_memcpy(p,ptr,copy_size);

   if (p == NULL) puts("error");

   new_free(ptr);

   
   return p;
}

// allocate array by number of elements and they size, all initialize with 0
void* new_calloc(size_t num, size_t size)
{

   void* p = new_malloc(num * size);

   new_memset(0, p, num * size);

   return p;   

}

// for testing purposes, this will check the allocated array
void traverse_array(void *ptr, size_t size, size_t element_size)
{
   
   if(ptr == NULL) {puts("null"); return; }

   unsigned char *p = (unsigned char *)ptr;

      
       for (size_t i = 0; i < size; ++i)
           printf("element %zu: %d\n", i + 1, *(p + i * element_size));
}

// just init() ans tests, everything in order
int main (int argc, char *argv[]) 
{

   init();

   puts("MALLOC\n");
   int *p = (int*)new_malloc(4 * sizeof(int));

   traverse_array(p,4 ,sizeof(int));

   puts("\nREALLOC 1\n");

   int *d = new_malloc(4 * sizeof(int));

   for(int i = 0; i <= 4; ++i) {d[i]  = 7 * i;}

   int *q = (int*)new_realloc(d, 6 * sizeof(int));

   traverse_array(q,6 ,sizeof(int));

   puts("\nREALLOC 2\n");

   int *r = (int *)new_realloc(q, 3 * sizeof(int));

   traverse_array(r, 3 , sizeof(int));

   puts("\nCALLOC\n");

   int *j = new_calloc(20, sizeof(int));

   traverse_array(j,20, sizeof(int));

   return EXIT_SUCCESS;
}