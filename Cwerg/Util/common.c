#include "common.h"

#ifdef _WIN32
#include "THAllocator.h"
#include <windows.h>
#else
#include <stdlib.h>
#include <sys/mman.h>
#endif

void* p_mmap(void** _ctx,int size){
    //for allocte a memory which can execute,
    //https://stackoverflow.com/questions/40936534/how-to-alloc-a-executable-memory-buffer

#ifdef _WIN32
//  THMapAllocatorContext *ctx = THMapAllocatorContext_new(NULL, TH_ALLOCATOR_MAPPED_SHARED);
//  char* memory = (char*)THMapAllocator.malloc(ctx, size);
//  _ctx[0] = ctx;
    char* memory = (char*)malloc(size);
   // void* memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
   // DWORD dummy;
   // VirtualProtect(memory, size, PAGE_EXECUTE_READWRITE, &dummy);
#else
   //malloc doesn't work here
   char* const memory =
      (char*)mmap(nullptr, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
   return memory;
}

void* p_mark_executable(const void* _data, int size){
    void* memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    memcpy(memory, _data, size);
    DWORD dummy;
    VirtualProtect(memory, size, PAGE_EXECUTE_READ, &dummy);
    return memory;
}

void p_ummap(void** _ctx,void* data){
#ifdef _WIN32
   //THMapAllocator.free(_ctx[0], data);
   free(data);
   //VirtualFree(data, 0, MEM_RELEASE);
#endif
}
