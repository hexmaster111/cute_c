global _start

section .text

_start:
    mov rax, 0x01 ;write syscall
    mov rdi, 0x01 ;file discripter 1 for std out
    mov rsi, msg ;msg ptr to write
    mov rdx, msglen ;msg len of str
    syscall ;do the syscall

    mov rax, 0x3c ;exit syscall
    mov rdi, 0x00 ;exit code 0
    syscall ;exit 0

section .data
    msg: DB "Hello World", 0xA
    msglen: EQU $ - msg ;len of string