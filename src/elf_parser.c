#include "elf_parser.h"

int open_elf_file(const char* elf_path){
    int fd = open(elf_path, O_RDONLY);

    if(fd < 0){
        perror("Error opening ELF file");
        return -1;
    }

    return fd;
}

bool read_elf_header(elfptr_s* elf_ptr, elf64header_s* elf_hdr){
    switch (elf_ptr->type)
    {
    case ELF_FILE_DESCRIPTOR: {
        ssize_t num_bytes = pread(elf_ptr->edata.e_fd, elf_hdr, sizeof(elf64header_s), 0);

        if (num_bytes < 0 || num_bytes == 0){
            printf("Nothing was read, ELF read failed");
            return false;
        }

        return true;
    }
    case ELF_MEMORY_POINTER: {
        memcpy(elf_hdr, elf_ptr->edata.e_ptr, sizeof(elf64header_s));

        if (NULL == elf_hdr){
            printf("Nothing was read, ELF read failed");
            return false;
        }

        return true;
    }
    default:
        printf("Should never get here, something is wrong!");
        return false;
    }
}

bool read_program_headers(elfptr_s* elf_ptr, elf64programheader_s* prog_hdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size){

    switch (elf_ptr->type)
    {
    case ELF_FILE_DESCRIPTOR:{
        for(int i = 0; i < num_entries; i++){
            ssize_t num_bytes = pread(elf_ptr->edata.e_fd, &prog_hdr_arr[i], phent_size, prog_hdr_offset);
            if(num_bytes < 0){
                perror("Error reading program headers\n");
                return false;
            }
            prog_hdr_offset += phent_size;
        }

        return true;
    }
    case ELF_MEMORY_POINTER:{
        for(int i = 0; i < num_entries; i++){
            memcpy(&prog_hdr_arr[i], (elf_ptr->edata.e_ptr + prog_hdr_offset), phent_size);

            if(NULL == &prog_hdr_arr[i]){
                printf("Error reading in number:%d program header", i);
                return false;
            }
            prog_hdr_offset += phent_size;
        }

        return true;
    }
    default:
        printf("Should never get here, something is wrong!");
        return false;
    }
}

bool read_section_headers(elfptr_s* elf_ptr, elf64sectionheader_s* section_hdr_arr, uint16_t num_entries, size_t section_hdr_offset, uint16_t sh_entsize){
    
    switch (elf_ptr->type)
    {
    case ELF_FILE_DESCRIPTOR:{
        for(int i = 0; i < num_entries; i++){
            ssize_t num_bytes = pread(elf_ptr->edata.e_fd, &section_hdr_arr[i], sh_entsize, section_hdr_offset);
            if(num_bytes < 0){
                perror("Error reading section headers\n");
                return false;
            }
            section_hdr_offset += sh_entsize;
        }
    
        return true;
    }
    case ELF_MEMORY_POINTER:{
        for(int i = 0; i < num_entries; i++){
            memcpy(&section_hdr_arr[i], (elf_ptr->edata.e_ptr + section_hdr_offset), sh_entsize);

            if(NULL == &section_hdr_arr[i]){
                printf("Error reading in number:%d program header", i);
                return false;
            }
            section_hdr_offset += sh_entsize;
        }

        return true;
    }

    default:
        printf("Should never get here, something is wrong!");
        return false;
    }
}

/*
in: elf -> file path or void ptr
in: type -> either file descriptor or void ptr
out: elf_internel -> struct to hold information about the elf executable
*/
bool parse_elf_internal(const char* elf, elf_ptr_type_s type, elfinternal_s* elf_internel){
    elf64header_s* elf_header = NULL;
    elf64programheader_s* prg_hdrs = NULL;
    elf64sectionheader_s* sct_hdrs = NULL;
    elfptr_s* elf_ptr = NULL;
    int num_prg_hdrs = 0;
    int num_sct_hdrs = 0;

    switch (type){
        case ELF_FILE_DESCRIPTOR:{
            // if fd. then we call open_elf_file, if not then we just have a ptr to use
            int fd = open_elf_file(elf);
            if(fd < 0){
                return false;
            }

            elf_ptr = malloc(sizeof(elfptr_s));
            if(NULL == elf_ptr){
                goto cleanup;
            }
            elf_ptr->type = ELF_FILE_DESCRIPTOR;
            elf_ptr->edata.e_fd = fd;

            elf_header = malloc(sizeof(elf_header));
            if(!read_elf_header(elf_ptr, elf_header)){
                printf("Failed to read in ELF header\n");
                goto cleanup;
            }

            printf("Read ELF Header\n");

            // need to he lengths from elf header
            elf64programheader_s* pgm_hdrs = calloc(elf_header->e_phnum, sizeof(elf64programheader_s));
            if(!read_program_headers(elf_ptr, pgm_hdrs, elf_header->e_phnum, elf_header->e_phoff, elf_header->e_phentsize)){
                goto cleanup;
            }

            elf64sectionheader_s* sct_hdrs = calloc(elf_header->e_shnum, sizeof(elf64sectionheader_s));
            if(!read_section_headers(elf_ptr, sct_hdrs, num_sct_hdrs, elf_header->e_shoff, elf_header->e_shnum)){
                goto cleanup;
            }
            break;

        }
        case ELF_MEMORY_POINTER:{

            elf_ptr = malloc(sizeof(elfptr_s));
             if(NULL == elf_ptr){
                goto cleanup;
            }
            elf_ptr->type = ELF_MEMORY_POINTER;
            elf_ptr->edata.e_ptr = elf;

            elf_header = malloc(sizeof(elf_header));
            if(!read_elf_header(elf_ptr, elf_header)){
                goto cleanup;
            }

            // need to he lengths from elf header
            elf64programheader_s* pgm_hdrs = calloc(elf_header->e_phnum, sizeof(elf64programheader_s));
            if(!read_program_headers(elf_ptr, pgm_hdrs, elf_header->e_phnum, elf_header->e_phoff, elf_header->e_phentsize)){
                goto cleanup;
            }

            elf64sectionheader_s* sct_hdrs = calloc(elf_header->e_shnum, sizeof(elf64sectionheader_s));
            if(!read_section_headers(elf_ptr, num_sct_hdrs, elf_header->e_shnum, elf_header->e_shoff, elf_header->e_shentsize)){
                goto cleanup;
            }
            break;
        }
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

