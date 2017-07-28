#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12

#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19

.globl    pole
.data
.align    2

pole:

.word    5,3,4,1,2

.text
.globl start
.ent start

start:
// Zde muzete psat Vas program
//s0 for array address 
//s1 for i
//s2 for j
	
	la s0, pole	//read the address of the array
	mov s1, $0	//i=0
	addi t1, $0 ,4	//t= 4
	
	OutterLoop:	
		beq  s1, t1, exit	//if (i==4) exit
		
		move s2, $0		//j=0 
		sub t2, t1, s1		//t2= 4-i
		addi s1, s1, 1		//i++
		move t0, s0		//move to t0 the address of the first element for the inner loop
		InnerLoop:
			beq s2, t2, OutterLoop	//if (j==4-i)
		
			lw t3, 0(t0)   //t3 = array[j]
			lw t4, 4(t0)   //t4 = array [j+1]
			
			slt t5, t3, t4 //if (t3 < t4) t5=1	
			bne t5, $0 , notswap  //if (t3 < t4) notswap

			sw t4, 0(t0)
			sw t3, 4(t0)
			notswap:
			addi t0,t0, 4		//add 4 to move to next item

			addi s2,s2, 1		//j++
			j InnerLoop
	

			
			
exit:
	nop

nop
.end start


