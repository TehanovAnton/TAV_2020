.586
.model flat, stdcall
includelib kernel32.lib
includelib msvcrt.lib

.code

int_to_char PROC uses eax ebx ecx edi esi,
	pstr : sdword,		; ����� ������ �������� 
	intfield : sdword	; ����� ��� ��������������
	mov edi, pstr
	mov esi, 0
	mov eax, intfield
	cdq
	mov ebx, 10
	idiv ebx
	test intfield, 80000000h
	jz plus
	neg eax
	neg edx
	mov cl, '-'
	mov[edi], cl
	inc edi
plus:
	push dx
	inc esi
	test eax, eax
	jz fin
	cdq
	idiv ebx
	jmp plus
fin:
	mov ecx, esi
write:
	pop bx
	add bl, '0'
	mov [edi], bl
	inc edi
	loop write
	ret
int_to_char ENDP
end