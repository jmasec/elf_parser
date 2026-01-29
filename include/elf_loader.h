
#include "elf_parser.h"

#ifndef ELF_LOADER
#define ELF_LOADER


// trying to not need to loop through program sections all the time
// struct to hold offsets and sizes?
// or we just loop through headers many times, not big
// typedef struct phdr_description_s{

// } phdr_description_s;

typedef struct elfsize_s{
    Elf64_Addr max_vaddr;
    Elf64_Addr min_vaddr;
    size_t total_size;
} elfsize_s;


size_t prog_section_size(elf64programheader_s *phdr_arr, uint16_t num_entries);
void create_child(elf64programheader_s* arr, int fd, uint16_t num_entries, uintptr_t entry_offset);
void *mmap_prog_section(size_t total_size);
void *load_segment_to_memory(void *mem, elf64programheader_s phdr, int elf_fd, elfsize_s *sizes);
void load_ptload_segements(void* mem, elf64programheader_s *phdr_arr, int fd, uint16_t num_entries, elfsize_s *sizes);
void change_mem_protection(void* mem, size_t size, int flag);

#endif