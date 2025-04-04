	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0X10001000
	DCD Reset_Handler
	ALIGN
	AREA mycode, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR R0, =SRC
	LDR R1, [R0]
	MOV R2, #0		;RESULT
	MOV R3, #0XA
	MOV R5, #0		;SHIFTER
LOOP 
	CMP R1, #0
	BEQ DONE
	BL DIV
	LSL R1, R5
	ORR R2, R2, R1
	ADD R5, R5, #4
	MOV R1, R7
	B LOOP
DONE
	LDR R6, =DST
	STR R2, [R6]
STOP B STOP
DIV
	MOV R7, #0
DIVLOOP
	CMP R1, R3
	BLO DIVDONE
	SUB R1, R1, R3
	ADD R7, R7, #1
	B DIVLOOP
DIVDONE 
	BX LR	
SRC DCD 0X04D2
	AREA mydata, DATA, READWRITE
DST DCD 0
	END