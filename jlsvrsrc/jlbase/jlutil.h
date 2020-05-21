/*
@author:lightjia
@date:2020/5/20
@desc:工具模块
*/
#ifndef __JLUTIL__H_
#define __JLUTIL__H_
#include "jlcommoninc.h"

#define JLARRAYLEN(X) (int)(sizeof(X) / sizeof(X[0]))

namespace jlsvr
{
    namespace jlbase
    {

        void SleepMs(jlsvr::jlbase::_ul dwMillions);

        jlsvr::jlbase::_u64 GetCurMs();
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLUTIL__H_