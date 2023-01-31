.intel_syntax noprefix
.globl plus, main

plus:
        mov rax, rdi
        add rax, rsi
        ret

main:
        mov rdi, 3
        mov rsi, 4
        call plus
        ret

