#include <cbe_mfc.h>
#include <spu_mfcio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/dma.h"

int main(unsigned long long speid, unsigned long long argp, unsigned long long * envp) {
	message_bus m __attribute__ ((aligned (128)));

	mfc_get(&m, argp, sizeof(m), 31, 0, 0);

	/* The tag bit is always 1 left-shifted by the tag specified */
	mfc_write_tag_mask(1<<31);

	/* Read and wait for the DMA completion */
	mfc_read_tag_status_all();

	printf("Address = %x\n", m.addr);

	return 0;
}
