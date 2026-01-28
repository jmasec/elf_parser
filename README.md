# ELF Parser 
- For educational purpose, elf parser to execute code manually without calling execve \
- Todo:
    1. fork a process, mapping the elf in using mmap with NULL and then fixing the ptrs within my process
    2. shellcode to read proc/self/map using syscalls to, fork a process, find memory open to use and then fix ptrs


gcc -static -fPIE -pie -nostdlib test.c -o test