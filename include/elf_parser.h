#ifndef ELF_PARSER
#define ELF_PARSER


#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>
#include "elf.h"

typedef struct elfinternal_s{
    elf64header_s* elf_hdr;
    elf64programheader_s* pgm_hdrs;
    elf64sectionheader_s* sct_hdrs;
    elfptr_s* elf_ptr;
} elfinternal_s;

int open_elf_file(const char* elf_path);
bool read_elf_header(int fd, elf64header_s* elf_hdr);
bool read_section_headers(int fd, elf64sectionheader_s* section_hdr_arr, uint16_t num_entries, size_t section_hdr_offset, uint16_t sh_entsize);
bool read_program_headers(int fd, elf64programheader_s* prog_hdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size);
bool parse_elf_internal(const char* elf, elfinternal_s* elf_internel, elf_ptr_type_s type);


#endif