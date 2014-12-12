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
	
	printf("SPE 0 has location at %x\n", (unsigned int)buffer);

	/* Send to PPE */
	spu_write_out_mbox((int)buffer);

	/* Receive the structure from PPE */
	csPtr = (addr_t *) spu_read_in_mbox();
	mfc_get(cs, (unsigned int)csPtr, addr_size, tag, 0, 0);
	wait_on_mask(tag_mask);

	/* Read the buffer */
	mfc_get(buffer, (unsigned int)cs->bPtr, cs->size, tag, 0, 0);
	wait_on_mask(tag_mask);
	printf("SPE 0 received \"%s\"\n", buffer);

	/* Change the buffer */
	strcpy (buffer, "Something else");

	/* Send the buffer to the SPE 1 */
	mfc_put(buffer, (unsigned int)cs->lstore[1], cs->size, tag, 0, 0);
	wait_on_mask(tag_mask);

	/* Advise the PPE */
	spu_write_out_mbox(1);

	/* Infinite loop - the PPE process will kill me */
	while(1) {}
}
