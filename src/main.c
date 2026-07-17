#include "elf_parser.h"
#include "elf_display.h"
#include "payload.h"


int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Missing Arguments\n");\
        help();
        return 0;
    }

    enum Command cmd;
    enum elf_ptr_type_s type;

    if((cmd = get_command(argv[1])) == -1){
        printf("Bad Argument \n");
        help();
        return -1;
    }

    if((type = get_elf_type(argv[2])) == -1){
        printf("Bad Argument \n");
        help();
        return -1;
    }

    elfinternal_s* elf_internal = calloc(1, sizeof(elfinternal_s));

    if(type == ELF_FILE_DESCRIPTOR){
        if(!parse_elf_internal(argv[3], type, elf_internal)){
            printf("Failed parsing ELF file\n");
            return -1;
        }
    }
    else{
        if(!parse_elf_internal((char *)bin_test_reloc_write, type, elf_internal)){
            printf("Failed parsing ELF memory pointer\n");
            return -1;
        }
    }

    if(NULL == elf_internal){
        return -1;
    }

    switch (get_command(argv[1]))
    {
    case ALL: {

        display_elf_header(elf_internal->elf_hdr);

        display_program_headers(elf_internal->pgm_hdrs, elf_internal->elf_hdr->e_phnum);

        display_section_headers(elf_internal->sct_hdrs, elf_internal->elf_hdr->e_shnum);

        return 0;
    }
    case PROGRAM: {

        display_program_headers(elf_internal->pgm_hdrs, elf_internal->elf_hdr->e_phnum);

        return 0;
    }
    case SECTION: {

        display_section_headers(elf_internal->sct_hdrs, elf_internal->elf_hdr->e_shnum);
        
        return 0;
    }
    case HELP: {
        help();
        return 0;
    }
    default:
        return -1;
    }

}