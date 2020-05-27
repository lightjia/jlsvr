#include "jllog.h"
#include "jlutil.h"
namespace jlsvr
{
    namespace jlbase
    {
#define MAX_LOG_FILE_SIZE 1024 * 1024 * 500 //500M
#define MAX_PER_LINE_LOG 2048
        CJlLog::CJlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir)
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

        CJlLog::~CJlLog()
        {
            CloseFile();
        }

        void CJlLog::AddLogItem(LogLevel iLevel, const char *format, ...)
        {
            if (iLevel > miLevel)
            {
                return;
            }

            unsigned long lBufferSize = MAX_PER_LINE_LOG;
            std::unique_ptr<CJlMemBuffer> buff(new CJlMemBuffer());
            CJlMemBuffer *pBuff = buff.get();
            char *pLog = (char *)pBuff->AllocBuffer(lBufferSize);
            unsigned long nPos = 0;
            if (iLevel <= LOG_LEVEL_INFO)
            {
                jlsvr::jlbase::tagJlTime stNow = jlsvr::jlbase::GetNowTime();
                nPos = snprintf(pLog, MAX_PER_LINE_LOG, " %s [%02d/%02d/%02d %02d:%02d:%02d.%03d] ", mStrCat.c_str(), stNow.tmyear, stNow.tmmon, stNow.tmmday, stNow.tmhour, stNow.tmmin, stNow.tmsec, stNow.tmmilliseconds);
            }

            for (;;)
            {
                va_list ap;
                va_start(ap, format);
                int iTmpPos = 0;
#if (defined PLATFORM_WINDOWS)
                iTmpPos = vsnprintf_s(pLog + nPos, lBufferSize - nPos, _TRUNCATE, format, ap);
#elif (defined PLATFORM_LINUX)
                iTmpPos = vsnprintf(pLog + nPos, lBufferSize - nPos, format, ap);
#endif
                va_end(ap);

                if (iTmpPos >= 0 && (unsigned long)iTmpPos <= lBufferSize)
                {
                    nPos += iTmpPos;
                    break;
                }
                else
                {
                    lBufferSize *= 2;
                    pLog = (char *)pBuff->AllocBuffer(lBufferSize);
                }
            }

            pLog[nPos] = '\n';
            nPos++;
            mMux.Lock();
            mMux.UnLock();
        }

        void CJlLog::CloseFile()
        {
            if (mpLog)
            {
                fclose(mpLog);
                mpLog = nullptr;
            }
        }

        void CJlLog::CheckLogFile()
        {
            if ((miType == LOG_TYPE_FILE || miType == LOG_TYPE_TEE) && miCount > MAX_LOG_FILE_SIZE && mpLog != stdout)
            {
                CloseFile();
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

        void CJlLog::FlushLog()
        {

        }
    } // namespace jlbase
} // namespace jlsvr