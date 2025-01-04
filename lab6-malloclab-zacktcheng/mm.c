/* 
 * mm-implicit.c -  Simple allocator based on implicit free lists, 
 *                  first fit placement, and boundary tag coalescing. 
 *
 * Each block has header and footer of the form:
 * 
 *      31                     3  2  1  0 
 *      -----------------------------------
 *     | s  s  s  s  ... s  s  s  0  0  a/f
 *      ----------------------------------- 
 * 
 * where s are the meaningful size bits and a/f is set 
 * iff the block is allocated. The list has the following form:
 *
 * begin                                                          end
 * heap                                                           heap  
 *  -----------------------------------------------------------------   
 * |  pad   | hdr(8:a) | ftr(8:a) | zero or more usr blks | hdr(8:a) |
 *  -----------------------------------------------------------------
 *          |       prologue      |                       | epilogue |
 *          |         block       |                       | block    |
 *
 * The allocated prologue and epilogue blocks are overhead that
 * eliminate edge conditions during coalescing.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
  /* Team name */
  "zacktcheng",
  /* First member's full name */
  "Zack Cheng",
  /* First member's email address */
  "tsch3115@colorado.edu",
  /* Second member's full name (leave blank if none) */
  "",
  /* Second member's email address (leave blank if none) */
  ""
};

/////////////////////////////////////////////////////////////////////////////
// Constants and macros
//
// These correspond to the material in Figure 9.43 of the text
// The macros have been turned into C++ inline functions to
// make debugging code easier.
//
/////////////////////////////////////////////////////////////////////////////
#define WSIZE       4       /* word size (bytes) */  
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */

//
// Custom constants and macros
//
#define MINSIZE     24      /* 8 bytes for header + footer, plus 16 bytes for 2 ptrs */
#define ALIGN(p) (((size_t)(p) + (DSIZE -1)) & ~0x7) /* align to multiples of 8 */
#define NEXT(bp) (*(void **)(bp + DSIZE)) /* get address of next free block */
#define PREV(bp) (*(void **)(bp)) /* get address of prev free block */

static inline int MAX(int x, int y) {
  return x > y ? x : y;
}

//
// Pack a size and allocated bit into a word
// We mask of the "alloc" field to insure only
// the lower bit is used
//
static inline uint32_t PACK(uint32_t size, int alloc) {
  return ((size) | (alloc & 0x1));
}

//
// Read and write a word at address p
//
static inline uint32_t GET(void *p) { 
  return  *(uint32_t *)p;
}

static inline void PUT(void *p, uint32_t val) {
  *((uint32_t *)p) = val;
}

//
// Read the size and allocated fields from address p
//
static inline uint32_t GET_SIZE(void *p)  { 
  return GET(p) & ~0x7;
}

static inline int GET_ALLOC(void *p ) {
  return GET(p) & 0x1;
}

//
// Given block ptr bp, compute address of its header and footer
//
static inline void *HDRP(void *bp) {
  return ((char *)bp) - WSIZE;
}

static inline void *FTRP(void *bp) {
  return ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE);
}

//
// Given block ptr bp, compute address of next and previous blocks
//
static inline void *NEXT_BLKP(void *bp) {
  return ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)));
}

static inline void* PREV_BLKP(void *bp) {
  return ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)));
}

/////////////////////////////////////////////////////////////////////////////
//
// Global Variables
//

static char *heap_listp;  /* pointer to first block */
static char* list_ptr; /* pointer to the head of the free list */

