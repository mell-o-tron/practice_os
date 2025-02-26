#include "../utils/typedefs.h"
#include "dynamic_mem.h"
#include "mem.h"
#include "../data_structures/allocator.h"
#include "../drivers/vga_text.h"
#include "../utils/conversions.h"


// A bitmap is used to keep track of the memory usage. 

allocator_t* kmallocator;

volatile void *dynamic_mem_loc = NULL;

void assign_kmallocator(allocator_t* allocator){
    kmallocator = allocator;
}

void set_dynamic_mem_loc (void *loc){
    dynamic_mem_loc = loc;
}

bitmap_t get_kmallocator_bitmap (){
    return get_allocator_bitmap(kmallocator);
}

void set_kmalloc_bitmap (bitmap_t loc, int length){
    set_alloc_bitmap(kmallocator, loc, length);
}

void * kmalloc (int size){
    int pos = allocate(kmallocator, size);
    
    return (void*)((int)dynamic_mem_loc + pos);
}

int kfree(void* loc, int size){
    int bitmap_index = (uint32_t) loc - (uint32_t) dynamic_mem_loc;

    return allocator_free(kmallocator, bitmap_index, size);
}

// like free, but zeroes out the memory
int kdisintegrate(void* loc, int size){
    int free_res = kfree(loc, size);
    if (free_res < 0)
        return free_res;
    
    for (int i = 0; i < size; i++){
        ((char*)loc)[i] = 0;
    }
    return 0;
}

void* krealloc (void* oldloc, int oldsize, int newsize){
                                                            // switch this to 1 to change realloc mode
#if 0
    void* newloc = kmalloc(newsize);
    if (newloc == NULL) return NULL;
    kfree(oldloc, oldsize);                                 // no risk to lose reference
    
    int min = (oldsize > newsize) ? newsize : oldsize;
    memcp(oldloc, newloc, min);
    return newloc;
#else
    kfree(oldloc, oldsize);                                 // less fragmentation this way, but if no memory there is risk to lose a reference.
    void* newloc = kmalloc(newsize);
    if (newloc == NULL) return NULL;
    
    int min = (oldsize > newsize) ? newsize : oldsize;
    memcp(oldloc, newloc, min);
    return newloc;    
    
#endif
}
