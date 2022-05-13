section .text

global crc32_asm


crc32_asm:
    mov eax, 0FFFFFFFFh

.loop:
    cmp byte [rdi], 00h
    je .exit

    movzx r10d, byte [rdi]

    crc32 eax, r10b

    inc rdi
    jmp .loop

.exit:
    not eax
    ret