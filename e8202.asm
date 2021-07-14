 [bits 16]

global do_e820

mmap_ent equ 0x1c00   
 do_e820:

    pushad
	xor ax,ax
	mov ax, 0x07c0
	mov es,ax
    xor ebp, ebp			;/* entry number */
    xor ebx, ebx		;	/* ebx must be 0 to start */
    ;mov edi, 0x1000		;/* e820 list(es:edi 0x07c0:0x1000) */  0x07c0*16 + 0x1000
	;mov edi,  0xeb00	;/* e820 list(es:edi 0x0000:0xeb00) */  0xeb000  ;TODO: VRES GIATI DEN DOUPEVE
	mov edi, 0x1000 ; es:edi 0x07c0:0x1000    
.loopy_e820:
	mov edx, 0x0534D4150		;/* magical number: "SMAP" */
	mov eax, 0xe820
	mov ecx, 24
	int 0x15
	jc short .end_e820
	cmp eax, edx
	jne short .fail_e820			;/* eax != SMAP for error */
	jcxz .next_e820			;/* ignore 0-length entry */
	inc ebp			;/* increase entry number */
	add edi, 24	;/* edi points next entry space */
.next_e820:
	test ebx, ebx			;/* if ebx equals 0, list ends */
	je short .end_e820
	mov eax, ebp
	cmp eax, 128
	je short .fail_e820			;/* entry number >= E820_MAX for error */
	jmp short .loopy_e820
.end_e820:
	cmp eax, 0x01		;/* entry number <= 1 for error */
	je short .fail_e820
	mov  [ 0x00000dfd0], ebp ;how many entries
	xor eax, eax
	mov eax, es:0xeb00
	mov  [0x000002fd0],eax
	;mov [mmap_ent], eax
    ;mov ah, 0x0e
    ;mov esi, ebp
	;call printmsg
	 ; mov eax, strMuf
	;mov [mmap_ent], eax	;/* save e820 entry number to 0x9800 */ ;  0x07c0*16 and 0x1c00 = 0x9800
	xor ebp,ebp
    xor edi,edi
    popad
    ret
.fail_e820:
    stc
	jmp .fail_e820			;/* end of boot trap */


    ;  0x07c0*16 + 0x1000    0x8C00
;  0x07c0*16 and 0x1c00 = 0x9800