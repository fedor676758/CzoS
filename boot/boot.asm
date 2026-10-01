[bits 16]
[org 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Переключить в 320x200x256 (режим 0x13)
    mov ax, 0x0013
    int 0x10

    ; Перейти в 32-битный защищённый режим (упрощённо для QEMU)
    lgdt [gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp 0x08:code32

[bits 32]
code32:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    extern kernel_main
    call kernel_main

hang:
    hlt
    jmp hang

gdt_start:
    dq 0x0
gdt_null:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 0x9A
    db 0xCF
    db 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 0x9A
    db 0xCF
    db 0x0
gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 0x92
    db 0xCF
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
