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
	MOV R4, #1			;POWER
	MOV R5, #10			;BASE
	MOV R7, #4			;4 BYTES
	MOV R6, #0			;RESULT
LOOP
	LDRB R1, [R0], #1
	AND R2, R1, #0X0F
	MLA R6, R2, R4, R6
	MUL R4, R4, R5
	
	AND R3, R1, #0XF0
	LSR R3, R3, #4
	MLA R6, R3, R4, R6
	MUL R4, R4, R5
	
	SUBS R7, R7, #1
	BNE LOOP
	
	LDR R8, =DST
	STR R6, [R8]
	
STOP B STOP
SRC DCD 0X12345678
	AREA mydata, DATA, READWRITE
DST DCD 0
	END
    