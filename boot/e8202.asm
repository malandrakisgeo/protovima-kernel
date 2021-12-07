 [bits 16]

global do_e820

 do_e820:
    pushad
	xor ax,ax
	mov es,ax
    xor ebp, ebp			;/* entry number */
    xor ebx, ebx		;	/* ebx must be 0 to start */
    ;mov edi, 0x1000		;/* e820 list(es:edi 0x07c0:0x1000) */  0x07c0*16 + 0x1000
	;mov edi,  0xeb00	;/* e820 list(es:edi 0x0000:0xeb00) */  0xeb000  ;TODO: VRES GIATI DEN DOUPEVE
	mov edi, 0x00000B00 ; es:edi 0x07c0:0x1000     H 0x0000:0xA000 K.O.K
.loopy_e820:
	mov edx, 0x0534D4150		;/* magical number: "SMAP" */
	mov eax, 0xe820
	mov dword [es:di + 20], 1 ; Force a valid ACPI 3.X entry
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
	xor eax, eax
	mov  [ 0x00000dfd0], ebp ;how many entries
	mov eax, es:edi
    popad
    ret
.fail_e820:
    stc
	jmp .fail_e820			;/* end of boot trap */