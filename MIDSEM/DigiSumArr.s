/*Another set
An array of hexadecimal numbers (each of 8 bits)
Extract the digits and give the sum.
(Like 0x23, the value should be 5)
*/
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
	LDR R1, =N
	LDR R1, [R1]
	LDR R2, =DST
LOOP
	MOV R6, #0			;SUM
	LDR R3, [R0], #4
	AND R4, R3, #0X0F
	AND R5, R3, #0XF0
	LSR R5, #4
	ADD R6, R4, R5
	STR R6, [R2], #4
	SUBS R1, R1, #1
	BNE LOOP	
STOP B STOP
N DCD 5
SRC DCD 0XAB, 0XBC, 0X1F, 0X43, 0XD5
	AREA mydata, DATA, READWRITE
DST DCD 0
	END