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
#define __FILENAME__ (__FILE__ + SOURCE_PATH_SIZE)
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
            LOG_LEVEL_MAX,
        };

        class CJlLogManager;
        class CJlLog
        {
        public:
            CJlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir);
            ~CJlLog();

        public:
            std::string &GetLogFileName() { return mStrFileName; }
            void AddLogItem(LogLevel iLevel, const char *format, ...);
            void FlushLog();

        private:
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
            jlsvr::jlbase::CJlMemBuffer *mpBuffs[LOG_LEVEL_MAX];
        };

#define JL_LOG_ERR(sLog, fmt, ...) sLog->AddLogItem(jlsvr::jlbase::LOG_LEVEL_ERR, "[ERROR](%s:%s:%d)[Thread:%u] " fmt, __FILENAME__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define JL_LOG_DBG(sLog, fmt, ...) sLog->AddLogItem(jlsvr::jlbase::LOG_LEVEL_DBG, "[DEBUG](%s:%s:%d)[Thread:%u] " fmt, __FILENAME__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define JL_LOG_INFO(sLog, fmt, ...) sLog->AddLogItem(jlsvr::jlbase::LOG_LEVEL_INFO, "[Thread:%u] " fmt, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)

        class CJlLogManager : public jlsvr::jlbase::CJlSingleton<CJlLogManager>, public jlsvr::jlplus11::CJl11Thread
        {
            SINGLE_CLASS_INITIAL(CJlLogManager);

            ~CJlLogManager() {}

        public:
            std::shared_ptr<CJlLog> &NewLogger(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir);
            std::shared_ptr<CJlLog> &DefaultLogger() { return mVecLoggers[0]; };

        protected:
            void OnThreadRun() override final;

        private:
            bool mbInit = false;
            std::vector<std::shared_ptr<CJlLog>> mVecLoggers;
            jlsvr::jlplus11::CPlus11Mutex mMux;
        };

#define LOG_ERR(fmt, ...) jlsvr::jlbase::CJlLogManager::Instance()->DefaultLogger()->AddLogItem(jlsvr::jlbase::LOG_LEVEL_ERR, "[ERROR](%s:%s:%d)[Thread:%u] " fmt, __FILENAME__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define LOG_DBG(fmt, ...) jlsvr::jlbase::CJlLogManager::Instance()->DefaultLogger()->AddLogItem(jlsvr::jlbase::LOG_LEVEL_DBG, "[DEBUG](%s:%s:%d)[Thread:%u] " fmt, __FILENAME__, __FUNCTION__, __LINE__, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) jlsvr::jlbase::CJlLogManager::Instance()->DefaultLogger()->AddLogItem(jlsvr::jlbase::LOG_LEVEL_INFO, "[Thread:%u] " fmt, (unsigned int)jlsvr::jlbase::GetThreadId(), ##__VA_ARGS__)

#define ASSERT(expr)                                \
    do                                              \
    {                                               \
        if (!(expr))                                \
        {                                           \
            LOG_ERR("assert \"%s\" failed", #expr); \
        }                                           \
    } while (0)
#define ASSERT_RET(expr)                            \
    do                                              \
    {                                               \
        if (!(expr))                                \
        {                                           \
            LOG_ERR("assert \"%s\" failed", #expr); \
            return;                                 \
        }                                           \
    } while (0)
#define ASSERT_RET_VALUE(expr, retval)              \
    do                                              \
    {                                               \
        if (!(expr))                                \
        {                                           \
            LOG_ERR("assert \"%s\" failed", #expr); \
            return retval;                          \
        }                                           \
    } while (0)
#define ASSERT_ERR_MSG(expr, fmt, ...)                                \
    do                                                                \
    {                                                                 \
        if (!(expr))                                                  \
        {                                                             \
            LOG_ERR("assert \"%s\", msg:" fmt, #expr, ##__VA_ARGS__); \
        }                                                             \
    } while (0);

#define ASSERT_ERR_MSG_RET(expr, retval, fmt, ...)                    \
    do                                                                \
    {                                                                 \
        if (!(expr))                                                  \
        {                                                             \
            LOG_ERR("assert \"%s\", msg:" fmt, #expr, ##__VA_ARGS__); \
            return retval;                                            \
        }                                                             \
    } while (0);
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_LOG__H_