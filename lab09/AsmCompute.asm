;;*****************************************************************************
;;
;; Assembly language subroutine
;;
;; Author: Gary J. Minden
;; Organization: KU/EECS/EECS 388
;; Date: 2014-04-18
;; Version: 1.0
;;
;; Purpose: Assembly language subroutine
;;
;; Notes:
;;
;;*****************************************************************************
;;
;;
;; This subroutine computes a value based on four input arguments.
;; The arguments are assumed to be in CPU registers R0 - R3
;; (aka A1 - A4).
;; The result is returned in R0.
;;
;; Declare sections and external references
	.global AsmCompute ; Declare entry point as a global symbol
;; No constant data
;; No variable allocation
;; Program instructions
	.text ; Program section
AsmCompute: ; Entry point
;;
;; Save necessary registers
;;
;; Since this subroutine does not use registers other than R0 - R3,
;; and we do not call another subroutine, we don't need to save
;; any registers.
;;
;;
;; Allocate local variables on the Stack
;;
;; Since this subroutine does not have local variables,
;; no Stack space need be allocated.
;;
;;
;; For demonstration, this subroutine computes R0 + R1 + R2 + R3
;; and returns the result in R0.
;;
;; You should replace the following three instructions with the
;; the instructions necessary to carry out your assigned
;; algebraic assignment statements.
;;
	PUSH    {R4, R5, R6}
	UDIV	R4, R0, R2
	MULT	R4, R4, R3
	ADD		R4, R4, R0
	AND		R5, R1, R2
	EOR     R5, R4, R5

	UDIV    R6, R1, R2
	ADD     R6, R6, R0

	ADD     R0, R6, R5

	POP     {R4, R5, R6}
;;
;; Return from the subroutine with the result in R0
;;
	BX LR ; Branch to the program address in the Link Register
.end
