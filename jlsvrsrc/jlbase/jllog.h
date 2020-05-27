/*
@author:lightjia
@date:2020/5/21
@desc:日志模块
*/
#ifndef __JL_LOG__H_
#define __JL_LOG__H_
#include "jlcommoninc.h"
#include "jlmembuffer.h"
#include "jlcolordef.h"
#include "jl11mutex.h"
#include <memory>
#include <cstdio>
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

        struct tagLogHandler
        {
            LogLevel iLevel;
            std::function<void(const char *)> funcPrint;
            std::unique_ptr<CJlMemBuffer> buffer;
        };

        struct tagLogHandler logHandles[] = {
            {LOG_LEVEL_ERR, jlsvr::jlbase::RedPrint, std::unique_ptr<CJlMemBuffer>(new CJlMemBuffer())},
            {LOG_LEVEL_DBG, jlsvr::jlbase::WhitePrint, std::unique_ptr<CJlMemBuffer>(new CJlMemBuffer())},
            {LOG_LEVEL_INFO, jlsvr::jlbase::GreenPrint, std::unique_ptr<CJlMemBuffer>(new CJlMemBuffer())}};

        class JlLog
        {
        public:
            JlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir);
            ~JlLog();

        public:
            void LogErr();
            void LogDbg();
            void LogInfo();
            std::string &GetLogFileName() { return mStrFileName; }

        private:
            void AddLogItem(LogLevel iLevel, const char *format, ...);
            void CheckLogFile();

        private:
            LogType miType;
            LogLevel miLevel;
            std::string mStrCat;
            std::string mStrFileName;
            std::string mstrLogDir;
            jlsvr::jlbase::_u64 miCount = 0;
            FILE *mpLog = nullptr;
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_LOG__H_