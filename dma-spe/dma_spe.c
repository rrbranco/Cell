/*
 * Copyleft (C) 2009
 * 	Rodrigo Rubira Branco (BSDaemon) 
 *	<rodrigo_branco *noSPAM* research.coseinc.com>
*/

#include <stdio.h>
#include <libspe.h>
#include <libmisc.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "include/dma.h"

extern spe_program_handle_t spe1;
extern spe_program_handle_t spe2;

int main (void)
{
	speid_t speids[NR_SPEIDS];
	int i=0;
	char * buffer;

	addr_t * addresses;

	if ( (buffer = malloc_align(128,7) ) == NULL ) {
		fprintf(stderr, "malloc_align 1\n");
		exit(EXIT_FAILURE);
	}
	if ( (addresses = malloc_align(sizeof(addr_t),7) ) == NULL ) {
		fprintf(stderr, "malloc_align 2\n");
		exit(EXIT_FAILURE);
	}

	memset(buffer, 0, 128);
	strcpy (buffer, "Something...");
	
	/* Our structure */
	addresses->bPtr = buffer;
	addresses->size = 128;

	if ( (speids[0] = spe_create_thread (0, &spe1, NULL, NULL, -1, 0)) == NULL ) {
		fprintf(stderr, "spe_create_thread 1 error %d\n",errno);
		exit(EXIT_FAILURE);
	}
	if ( (speids[1] = spe_create_thread (0, &spe2, NULL, NULL, -1, 0)) == NULL ) {
		fprintf(stderr, "spe_create_thread 2 error %d\n",errno);
		exit(EXIT_FAILURE);
	}

	/* Defining the address into the local store */
	for (i=0; i<NR_SPEIDS; i++)
		addresses->lstore[i] = spe_get_ls(speids[i]);

	/* For each SPE thread */
	for (i=0; i<NR_SPEIDS; i++){
		/* Wait for the byte offset in the local store */
		while (!spe_stat_out_mbox(speids[i])) {}
			/* Add the returned value to the specific local store virtual address */
			addresses->lstore[i] += spe_read_out_mbox(speids[i]);
		printf("SPE %d has location at %x\n", i, (unsigned int) addresses->lstore[i]);
	}

	/* Giving a pointer to the structure */
	for (i=0;i<NR_SPEIDS;i++)
		spe_write_in_mbox(speids[i], (int) addresses);

	/* While SPE 0 not completed, wait... */
	while (!spe_stat_out_mbox(speids[0])) {}
	int trigger = spe_read_out_mbox(speids[0]);

	for (i=1; i<NR_SPEIDS; i++) {
		/* Modify buffer */
		spe_write_in_mbox(speids[i], 1);

		/* While SPE 1 not completed, wait... */
		while (!spe_stat_out_mbox(speids[i])) {}
		trigger = spe_read_out_mbox(speids[i]);
	}

	/* Kill the threads */
	for (i=0; i<NR_SPEIDS; i++)
		spe_kill(speids[i], SIGKILL);

	/* Why not sync? */
	__asm__ __volatile__ ("sync" : : : "memory");

	printf("PPE   received \"%s\"\n", buffer);

	return 0;
}
