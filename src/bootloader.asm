%define BASE 0x100  ; l'adresse de base du kernel (0x0100:0x0 = 0x1000)
%define KSIZE 50     ; nombre de secteurs à charger

[BITS 16]     ;processeur en mode réel 16 bits
[ORG 0x0]     ;décalage a ajouter a toutes les adresses

jmp start
;---------------------------------------------------------
; Synopsis: Affiche une chaîne de caractères se terminant par 0x0
; Entrée:   DS:SI -> pointe sur la chaîne à afficher
;---------------------------------------------------------
%include "src/AFFICHAGE.INC"

start:

; initialisation des segments 
    mov ax, 0x07C0
    mov ds, ax  ;ds <- 0x07C0 : adresse physique =0x7C00+[org]=0x7C00segment de données
    mov es, ax  ;ds <- 0x07C0;
    ;pile   0x8F000 ----> 0x80000
    mov ax, 0x8000    
    mov ss, ax      ;ss <- 0x8000 : adresse physique =0x80000+[org]=0x80000
    mov sp, 0xf000  ;sp <- 0xf000

; récupération de l'unité de boot
    mov [bootdrv], dl     ;dl->[bootdrv] en mémoire

; affiche un msg
    mov si, msgDebut
    call afficher

; charger le noyau
    xor ax, ax ;initialisation de ax à 0

    push es
    mov ax, BASE
    mov es, ax ;es->base
    mov bx, 0

    mov ah, 2 ; mode lecture
    mov al, KSIZE ;nombre de secteurs
    mov ch, 0 ;cylindre 0
    mov cl, 2 ;secteur N°2
    mov dh, 0 ;tete N°0
    mov dl, [bootdrv]
    int 0x13
    pop es

; initialisation du pointeur sur la GDT
; calcule la limite de GDT
    mov ax, gdtend    
    mov bx, gdt
    sub ax, bx ;ax <-  gdtend - gtd
    mov word [gdtptr], ax ;gdt.limite <- ax

; calcule l'adresse linéaire de GDT
    xor eax, eax    ;eax <- 0
    xor ebx, ebx    ;ebx <- 0
    mov ax, ds      ;ax <- ds
    mov ecx, eax    ;ecx <- 0:ds
    shl ecx, 4      ;ecx <- 0:ds*4
    mov bx, gdt     ;bx <- gdt
    add ecx, ebx    ;ecx <- ds*4+gdt
    mov dword [gdtptr+2], ecx  ;gdt.base <- ecx

; passage en mode protégé
    cli   ;désactivation des interruptions
    lgdt [gdtptr]    ; charge la gdt   gdtr <- adresse de la gdt
    mov eax, cr0
    or  ax, 1
    mov cr0, eax        ; mise a 1 du bit 0 du registre (CR0)

    jmp next
next:
    mov ax, 0x10        ; réinitialisation du segment de code  (pointe vers gdt_ds)
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x9F000    

    jmp dword 0x8:0x1000    ; réinitialise le segment de code (pointe vers gdt_cs) saut vers le kernel

;--------------------------------------------------------------------
bootdrv:  db 0
msgDebut: db "Chargement du kernel",0
;--------------------------------------------------------------------
gdt:
    db 0, 0, 0, 0, 0, 0, 0, 0
gdt_cs: ;adresse toutes la mémoire 0:0 -> F:F
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10011011b, 11011111b, 0x0
gdt_ds: ;adresse toutes la mémoire 0:0 -> F:F
    db 0xFF, 0xFF, 0x0, 0x0, 0x0, 10010011b, 11011111b, 0x0
gdtend:
;--------------------------------------------------------------------
gdtptr:
    dw 0  ; limite
    dd 0  ; base
;--------------------------------------------------------------------
;--- NOP jusqu'à 510 ---
    times 510-($-$$) db 144
    dw 0xAA55
