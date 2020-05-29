/*
@author:lightjia
@date:2020/5/23
@desc:线程基类
*/
#ifndef __JLTHREAD__H_
#define __JLTHREAD__H_
#include "jlcommoninc.h"
namespace jlsvr
{
    namespace jlbase
    {
        typedef void (*THREAD_ENTRY)(void*);
        class CJlThread
        {
        public:
            CJlThread();
            virtual ~CJlThread();

        public:
            void ThreadBegin() { OnThreadBegin(); }
            void ThreadJoin() { OnThreadJoin(); }
            void ThreadWait(jlsvr::jlbase::_u64 iUsec) { OnThreadWait(iUsec); }
            void ThreadActivate() { OnThreadActivate(); }
            void ThreadRun()
            {
                while (!mbJoin)
                    OnThreadRun();
            }

        protected:
            virtual void OnThreadBegin() {}
            virtual void OnThreadJoin() {}
            virtual void OnThreadRun() {}
            virtual void OnThreadWait(jlsvr::jlbase::_u64 iUsec) {}
            virtual void OnThreadActivate(){};

        protected:
            //std::function<void (void *)> mThreadEntry;
            THREAD_ENTRY mThreadEntry;
            bool mbJoin;
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLTHREAD__H_