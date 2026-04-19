#ifndef ELF_DISPLAY
#define ELF_DISPLAY

void display_section_headers(elf64sectionheader_s* section_hdr_arr, uint16_t num_entries);
void display_program_headers(elf64programheader_s* prog_hdr_arr, uint16_t num_entries);
void display_elf_header(elf64header_s* elf_hdr);
const char * elf_type_string(uint16_t type);
const char * elf_machine_string(uint16_t type);

#endif