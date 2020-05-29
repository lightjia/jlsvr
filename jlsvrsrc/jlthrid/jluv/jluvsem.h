/*
@author:lightjia
@date:2020/5/29
@desc:libuv semphare操作
*/
#ifndef __JLUVSEM__H_
#define __JLUVSEM__H_
#include "jluvdef.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvSem
        {
        public:
            CJlUvSem() { uv_sem_init(&mstUvSem, 0); }
            ~CJlUvSem() { uv_sem_destroy(&mstUvSem); }

        public:
            void Post() { uv_sem_post(&mstUvSem); }
            void Wait() { uv_sem_wait(&mstUvSem); }
            int TryWait() { return uv_sem_trywait(&mstUvSem); }

        private:
            uv_sem_t mstUvSem;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVSEM__H_