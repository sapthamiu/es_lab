/*One array.
One key.
Search the key for the number of occurrences.
Store the number of occurrences in the data memory.
Array and search key in code memory.
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
	LDR R0, =ARR
	LDR R1, =N
	LDR R1, [R1]
	LDR R2, =K
	LDR R2, [R2]
	MOV R4, #0
LOOP
	cmp r1, #0
	beq store
	LDR R3, [R0], #4
	CMP R3, R2
	ADDEQ R4, R4, #1
	SUBS R1, R1, #1
	BNE LOOP
store
	LDR R5, =FREQ
	STR R4, [R5]
STOP B STOP
N DCD 6
ARR DCD 1, 2, 3, 4, 4, 4
K DCD 4
	AREA mydata, DATA, READWRITE
FREQ DCD 0
	END
		