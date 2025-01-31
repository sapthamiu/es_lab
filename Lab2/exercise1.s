	AREA RESET, DATA, READONLY
		EXPORT __Vectors
__Vectors
			DCD 0x10001000
			DCD Reset_Handler
			ALIGN
			AREA mycode, CODE, READONLY
			ENTRY
			EXPORT Reset_Handler
Reset_Handler
	LDR R1, =SRC
	LDR R2, =0x7
	LDR R3, [R1]
STOP B STOP
SRC DCD 8
	END