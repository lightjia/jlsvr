/*
@author:lightjia
@date:2020/5/20
@desc:工具模块
*/
#ifndef __JLUTIL__H_
#define __JLUTIL__H_
#include "jlcommoninc.h"

#define JLARRAYLEN(X) (int)(sizeof(X) / sizeof(X[0]))

namespace jlsvr
{
    namespace jlbase
    {
#define BZERO(P) memset(P, 0, sizeof(*P))
#define DODELETE(P)   \
    do                \
    {                 \
        if (P)        \
            delete P; \
        P = nullptr;  \
    } while (0)
#define DOFREE(P)    \
    do               \
    {                \
        if (P)       \
            free(P); \
        P = nullptr; \
    } while (0)

        //time oper
        void SleepMs(jlsvr::jlbase::_ul dwMillions);
        jlsvr::jlbase::_u64 GetCurMs();
        jlsvr::jlbase::tagJlTime GetNowTime();

        //os oper
        jlsvr::jlbase::_ul GetThreadId();
        std::string GetProgramRunPath();

        //file oper
        bool IsFileExist(const char *strFileName);
        bool IsDirExist(const char *strDirectory);
        int MkDir(const char *path);
        int MkDirs(const char *path);
        int RmFile(const char *pFileName);
        int RmFile(const char *pFileName);
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLUTIL__H_