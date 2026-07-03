#ifndef ELF_DISPLAY
#define ELF_DISPLAY

#include <string.h>
#include <stdlib.h>
#include "elf.h"

enum Command {ALL, PROGRAM, SECTION};

void display_section_headers(elf64sectionheader_s* section_hdr_arr, uint16_t num_entries);
void display_program_headers(elf64programheader_s* prog_hdr_arr, uint16_t num_entries);
void display_elf_header(elf64header_s* elf_hdr);
const char * elf_type_string(uint32_t type);
const char * elf_machine_string(uint32_t machine);
const char * section_type_string(uint32_t type);
const char * program_type_string(uint32_t type);
const char * section_flag_string(uint32_t flag);
enum Command get_command(char * input_cmd);

#endif