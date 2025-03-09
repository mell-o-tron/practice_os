# KWSL Workshop

*If you're here for the OS Development Workshop by KWSL:*

**!!!! Install the GCC cross compiler BEFORE the workshop (it can take > 30 minutes) !!!!** 

See "Building" section.

# PracticeOS

This is a *practice* version of the MellOS kernel. Its disk drivers and file system code are substituted by stubs. This way you can write your own without caring too much about the infrastructure.

## Building

You will find scripts to build the GCC cross compiler in A_Setup. This can take a lot of time (like 30 min).

- If you're using a Debian derivative, run the `setup-gcc-debian.sh` script. 
- If you're using Arch, run the `setup-gcc-arch.sh` script. 
- If you're using something else, you may either use an emulator or make your own script (and make a pull request)

If you're on Windows, you may use a Linux virtual machine as your development environment.



## The kernel

The kernel code is in `kernel/kernel.c`. It consists of the `main` function. It begins with some bureaucracy:

```c
    // identity-maps 0x0 to 4MB (i.e. 0x400000 - 1)
    init_paging(page_directory, first_page_table);

    // maps 4MB to 8MB (0x400000 to 0x800000 - 1) -> 16 MB to 20 MB (0x1000000 to 0x1400000 - 1)
    add_page(page_directory, second_page_table, 1, 0x1000000, first_page_table_flags, page_directory_flags);

    gdt_init();
    idt_install();
    isrs_install();
    irq_install();
    asm volatile ("sti");
    timer_install();
    clear_screen_col(DEFAULT_COLOUR);
    set_cursor_pos_raw(0);

    allocator_t allocator;
    assign_kmallocator(&allocator);

    set_kmalloc_bitmap((bitmap_t) 0x400000, 100000);   // dynamic memory allocation setup test
    set_dynamic_mem_loc ((void*)0x400000 + 100000/2);

    kb_install();
    initialize_fake_disk();
```

Which sets up paging, interrupts, memory allocation, the timer and keyboard drivers and the fake disk. Everything you implement, you will call from here.

## Useful functions and headers

| Header        | Function      | Description      |
| ------------- | ------------- | ------------- |
| `utils/typedefs.h` | - | Contains useful type defs, e.g. `uint32_t` |
| `misc/colours.h` | - | Defines some colours |
| `drivers/vga_text.h` |`void kprint(const char* s);` | Print to screen. |
|`drivers/vga_text.h`|`void clear_screen_col (Colour col);`| Clears the screen
|`drivers/vga_text.h`|`void kprint_col(const char* s, Colour col);`|Prints in colour
| `utils/conversions.h` | `const char* tostring_inplace(int n, int base);` | int to string in arbitrary base
| `memory/dynamic_mem.h` | `void * kmalloc (int size);` | Allocates `size` bytes to memory
|`memory/dynamic_mem.h`|`int kfree(void* loc, int size);`| Frees `size` bytes at location `loc`