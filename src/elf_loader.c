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
        elfsize_s* sizes = prog_section_size(arr, num_entries);
        printf("TOTAL: %ld\n", sizes->total_size);

        void* mem = mmap_prog_section(sizes->total_size);

        load_ptload_segements(mem, arr, fd, num_entries, sizes);

        // call the entry point here base + entry offset
        void (*entry)(void);

        entry = (char *)mem + (entry_offset - sizes->min_vaddr);

        (*entry)();
    }
}

size_t prog_section_size(elf64programheader_s *phdr_arr, uint16_t num_entries){
    size_t total_size = 0;
    Elf64_Addr max_vaddr = 0;
    Elf64_Addr min_vaddr = phdr_arr[0].p_vaddr;
    size_t max_size = 0;
    elfsize_s* sizes = (elfsize_s*)malloc(sizeof(elfsize_s));
    //TODO : add when filesz and memsz are not the same to handle bss
    // need to track to zero out that mem and increase total mem size
    for(int i = 0; i < num_entries; i++ ){
        if(phdr_arr[i].p_type == PT_LOAD){
            if (phdr_arr[i].p_vaddr > max_vaddr){
                max_vaddr = phdr_arr[i].p_vaddr;
                max_size = phdr_arr[i].p_filesz;
            }
            if(phdr_arr[i].p_vaddr < min_vaddr){
                min_vaddr = phdr_arr[i].p_vaddr;
            }
        }
    }

    sizes->max_vaddr = max_vaddr;
    sizes->min_vaddr = min_vaddr;
    sizes->total_size = max_vaddr + max_size;;

    return sizes;
}

void *mmap_prog_section(size_t total_size){
    void *base = mmap(NULL, total_size, PROT_READ | PROT_WRITE | PROT_EXEC, 
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    return base;
}

void *load_segment_to_memory(void *mem, elf64programheader_s phdr, int elf_fd, elfsize_s *sizes) {
    // size_t mem_size = phdr.p_memsz;
    off_t mem_offset = phdr.p_offset;
    size_t file_size = phdr.p_filesz;
    uintptr_t vaddr = (void *)(phdr.p_vaddr);
    int prot = 0;
    if (phdr.p_flags & PF_R) prot |= PROT_READ;
    if (phdr.p_flags & PF_W) prot |= PROT_WRITE;
    if (phdr.p_flags & PF_X) prot |= PROT_EXEC;

    // read from fd and get the PT_LOAD we need
    size_t bytes_read = pread(elf_fd, (char*)mem + (vaddr - sizes->min_vaddr), file_size, mem_offset);

    if(bytes_read < 0){
        printf("Error while reading file");
    }

    //change_mem_protection(mem,file_size, prot);

}

void change_mem_protection(void* mem, size_t size, int flag){
    return;
}

void load_ptload_segements(void* mem, elf64programheader_s *phdr_arr, int fd, uint16_t num_entries, elfsize_s *sizes){
    for(int i = 0; i < num_entries; i++ ){
        load_segment_to_memory(mem, phdr_arr[i], fd, sizes);
        phdr_arr++;
    }
}
