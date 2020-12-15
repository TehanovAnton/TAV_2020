.586
	.model flat, stdcall
	includelib libucrt.lib
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

	conca PROTO : SDWORD, : SDWORD
	compa PROTO : SDWORD, : SDWORD
printCons PROTO : SDWORD

.stack 4096
.const
	L1 SDWORD 5
	L2 SDWORD 3
	L3 SBYTE 'x less then y', 0
	L4 SBYTE 'x more then y', 0
	L5 SDWORD 0
	L6 SDWORD 0ah
	L7 SDWORD 1h
	L8 SDWORD 0dh
	L9 SDWORD 2h
	L10 SBYTE 'strings concatenation:', 0
	L11 SBYTE 'l', 0
	L12 SBYTE 'string comparison:', 0
	L13 SDWORD 1
	L14 SDWORD 2
	L15 SDWORD 7
	L16 SBYTE 'andrewtehanov', 0
.data
	w256 SDWORD ?
	add60127 SDWORD ?
	sub60127 SDWORD ?
	mult60127 SDWORD ?
	div60127 SDWORD ?
	modul60127 SDWORD ?
	scnct131174 SDWORD ?
	scmpr131174 SDWORD ?
	first177213 SDWORD ?
	secon177213 SDWORD ?
	i177213 SDWORD ?
	ConsoleTitle SBYTE "Console",0
	result SBYTE 100 dup(0)

.code
condi PROTO :SDWORD, :SDWORD
arith PROTO :SDWORD, :SDWORD
fstan PROTO :SDWORD
main PROC
	push L13
	push L14
	call condi
	push eax
	pop first177213

	push L14
	push L15
	call arith
	push eax
	pop secon177213

	push offset L16
	call fstan
	push eax
	pop i177213

	push L5
	call ExitProcess
main ENDP
condi PROC x:SDWORD, y:SDWORD
	push L1
	push L2
	pop ebx
	pop eax
	cdq
	idiv ebx
modul220:
	neg edx
	cmp edx, 0
jl modul220
	mov eax, edx
	push eax
	pop w256

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	jl is32
	jmp els32
is32:
	push offset L3
	call printCons
	jmp continue32
els32:
	push offset L4
	call printCons
continue32:

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	jg is82
	jmp els82
is82:
	push offset L4
	call printCons
	jmp continue82
els82:
	push offset L3
	call printCons
continue82:

	mov eax,L5
	ret
condi ENDP

arith PROC x:SDWORD, y:SDWORD
	push L6
	push L7
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop add60127

	push add60127
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop sub60127

	push sub60127
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	mul ebx
	push eax
	pop mult60127

	push mult60127
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push L8
	push L9
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop div60127

	push div60127
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	cdq
	idiv ebx
modul2425:
	neg edx
	cmp edx, 0
jl modul2425
	mov eax, edx
	push eax
	pop modul60127

	push modul60127
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	mov eax,L5
	ret
arith ENDP

fstan PROC s:SDWORD
	push s
	push s
	call conca
	push eax
	pop scnct131174

	push offset L10
	call printCons
	push scnct131174
	call printCons
	push s
	push offset L11
	call compa
	push eax
	pop scmpr131174

	push offset L12
	call printCons
	push scmpr131174
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	mov eax,L5
	ret
fstan ENDP

end main.586
	.model flat, stdcall
	includelib libucrt.lib
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

	conca PROTO : SDWORD, : SDWORD
	compa PROTO : SDWORD, : SDWORD
printCons PROTO : SDWORD

.stack 4096
.const
	L1 SBYTE 'a', 0
	L2 SBYTE 'l', 0
	L3 SDWORD 5
	L4 SDWORD 3
	L5 SBYTE 'x less then y', 0
	L6 SBYTE 'x more then y', 0
	L7 SDWORD 0
	L8 SDWORD 0ah
	L9 SDWORD 1h
	L10 SDWORD 0dh
	L11 SDWORD 2h
	L12 SBYTE 'strings concatenation:', 0
	L13 SBYTE 'string comparison:', 0
	L14 SDWORD 1
	L15 SDWORD 2
	L16 SDWORD 7
	L17 SBYTE 'andrewtehanov', 0
.data
	res24100 SDWORD ?
	r24100 SDWORD ?
	w24100 SDWORD ?
	add104171 SDWORD ?
	sub104171 SDWORD ?
	mult104171 SDWORD ?
	div104171 SDWORD ?
	modul104171 SDWORD ?
	scnct175218 SDWORD ?
	scmpr175218 SDWORD ?
	first221257 SDWORD ?
	secon221257 SDWORD ?
	i221257 SDWORD ?
	ConsoleTitle SBYTE "Console",0
	result SBYTE 100 dup(0)

.code
conca PROTO :SDWORD, :SDWORD
compa PROTO :SDWORD, :SDWORD
condi PROTO :SDWORD, :SDWORD
arith PROTO :SDWORD, :SDWORD
fstan PROTO :SDWORD
main PROC
	push L14
	push L15
	call condi
	push eax
	pop first221257

	push L15
	push L16
	call arith
	push eax
	pop secon221257

	push offset L17
	call fstan
	push eax
	pop i221257

	push L7
	call ExitProcess
main ENDP
conca PROC s:SDWORD, s:SDWORD
	push offset L1
	push offset L2
	call conca
	push eax
	pop res24100

	push offset L1
	push offset L2
	call compa
	push eax
	pop r24100

	push L3
	push L4
	pop ebx
	pop eax
	cdq
	idiv ebx
modul620:
	neg edx
	cmp edx, 0
jl modul620
	mov eax, edx
	push eax
	pop w24100

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	jl is72
	jmp els72
is72:
	push offset L5
	call printCons
	jmp continue72
els72:
	push offset L6
	call printCons
continue72:

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	jg is122
	jmp els122
is122:
	push offset L6
	call printCons
	jmp continue122
els122:
	push offset L5
	call printCons
continue122:

	mov eax,L7
	ret
conca ENDP

arith PROC x:SDWORD, y:SDWORD
	push L8
	push L9
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop add104171

	push add104171
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop sub104171

	push sub104171
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	mul ebx
	push eax
	pop mult104171

	push mult104171
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push L10
	push L11
	pop ebx
	pop eax
	cdq
	idiv ebx
	push eax
	pop div104171

	push div104171
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	push x
	push y
	pop ebx
	pop eax
	cdq
	idiv ebx
modul2825:
	neg edx
	cmp edx, 0
jl modul2825
	mov eax, edx
	push eax
	pop modul104171

	push modul104171
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	mov eax,L7
	ret
arith ENDP

fstan PROC s:SDWORD
	push s
	push s
	call conca
	push eax
	pop scnct175218

	push offset L12
	call printCons
	push scnct175218
	call printCons
	push s
	push offset L2
	call compa
	push eax
	pop scmpr175218

	push offset L13
	call printCons
	push scmpr175218
cld
	mov edi, offset result
	mov ecx, 100
	sub al, al
	rep stosb
	push offset result
	call int_to_char
	push offset result
	call printCons
	mov eax,L7
	ret
fstan ENDP

end main