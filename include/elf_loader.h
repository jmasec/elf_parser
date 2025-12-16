
#include "elf_parser.h"

#ifndef ELF_LOADER
#define ELF_LOADER


// trying to not need to loop through program sections all the time
// struct to hold offsets and sizes?
// or we just loop through headers many times, not big
// typedef struct phdr_description_s{

// } phdr_description_s;

size_t prog_section_size(elf64programheader_s *phdr_arr, uint16_t num_entries);
void create_child(elf64programheader_s* arr, int fd, uint16_t num_entries);
void *mmap_prog_section(size_t total_size);
void *load_segment_to_memory(void *mem, elf64programheader_s phdr, int elf_fd);
void load_ptload_segements(void* mem, elf64programheader_s *phdr_arr, int fd, uint16_t num_entries);

#endif