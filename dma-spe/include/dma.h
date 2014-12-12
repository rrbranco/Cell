#ifndef H_ADDR
#define H_ADDR

#define NR_SPEIDS 2

typedef struct {
	void * lstore[8];
	void * bPtr;
	int size;
} addr_t;

#endif
