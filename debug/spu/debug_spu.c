/*
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cbe_mfc.h>
#include <spu_mfcio.h>

extern int _etext;
extern int _edata;
extern int _end;

int
main(void)
{
	printf("\n&_etext: %p\n",&_etext);
	printf("\n&_edata: %p\n",&_edata);
	printf("\n&_end: %p\n",&_end);
	printf("\nsbrk(0): %p\n",sbrk(0));
	printf("\nmalloc(1024): %p\n",malloc(1024));
	printf("\nsbrk(0): %p\n",sbrk(0));

	return 0;
}
