/*
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
*/

#include <sched.h>
#include <libspe.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "include/dma.h"

extern spe_program_handle_t dma_spu;

int main(void) {
	spe_gid_t gid;
	speid_t speid;
	int status, *data; 
	message_bus m[1] __attribute__ ((aligned(128)));

	/* here is the malloc call */ 
	data = (int *) malloc(128);

	/* Create an SPE group */
	if ((gid = spe_create_group(SCHED_OTHER,0,1)) == NULL) {
	  fprintf(stderr, "spe_create_group error: %d\n", errno);
	  exit(EXIT_FAILURE);
	}

	if (spe_group_max (gid) < 1) {
		fprintf(stderr, "System doesn't have a working SPE.  I'm leaving.\n");
		exit(EXIT_FAILURE);
	}

	printf("Data address: %x\n", (unsigned int) data);
	m[0].addr = (unsigned int) data;

	if ( (speid = spe_create_thread (gid, &dma_spu, (unsigned long long *) &m[0], NULL, -1, 0)) == NULL ) {
		fprintf (stderr, "spe_create_thread error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	spe_wait(speid, &status, 0);

	/* Why not sync? */
	__asm__ __volatile__ ("sync" : : : "memory");

	return 0;
}
