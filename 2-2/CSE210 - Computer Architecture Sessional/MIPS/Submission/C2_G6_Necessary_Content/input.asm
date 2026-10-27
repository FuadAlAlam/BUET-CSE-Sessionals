addi $t0, $zero, 5		// $t0 = 5 		// addi ok
 		addi $t1, $zero, 3		// $t1 = 3 		// addi ok
 		sub $t2, $t0, $t1 		// $t2 = 2 		// sub ok
		and $t3, $t0, $t1		// $t3 = 1		// and ok (0101 & 0011 = 0001)
	    ori $t3, $t3, 6		    // $t3 = 7 		// ori ok (0001 | 0110 = 0111)
	    srl $t0, $t0, 1		    // $t0 = 2 		// srl ok (5 >> 1 = 2)
 		beq $t0, $t2, matched	// branching will execute (2 == 2) // beq ok 	
 		j end_program           // skipped because of branch
 
 matched:	sw $t3, 4($zero)		// m[4] = 7		// sw ok
 		lw $t4, 4($zero)		// $t4 = 7		// lw ok                    
		push $t4			    // m[F] = 7 	// push ok                  
 		pop $t1			        // $t1 = 7 		// pop ok		            
 		bneq $t1, $zero, skip	// jump will execute (7 != 0) // bneq ok		    

        add $t0, $t1, $t2		// skipped because of branch

 skip:   nor $t4, $t1, $t0		// $t4 = -8 (or 8 in 4-bit unsigned) // nor ok
 
 end_program: j end_program     // infinite loop to halt program // j ok