/*
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
*/


#include <stdio.h>
#include <libspe.h>

extern spe_program_handle_t debug_spu;

int main(void)
{
	int status;
	speid_t speid;

	speid = spe_create_thread (0, &debug_spu, NULL, NULL, -1, 0 );
	spe_wait (speid, &status, 1);

	return 0;
}
