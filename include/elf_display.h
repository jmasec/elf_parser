#ifndef ELF_DISPLAY
#define ELF_DISPLAY

#include <string.h>
#include <stdlib.h>
#include "elf.h"

enum Command {ALL, PROGRAM, SECTION, HELP};

void display_section_headers(elf64sectionheader_s* section_hdr_arr, uint16_t num_entries);
void display_program_headers(elf64programheader_s* prog_hdr_arr, uint16_t num_entries);
void display_elf_header(elf64header_s* elf_hdr);
enum Command get_command(char * input_cmd);
enum elf_ptr_type_s get_elf_type(char * input_type);
void help();

#endif