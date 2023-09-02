global  _start

section .text
_start:

    ;read? (&msg, msglen)
    mov     rax, 0x0    ;read syscall
    mov     rdi, 0x0    ;file discripter 0 for std in
    mov     rsi, msg    ;msg ptr to read into
    mov     rdx, msglen ;msg len of str
    syscall             ;do the syscall

    ;write(&msg, msglen)
    mov     rax, 0x01   ;write syscall
    mov     rdi, 0x01   ;file discripter 1 for std out
    mov     rsi, msg    ;msg ptr to write
    mov     rdx, msglen ;msg len of str
    syscall             ;do the syscall

    ;TODO: if strcmp(entered, exit) jmp _exit ok
    cmpsb msg, 


    jmp _start ;


    call _exitOk

    ; cmp rax, 23 
    ; je _doThis  ;Jump if equal from last cmp



    jmp _start ; GO loop at start

_exitOk:
    mov     rax, 0x3c ;exit syscall
    mov     rdi, 0x00 ;exit code 0
    syscall           ;exit 0

section .data
    msg:    DB "exit", 0xA
    msglen: EQU 20                      ;buffer size
    

    