#ifndef _H_MESSAGE_BUS
#define _H_MESSAGE_BUS

typedef struct {
	unsigned int  element_size; /* Size of the elements 		   */
	unsigned int  addr;         /* Address filled by the DMA operation */
	unsigned char pad[120];     /* Align to cache (128 bytes)   	   */
} message_bus;

#endif
