bits 16
; BIOS loads our bootloader at 0x7C0:0x0 which is
; 0x7C00 because of the following:
; (0x7C0 << 4) + 0x0 = 0x7C00.
; setting the origin tells nasm not to assume 
; our program is loaded at 0x0 which when adding
; to the offset would get the wrong memory location,
; hence we set the orign to 0x7C00.
; Remember, lables are just offsets.

org 0x7C00


; BIOS puts whatever it wants in our
; segment registers therfore we will 
; not know what segment:offset combos
; are being used, hence we set them to zero
; to initalize them and put them in a 
; "known" state.
; Remember, our bootloader is loaded at 0x7C00:0.
; So.. segment * 0x10 + offset.

xor bx, bx
mov ds, bx 
mov es, bx         
mov ss, bx

; Setup our stack.
mov bp, 0x9C00      ; 0x2000 past our bootloader.
mov sp, bp

cld     ; Set direction flag to make strings start from the lowest address.

%define KERNEL_OFFSET 0x1000

_start:
    mov [BOOT_DRIVE], dl

    mov si, start_msg
    call puts

    mov si, boot_choice
    call puts

    mov si, reboot_choice
    call puts

    call get_keys

    mov ah, 0x00    ; Change video mode function.
    mov al, 0x02    ; Video mode (80x25).
    int 0x10        ; Video services interrupt.

    mov si, boot_msg
    call puts

    ; Delay.
    mov cx, 0xF
    mov dx, 0x4240
    mov ah, 0x86
    int 0x15

    mov cx, 0xF
    mov dx, 0x4240
    mov ah, 0x86
    int 0x15

    ; Read kernel into memory.
    mov bx, KERNEL_OFFSET
    mov dh, 0x05
    mov dl, [BOOT_DRIVE]
    call disk_load

    cli
    lgdt [gdt_desc]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; We are now in 32-bit protected mode.

    ; CS is supposed to contain current code descriptor in protected mode.
    ; but it doesn't currently. To fix that we make a far jump to our code descriptor
    ; offsetted by our label.
    ; This far jump also flushes the CPU's pipeline.

    jmp CODE_SEG:init_pm

BOOT_DRIVE: db 0

disk_load:
    push dx         ; Save DH (highest bit of DX) by saving DX.
    mov ah, 0x02    ; Read sectors function.
    mov al, dh      ; Read DH sectors.
    mov ch, 0x00    ; Cylinder 0.
    mov cl, 0x02    ; Start reading after boot sector.
    mov dh, 0x00    ; Head 0.
    int 0x13        ; Disk services interrupt.
    jc disk_error
    pop dx
    cmp dh, al
    jne disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call puts
    cli                     ; Make CPU ignore interrupts.
    hlt                     ; Halt CPU.

get_keys:
    mov ah, 0x00    ; Getchar function.
    int 0x16        ; Keyboard interrupt.
    cmp al, 'r'
    je reboot
    cmp al, 'b'
    je rm_ret
    jmp get_keys

; Jump to (0xFFFF << 4) + 0x0 which is reset vector.
reboot: jmp 0xFFFF:0x0

puts:
    mov ah, 0x0E      ; Teletype video function.
    lodsb             ; Loads a byte from SI into AL and increments SI.
    or al, al         ; If AL is zero, then zero flag will be set.
    jz rm_ret         ; If zero flag is set, return.
    int 0x10          ; Video interrupt.
    jmp puts          ; Continue. 

rm_ret: ret

; GDT: It defines what 
; memory can be executed (The Code Descriptor), 
; and what area contains data (Data Descriptor). 

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0
    gdt_code:
        ; Type flags:
        ; Present: 1 since we are using code.
        ; Privilege: 00 higest privilige.
        ; Descriptor type: 1 for code/data.
        ; Code: 1.
        ; Conforming: 0 so segments with a lower privilege may not call code in this segment.
        ; Readable: 1.
        ; Accessed: 0.

        ; Other flags:
        ; Granularity: 1 so we can reach father into memory.
        ; 32-bit default: 1 since our segment will have 32-bit code.
        ; 64-bit code segment: 0.
        ; AVL 0.
        ; Limit: 1111.

        dw 0xFFFF       ; Limit.
        dw 0x0          ; Base.
        db 0x0          ; Base.
        db 10011010b    ; 1st flags, type flags.
        db 11001111b    ; 2nd flags, type flags.
        db 0x0
    gdt_data:
        ; Type flags: 
        ; Code: 0.
        ; Expand down: 0.
        ; Writable: 0.
        ; Accessed: 0.

        dw 0xFFFF       ; Limit.
        dw 0x0          ; Base.
        db 0x0          ; Base.
        db 10010010b    ; 1st flags, type flags.
        db 11001111b    ; 2nd flags, type flags.
        db 0x0
gdt_end:


gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

start_msg: db "Welcome to SkydeOS, what are you doing in my swamp?", 0x0D, 0x0A, 0
boot_choice: db 0x0D, 0x0A, "[B]oot", 0x0D, 0x0A, 0
reboot_choice: db "[R]eboot", 0x0D, 0x0A, 0
boot_msg: db "Booting SkydeOS..", 0x0d, 0x0A, 0
disk_error_msg: db "FATAL: Failed to read from disk.", 0x0D, 0x0A, 0

bits 32

init_pm:
    ; Our segments are incorrect so we reset them.
    mov ax, DATA_SEG        ; Set AX to data segments.
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000
    mov esp, ebp

    jmp _PM

_PM:
    call KERNEL_OFFSET
    hlt

; Pads the sector with zeros so it is exactly 512 bytes.
times 510-($-$$) db 0

; Boot signiture to tell BIOS this is a valid MBR.
dw 0xAA55
