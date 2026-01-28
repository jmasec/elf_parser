#include "elf_loader.h"
#include <string.h>


void create_child(elf64programheader_s* arr, int fd, uint16_t num_entries, uintptr_t entry_offset){
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

        printf("HERE");

        load_ptload_segements(mem, arr, fd, num_entries);

        // call the entry point here base + entry offset
        void (*entry)(void);

        entry = (char *)mem + entry_offset;

        (*entry)();

        printf("After entry executed");
    }
}

size_t prog_section_size(elf64programheader_s *phdr_arr, uint16_t num_entries){
    size_t total_size = 0;
    int count = 0;
    //TODO : add when filesz and memsz are the same to handle bss
    // need to track to zero out that mem and increase total mem size
    for(int i = 0; i < num_entries; i++ ){
        if(phdr_arr[i].p_type == PT_LOAD){
            total_size += phdr_arr[i].p_filesz;
            count = i;
        }
    }

    total_size += phdr_arr[count].p_vaddr;

    return total_size;
}

void *mmap_prog_section(size_t total_size){
    void *base = mmap(NULL, total_size, PROT_READ | PROT_WRITE | PROT_EXEC, 
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return base;
}

void *load_segment_to_memory(void *mem, elf64programheader_s phdr, int elf_fd) {
    // size_t mem_size = phdr.p_memsz;
    off_t mem_offset = phdr.p_offset;
    size_t file_size = phdr.p_filesz;
    uintptr_t vaddr_offset = (void *)(phdr.p_vaddr);
    int prot = 0;
    if (phdr.p_flags & PF_R) prot |= PROT_READ;
    if (phdr.p_flags & PF_W) prot |= PROT_WRITE;
    if (phdr.p_flags & PF_X) prot |= PROT_EXEC;

    // read from fd and get the PT_LOAD we need

    memcpy((char*)mem + vaddr_offset, mem, file_size);

    change_mem_protection(mem,file_size, prot);

}

void change_mem_protection(void* mem, size_t size, int flag){
    return;
}

void load_ptload_segements(void* mem, elf64programheader_s *phdr_arr, int fd, uint16_t num_entries){
    for(int i = 0; i < num_entries; i++ ){
        load_segment_to_memory(mem, phdr_arr[i], fd);
        phdr_arr++;
    }
}
