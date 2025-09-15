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
*/

FILE *open_exe(const char* executable_path){
    FILE* fptr = fopen(executable_path, "rb");

    if(fptr == NULL){
        perror("Error opening executable");
        return NULL;
    }

    return fptr;
}

int read_elf_header(FILE* fptr, elfheader_s* hdr){
    size_t count = fread(hdr, sizeof(elfheader_s), 1, fptr);
    if (count < 1){
        printf("Count: %i\n", count);
        return -1;
    }
    return 0;
}

int read_program_header(FILE* fptr, programheader_s* arr, uint16_t num_entries){
    for(int i = 0; i < num_entries; i++){
        size_t count = fread(arr, sizeof(programheader_s), 1, fptr);
        if(count < 1){
            printf("Count: %i\n", count);
            return -1;
        }
        arr++;
        
    }
    return 0;
}

int elf_check_file(elfheader_s* hdr){
    if(!hdr) return -1;
    if(hdr->e_indent[EI_MAG0] != ELFMAG0){
        printf("ELF Header EI_MAG0 incorrect..\n");
        return -1;
    }
    if(hdr->e_indent[EI_MAG1] != ELFMAG1){
        printf("ELF Header EI_MAG1 incorrect..\n");
        return -1;
    }
    if(hdr->e_indent[EI_MAG2] != ELFMAG2){
        printf("ELF Header EI_MAG2 incorrect..\n");
        return -1;
    }
    if(hdr->e_indent[EI_MAG3] != ELFMAG3){
        printf("ELF Header EI_MAG3 incorrect..\n");
        return -1;
    } 
    return 0;
}

int elf_check_supported(elfheader_s* hdr){
    if(elf_check_file(hdr) != 0){
        printf("Invalid ELF File.\n");
        return -1;
    }
    if(hdr->e_indent[EI_CLASS] != ELFCLASS64){
        printf("Unsupported ELF File Class.\n");
        return -1;
    }
    if(hdr->e_indent[EI_DATA] != ELFDATA2LSB){
        printf("Unsupported ELF File Byte Order.\n");
        return -1;
    }
    if(hdr->e_machine != AMD_x86_64){
        printf("Unsupported ELF File Target.\n");
        return -1;
    }
    if(hdr->e_indent[EI_VERSION] != EV_CURRENT){
        printf("Unsupported ELF File version.\n");
        return -1;
    }
    if(hdr->e_type != ET_EXEC){
        printf("Unsupported ELF File Type.\n");
        return -1;
    }

    return 0;
}

int main(){
    FILE* fptr = open_exe("test");
    elfheader_s* elf_header = calloc(1, sizeof(elfheader_s));

    if(read_elf_header(fptr, elf_header) != 0){
        printf("Failed to read in elf header\n");
        return -1;
    }

    if(elf_check_supported(elf_header) != 0){
        return -1;
    }

    uint16_t num_entries = elf_header->e_phnum-1;

    programheader_s prog_arr[num_entries];

    if(read_program_header(fptr, prog_arr, num_entries) != 0){
        printf("Failed to read in program header");
    }

    free(elf_header);
    fclose(fptr);

    return 0;
}