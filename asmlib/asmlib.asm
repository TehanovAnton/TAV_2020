.586
.model flat, stdcall
includelib kernel32.lib
includelib msvcrt.lib

WriteConsoleA PROTO : SDWORD, : SDWORD, : SDWORD, : SDWORD, : SDWORD
SetConsoleTitleA PROTO : SDWORD
GetStdHandle PROTO : SDWORD

.code
int_to_char PROC uses eax ebx ecx edi esi,
	pstr : sdword,		; адрес строки возврата 
	intfield : sdword	; число для преобразования
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

printconsole PROC uses eax ebx ecx edi esi,
	pstr :dword,
	ptitle :dword

	push ptitle
	call SetConsoleTitleA

	push -11
	call GetStdHandle

	mov esi, pstr
	mov edi, -1

count:
	inc edi
	cmp sbyte ptr [esi + edi], 0
	jne count

	push 0
	push 0
	push edi
	push pstr
	push eax
	call WriteConsoleA

	ret
printconsole ENDP
end