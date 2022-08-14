#ifndef COMMON_H
#define COMMON_H

#ifndef u_int8_t
typedef signed char u_int8_t;
#endif

#ifdef _WIN32
#include "THAllocator.h"
#else
#include <sys/mman.h>
#endif

inline void* p_mmap(void** _ctx,int size){
#ifdef _WIN32
//  THMapAllocatorContext *ctx = THMapAllocatorContext_new(NULL, TH_ALLOCATOR_MAPPED_SHARED);
//  char* memory = (char*)THMapAllocator.malloc(ctx, size);
//  _ctx[0] = ctx;
    char* memory = (char*)malloc(size);
#else
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
