#include "elf_loader.h"
#include <string.h>


void create_child(elf64programheader_s* arr, int fd, uint16_t num_entries){
    pid_t child_pid;

    child_pid = fork();

    if(child_pid < 0){
        perror("Fork Failed");
        exit(1);
    }
    else if (child_pid == 0){
        size_t total_size = prog_section_size(arr, num_entries);
        printf("TOTAL: %ld\n", total_size);

        void* mem = mmap_prog_section(total_size);
    }
}

size_t prog_section_size(elf64programheader_s *phdr_arr, uint16_t num_entries){
    size_t total_size = 0;
    //TODO : add when filesz and memsz are the same to handle bss
    // need to track to zero out that mem and increase total mem size
    for(int i = 0; i < num_entries; i++ ){
        if(phdr_arr[i].p_type == PT_LOAD){
            total_size += phdr_arr[i].p_filesz;
        }
    }

    return total_size;
}

void *mmap_prog_section(size_t total_size){
    void *base = mmap(NULL, total_size, PROT_READ | PROT_WRITE, 
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return base;
}

void *load_segment_to_memory(void *mem, elf64programheader_s phdr, int elf_fd) {
    // size_t mem_size = phdr.p_memsz;
    // how will I get the actual data for program section, wont the vaddr be different
    // and fd be invalid in the child process? they share virtual and fds nvm
    off_t mem_offset = phdr.p_offset;
    size_t file_size = phdr.p_filesz;
    void* vaddr = phdr.p_vaddr;
    int prot = 0;
    if (phdr.p_flags & PF_R) prot |= PROT_READ;
    if (phdr.p_flags & PF_W) prot |= PROT_WRITE;
    if (phdr.p_flags & PF_X) prot |= PROT_EXEC;

    memcpy(mem, vaddr, file_size);

    change_mem_prot(mem,file_size, prot);

}

void change_mem_prot(void* mem, size_t size, int flag){

}

void load_ptload_segements(void* mem, elf64programheader_s *phdr_arr, int fd, uint16_t num_entries){
    for(int i = 0; i < num_entries; i++ ){
        load_segment_to_memory(mem, phdr_arr[i], fd);
        phdr_arr++;
    }
}
