#include "jl11thread.h"
#include "jlcolordef.h"
namespace jlsvr
{
    namespace jlplus11
    {
        static void ThreadDeleter(std::thread *x)
        {
            char szBuf[50];
            snprintf(szBuf, 50, "delete std::thread:%p", x);
            jlsvr::jlbase::YellowPrint(szBuf);
            delete x;
        }

        void CJl11Thread::OnThreadBegin()
        {
            std::unique_ptr<std::thread, std::function<void(std::thread *)>> thr(new std::thread(mThreadEntry, (void *)this), ThreadDeleter);
            mThread = std::move(thr);
        }

        void CJl11Thread::OnThreadJoin()
        {
            mThread->join();
        }

        void CJl11Thread::OnThreadWait(jlsvr::jlbase::_u64 iUsec)
        {
            std::unique_lock<std::mutex> lck(mMtx);
            mCv.wait_for(lck, std::chrono::seconds(iUsec));
        }

        void CJl11Thread::OnThreadActivate()
        {
            mCv.notify_all();
        }
    } // namespace jlplus11
} // namespace jlsvr