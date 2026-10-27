.MODEL SMALL


.STACK 100H


.DATA

.CODE

MAIN PROC
                     
	MOV 	CX, 0	; number of ‘*’ to display
	MOV 	AH, 2	; char display function
	MOV 	DL, '*'	; char to display 
	JCXZ    OUTSIDE_LOOP
TOP:
	INT 	21h	; display a star
	
	LOOP	TOP	; repeat 80 times    
                      
OUTSIDE_LOOP:
                      
    ;DOS EXIT
    MOV AH, 4CH
    INT 21H

MAIN ENDP
END MAIN