//
// function prototypes for internal helper routines
//
static void *extend_heap(uint32_t words);
static void place(void *bp, uint32_t asize);
static void *find_fit(uint32_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkblock(void *bp);

//
// Custom functions
//

// ALways add the free block to the head of the free list. This method uses LIFO organization.
static void add_node(void *bp)
{
  NEXT(bp) = list_ptr; /* Set next to the old head of list */ 
  PREV(bp) = NULL; /* Set prev to NULL */
  PREV(list_ptr) = bp; /* Set previous of old head to bp */
  list_ptr = bp; /* Update head to point to newly added node */
}

// Delete the node by updating its next and prev pointers.
static void delete_node(void *bp)
{
  // Store old prev and next links to update
  void* oldprev = PREV(bp);
  void* oldnext = NEXT(bp);
  if (PREV(bp)) NEXT(oldprev) = NEXT(bp); /* If prev link is not NULL, set next link of proceeding bp to skip bp */
  else list_ptr = NEXT(bp); /* otherwise, set new head to be next node after bp */
  PREV(oldnext) = PREV(bp); /* Fix prev link to skip bp, by setting to old prev */
}

//
// mm_init - Initialize the memory manager 
//
int mm_init(void) 
{
  if (!(heap_listp = mem_sbrk(MINSIZE * 2))) return -1; /* Create initial memory heap, extend brk by 48 bytes */
  PUT(heap_listp, 0); /* Create alignment padding of one word */
  PUT(heap_listp + WSIZE, PACK(MINSIZE, 1)); /* Create prologue header */
  PUT(heap_listp + DSIZE, 0); /* Initialize previous pointer */
  PUT(heap_listp + (DSIZE + DSIZE), 0); /* Initialize NEXT pointer */
  PUT(heap_listp + MINSIZE, PACK(OVERHEAD, 1)); /* Free block footer */
  PUT(heap_listp + MINSIZE + WSIZE, PACK(0,1)); /* Epilogue header */
  list_ptr = heap_listp + DSIZE; /* Initialize start of free list */
  return extend_heap(CHUNKSIZE / WSIZE) ? 0 : -1; /* Extend the heap with a free block of 4096 bytes */
}

//
// extend_heap - Extend heap with free block and return its block pointer
//
static void *extend_heap(uint32_t words) 
{
  char* bp; /* bp points to payload of block */
  size_t size;
  size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE; /* Allocate even number of words to maintain alignment */
  if (size < MINSIZE) size = MINSIZE; /* Correct the minimum size to MINSIZE */
  if ((long)(bp = mem_sbrk(size)) == -1) return NULL; /* Try to extend the heap by size, keep the old mem_brk in bp */
  // Initialize free block header and footer, then epilogue header
  PUT(HDRP(bp), PACK(size, 0)); /* Free the block header */
  PUT(FTRP(bp), PACK(size, 0)); /* Free the block footer */
  PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* Update NEW epilogue header */
  return coalesce(bp); /* Coalesce with adjacent blocks if possible */
}

//
// Practice problem 9.8
//
// find_fit - Find a fit for a block with asize bytes 
//
static void *find_fit(uint32_t asize)
{
  void* bp;
  // Initliaze search at heap_listp, check condition: the current block is non allocated
  for (bp = list_ptr; GET_ALLOC(HDRP(bp)) == 0; bp = NEXT(bp)) {
    if (asize <= GET_SIZE(HDRP(bp))) return bp; /* If current block is big enough, return */
  }
  return NULL; /* no fit */
}

// 
// mm_free - Free a block 
//
void mm_free(void *bp)
{
  if (!bp) return; /* Don't attempt to free NULL pointers */
  size_t size=GET_SIZE(HDRP(bp)); /* Get current block size */
  // Clear headers and footers to nonallocated
  PUT(HDRP(bp), PACK(size, 0));
  PUT(FTRP(bp), PACK(size, 0));
  coalesce(bp);
}

//
// coalesce - boundary tag coalescing. Return ptr to coalesced block
//
static void *coalesce(void *bp) 
{
  // Determine if prev and next block are allocated, and the current block's size
  // If prev footer is alloc, or if bp is at head of list
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))) || PREV_BLKP(bp) == bp;
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));
  if (prev_alloc && !next_alloc) {
    // Case 1: merge with next block
    size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
    // Adjust pointers in free list
    delete_node(NEXT_BLKP(bp));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  } else if (!prev_alloc && next_alloc) {
    // Case 2: merge with prev block
    size += GET_SIZE(HDRP(PREV_BLKP(bp)));
    bp = PREV_BLKP(bp);
    delete_node(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  } else if (!prev_alloc && !next_alloc) {
    // Case 3: merge all 3 blocks
    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
    delete_node(PREV_BLKP(bp));
    delete_node(NEXT_BLKP(bp));
    bp = PREV_BLKP(bp);
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
  }
  add_node(bp); /* Add new coalesced block to start of free list */
  return bp;
}

