#include"jlutil.h"

namespace jlsvr
{
    namespace jlbase
    {
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
    } // namespace jlbase
} // namespace jlsvr