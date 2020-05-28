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

        void CJlLogManager::OnThreadRun()
        {
            ThreadWait(1);
            for(auto& log:mVecLoggers)
            {
                log->FlushLog();
            }
        }
    } // namespace jlbase
} // namespace jlsvr