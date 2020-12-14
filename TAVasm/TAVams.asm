.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib libucrt.lib
	includelib kernel32.lib
	ExitProcess PROTO :SDWORD

	includelib ..\Debug\TAV_2020_standartLibary.lib
	includelib ..\asmlib.lib

	WriteConsoleA PROTO : SDWORD, : SDWORD, : SDWORD, : SDWORD, : SDWORD
	SetConsoleTitleA PROTO : SDWORD
	GetStdHandle PROTO : SDWORD
	printconsole PROTO : SDWORD, : SDWORD
	int_to_char PROTO : SDWORD, : SDWORD

	concat PROTO :SDWORD
	compare PROTO :SDWORD

.stack 4096
.const
	L1 SDWORD 5
	L2 SDWORD 3
	L3 SBYTE 'x less then y', 0
	L4 SBYTE 'x more then y', 0
	L5 SDWORD 0
	L6 SDWORD 0xa
	L7 SDWORD 0x1
	L8 SDWORD 0xd
	L9 SDWORD 0x2
	L10 SBYTE 'strings concatenation:', 0
	L11 SBYTE 'l', 0
	L12 SBYTE 'string comparison:', 0
	L13 SDWORD 1
	L14 SDWORD 2
	L15 SDWORD 7
	L16 SBYTE 'andrewtehanov', 0
.data
	w2-1 SDWORD ?
	add60-1 SDWORD ?
	sub60-1 SDWORD ?
	mult60-1 SDWORD ?
	div60-1 SDWORD ?
	modul60-1 SDWORD ?
	scnct131174 SDWORD ?
	scmpr131174 SDWORD ?
	first177213 SDWORD ?
	secon177213 SDWORD ?
	i177213 SDWORD ?
	ConsoleTitle SBYTE "Console",0
	result SBYTE 100 dup(0)

.code
condi PROTO :SDWORD, :SDWORD
condi PROC x:SDWORD, y:SDWORD
	push offset L1
	push offset L2
	pop ebx
	pop eax
	cdq
	idiv ebx
modul220:
	neg edx
	cmp edx, 0
jl modul220:
	mov eax, edx
	push eax
	pop w

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	jb is37
	jmp els37
is37:
	push offset ConsleTitle
	push offset L3
	call printconsole
els32:
	push offset ConsleTitle
	push offset L4
	call printconsole

	push x
	push y
	pop ebx
	pop eax
cmp eax,ebx
	ja is87
	jmp els87:
is87:
	push offset ConsleTitle
	push offset L4
	call printconsole
els82:
	push offset ConsleTitle
	push offset L3
	call printconsole

	mov eax,L5
	ret
condi ENDP

arith PROTO :SDWORD, :SDWORD
arith PROC x:SDWORD, y:SDWORD
	push offset L6
	push offset L7
	pop ebx
	pop eax
	add eax, ebx
	push eax
	pop add

	push add
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	push x
	push y
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop sub

	push sub
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	push x
	push y
	pop ebx
	pop eax
	mul ebx
	push eax
	pop mult

	push mult
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	push offset L8
	push offset L9
	pop ebx
	pop eax
	cdq
	div ebx
	push eax
	pop div

	push div
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	push x
	push y
	pop ebx
	pop eax
	cdq
	idiv ebx
modul2425:
	neg edx
	cmp edx, 0
jl modul2425:
	mov eax, edx
	push eax
	pop modul

	push modul
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	mov eax,L5
	ret
arith ENDP

fstan PROTO :SDWORD
fstan PROC s:SDWORD
	push scnct
	push scnct
	call conca
	push eax
	pop scnct

	push offset ConsleTitle
	push offset L10
	call printconsole
	push offset ConsleTitle
	push offset scnct
	call printconsole
	push scmpr
	push offset L11
	call compa
	push eax
	pop scmpr

	push offset ConsleTitle
	push offset L12
	call printconsole
	push scmpr
	push offset result
	call int_to_char
	push offset ConsleTitle
	push offset result
	call printconsole
	mov eax,L5
	ret
fstan ENDP

main PROC
	push offset L13
	push offset L14
	call condi
	push eax
	pop first

	push offset L15
	push offset L14
	call arith
	push eax
	pop secon

	push offset L16
	call fstan
	push eax
	pop i

	push L5
	call ExitProcess
main ENDP
end main