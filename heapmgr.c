#include <stdio.h>
#include <stdlib.h>

// #include "memorymgr.h"

int *firstblk;
int *sentblk;
int *lastblk;
int remaining_bytes = 0;

int isAllocated(int *p);  // is the block at location p allocated?
int *nextBlock(int *p);    // return a pointer to the block that follows p
int *firstBlock();         // return a pointer to the first block on the heap
int *lastBlock();          // return a pointer to the sentinel block
void initmemory(int size);  // initialize a simulated heap
void *myalloc(int length);   // allocate a block of length (or more) bytes
void myfree(void *ptr);     // free an allocated block
void coalesce();            // go through the heap, coalescing unallocated blocks
void printallocation();  // print info about the simulated heap, for debugging purposes
void printresult(int *p);


int main(){
  int *p1, *p2;
  printf("initial allocation\n");
  initmemory(56);
  printf("-------\n");
  printf("malloc 20\n");
  p1 = (int *)myalloc(20);
  printallocation();
  printf("-------\n");

  printf("malloc 10\n");
  p2 = (int *)myalloc(10);
  printallocation();
  printf("-------\n");

  printf("free (malloc 20)\n");
  myfree(p1);
  p1 = NULL;
  printallocation();

  printf("malloc 4\n");
  p1 = (int *)myalloc(4);
  printresult(p1);

  printf("free (malloc 10)\n");
  myfree(p2);
  p2 = NULL;
  printallocation();

  printf("malloc 30: should fail\n");    // this will fail because we need a block of at least 40
  p2 = (int *)myalloc(30);
  printresult(p2);

  printf("coalesce\n");
  coalesce();
  printallocation();

  printf("malloc 30 \n");  // now this succeeds
  p2 = (int *) myalloc(30);
  printresult(p2);

  printf("free everything\n");
  myfree(p1); p1 = NULL;
  myfree(p2); p2 = NULL;
  printallocation();
  printf("malloc 56: should fail\n");

  p1 = (int *)myalloc(56);
  printresult(p1);
  printf("coalesce\n");
  coalesce();
  printallocation();
  printf("malloc 56\n");
  p1 = (int *)myalloc(56);
  printresult(p1);
}

void *myalloc(int length){
  int save;
  int *save_ptr;
  int *ptr = firstblk;
  int newlen = length + 4;
  while(ptr <= lastblk){
    if((*ptr) % 2 != 1){
      if(*(ptr - 1) >= newlen){
        *ptr = 1;
        save = *(ptr-1);
        save_ptr = ptr;
        *(ptr-1) = newlen;
        ptr = ptr + newlen;
        *(ptr) = 0;
        remaining_bytes = remaining_bytes - newlen;
        *(ptr-1) = save - newlen;
        return save_ptr;
      }
    }
    ptr = nextBlock(ptr);
  }
  return NULL;
}

void initmemory(int size){
  int newlen = size + 12;
  remaining_bytes = newlen;
  int *heap = (void*) malloc(newlen);
  printf("%d : heap \n", heap);
  lastblk = heap + newlen - 4;
  printf("%d : lastblk \n", lastblk);
  firstblk = heap + 1;
  *firstblk = 0;
  *(firstblk - 1) = (newlen - 4);
  sentblk = nextBlock(firstblk);
}

void myfree(void *p){
  int *ptr = (int*) p;
  *ptr = 0;
}

void coalesce(){
  int *p2;
  int x1, x2 = 0;
  int *ptr = firstblk;
  p2 = nextBlock(ptr);
  while(p2 <= lastblk){
    if((*ptr) % 2 != 1){
      if((*p2) % 2 != 1){
        x1 = *(ptr-1);
        x2 = *(p2-1);
        *(ptr-1) = x1 + x2;
        p2 = nextBlock(ptr);
      }
      else{
        ptr = nextBlock(ptr);
        p2 = nextBlock(ptr);
      }
    }
    else{
      ptr = nextBlock(ptr);
      p2 = nextBlock(ptr);
    }
  }
}

void printallocation(){
  int count = 0;
  int *ptr = firstblk;
  while(ptr <= lastblk){
    if(count == 7){
      break;
    }
    if((*ptr) % 2 == 1){
      printf("Block %d: ", count);
      printf("size: %d    allocated\n", *(ptr-1));
    }
    else {
      printf("Block %d: ", count);
      printf("size: %d    unallocated\n", *(ptr-1));
    }
    ptr = nextBlock(ptr);
    count ++;
  }
}

int *nextBlock(int *p){
  int *ptr = p + *(p-1);
  return ptr;
}

int *firstBlock(){
  return firstblk;
}

int *lastBlock(){
  return lastblk;
}

int  isAllocated(int *p){
  return *(p-1) != 0;
}

void printresult(int *p) {
  if (p == NULL){
    printf("allocation failed.\n");
  }
  else{
    printallocation();
    printf("\n");
  }
}
