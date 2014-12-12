#include <stdio.h>
#include <libmisc.h>
#include <spu_mfcio.h>
#include <string.h>
#include "../../include/dma.h"

#define wait_on_mask(x)   mfc_write_tag_mask(x); mfc_read_tag_status_any();

int main(void)
{
	char * buffer;
	addr_t * csPtr;
	addr_t * cs;
	int tag = 1, tag_mask = 1<<tag;
	int addr_size = (sizeof(addr_t)+15)&(~15);

	buffer = malloc_align(128, 7);
	cs     = malloc_align(sizeof(addr_t), 7);
	
	printf("SPE 1 has location at %x\n", (unsigned int)buffer);

	/* Send to PPE */
	spu_write_out_mbox((int) buffer);

	/* Receive the structure from PPE */
	csPtr = (addr_t *) spu_read_in_mbox();
	mfc_get(cs, (unsigned int)csPtr, addr_size, tag, 0, 0);
	wait_on_mask(tag_mask);

	/* Wait for the PPE */
	spu_read_in_mbox();

	/* Change the buffer too */
	printf("SPE 1 received \"%s\"\n", buffer);
	strcpy(buffer, "Modified by me");

	/* Send it out */
	mfc_put(buffer, (int) cs->bPtr, cs->size, tag, 0, 0);
	wait_on_mask(tag_mask);

	/* Advise the PPE */
	spu_write_out_mbox(1);

	/* Infinite loop - the PPE process will kill me */
	while(1) {}
}
