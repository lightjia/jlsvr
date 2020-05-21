#include "jltest.h"
#include "jlutil.h"
#include "jlmutex.h"
int main(int argc, char const *argv[])
{
    /* code */
    Test();

    for (;;)
    {
        jlsvr::jlbase::SleepMs(100);
    }
    return 0;
}
