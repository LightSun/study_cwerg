#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#ifndef u_int8_t
typedef signed char u_int8_t;
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

void* p_mmap(void** _ctx,int size);

void* p_mark_executable(const void* data, int size);

void p_ummap(void** _ctx,void* data);

#ifdef __cplusplus
}
#endif

#endif // COMMON_H
