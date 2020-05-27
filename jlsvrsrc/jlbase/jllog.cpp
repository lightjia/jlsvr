#include "jllog.h"
#include "jlutil.h"
namespace jlsvr
{
    namespace jlbase
    {
#define MAX_LOG_FILE_SIZE 1024 * 1024 * 500 //500M
        JlLog::JlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir)
        {
            miType = iType;
            miLevel = iLevel;
            mStrCat = strCat;
            if (jlsvr::jlbase::MkDirs(strLogDir.c_str()))
            {
                mstrLogDir = jlsvr::jlbase::GetProgramRunPath() + "/log";
                jlsvr::jlbase::MkDirs(mstrLogDir.c_str());
                char szBuf[500];
                snprintf(szBuf, 500, "Dir:%s NotExist And CreateLogDir:%s", strLogDir.c_str(), mstrLogDir.c_str());
                jlsvr::jlbase::RedPrint("szBuf");
            }
            else
            {
                mstrLogDir = strLogDir;
            }
        }

        void JlLog::LogErr()
        {
        }

        void JlLog::LogDbg()
        {
        }

        void JlLog::LogInfo()
        {
        }

        void JlLog::AddLogItem(LogLevel iLevel, const char *format, ...)
        {
        }

        void JlLog::CheckLogFile()
        {
            if ((miType == LOG_TYPE_FILE || miType == LOG_TYPE_TEE) && miCount > MAX_LOG_FILE_SIZE && mpLog != stdout)
            {
                if (mpLog)
                {
                    fclose(mpLog);
                    mpLog = nullptr;
                }

                jlsvr::jlbase::tagJlTime stNow = jlsvr::jlbase::GetNowTime();
                char szFileName[500];
                snprintf(szFileName, 500, "%02d-%02d-%02d-%02d-%02d-%02d.log", stNow.tmyear, stNow.tmmon, stNow.tmmday, stNow.tmhour, stNow.tmmin, stNow.tmsec);
                std::string strFile = mstrLogDir + "/" + szFileName;
                mpLog = fopen(strFile.c_str(), "a+");
                if (mpLog)
                {
                    mstrLogDir = strFile;
                }
            }
        }
    } // namespace jlbase
} // namespace jlsvr