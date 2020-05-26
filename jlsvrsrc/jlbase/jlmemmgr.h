/*
@author:lightjia
@date:2020/5/20
@desc:内存管理模块
*/
#ifndef __JL_MEMMGR__H_
#define __JL_MEMMGR__H_
#include "jlsingleton.h"
#include "jldef.h"
#include <atomic>
namespace jlsvr
{
    namespace jlbase
    {
        class CJlMemMgr : public jlsvr::jlbase::CJlSingleton<CJlMemMgr>
        {
            SINGLE_CLASS_INITIAL(CJlMemMgr);

        public:
            static void *MemMalloc(size_t iLen);
            static void MemFree(void *pData);
            static void *MemCalloc(size_t count, size_t size);
            static void *MemRealloc(void *ptr, size_t size);

        private:
            void *DoMalloc(size_t iLen);
            void DoFree(void *pData);
            void *DoCalloc(size_t count, size_t size);
            void *DoRealloc(void *ptr, size_t size);
            size_t GetMemLen(void *pData);

        private:
            std::atomic_uint_least64_t miMalloc;
            std::atomic_uint_least64_t miFree;
        };

        const struct jl_mem_oper_func JL_MEMMGR_MEM_FUNC = {CJlMemMgr::MemMalloc, CJlMemMgr::MemFree, CJlMemMgr::MemCalloc, CJlMemMgr::MemRealloc};

        class CJlClassMemMgr
        {
        public:
            static void *operator new(size_t size);
            static void operator delete(void *p);
        };
    } // namespace jlbase
} // namespace jlsvr

#endif //__JL_MEMMGR__H_