#include "jlutil.h"

namespace jlsvr
{
    namespace jlbase
    {
        //time oper
        void SleepMs(jlsvr::jlbase::_ul dwMillions)
        {
#if (defined PLATFORM_WINDOWS)
            Sleep(dwMillions);
#elif (defined PLATFORM_LINUX)
            usleep(dwMillions * 1000);
#endif
        }

        jlsvr::jlbase::_u64 GetCurMs()
        {
            jlsvr::jlbase::_u64 iRet = 0;
#if (defined PLATFORM_WINDOWS)
            iRet = GetTickCount();
#elif (defined PLATFORM_LINUX)
            struct timeval current;
            gettimeofday(&current, NULL);
            iRet = (jlsvr::jlbase::_u64)(current.tv_sec * 1000) + current.tv_usec / 1000;
#endif

            return iRet;
        }

        //os oper
        jlsvr::jlbase::_ul GetThreadId()
        {

            jlsvr::jlbase::_ul iRet = 0;
#if (defined PLATFORM_WINDOWS)
            iRet = ::GetCurrentThreadId();
#elif (defined PLATFORM_LINUX)
            iRet = pthread_self();
#endif

            return iRet;
        }

        //file oper
        bool IsFileExist(const char *strFileName)
        {
            bool bRet = false;
#if (defined PLATFORM_WINDOWS)
            DWORD dwRet = GetFileAttributesA(strFileName);
            bRet = (dwRet != INVALID_FILE_ATTRIBUTES) && (!(dwRet & FILE_ATTRIBUTE_DIRECTORY));
#elif (defined PLATFORM_LINUX)
            bRet = (access(strFileName, F_OK) == 0);
#endif
            return bRet;
        }

        bool IsDirExist(const char *strDirectory)
        {
            bool bRet = false;
#if (defined PLATFORM_WINDOWS)
            DWORD dwRet = GetFileAttributesA(strDirectory);
            bRet = (dwRet != INVALID_FILE_ATTRIBUTES) && (dwRet & FILE_ATTRIBUTE_DIRECTORY);
#elif (defined PLATFORM_LINUX)
            struct stat sb;
            bRet = (lstat(strDirectory, &sb) == 0 && S_ISDIR(sb.st_mode));
#endif
            return bRet;
        }
    } // namespace jlbase
} // namespace jlsvr