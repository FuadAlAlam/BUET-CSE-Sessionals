.MODEL SMALL


.STACK 100H


.DATA

.CODE

MAIN PROC
        
    MOV 	AH, 1	; read character function
	INT 	21h	; char in AL

	CMP	AL, 'A'	; char >= ‘A’
	JNGE 	END_IF	; no, exit
	CMP	AL, 'Z'	; char <= ‘Z’
	JNLE 	END_IF	; no, exit

	MOV 	DL, AL	; get char
	MOV 	AH, 2	; display character function
	INT 	21h	; display the character
END_IF:
    
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN
