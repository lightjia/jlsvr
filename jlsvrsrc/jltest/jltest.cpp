#include "jltest.h"
#include "jlcolordef.h"

void Test()
{
    jlsvr::jlbase::_i8 t = 99;
    std::cout << t << std::endl;
    const char *pData = "Hello lightjia\n";
    for (int i = 0; i < 0xFFF; ++i)
    {
        jlsvr::jlbase::SleepMs(1000);
        //jlsvr::jlbase::BlackPrint(pData);
        jlsvr::jlbase::WhitePrint(pData);
    }
}