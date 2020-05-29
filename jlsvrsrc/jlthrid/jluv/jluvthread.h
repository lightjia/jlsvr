/*
@author:lightjia
@date:2020/5/29
@desc:libuv thread操作
*/
#ifndef __JLUVTHREAD__H_
#define __JLUVTHREAD__H_
#include "jlthread.h"
#include "jluvdef.h"
#include "jluvcond.h"
#include "jlcolordef.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvThread : public jlsvr::jlbase::CJlThread
        {
        protected:
            void OnThreadBegin() override final
            {
                if (uv_thread_create(&mstThread, mThreadEntry, (void *)this))
                {
                    jlsvr::jlbase::RedPrint("uv_thread_create error");
                }
            }

            void OnThreadJoin() override final
            {
                uv_thread_join(&mstThread);
            }

            void OnThreadWait(jlsvr::jlbase::_u64 iUsec) override final
            {
                if (iUsec)
                {
                    mcUvThreadCond.TimedWait(iUsec);
                }
                else
                {
                    mcUvThreadCond.Wait();
                }
            }

            void OnThreadActivate() override final
            {
                mcUvThreadCond.Signal();
            }

        private:
            uv_thread_t mstThread;
            jlsvr::jlthrid::CJlUvCond mcUvThreadCond;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVTHREAD__H_