/*
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
*/

#include <stdio.h>
#include <cbe_mfc.h>
#include <spu_mfcio.h>
#include <stdlib.h>

int main (void)
{
	char * pointer1 = (char *) malloc(128);
	char * pointer2 = (char *) malloc(128);
	char * pointer3 = (char *) malloc(128);
	
	printf("\nAllocated Pointer Addresses (128 bytes pointers): %p %p %p\n",pointer1,pointer2,pointer3);

	free(pointer1);
	free(pointer2);
	free(pointer3);

	return 0;
}
