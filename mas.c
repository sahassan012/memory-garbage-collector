#include "memlib.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mm.h"
#include "mas.h"

#define WSIZE 4
#define GET(p) (*(unsigned int *)(p))

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7) //line:vm:mm:getsize

struct node * root_table[ROOT_NR] = {};

int in_use(unsigned int * hdrp) {
  return (*hdrp) & 0x1u;
}

// the three lowest bits are not part of the size
//0x1 is already used for free blocks; use 0x2
#define MARK_BIT (((unsigned int)0x2))

// marking related operations
int is_marked(unsigned int * hdrp) {
  return ((*hdrp) & MARK_BIT) >> 1; // return 1 or 0
}

void mark(unsigned int * hdrp) {
  (*hdrp) |= MARK_BIT;
}

void unmark(unsigned int * hdrp) {
  (*hdrp) &= ~MARK_BIT;
}

// same to GET_SIZE in mm.c
unsigned int block_size(unsigned int * hdrp) {
  return (*hdrp) & (~7u);
}

void* next_hdrp(unsigned int * hdrp) {
  const unsigned int size = block_size(hdrp);
  if (size == 0) {
    fprintf(stderr,"%s Panic, chunk is of zero size.\n", __func__);
    exit(-1);
  }
  hdrp = (unsigned int *)(((char *)hdrp) + size);
  return block_size(hdrp) ? hdrp : NULL;
}

void heap_stat(const char * msg)
{
  void *hdrp = mm_first_hdr();
  size_t nr_inuse = 0;
  size_t sz_inuse = 0;
  size_t nr_free = 0;
  size_t sz_free = 0;

  while (hdrp && block_size(hdrp)) {
    if (in_use(hdrp)) {
      nr_inuse++;
      sz_inuse += block_size(hdrp);
    } else {
      nr_free++;
      sz_free += block_size(hdrp);
    }
    if (is_marked(hdrp))
      printf("%s WARNING: found a mark\n", __func__);
    hdrp = next_hdrp(hdrp);
  }
  printf("%s: %s: heapsize %zu  inuse %zu %zu  free %zu %zu\n", __func__, msg,
      mem_heapsize(), nr_inuse, sz_inuse, nr_free, sz_free);
}


//
// gc_mark:
// 	(Reference: algorithm from Compueter Systems book Section 9.5)
void gc_mark(){ 

	struct node *p;
	for(int i = 0; i < ROOT_NR; i++){
		
		p = root_table[i];
		while(p != NULL){
			void *x = HDRP(p);
			if (!is_marked(x)){
				mark(x);
       	 		}
			p = p->next;
		}
	}
	return;
}

//
//  gc_sweep():
//
void gc_sweep(){
	
	void *hdrp = mm_first_hdr();
	while(hdrp && block_size(hdrp)){
		
		if (is_marked(hdrp)){
			unmark(hdrp);
		}
		else if (in_use(hdrp)){
			mm_free(hdrp+4);
		}
		hdrp = next_hdrp(hdrp);
	}
	return;
}

//
// gc():
//   function that calls mark and sweep
void gc() {
	gc_mark();
	gc_sweep();
	return;
}

