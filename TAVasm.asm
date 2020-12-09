test
	.model flat, stdcall
	includelib libucrt.lib
	includelib libucrt.lib
	includelib kernel32.lib
	ExitProcess PROTO :DWORD

	C:\Users\Anton\source\repos\programming languages\TAV_2020\Debug\TAV_2020_standartLibary.lib
strlength PROTO :DWORD
substr PROTO :DWORD
	strl PROTO :DWORD
strcompare PROTO :DWORD, :DWORD

.stack 4096
