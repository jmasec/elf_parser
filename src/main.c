#include "elf_parser.h"
#include "elf_display.h"


int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Missing Arguments\n");\
        help();
        return 0;
    }

    enum Command cmd;

    if(cmd = get_command(argv[1]) == -1){
        printf("Bad Argument \n");
        help();
        return -1;
    }

    elfinternal_s* elf_internal = calloc(sizeof(elfinternal_s), 1);

    switch (get_command(argv[1]))
    {
    case ALL: {

        parse_elf_internal(argv[2], ELF_FILE_DESCRIPTOR, elf_internal);

        display_elf_header(elf_internal->elf_hdr);

        display_program_headers(elf_internal->pgm_hdrs, elf_internal->elf_hdr->e_phnum);

        display_section_headers(elf_internal->sct_hdrs, elf_internal->elf_hdr->e_shnum);

        break;
    }
    case PROGRAM: {
        parse_elf_internal(argv[2], ELF_FILE_DESCRIPTOR, elf_internal);

        display_elf_header(elf_internal->elf_hdr);

        display_program_headers(elf_internal->pgm_hdrs, elf_internal->elf_hdr->e_phnum);

        display_section_headers(elf_internal->sct_hdrs, elf_internal->elf_hdr->e_shnum);
        break;
    }
    case SECTION: {
        if(!parse_elf_internal(argv[2], ELF_FILE_DESCRIPTOR, elf_internal)){
            goto cleanup;
        }

        display_elf_header(elf_internal->elf_hdr);

        display_program_headers(elf_internal->pgm_hdrs, elf_internal->elf_hdr->e_phnum);

        display_section_headers(elf_internal->sct_hdrs, elf_internal->elf_hdr->e_shnum);
    }
    case HELP: {
        help();
        break;
    }
    default:
        break;
    }

cleanup:
    return -1;
}