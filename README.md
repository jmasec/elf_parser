# ELF PARSER
- [x] parse out the elf header
- [x] parse out the program headers
- [x] parse out the section headers

## Improvments
- [ ] command like util options like readelf
- [x] print out more things for the elf header
- [ ] print out the symbol meaning of values for all the headers - Section Type 7 = SHT_NOTE
- [ ] show the link between the program and section headers
- [x] move validations of legitimate elf to loader code
- [ ] add robust checks for the loader to ensure it should be able to run

# ELF LOADER
- [x] create a child process
- [x] mmap section in the process for our binary
- [x] load the program segments into memory

## Improvments
- [ ] no binary file, use a byte array of code
- [ ] use target process or our own process instead of child
- [ ] handle relocations
- [ ] handle dynamic resolutions

gcc -fPIE -pie -nostdlib test_reloc.c -o test_reloc
gcc -static -fPIE -pie -nostdlib test.c -o test
set follow-fork-mode child
set detach-on-fork off
break elf_loader.c:27