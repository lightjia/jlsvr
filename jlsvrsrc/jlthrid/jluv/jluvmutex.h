/*
@author:lightjia
@date:2020/5/29
@desc:libuv 锁定义
*/
#ifndef __JLUVMUTEX__H_
#define __JLUVMUTEX__H_
#include "jlmutex.h"
#include "jluvdef.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvMutex : public jlsvr::jlbase::CJlMutex
        {
        public:
            CJlUvMutex(bool bRecursive = false)
            {
                if (bRecursive)
                {
                    uv_mutex_init_recursive(&mstUvMutex);
                }
                else
                {
                    uv_mutex_init(&mstUvMutex);
                }
            }

            ~CJlUvMutex()
            {
                uv_mutex_destroy(&mstUvMutex);
            }

            void Lock() override
            {
                uv_mutex_lock(&mstUvMutex);
            }

            void UnLock() override
            {
                uv_mutex_unlock(&mstUvMutex);
            }

            int TryLock() override
            {
                return uv_mutex_trylock(&mstUvMutex);
            }

            uv_mutex_t *GetMutex()
            {
                return &mstUvMutex;
            }

        private:
            uv_mutex_t mstUvMutex;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVMUTEX__H_