        ;;

        BITS 16

        ORG 0

        ;; Header: Offset to entry point
        dw int10
continue_ptr:
        dw continue

serial_port:
        dw 0

int10:
        push ds
        push es
        pushad

        mov ax, cs
        mov ds, ax
        mov es, ax

        mov ecx, 9               ; 8 regs plus 2 16-bit segments
        xor ebp, ebp
        mov bp, sp
word_loop:
        mov edi, [ebp + 4 * ecx - 4]
        cmp cx, 1
        mov si, ' '
        mov ax, 10
        cmove si, ax

        push cx
        call out_hex
        pop cx
        loop word_loop

        popad
        pop es
        pop ds

        db 0xea
continue:
        dd 0

        ;; Outputs a 32-bit word to serial plus a space
        ;; EDI - value to print
        ;; ESI - separator
out_hex:
        mov cx, 9
        mov bx, hex
loop:
        mov dx, [serial_port]
        add dx, 5
        in al, dx
        test al, 0x20
        jz loop
        sub dx, 5

        cmp cx, 1
        jne digit
        mov ax, si
        jmp outc
digit:
        rol edi, 4
        mov ax, di
        and ax, 0x0F
        xlatb
outc:
        out dx, al
        loop loop
        ret


hex:    db "0123456789ABCDEF"

        ;; EOF
