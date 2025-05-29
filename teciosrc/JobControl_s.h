#pragma once
#include "ThirdPartyHeadersBegin.h"
 #if defined _WIN32
#include <windows.h>
 #else
#include <pthread.h>
 #if defined __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
 #endif
 #endif
#include "ThirdPartyHeadersEnd.h"
#include "MASTER.h"
#include "GLOBAL.h"
#include "Mutex_s.h"
 #if defined _WIN32
// 避免pthread_t类型冲突 - 使用自定义类型名称
#ifndef TECIO_PTHREAD_T_DEFINED
#define TECIO_PTHREAD_T_DEFINED
typedef uintptr_t tecio_pthread_t;
#endif
 #endif
struct ___2122 { 
    ___2664 ___2494; 
    #if defined _WIN32
    std::vector<tecio_pthread_t> ___2648; 
    #else
    std::vector<pthread_t> ___2648; 
    #endif
    static int ___2827(); 
    ___2122() { ___2494 = new ___2665(); } 
    ~___2122() { delete ___2494; } 
    struct ThreadJobData { 
        ___4160 m_job; 
        ___90 m_jobData; 
        ThreadJobData(___4160 ___2118, ___90 ___2123) : m_job(___2118) , m_jobData(___2123) {} 
    };
 #if defined _WIN32
static DWORD WINAPI ___4162(LPVOID data);
 #else
static void *___4162(void* data);
 #endif
void addJob(___4160 ___2118, ___90 ___2123); void wait(); void lock() { ___2494->lock(); } void unlock() { ___2494->unlock(); } }; inline int ___2122::___2827() { int ___2828 = 0;
 #if defined _WIN32
SYSTEM_INFO sysinfo; GetSystemInfo(&sysinfo); ___2828 = static_cast<int>(sysinfo.dwNumberOfProcessors);
 #elif defined __APPLE__
int nm[2]; size_t len = 4; uint32_t count; nm[0] = CTL_HW; nm[1] = HW_AVAILCPU; sysctl(nm, 2, &count, &len, NULL, 0); if(count < 1) { nm[1] = HW_NCPU; sysctl(nm, 2, &count, &len, NULL, 0); if(count < 1) count = 1; } ___2828 = static_cast<int>(count);
 #else
___2828 = static_cast<int>(sysconf(_SC_NPROCESSORS_ONLN));
 #endif
return ___2828; }
 #if defined _WIN32
inline DWORD WINAPI ___2122::___4162(LPVOID data)
 #else
inline void *___2122::___4162(void* data)
 #endif
{ ThreadJobData* ___2123 = reinterpret_cast<ThreadJobData*>(data); ___2123->m_job(___2123->m_jobData); delete ___2123; return NULL; } inline void ___2122::addJob(___4160 ___2118, ___90 ___2123) { lock(); ___2122::ThreadJobData* threadJobData = new ThreadJobData(___2118, ___2123);
 #if defined _WIN32
// 修改类型转换问题
HANDLE threadHandle = CreateThread(NULL, 0, ___4162, threadJobData, 0, NULL);
___2648.push_back(static_cast<tecio_pthread_t>(reinterpret_cast<uintptr_t>(threadHandle)));
 #else
pthread_t thread; pthread_create(&thread, NULL, ___4162, (void*)threadJobData); ___2648.push_back(thread);
 #endif
unlock(); } inline void ___2122::wait() { size_t i; for(i = 0; i < ___2648.size(); ++i) { lock(); 
 #if defined _WIN32
tecio_pthread_t thr = ___2648[i]; 
unlock();
// 修复类型转换问题
WaitForSingleObject(reinterpret_cast<HANDLE>(static_cast<uintptr_t>(thr)), INFINITE);
 #else
pthread_t thr = ___2648[i];
unlock();
pthread_join(thr, NULL);
 #endif
} lock(); ___2648.erase(___2648.begin(), ___2648.begin() + i); unlock(); }
