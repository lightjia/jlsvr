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
    }

private:
    int iIndex = 0;
};

void TestThread()
{
    CTestThread *pThrd = new CTestThread();
    pThrd->ThreadBegin();
    pThrd->ThreadWait(90000000);
    printf("AHAHHAHAHAHAHHAHHA\n");
}