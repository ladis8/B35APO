//v kodu netestuju jestli byl uspesny -a3 --> nastavuju navratovou hodnotu na -1???
#include "subroutine.h"

int subroutine_fnc(int filedescriptor)

{
	int res;
	char ch;
	
	//a0 fd //a1 input buffer //a2 maxnum	
	//res = INTERNAL_SYSCALL(read, err_res,3, filedescriptor, &ch,1);
	res = read(filedescriptor, &ch, 1);
	//printf ("subroutine: %d\n", ch);

	if (res < 0){	//maybe !=-1
		//INTERNAL_SYSCALL(exit,err_res, 1,1);
		exit(1);
	}
	if (res!= 0)
		return (int) ch;
	else
		return 0;
		
}



