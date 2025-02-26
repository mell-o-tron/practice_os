#include "paging_utils.h"
#include "paging.h"


void initialize_page_directory (unsigned int* page_directory){
  int i;
    for(i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000000 | (PT_READWRITE);    // Last 3 bits are 010:
                                                            // - U/S = 0 : only the supervisor can access the page
                                                            // - R/W = 1 : the page is read/write
                                                            // - P   = 0 : the page is not present
    }   
}


void add_page (unsigned int * page_directory, unsigned int * page_table, int index, int offset, PT_FLAGS ptf, PD_FLAGS pdf){
    for(unsigned int i = 0; i < 1024; i++) {
        page_table[i] = (offset + (i * 0x1000)) | pdf; // U/S, R/W same. P = 1 present.
    }

    page_directory[index] = ((unsigned int)page_table) | ptf;
}

void init_paging(unsigned int * page_directory, unsigned int * first_page_table) {

    initialize_page_directory(page_directory);

    PD_FLAGS page_directory_flags = PD_PRESENT | PD_READWRITE;
    PT_FLAGS first_page_table_flags = PT_PRESENT | PT_READWRITE;

    add_page(page_directory, first_page_table,  0, 0, first_page_table_flags, page_directory_flags);


    loadPageDirectory(page_directory);
    enablePaging();
}

void stop_paging(){
    disablePaging();
}

void switch_page_directory(unsigned int * page_directory){
    loadPageDirectory(page_directory);
    enablePaging();
}
