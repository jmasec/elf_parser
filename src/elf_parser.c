#include <elf_parser.h>

/*
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 03 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - GNU
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x401740
  Start of program headers:          64 (bytes into file)
  Start of section headers:          783448 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         10
  Size of section headers:           64 (bytes)
  Number of section headers:         28
  Section header string table index: 27

  https://wiki.osdev.org/ELF_Tutorial 
*/

bool elf_check_file(elf64header_s* hdr){
    if(!hdr) return false;
    if(hdr->e_ident[EI_MAG0] != ELFMAG0){
        printf("ELF Header EI_MAG0 incorrect..\n");
        return false;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1){
        printf("ELF Header EI_MAG1 incorrect..\n");
        return false;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2){
        printf("ELF Header EI_MAG2 incorrect..\n");
        return false;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3){
        printf("ELF Header EI_MAG3 incorrect..\n");
        return false;
    } 
    return true;
}

bool elf_check_supported(elf64header_s* hdr){
    if(elf_check_file(hdr) != true){
        printf("Invalid ELF File.\n");
        return false;
    }
    if(hdr->e_ident[EI_CLASS] != ELFCLASS64){
        printf("Unsupported ELF File Class.\n");
        return false;
    }
    if(hdr->e_ident[EI_DATA] != ELFDATA2LSB){
        printf("Unsupported ELF File Byte Order.\n");
        return false;
    }
    if(hdr->e_machine != AMD_x86_64){
        printf("Unsupported ELF File Target.\n");
        return false;
    }
    if(hdr->e_ident[EI_VERSION] != EV_CURRENT){
        printf("Unsupported ELF File version.\n");
        return false;
    }
    if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC) {
		printf("Unsupported ELF File type.\n");
		return false;
	}
    return true;
}

int open_exe(const char* executable_path){
    int fd = open(executable_path, O_RDONLY);

    if(fd == -1){
        perror("Error opening executable");
        return -1;
    }

    return fd;
}

int read_elf_header(int fd, elf64header_s* elf_hdr){
    size_t num_bytes = read(fd, elf_hdr, sizeof(elf64header_s));
    if (num_bytes < 0){
        printf("Failed read");
        return -1;
    }

    if(lseek(fd, 0, SEEK_SET) < 0){
        printf("Failed lseek");
        return -1;
    }

    return 0;
}

int read_program_headers(int fd, elf64programheader_s* phdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size){
    if(lseek(fd, prog_hdr_offset, SEEK_SET) < 0){
        printf("Failed lseek");
        return -1;
    }
    for(int i = 0; i < num_entries; i++){
        size_t num_bytes = read(fd, phdr_arr,phent_size);
        if(num_bytes < 0){
            printf("Count: %i\n", num_bytes);
            return -1;
        }
        phdr_arr++;
        
    }
    if(lseek(fd, 0, SEEK_SET) < 0){
        printf("Failed lseek");
        return -1;
    }
    return 0;
}

void display_elf_header(elf64header_s* elf_hdr){
    printf("Elf Header:\n");
    printf("    Magic: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n", elf_hdr->e_ident[EI_MAG0], elf_hdr->e_ident[EI_MAG1]
    , elf_hdr->e_ident[EI_MAG2], elf_hdr->e_ident[EI_MAG3], elf_hdr->e_ident[EI_CLASS], elf_hdr->e_ident[EI_DATA]
    , elf_hdr->e_ident[EI_VERSION], elf_hdr->e_ident[EI_OSABI], elf_hdr->e_ident[EI_ABIVERSION], elf_hdr->e_ident[EI_PAD]
    , elf_hdr->e_ident[10], elf_hdr->e_ident[11], elf_hdr->e_ident[12], elf_hdr->e_ident[13]
    , elf_hdr->e_ident[13], elf_hdr->e_ident[14], elf_hdr->e_ident[15], elf_hdr->e_ident[16]);
    printf("entry: %x\n", elf_hdr->e_entry);
}

void display_program_headers(elf64programheader_s* phdr_arr, uint16_t prog_num){
    printf("Program Headers: \n");
    printf("    %d headers\n", prog_num);

    for(int i = 0; i < prog_num; i++){
        printf("Program Header %d\n", i);
        printf("    Type: %x\n", phdr_arr[i].p_type);
        printf("    Offset: %lx\n", phdr_arr[i].p_offset);
        printf("    Virtual Addr: %lx\n", phdr_arr[i].p_vaddr);
        printf("    Physical Addr: %lx\n", phdr_arr[i].p_paddr);
        printf("    File Size: %lx\n", phdr_arr[i].p_filesz);
        printf("    Mem Size: %lx\n", phdr_arr[i].p_memsz);
        printf("    Flags: %x\n", phdr_arr[i].p_flags);
        printf("    Align: %lx\n", phdr_arr[i].p_align);
        printf("\n\n");
    }
}

void display_section_headers(elf64sectionheader_s* shdr_arr, uint16_t num_sections){

}