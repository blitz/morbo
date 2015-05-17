        ;; Switch back to real mode

        CPU P3

        GLOBAL enable_real_mode

        SECTION .text EXEC NOWRITE

enable_real_mode:
        mov [jmp_sel], ax
        mov [jmp_ptr], dx

        cli
        lidt [rm_idtp]
        lgdt [rm_gdtp]

        mov ebx, 16
        mov ds, bx
        mov es, bx
        mov ss, bx
        mov fs, bx
        mov gs, bx

        mov eax, 0x0E30
        mov ebx, 0x07

        jmp 8:pm16
        BITS 16
pm16:
        mov ecx, cr0
        and ecx, ~1
        mov cr0, ecx

         db 0xEA
jmp_ptr: dw 0
jmp_sel: dd 0

        SECTION .data

rm_gdt: dd 0
        dd 0
        dd 0x0000FFFF           ; CS
        dd 0x008F9A00
        dd 0x0000FFFF           ; DS
        dd 0x008F9200
rm_gdt_end:

rm_idtp:
        dw 256 * 4 - 1
        dd 0

rm_gdtp:
        dw rm_gdt_end - rm_gdt - 1
        dd rm_gdt

        ;; EOF
