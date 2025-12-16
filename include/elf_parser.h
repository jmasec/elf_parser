#ifndef ELF_PARSER
#define ELF_PARSER


#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>


// OS ABI
#define LINUX 0x03

// ISA
#define NoISA       0x00
#define x86         0x03
#define MIPS        0x08
#define ARM         0x28
#define AMD_x86_64  0x3E
#define ARM_64      0xB7
#define RISC_V      0xF3

// elf values
#define ELFMAG0             0x7f
#define ELFMAG1             'E'
#define ELFMAG2             'L'
#define ELFMAG3             'F'
#define ELFCLASS64          2
#define ELFDATA2LSB         1
#define ELFOSABI            3
#define EV_CURRENT          1

#define EI_NINDENT 16

enum elf_types{
    ET_NONE,
    ET_REL,
    ET_EXEC,
    ET_DYN,
    ET_CORE
};

enum elf_indent{
    EI_MAG0,
    EI_MAG1,
    EI_MAG2,
    EI_MAG3,
    EI_CLASS,
    EI_DATA,
    EI_VERSION,
    EI_OSABI,
    EI_ABIVERSION,
    EI_PAD
};

typedef uint16_t Elf64_Half;    // 2 bytes, unsigned
typedef uint32_t Elf64_Word;    // 4 bytes, unsigned
typedef uint64_t Elf64_Addr;    // 8 bytes, unsigned (virtual address)
typedef uint64_t Elf64_Off;     // 8 bytes, unsigned (file offset)
typedef uint64_t Elf64_Xword;   // 8 bytes, unsigned (extended word)

typedef struct {
	uint8_t		e_ident[EI_NINDENT];
	Elf64_Half	e_type;
	Elf64_Half	e_machine;
	Elf64_Word	e_version;
	Elf64_Addr	e_entry;
	Elf64_Off	e_phoff;
	Elf64_Off	e_shoff;
	Elf64_Word	e_flags;
	Elf64_Half	e_ehsize;
	Elf64_Half	e_phentsize;
	Elf64_Half	e_phnum;
	Elf64_Half	e_shentsize;
	Elf64_Half	e_shnum;
	Elf64_Half	e_shstrndx;
} elf64header_s;

// typedef struct elfheader_s{
//     unsigned char e_indent[EI_NINDENT]; 
//     uint16_t e_type; // object file type
//     uint16_t e_machine; // isa
//     uint32_t e_version; // 1
//     uintptr_t e_entry; // mem addr of entry point
//     size_t e_phoff; // points to start of program header table, usually follows the file header immediatly following this one, offset 0x34 or 0x40 32 or 64 bit
//     size_t e_shoff;
//     uint32_t e_flags; // this depends on arch
//     uint16_t e_ehsize; // contains size of header, normally 64 bytes for 64bit, and 52 bytes for 32bit
//     uint16_t e_phentsize; // contains size of a program header table entry. 0x20 32bit and 0x38 64bit
//     uint16_t e_phnum; // contains num of entries in the program header table
//     uint16_t e_shentsize; // contains the size of a section header table entry. 0x28 32bit, 0x40 64bit
//     uint16_t e_shnum; // contains the num of entries in section header
//     uint16_t e_shstrndx; // contains index of the section header table entry that contains section names
// } elfheader_s;


// ptype
#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC  2
#define PT_INTERP   3

// 3 WX, 5 RE, 6 RW, 7 RWX
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4

// typedef struct programheader_s{
//     uint32_t p_type;
//     uint32_t p_flags; // flag pos changes for 32 bit, need to add that 
//     size_t p_offset;
//     uintptr_t p_vaddr;
//     uintptr_t p_paddr;
//     size_t p_filesz;
//     size_t p_memsz;
//     size_t p_align;
// } programheader_s;

typedef struct elf64programheader_s{
    Elf64_Word p_type;
    Elf64_Word p_flags;
    Elf64_Off p_offset;
    Elf64_Addr p_vaddr;
    Elf64_Addr p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
} elf64programheader_s;


// may not need the section header here

// sh_type
#define SHT_NULL        0
#define SHT_PROGBITS    1
#define SHT_SYMTAB      2
#define SHT_STRTAB      3

// sh_flags
#define SHF_WRITE       1
#define SHF_ALLOC       2
#define SHF_EXECINSTR   3
#define SHF_MERGE       4
#define SHF_STRINGS     5

typedef struct sectionheader_s{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uintptr_t sh_addr;
    size_t sh_offset;
    size_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;;
    size_t sh_addralign;
    size_t sh_entsize;
}sectionheader_s;

int open_exe(const char* executable_path);
int read_elf_header(int fd, elf64header_s* elf_header);
int read_program_headers(int fd, elf64programheader_s* phdr_arr, uint16_t num_entries, size_t prog_hdr_offset, uint16_t phent_size);
bool elf_check_file(elf64header_s* hdr);
bool elf_check_supported(elf64header_s* hdr);
void display_elf_header(elf64header_s* elf_hdr);
void display_program_headers(elf64programheader_s* phdr_arr, uint16_t prog_count);

#endif