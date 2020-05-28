#include "jllog.h"
namespace jlsvr
{
    namespace jlbase
    {
        CJlLogManager::CJlLogManager()
        {
            if (!mbInit)
            {
                ThreadBegin();
                std::shared_ptr<CJlLog> defaultLog(new CJlLog(LOG_TYPE_TEE, LOG_LEVEL_MAX, "", ""));
                mVecLoggers.push_back(defaultLog);
                mbInit = true;
            }
        }

        std::shared_ptr<CJlLog> &CJlLogManager::NewLogger(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir)
        {
            jlsvr::jlbase::CJlAutoMutex mux(&mMux);
            std::shared_ptr<CJlLog> defaultLog(new CJlLog(iType, iLevel, strCat, strLogDir));
            mVecLoggers.push_back(defaultLog);
            return mVecLoggers.back();
        }

        void CJlLogManager::OnThreadRun()
        {
            ThreadWait(100);
            jlsvr::jlbase::CJlAutoMutex mux(&mMux);
            int i = 0;
            for (auto log = mVecLoggers.begin(); log != mVecLoggers.end(); i++)
            {
                log->get()->FlushLog();
                if (i != 0 && log->use_count() == 1)
                {
                    printf("==========del loger==============\n");
                    log = mVecLoggers.erase(log);
                    continue;
                }

                log++;
            }
        }
    } // namespace jlbase
} // namespace jlsvr