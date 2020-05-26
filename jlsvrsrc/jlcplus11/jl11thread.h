/*
@author:lightjia
@date:2020/5/23
@desc:C++11线程模块
*/
#ifndef __CJL11THREAD__H_
#define __CJL11THREAD__H_
#include "jlthread.h"
#include <thread>
#include <mutex>
#include <condition_variable>
namespace jlsvr
{
    namespace jlplus11
    {
        class CJl11Thread : public jlsvr::jlbase::CJlThread
        {
        protected:
            void OnThreadBegin() override final;
            void OnThreadJoin() override final;
            void OnThreadWait(jlsvr::jlbase::_u64 iUsec) override final;
            void OnThreadActivate() override final;

        private:
            std::unique_ptr<std::thread, std::function<void(std::thread *)>> mThread;
            std::mutex mMtx;
            std::condition_variable mCv;
        };
    } // namespace jlplus11
} // namespace jlsvr
#endif //__CJL11THREAD__H_