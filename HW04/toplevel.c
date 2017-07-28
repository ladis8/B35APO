/* Implementation of the subroutine toplevel_fnc */

#include "subroutine.h"

char mydata[2] = {'\\', '\\'};

int toplevel_fnc(void)
{
  
  int counter =0;
   do {

   	int  c; //maybe unsigned //lbu
   	c = subroutine_fnc(0);
	if (c <= 0)
		break;


   	if (c == 92){
		int ret;
   		//ret = INTERNAL_SYSCALL(write, err_res,1, mydata,2);
		counter++;
		ret = write(1, mydata, 2); 
		if (ret < 0)	//maybe !=-1
			//INTERNAL_SYSCALL(exit,err_res, 1,1);
			exit(1);

	}
	else {
		
		int ret;
   	//	ret = INTERNAL_SYSCALL(write, err_res,1, &c,1);
		counter++;
		ret = write(1, &c, 1);
		if (ret < 0)	//maybe !=-1
			exit(1);
	}

     } while (1);
     return counter;
 }
		//else jump to do{

		

	



  /*
   * fill in code with equivalent function
   * to the toplevel_fnc function from
   * the assembly program listing.
   */

  /*
   * The code calls function subroutine_fnc
   * and processes its return value.
   * 
   * Then it resturn final result.
   */

