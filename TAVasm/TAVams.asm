.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	ExitProcess PROTO :SDWORD

	includelib ..\Debug\TAV_2020_standartLibary.lib
	includelib ..\Debug\asmlib.lib

	WriteConsoleA PROTO : SDWORD, : SDWORD, : SDWORD, : SDWORD, : SDWORD
	SetConsoleTitleA PROTO : SDWORD
	GetStdHandle PROTO : SDWORD
	printconsole PROTO : SDWORD, : SDWORD
	int_to_char PROTO : SDWORD, : SDWORD

printCons PROTO : SDWORD

.stack 4096
.const
	L1 SDWORD 3
	L2 SDWORD 0
	L3 SBYTE 'res =>:', 0
	L4 SBYTE ' ', 0
	L5 SDWORD -0fh
	L6 SBYTE 'x less then 0fhm', 0
	L7 SBYTE 'x isnt less then 0fhm', 0
	L8 SDWORD 10o
	L9 SBYTE 'y more or equal 10o', 0
	L10 SDWORD 10
	L11 SDWORD 5
	L12 SDWORD 20
	L13 SDWORD 4
	L14 SBYTE 'anton', 0
	L15 SBYTE 'antonTehanov', 0
	L16 SDWORD 1
.data
	b3585 SDWORD ?
	a3585 SDWORD ?
	res3585 SDWORD ?
	res134163 SDWORD ?
	first166215 SDWORD ?
	secon166215 SDWORD ?
	i166215 SDWORD ?
	result SBYTE 100 dup(0)

.code
strle PROTO :SDWORD
strsu PROTO :SDWORD, :SDWORD, :SDWORD
strca PROTO :SDWORD, :SDWORD
fstan PROTO :SDWORD
condi PROTO :SDWORD, :SDWORD
arith PROTO :SDWORD, :SDWORD
main PROC
	push L12
	push L8
	call condi
	push eax
	pop first166215

	push L13
	push offset L14
	call arith
	push eax
	pop secon166215

	push offset L15
	call fstan
	push eax
	pop i166215

RIF312:
	push secon166215
	push L2
	pop ebx
	pop eax
cmp eax,ebx
	jge is312
	jmp continue312
is312:
	push secon166215
	push L16
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop secon166215

	push secon166215
	cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
jmp RIF312
continue312:
	push L2
	call ExitProcess
main ENDP
fstan PROC s:SDWORD
	push s
	call strle
	push eax
	push L1
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop b3585

	push s
	push L2
	push b3585
	call strsu
	push eax
	pop a3585

	push s
	push a3585
	call strca
	push eax
	pop res3585

	push offset L3
	call printCons
	push res3585
	cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	mov eax, offset L4
	ret
fstan ENDP

condi PROC y:SDWORD, x:SDWORD
	push x
	push L5
	pop ebx
	pop eax
cmp eax,ebx
	jl is132
	jmp els132
is132:
	push offset L6
	call printCons
	jmp continue132
els132:
	push offset L7
	call printCons
	push y
	push L8
	pop ebx
	pop eax
cmp eax,ebx
	jge is173
	jmp els173
is173:
	push offset L9
	call printCons
els173:
continue132:

	mov eax, L2
	ret
condi ENDP

arith PROC y:SDWORD, x:SDWORD
	push x
	push L10
	pop ebx
	pop eax
	mul ebx
	push eax
	push L11
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	push y
	call strle
	push eax
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop res134163

	mov eax,res134163
	ret
arith ENDP

end main