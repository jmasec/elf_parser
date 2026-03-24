#include "elf_parser.h"

int open_elf_file(const char* elf_path){
    int fd = open(elf_path, O_RDONLY);

    if(fd < 0){
        perror("Error opening ELF file");
        return -1;
    }

    return fd;
}

int read_elf_header(int fd, elf64header_s* elf_hdr){
    size_t num_bytes = pread(fd, elf_hdr, sizeof(elf64header_s), 0);

    if (num_bytes < 0 || num_bytes == 0){
        printf("Nothing was read, ELF read failed");
        return -1;
    }

    return 0;
}

int read_program_headers(int fd, elf64programheader_s* prog_hdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size){

    for(int i = 0; i < num_entries; i++){
        size_t num_bytes = pread(fd, prog_hdr_arr, phent_size, prog_hdr_offset);
        if(num_bytes < 0){
            perror("Error reading program headers\n");
            return -1;
        }
        prog_hdr_arr++;
        prog_hdr_offset += sizeof(elf64programheader_s);
    }

    return 0;
}

int read_section_headers(int fd, elf64sectionheader_s* section_hdr_arr, uint16_t num_entries, size_t section_hdr_offset, uint16_t sh_entsize){
    
    for(int i = 0; i < num_entries; i++){
        size_t num_bytes = pread(fd, section_hdr_arr, sh_entsize, section_hdr_offset);
        if(num_bytes < 0){
            perror("Error reading section headers\n");
            return -1;
        }
        section_hdr_arr++;
        section_hdr_offset += sizeof(elf64sectionheader_s);
    }
    
    return 0;
}

void display_section_headers(elf64sectionheader_s* section_hdr_arr, uint16_t num_entries){
    printf("Section Headers:  \n");
    printf("    %d headers\n", num_entries);

    for(int i = 0; i < num_entries; i++){
        printf("Section Header %d\n", i);
        printf("    Name: %d\n", section_hdr_arr[i].sh_name);
        printf("    Type: %x\n", section_hdr_arr[i].sh_type);
        printf("    Flags: %lx\n", section_hdr_arr[i].sh_flags);
        printf("    Address: %lx\n", section_hdr_arr[i].sh_addr);
        printf("    Offset: %lx\n", section_hdr_arr[i].sh_offset);
        printf("    Section Size: %lx\n", section_hdr_arr[i].sh_size);
        printf("    Link: %lx\n", section_hdr_arr[i].sh_link);
        printf("    Info: %lx\n", section_hdr_arr[i].sh_info);
        printf("    Address Alignment: %lx\n", section_hdr_arr[i].sh_addralign);
        printf("    Entry Table Size: %lx\n", section_hdr_arr[i].sh_entsize);
        printf("\n\n");
    }
}

void display_program_headers(elf64programheader_s* prog_hdr_arr, uint16_t num_entries){
    printf("Program Headers:  \n");
    printf("    %d headers\n", num_entries);

    for(int i = 0; i < num_entries; i++){
        printf("Program Header %d\n", i);
        printf("    Type: %x\n", prog_hdr_arr[i].p_type);
        printf("    Flags: %x\n", prog_hdr_arr[i].p_flags);
        printf("    Offset: %x\n", prog_hdr_arr[i].p_offset);
        printf("    Virtual Address: %x\n", prog_hdr_arr[i].p_vaddr);
        printf("    Physical Address: %x\n", prog_hdr_arr[i].p_paddr);
        printf("    File Size: %x\n", prog_hdr_arr[i].p_filesz);
        printf("    Mem Size: %x\n", prog_hdr_arr[i].p_memsz);
        printf("    Align: %x\n", prog_hdr_arr[i].p_align);
        printf("\n\n");
    }
}

void display_elf_header(elf64header_s* elf_hdr){
    printf("ELF Header:\n");
    printf("    Magic: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n", elf_hdr->e_ident[EI_MAG0], elf_hdr->e_ident[EI_MAG1]
    , elf_hdr->e_ident[EI_MAG2], elf_hdr->e_ident[EI_MAG3], elf_hdr->e_ident[EI_CLASS], elf_hdr->e_ident[EI_DATA]
    , elf_hdr->e_ident[EI_VERSION], elf_hdr->e_ident[EI_OSABI], elf_hdr->e_ident[EI_ABIVERSION], elf_hdr->e_ident[EI_PAD]
    , elf_hdr->e_ident[10], elf_hdr->e_ident[11], elf_hdr->e_ident[12], elf_hdr->e_ident[13]
    , elf_hdr->e_ident[14], elf_hdr->e_ident[15] );

    printf("Entry: %lx\n", elf_hdr->e_entry);
    printf("Type: %x\n", elf_hdr->e_type);
}

bool elf_check_valid_file(elf64header_s* elf_hdr){
    if(elf_hdr->e_ident[EI_MAG0] != ELFMAG0){
        return false;
    }
    if(elf_hdr->e_ident[EI_MAG1] != ELFMAG1){
        printf("ELF header EI_MAG1 incorrect...\n");
        return false;
    }
    if(elf_hdr->e_ident[EI_MAG2] != ELFMAG2){
        printf("ELF header EI_MAG2 incorrect...\n");
        return false;
    }
    if(elf_hdr->e_ident[EI_MAG3] != ELFMAG3){
        printf("ELF header EI_MAG3 incorrect...\n");
        return false;
    }
    return true;

}

bool elf_check_support(elf64header_s* elf_hdr){
    if(elf_check_valid_file(elf_hdr) != true){
        printf("Invalid ELF file\n");
        return false;
    }
    if(elf_hdr->e_ident[EI_CLASS] != ELFCLASS64){
        printf("Unsupported ELF file class.\n");
        return false;
    }
    if(elf_hdr->e_ident[EI_DATA] != ELFDATA2LSB){
        printf("Unsupported ELF file byte order.\n");
        return false;
    }
    if(elf_hdr->e_machine != AMD_x86_64){
        printf("Unsupported ELF file target.\n");
        return false;
    }
    if(elf_hdr->e_ident[EI_VERSION] != EV_CURRENT){
        printf("Unsupported ELF file version.\n");
        return false;
    }
    if(elf_hdr->e_type != ET_REL && elf_hdr->e_type != ET_EXEC){
        printf("Unsupported ELF file type.\n");
        return false;
    }
    return true;
}