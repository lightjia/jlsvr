/*
@author:lightjia
@date:2020/5/20
@desc:工具模块
*/
#ifndef __JLUTIL__H_
#define __JLUTIL__H_
#include "jlcommoninc.h"

namespace jlsvr
{
    namespace jlbase
    {
        void SleepMs(jlsvr::jlbase::_u32 dwMillions)
        {
#if (defined PLATFORM_WINDOWS)
            Sleep(dwMillions);
#elif (defined PLATFORM_LINUX)
            usleep(dwMillions * 1000);
#endif
        }
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLUTIL__H_