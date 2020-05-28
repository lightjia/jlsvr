#include "jllog.h"
#include "jlutil.h"
namespace jlsvr
{
    namespace jlbase
    {
#define MAX_LOG_FILE_SIZE 1024 * 1024 * 500 //500M
#define MAX_PER_LINE_LOG 2048

        struct tagLogHandler
        {
            LogLevel iLevel;
            std::function<void(const char *)> funcPrint;
        } logPrintHandles[] = {
            {LOG_LEVEL_ERR, jlsvr::jlbase::RedPrint},
            {LOG_LEVEL_DBG, jlsvr::jlbase::WhitePrint},
            {LOG_LEVEL_INFO, jlsvr::jlbase::GreenPrint}};

        CJlLog::CJlLog(LogType iType, LogLevel iLevel, const std::string &strCat, const std::string &strLogDir)
        {
            miType = iType;
            miLevel = iLevel;
            mStrCat = strCat;
            for (int i = 0; i < LOG_LEVEL_MAX; i++)
            {
                mpBuffs[i] = nullptr;
            }

            if (jlsvr::jlbase::MkDirs(strLogDir.c_str()))
            {
                mstrLogDir = jlsvr::jlbase::GetProgramRunPath() + "/log";
                jlsvr::jlbase::MkDirs(mstrLogDir.c_str());
                char szBuf[500];
                snprintf(szBuf, 500, "Dir:%s NotExist And CreateLogDir:%s\n", strLogDir.c_str(), mstrLogDir.c_str());
                jlsvr::jlbase::RedPrint(szBuf);
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
            jlsvr::jlbase::CJlMemBuffer *pBuff = new jlsvr::jlbase::CJlMemBuffer();
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
            pBuff->SetBuffLen(nPos);
            mMux.Lock();
            if (mpBuffs[iLevel] == nullptr)
            {
                mpBuffs[iLevel] = pBuff;
            }
            else
            {
                mpBuffs[iLevel]->Append(pLog, nPos);
                delete pBuff;
            }
            mMux.UnLock();
            CJlLogManager::Instance()->ThreadActivate();
        }

        void CJlLog::CloseFile()
        {
            mStrFileName = "";
            if (mpLog)
            {
                fclose(mpLog);
                mpLog = nullptr;
            }
        }

        void CJlLog::CheckLogFile()
        {
            if ((miType == LOG_TYPE_FILE || miType == LOG_TYPE_TEE) && (!mpLog || miCount > MAX_LOG_FILE_SIZE) && mpLog != stdout)
            {
                CloseFile();
                jlsvr::jlbase::tagJlTime stNow = jlsvr::jlbase::GetNowTime();
                char szFileName[500];
                snprintf(szFileName, 500, "%02d-%02d-%02d-%02d-%02d-%02d.log", stNow.tmyear, stNow.tmmon, stNow.tmmday, stNow.tmhour, stNow.tmmin, stNow.tmsec);
                std::string strFile = mstrLogDir + "/" + szFileName;
                mpLog = fopen(strFile.c_str(), "a+");
                if (mpLog)
                {
                    mStrFileName = strFile;
                }
                else
                {
                    jlsvr::jlbase::RedPrint("Can not create log file:");
                    jlsvr::jlbase::RedPrint(strFile.c_str());
                }
            }
        }

        void CJlLog::FlushLog()
        {
            jlsvr::jlbase::CJlMemBuffer *tmpLogs[LOG_LEVEL_MAX];
            for (int i = 0; i < LOG_LEVEL_MAX; i++)
            {
                tmpLogs[i] = nullptr;
            }

            if (!mMux.TryLock())
            {
                for (int i = 0; i < LOG_LEVEL_MAX; i++)
                {
                    tmpLogs[i] = mpBuffs[i];
                    mpBuffs[i] = nullptr;
                }

                mMux.UnLock();
            }

            for (int i = 0; i < LOG_LEVEL_MAX; i++)
            {
                if (tmpLogs[i] != nullptr)
                {
                    CheckLogFile();
                    if (miType == LOG_TYPE_SCREEN || miType == LOG_TYPE_TEE)
                    {
                        tmpLogs[i]->AppendNul();
                        logPrintHandles[i].funcPrint((char *)tmpLogs[i]->GetBuffer());
                    }

                    if (mpLog)
                    {
                        size_t iOff = 0;
                        while (iOff < tmpLogs[i]->GetBuffLen())
                        {
                            int iWrite = (int)fwrite((char *)tmpLogs[i]->GetBuffer() + iOff, 1, tmpLogs[i]->GetBuffLen() - iOff, mpLog);
                            if (iWrite < 0)
                            {
                                jlsvr::jlbase::RedPrint("Write Log Error\n");
                                break;
                            }

                            iOff += iWrite;
                        }
                    }

                    delete tmpLogs[i];
                }
            }
        }
    } // namespace jlbase
} // namespace jlsvr