//
// mm_malloc - Allocate a block with at least size bytes of payload 
//
void *mm_malloc(uint32_t size) 
{
  size_t asize; /* Adjusted block size */
  size_t finalsize; /* Amount to extend heap if no fit found */
  char *bp;
  if (!size) return NULL; /* Bad request */
  // Adjust block size to include overhead and alignment requirements,
  // each block must be at least 24 bytes, and be aligned to multiples of 8
  asize = MAX(ALIGN(size) + DSIZE, MINSIZE);
  if ((bp = find_fit(asize))) {
    // Search free list for a fit, place it if found
    place(bp, asize);
    return bp;
  }
  // No fit exists, extend the heap, extend heap by Minimum of 4096 bytes 
  finalsize = MAX(asize, CHUNKSIZE);
  if (!(bp = extend_heap(finalsize / WSIZE))) return NULL;
  place(bp, asize); /* Place according to first fit, and possibly split */
  return bp;
} 

//
//
// Practice problem 9.9
//
// place - Place block of asize bytes at start of free block bp 
//         and split if remainder would be at least minimum block size
//
static void place(void *bp, uint32_t asize)
{
  size_t csize = GET_SIZE(HDRP(bp)); /* Store current size of block */
  if ((csize - asize) >= MINSIZE) {
    // If 24 blocks are left over after placing the new block, split the block
    // Place current header and footer
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
    delete_node(bp); /* Update pointers */
    bp = NEXT_BLKP(bp); /* Move bp to the next block payload, this was changed by first placing the new block */
    // Place header and footer as empty for split part
    PUT(HDRP(bp), PACK(csize - asize, 0));
    PUT(FTRP(bp), PACK(csize - asize, 0));
    coalesce(bp);
  } else {
    // otherwise, just place block, adjust headers and footers
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
    delete_node(bp); /* Update pointers */
  }
}


//
// mm_realloc -- implemented for you
//
void *mm_realloc(void *ptr, uint32_t size)
{
  void *newp;
  uint32_t copySize;

  newp = mm_malloc(size);
  if (newp == NULL) {
    printf("ERROR: mm_malloc failed in mm_realloc\n");
    exit(1);
  }
  copySize = GET_SIZE(HDRP(ptr));
  if (size < copySize) {
    copySize = size;
  }
  memcpy(newp, ptr, copySize);
  mm_free(ptr);
  return newp;
}

//
// mm_checkheap - Check the heap for consistency 
//
void mm_checkheap(int verbose) 
{
  //
  // This provided implementation assumes you're using the structure
  // of the sample solution in the text. If not, omit this code
  // and provide your own mm_checkheap
  //
  void *bp = heap_listp;
  
  if (verbose) {
    printf("Heap (%p):\n", heap_listp);
  }

  if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp))) {
	printf("Bad prologue header\n");
  }
  checkblock(heap_listp);

  for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
    if (verbose)  {
      printblock(bp);
    }
    checkblock(bp);
  }
     
  if (verbose) {
    printblock(bp);
  }

  if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp)))) {
    printf("Bad epilogue header\n");
  }
}

static void printblock(void *bp) 
{
  uint32_t hsize, halloc, fsize, falloc;

  hsize = GET_SIZE(HDRP(bp));
  halloc = GET_ALLOC(HDRP(bp));  
  fsize = GET_SIZE(FTRP(bp));
  falloc = GET_ALLOC(FTRP(bp));  
    
  if (hsize == 0) {
    printf("%p: EOL\n", bp);
    return;
  }

  printf("%p: header: [%d:%c] footer: [%d:%c]\n",
	 bp, 
	 (int) hsize, (halloc ? 'a' : 'f'), 
	 (int) fsize, (falloc ? 'a' : 'f')); 
}

static void checkblock(void *bp) 
{
  if ((uintptr_t)bp % 8) {
    printf("Error: %p is not doubleword aligned\n", bp);
  }
  if (GET(HDRP(bp)) != GET(FTRP(bp))) {
    printf("Error: header does not match footer\n");
  }
}
