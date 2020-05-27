#include "jltest.h"
#include "jlutil.h"
#include "jlmutex.h"
#include"jlmemmgr.h"
int main(int argc, char const *argv[])
{
    /* code */
    TestThread();
    for (;;)
    {
        jlsvr::jlbase::SleepMs(5000);
        jlsvr::jlbase::CJlMemMgr::Instance()->Print();
    }
    return 0;
}
