/*
@author:lightjia
@date:2020/5/29
@desc:libuv condition定义
*/
#ifndef __JLUVCOND__H_
#define __JLUVCOND__H_
#include "jluvmutex.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvCond
        {
        public:
            CJlUvCond()
            {
                uv_cond_init(&mstCond);
            }

            ~CJlUvCond()
            {
                uv_cond_destroy(&mstCond);
            }

        public:
            void Signal()
            {
                uv_cond_signal(&mstCond);
            }

            void BroadCast()
            {
                uv_cond_broadcast(&mstCond);
            }

            void Wait()
            {
                mcMutex.Lock();
                uv_cond_wait(&mstCond, mcMutex.GetMutex());
                mcMutex.UnLock();
            }

            int TimedWait(uint64_t iTimeOut)
            {
                int iRet = 0;
                mcMutex.Lock();
                iRet = uv_cond_timedwait(&mstCond, mcMutex.GetMutex(), iTimeOut);
                mcMutex.UnLock();

                return iRet;
            }

        private:
            jlsvr::jlthrid::CJlUvMutex mcMutex;
            uv_cond_t mstCond;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVCOND__H_