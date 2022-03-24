global _start 

section .text

_start:
	
	xor eax, eax	
	mov al, 0x4
    
	xor ebx, ebx
	mov bl, 0x1
    
	xor edx, edx
    push edx

    push 0x6948203a
    push 0x69726168
    push 0x73654d

	mov ecx, esp

	mov dl, 12

	int 0x80

	xor edx, edx
	add edx, 12
	
	xor eax, eax	
    mov al, 0x1

    xor ebx, ebx
	int 0x80

