#include"jltest.h"
#include"jlutil.h"
int main(int argc, char const *argv[])
{
    /* code */
    TestMutex();
    for(;;)
    {
        jlsvr::jlbase::SleepMs(100);
    }
    return 0;
}
