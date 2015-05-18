        ;;


        ORG 0x7c00
entry:  jmp 0:entry2
entry2:

        mov ax, 0
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax
        mov sp, 0x8000
        sti

        int 0x19
lo:
        hlt
        jmp lo

        ;; EOF
