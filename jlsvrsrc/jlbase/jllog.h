/*
@author:lightjia
@date:2020/5/21
@desc:日志模块
*/
#ifndef __JL_LOG__H_
#define __JL_LOG__H_
#include "jlcommoninc.h"
namespace jlsvr
{
    namespace jlbase
    {
        enum LogType
        {
            LOG_TYPE_SCREEN = 0,
            LOG_TYPE_FILE,
            LOG_TYPE_TEE,
        };

        enum LogLevel
        {
            LOG_LEVEL_ERR = 0,
            LOG_LEVEL_DBG,
            LOG_LEVEL_INFO,
        };

        class JlLog
        {
            JlLog(LogType iType, LogLevel iLevel, const std::string& strCat);
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_LOG__H_