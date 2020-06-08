/*
@author:lightjia
@date:2020/6/8
@desc:libuv barrier定义
*/
#ifndef __JLUVBARRIER__H_
#define __JLUVBARRIER__H_
#include "jluvdef.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvBarrier
        {
        public:
            CJlUvBarrier() { mbInit = false; }
            ~CJlUvBarrier()
            {
                if (mbInit)
                {
                    uv_barrier_destroy(&mstBarrier);
                }
            }

        public:
            int Init(unsigned int iCount)
            {
                mbInit = true;
                return uv_barrier_init(&mstBarrier, iCount);
            }
            
            int Wait()
            {
                return uv_barrier_wait(&mstBarrier);
            }

        private:
            uv_barrier_t mstBarrier;
            bool mbInit;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVBARRIER__H_