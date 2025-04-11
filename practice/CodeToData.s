;Write an ARM assembly language program to copy 32-bit data from code memory to data memory.  
	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0X10001000		;full descending stack's stack pointer when it is empty
	DCD Reset_Handler	;reset vector
	ALIGN
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0, =SRC
	LDR R1, [R0]
	LDR R2, =DST
	STR R1, [R2]
STOP 
	B STOP
SRC DCD 0X1234
	AREA mydata, DATA, READWRITE
DST DCD 0
	END