#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#ifndef u_int8_t
typedef signed char u_int8_t;
#endif
#endif

#ifdef _WIN32
#include "THAllocator.h"
#else
#include <stdlib.h>
#include <sys/mman.h>
#endif

inline void* p_mmap(void** _ctx,int size){
    //for allocte a memory which can execute,
    //https://stackoverflow.com/questions/40936534/how-to-alloc-a-executable-memory-buffer

#ifdef _WIN32
//  THMapAllocatorContext *ctx = THMapAllocatorContext_new(NULL, TH_ALLOCATOR_MAPPED_SHARED);
//  char* memory = (char*)THMapAllocator.malloc(ctx, size);
//  _ctx[0] = ctx;
    char* memory = (char*)malloc(size);
#else
   //malloc doesn't work here
   char* const memory =
      (char*)mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
   return memory;
}

inline void p_ummap(void** _ctx,void* data){
#ifdef _WIN32
   //THMapAllocator.free(_ctx[0], data);
   free(data);
#endif
}

#endif // COMMON_H
