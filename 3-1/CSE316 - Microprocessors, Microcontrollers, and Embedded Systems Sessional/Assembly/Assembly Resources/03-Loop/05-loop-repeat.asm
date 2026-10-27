.MODEL SMALL

.STACK 100H

.DATA

.CODE

MAIN PROC             
               
	MOV 	AH, 1	; read char function 
	
REPEAT:
	INT 	21h	; read a char in AL
	CMP 	AL, ' '	; a blank?
	JNE	REPEAT	; no, keep reading
       
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN
