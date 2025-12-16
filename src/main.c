#include "elf_parser.h"
#include "elf_loader.h"


int main(){
    int fd = open_exe("test");
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

    uint16_t num_entries = elf_header->e_phnum-1;

    elf64programheader_s prog_arr[num_entries];
    
    if(read_program_headers(fd, prog_arr, num_entries, elf_header->e_phoff, elf_header->e_phentsize) != 0){
        printf("Failed to read in program header\n");
    }

    display_program_headers(prog_arr,num_entries);

    // reloc_memsz_s* reloc_data = get_total_memsz(prog_arr, num_entries, fd);

    // void* mmap_mem = mmap_total_mem(reloc_data);

    create_child(prog_arr, fd, num_entries);

    free(elf_header);
    close(fd);

    return 0;
}