# ELF PARSER
- [x] parse out the elf header
- [x] parse out the program headers
- [x] parse out the section headers

## Improvments
- [x] command like util options like readelf
- [x] print out more things for the elf header
- [x] print out the symbol meaning of values for all the headers - Section Type 7 = SHT_NOTE
- [ ] show the link between the program and section headers
- [ ] swap all of my string getter functions to take a buffer to fill, pass buffer to them
- [x] move validations of legitimate elf to loader code
- [ ] return the allocated structures 
- [ ] handle a blob ptr not just fd
- [ ] make read functions to work on a blob ptr vs a fd, move the existing to fd specific functions

gcc -fPIE -pie -nostdlib test_reloc.c -o test_reloc
gcc -static -fPIE -pie -nostdlib test.c -o test
set follow-fork-mode child
set detach-on-fork off
break elf_loader.c:27