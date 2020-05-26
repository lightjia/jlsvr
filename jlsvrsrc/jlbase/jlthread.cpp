#include "jlthread.h"

namespace jlsvr
{
    namespace jlbase
    {
        static void ThreadEntry(void *pEntry)
        {
            jlsvr::jlbase::CJlThread *pThread = dynamic_cast<jlsvr::jlbase::CJlThread *>((jlsvr::jlbase::CJlThread *)pEntry);
            if (pThread)
            {
                pThread->ThreadRun();
            }
        }

        CJlThread::CJlThread() : mbJoin(false), mThreadEntry(ThreadEntry)
        {
        }

        CJlThread::~CJlThread()
        {

        }
    } // namespace jlbase
} // namespace jlsvr