#include "jl11thread.h"
#include "jlmembuffer.h"
#include "jlcolordef.h"
#include "jltest.h"
class CTestThread : public jlsvr::jlplus11::CJl11Thread
{
protected:
    void OnThreadRun() override final
    {
        if (iIndex < 0xFFFF)
        {
            char szBuf[100];
            int iOff = snprintf(szBuf, 100, "cur loop:%d\n", iIndex++);
            mBuff.Append(szBuf, iOff);
            jlsvr::jlbase::WhitePrint(szBuf);
            ThreadWait(5);
        }
        else
        {
            mBuff.AppendNul();
            char *pData = (char *)mBuff.GetBuffer();
            jlsvr::jlbase::GreenPrint(pData);
            mbJoin = true;
        }
    }

private:
    jlsvr::jlbase::CJlMemBuffer mBuff;
    int iIndex = 0;
};

void TestThread()
{
    CTestThread *pThrd = new CTestThread();
    pThrd->ThreadBegin();
}