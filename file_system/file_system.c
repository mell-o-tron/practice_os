#include "../utils/typedefs.h"
#include "../memory/dynamic_mem.h"
#include "../drivers/disk.h"
#include "./file_system.h"
#include "../utils/string.h"
#include "../disk_interface/diskinterface.h"

void prepare_disk_for_fs (uint32_t n_sectors){
   kprint("prepare_disk_for_fs not implemented!\n");
}

int allocate_file(uint32_t req_sectors){
    kprint("allocate_file not implemented!\n");
    return -1;
}

int deallocate_file(uint32_t LBA, uint32_t num_sectors){
    kprint("deallocate_file not implemented!\n");
    return -1;
}

file_t* get_file_list (uint32_t disk, uint32_t LBA, uint32_t sectors){
    file_t* res = kmalloc(sizeof(file_t) * 32 * sectors);
    for (uint32_t i = 0; i < 32; i++) {
        res[i].n_sectors = 0;
        res[i].LBA = 0;
        res[i].name[0] = 0;
    }
    
    kprint("get_file_list not implemented!\n");

    return res;
}

void write_file_list (file_t * list, uint32_t disk, uint32_t LBA, uint32_t sectors){
     kprint("write_file_list not implemented!\n");
}


void new_file (char* name, uint32_t n_sectors){
    kprint("new_file not implemented!\n");
}

void remove_file(char* name){
    kprint("remove_file not implemented!\n");
}

void print_files(file_t * files, uint32_t n){
    kprint("print_files not implemented!\n");
}

void write_string_to_file(char* string, char* filename){
    kprint("write_string_to_file not implemented!\n");
}

char* read_string_from_file(char* filename){
    char* dummy = kmalloc(512);
    for (uint32_t i = 0; i < 512; i++) 
        dummy[i] = 0;
    
    kprint("read_string_from_file not implemented!\n");
    
    return dummy;
}
