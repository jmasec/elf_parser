#include "elf.h"
#include "elf_display.h"

void display_section_headers(elf64sectionheader_s* section_hdr_arr, uint16_t num_entries){
    printf("Section Headers:  \n");
    printf("    %d headers\n", num_entries);

    for(int i = 0; i < num_entries; i++){
        printf("Section Header %d\n", i);
        printf("    Name (Index): %d\n", section_hdr_arr[i].sh_name);
        char* type = section_type_string(section_hdr_arr[i].sh_type);
        printf("    Type: %s\n", type);
        printf("    Flags: %lx\n", section_hdr_arr[i].sh_flags);
        printf("    Address: 0x%lx\n", section_hdr_arr[i].sh_addr);
        printf("    Offset: 0x%lx\n", section_hdr_arr[i].sh_offset);
        printf("    Section Size: %d (bytes)\n", section_hdr_arr[i].sh_size);
        printf("    Link: %lx\n", section_hdr_arr[i].sh_link);
        printf("    Info: %lx\n", section_hdr_arr[i].sh_info);
        if(section_hdr_arr[i].sh_addralign == 1 || section_hdr_arr[i].sh_addralign == 0){
            printf("    Address Alignment: No Alignment Constraints\n");
        }
        else{
            printf("    Address Alignment: %lx\n", section_hdr_arr[i].sh_addralign);
        }
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
    printf("    Magic: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", elf_hdr->e_ident[EI_MAG0], elf_hdr->e_ident[EI_MAG1]
    , elf_hdr->e_ident[EI_MAG2], elf_hdr->e_ident[EI_MAG3], elf_hdr->e_ident[EI_CLASS], elf_hdr->e_ident[EI_DATA]
    , elf_hdr->e_ident[EI_VERSION], elf_hdr->e_ident[EI_OSABI], elf_hdr->e_ident[EI_ABIVERSION], elf_hdr->e_ident[EI_PAD]
    , elf_hdr->e_ident[10], elf_hdr->e_ident[11], elf_hdr->e_ident[12], elf_hdr->e_ident[13]
    , elf_hdr->e_ident[14], elf_hdr->e_ident[15] );

    const char* type_char = elf_type_string(elf_hdr->e_type);
    printf("    Type: %s\n", type_char);
    const char* machine_char = elf_machine_string(elf_hdr->e_machine);
    printf("    Machine: %s\n", machine_char);
    printf("    Version: 0x%02x\n", elf_hdr->e_version);
    printf("    Entry: 0x%lx\n", elf_hdr->e_entry);
    printf("    Program Header Offset: %d (bytes into the file)\n", elf_hdr->e_phoff);
    printf("    Section Header Offset: %d (bytes into the file)\n", elf_hdr->e_shoff);
    printf("    Flags: 0x%x\n", elf_hdr->e_flags);
    printf("    Size of ELF header: %d (bytes)\n", elf_hdr->e_ehsize);
    printf("    Size of Program Header: %d (bytes)\n", elf_hdr->e_phentsize);
    printf("    Size of Section Header: %d (bytes)\n", elf_hdr->e_shentsize);
    printf("    Number of Section Headers: %d\n", elf_hdr->e_shnum);
    printf("    Section header string table index: %d\n\n", elf_hdr->e_shstrndx);
}

const char * elf_type_string(uint16_t type){
    switch (type)
    {
    case 0:
        return "None (No file type)";
    case 1:
        return "REL (Relocatable file)";
    case 2:
        return "EXEC (Executable file)";
    case 3:
        return "DYN (Shared object file)";
    default:
        break;
    }
}

const char * elf_machine_string(uint16_t type){
    switch (type)
    {
    case AMD_x86_64:
        return "Advanced Micro Devices x86_64";
    case MIPS:
        return "MIPS Architecture";
    case x86:
        return "x86";
    case AARCH64:
        return "Advanced RISC Machines ARM 64bit";
    default:
        break;
    }
}

const char * section_type_string(uint16_t type){
    switch (type)
    {
    case SHT_NULL:
        return "Inactive (null)";
    case SHT_PROGBITS:
        return "PROGBITS (Info defined by program)"; 
    case SHT_SYMTAB:
        return "SYMTAB (Symbol Table)";
    case SHT_STRTAB:
        return "STRTAB (String Table)";
    case SHT_RELA:
        return "RELA (Relocation Entries with explicit addends)";
    case SHT_HASH:
        return "HASH (Symbol Hash Table)";
    case SHT_DYNAMIC:
        return "DYNAMIC (Dynamic Linking Information)";
    case SHT_NOTE:
        return "NOTE";
    case SHT_NOBITS:
        return "NOBITS";
    case SHT_REL:
        return "REL (Relocation entries with no explicit addends)";
    case SHT_SHLIB:
        return "RESERVED";
    case SHT_DYNSYM:
        return "DYNSYM (Symbol Table)";
    default:
        break;
    }
}

enum Command get_command(char * input_cmd){
    if(!strncmp(input_cmd, "-a", 2)){
        enum Command cmd = ALL;
        return cmd;
    }
    else if(!strncmp(input_cmd, "-p", 2)){
        enum Command cmd = PROGRAM;
        return cmd;
    }
    else if(!strncmp(input_cmd, "-s", 2)){
        enum Command cmd = SECTION;
        return cmd;
    }
    else{
        return -1;
    }
}