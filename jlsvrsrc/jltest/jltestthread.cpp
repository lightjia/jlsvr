#include "jl11thread.h"
#include "jlmembuffer.h"
#include "jlcolordef.h"
#include "jllog.h"
#include "jltest.h"
class CTestThread : public jlsvr::jlplus11::CJl11Thread
{
protected:
    void OnThreadRun() override final
    {
        if (!iIndex)
        {
            ThreadActivate();
        }

        char szBuf[100];
        int iOff = snprintf(szBuf, 100, "cur loop:%d\n", iIndex++);
        LOG_DBG("%s", szBuf);
        ThreadWait(2000);
    }

private:
    int iIndex = 0;
};

void TestThread()
{
    CTestThread *pThrd = new CTestThread();
    pThrd->ThreadBegin();
    pThrd->ThreadWait(90000000);
    // std::shared_ptr<jlsvr::jlbase::CJlLog> logger = jlsvr::jlbase::CJlLogManager::Instance()->NewLogger(jlsvr::jlbase::LOG_TYPE_SCREEN, jlsvr::jlbase::LOG_LEVEL_MAX, "test", "");
    // for(int i = 1; i < 10; i++)
    // {
    //     jlsvr::jlbase::SleepMs(1000);
    //     JL_LOG_INFO(logger.get(), "Hello:%d", i);
    // }
}