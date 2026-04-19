#include "elf_parser.h"
#include "elf_display.h"


int main(){
    int fd = open_elf_file("test_bss_data");
    if(fd < 0){
        return -1;
    }
    printf("Opened ELF\n");

    elf64header_s* elf_hdr = calloc(1, sizeof(elf64header_s));

    if(read_elf_header(fd, elf_hdr) != 0){
        perror("Failed to read in ELF header\n");
        return -1;
    }

    printf("Read ELF Header\n");


    display_elf_header(elf_hdr);

    uint16_t num_prog_entries = elf_hdr->e_phnum;

    elf64programheader_s prog_hdr_arr[num_prog_entries];

    if(read_program_headers(fd, prog_hdr_arr, num_prog_entries, elf_hdr->e_phoff, elf_hdr->e_phentsize) < 0){
        printf("Failed to read in program headers\n");
        return -1;
    }

    display_program_headers(prog_hdr_arr, num_prog_entries);

    size_t num_section_entries = elf_hdr->e_shnum;

    elf64sectionheader_s section_hdr_arr[num_section_entries];

    if(read_section_headers(fd, section_hdr_arr, num_section_entries, elf_hdr->e_shoff, elf_hdr->e_shentsize) < 0){
        printf("Failed to read in section headers\n");
        return -1;
    }

    display_section_headers(section_hdr_arr, num_section_entries);


    free(elf_hdr);
    close(fd);

    return 0;
}