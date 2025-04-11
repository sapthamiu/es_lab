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
	; Load source array base address
	LDR R0, =SRC
	LDR R1, [R0], #4
	LDR R2, [R0], #4
	LDR R3, [R0], #4
	LDR R4, [R0], #4
	LDR R5, [R0], #4
	LDR R6, [R0], #4
	LDR R7, [R0], #4
	LDR R8, [R0], #4
	LDR R9, [R0], #4
	LDR R10, [R0]
	; Store all values on the stack
	STM R13!, {R1-R10}

	; Initialize loop counters
	MOV R0, #0 ; counter for i (outer loop)
	MOV R8, R13
	MOV R12, R13 ; Backup the stack pointer

	ADD R8, #4 ; Nullify the first line on first entry to LOOP1

LOOP1 
	SUB R8, R8, #4 ; Move R8 to point to the ith element
	MOV R13, R8
	LDMDB R13!, {R1} ; Load minimum value
	MOV R2, R13      ; Store address of minimum
	MOV R6, R1       ; Store current value
	MOV R7, R2       ; Store current address

	ADD R3, R0, #1   ; Initialize j (inner loop)

LOOP2 
	LDMDB R13!, {R4} ; Load next element
	MOV R5, R13      ; Address of the next element

	CMP R1, R4       ; Compare current minimum with R4
	MOVLO R1, R4     ; Update minimum if R4 is smaller
	MOVLO R2, R5     ; Update minimum address

	CMP R3, #9       ; Check if we reached the end of the array
	ADD R3, #1       ; Increment j
	BLO LOOP2        ; Continue inner loop if not done

	STR R1, [R7]     ; Swap minimum with the ith element
	STR R6, [R2]     ; Swap original ith element with minimum

	CMP R0, #8       ; Check if outer loop is done
	ADD R0, #1       ; Increment i
	BLO LOOP1        ; Continue outer loop if not done

	; Restore sorted values from the stack
	LDMDB R12!, {R1-R10}

	; Store sorted values back to SRC
	LDR R0, =SRC
	STR R1, [R0], #4
	STR R2, [R0], #4
	STR R3, [R0], #4
	STR R4, [R0], #4
	STR R5, [R0], #4
	STR R6, [R0], #4
	STR R7, [R0], #4
	STR R8, [R0], #4
	STR R9, [R0], #4
	STR R10, [R0]

STOP 
	B STOP
	AREA mydata, DATA, READWRITE
SRC 
	DCD 2, 4, 6, 1, 3, 10, 7, 5, 9, 8
	END
