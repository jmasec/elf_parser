#include "elf_parser.h"
#include "elf_loader.h"


int main(){
    int fd = open_exe("test_bss_data");
    if(-1 == fd){
        return -1;
    }

    elf64header_s* elf_header = calloc(1, sizeof(elf64header_s));

    if(read_elf_header(fd, elf_header) != 0){
        printf("Failed to read in elf header\n");
        return -1;
    }

    if(elf_check_supported(elf_header) != true){
        return -1;
    }

    display_elf_header(elf_header);

    uint16_t num_entries = elf_header->e_phnum;

    uintptr_t entry_offset = elf_header->e_entry;

    elf64programheader_s prog_arr[num_entries];
    
    if(read_program_headers(fd, prog_arr, num_entries, elf_header->e_phoff, elf_header->e_phentsize) != 0){
        printf("Failed to read in program header\n");
    }

    display_program_headers(prog_arr,num_entries);

    uint16_t num_entries_sec = elf_header->e_shnum;

    elf64sectionheader_s shdr_arr[num_entries_sec];

    if(read_section_headers(fd, shdr_arr, num_entries_sec,  elf_header->e_shoff, elf_header->e_shentsize) != 0){
        printf("Failed to read in section headers\n");
    }

    display_section_headers(shdr_arr, num_entries_sec);

    // create_child(prog_arr, fd, num_entries, entry_offset);

    printf("%lu\n", sizeof(elf64sectionheader_s));

    free(elf_header);
    close(fd);

    return 0;
}