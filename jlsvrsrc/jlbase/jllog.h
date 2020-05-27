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
#include "jl11thread.h"
#include "jlsingleton.h"
#include <memory>
#include <cstdio>
#include <cstdarg>
#include <vector>
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

        class CJlLog
        {
        public:
            CJlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir);
            ~CJlLog();

        public:
            std::string &GetLogFileName() { return mStrFileName; }
            void FlushLog();

        private:
            void AddLogItem(LogLevel iLevel, const char *format, ...);
            void CheckLogFile();
            void CloseFile();

        private:
            LogType miType;
            LogLevel miLevel;
            std::string mStrCat;
            std::string mStrFileName;
            std::string mstrLogDir;
            jlsvr::jlplus11::CPlus11Mutex mMux;
            jlsvr::jlbase::_u64 miCount = 0;
            FILE *mpLog = nullptr;
        };

#define JL_LOG_ERR(sLog, fmt, ...) sLog->AddLogItem(LOG_LEVEL_ERR, "[ERROR](%s:%s:%d)[Thread:%u] " fmt, __FILE__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define JL_LOG_DBG(sLog, fmt, ...) sLog->AddLogItem(LOG_LEVEL_DBG, "[DEBUG](%s:%s:%d)[Thread:%u] " fmt, __FILE__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define JL_LOG_INFO(sLog, fmt, ...) sLog->AddLogItem(LOG_LEVEL_INFO, "[Thread:%u] " fmt, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)

        class CJlLogManager : public jlsvr::jlbase::CJlSingleton<CJlLogManager>, public jlsvr::jlplus11::CJl11Thread
        {
            SINGLE_CLASS_INITIAL(CJlLogManager);

        public:
            std::shared_ptr<CJlLog> &NewLogger(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir);
            std::shared_ptr<CJlLog> &DefaultLogger(){return mVecLoggers[0];};

        protected:
            void OnThreadRun() override final;

        private:
            bool mbInit = false;
            std::vector<std::shared_ptr<CJlLog>> mVecLoggers;
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_LOG__H_