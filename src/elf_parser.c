#include "elf_parser.h"

int open_elf_file(const char* elf_path){
    int fd = open(elf_path, O_RDONLY);

    if(fd < 0){
        perror("Error opening ELF file");
        return -1;
    }

    return fd;
}

bool read_elf_header(int fd, elf64header_s* elf_hdr){
    ssize_t num_bytes = pread(fd, elf_hdr, sizeof(elf64header_s), 0);

    if (num_bytes < 0 || num_bytes == 0){
        printf("Nothing was read, ELF read failed");
        return false;
    }

    return true;
}

bool read_program_headers(int fd, elf64programheader_s* prog_hdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size){

    for(int i = 0; i < num_entries; i++){
        ssize_t num_bytes = pread(fd, &prog_hdr_arr[i], phent_size, prog_hdr_offset);
        if(num_bytes < 0){
            perror("Error reading program headers\n");
            return false;
        }
        prog_hdr_offset += phent_size;
    }

    return true;
}

bool read_section_headers(int fd, elf64sectionheader_s* section_hdr_arr, uint16_t num_entries, size_t section_hdr_offset, uint16_t sh_entsize){
    
    for(int i = 0; i < num_entries; i++){
        ssize_t num_bytes = pread(fd, &section_hdr_arr[i], sh_entsize, section_hdr_offset);
        if(num_bytes < 0){
            perror("Error reading section headers\n");
            return false;
        }
        section_hdr_offset += sh_entsize;
    }
    
    return true;
}

bool parse_elf_internal(const char* elf, elfinternal_s* elf_internel, elf_ptr_type_s type){
    elf64header_s* elf_header = NULL;
    elf64programheader_s* prg_hdrs = NULL;
    elf64sectionheader_s* sct_hdrs = NULL;
    elfptr_s* elf_ptr = NULL;
    int num_prg_hdrs = 0;
    int num_sct_hdrs = 0;

    if(type == ELF_FILE_DESCRIPTOR){
        // if fd. then we call open_elf_file, if not then we just have a ptr to use
        int fd = open_elf_file(elf);
        if(fd < 0){
            return false;
        }

        elf_header = malloc(sizeof(elf_header));
        if(!read_elf_header(fd, elf_header)){
            goto cleanup;
        }

        // need to he lengths from elf header
        elf64programheader_s* pgm_hdrs = calloc(elf_header->e_phnum, sizeof(elf64programheader_s));
        if(!read_program_headers(fd, pgm_hdrs, elf_header->e_phnum, elf_header->e_phoff, elf_header->e_phentsize)){
            goto cleanup;
        }

        elf64sectionheader_s* sct_hdrs = calloc(elf_header->e_shnum, sizeof(elf64sectionheader_s));
        if(!read_section_headers(fd, num_sct_hdrs, elf_header->e_shnum, elf_header->e_shoff, elf_header->e_shentsize)){
            goto cleanup;
        }

        elf_ptr = malloc(sizeof(elfptr_s));
        elf_ptr->type = ELF_FILE_DESCRIPTOR;
        elf_ptr->edata.e_fd = fd;

    }
    else{
        return false;
    }

    elf_internel->elf_ptr = elf_ptr;
    elf_internel->elf_hdr = elf_header;
    elf_internel->pgm_hdrs = prg_hdrs;
    elf_internel->sct_hdrs = sct_hdrs;

    return true;

cleanup:
    free(elf_header);
    free(prg_hdrs);
    free(sct_hdrs);
    return false;
}

