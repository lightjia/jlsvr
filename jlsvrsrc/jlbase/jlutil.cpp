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

        jlsvr::jlbase::tagJlTime GetNowTime()
        {
            jlsvr::jlbase::tagJlTime stRet;
            time_t rawtime;
            struct tm tmnow;
#if (defined PLATFORM_WINDOWS)
            time(&rawtime);
            localtime_s(&tmnow, &rawtime);
            struct timeb tp;
            ftime(&tp);
            stRet.tmmilliseconds = tp.millitm;
#elif (defined PLATFORM_LINUX)
            struct timeval tv_now;
            gettimeofday(&tv_now, NULL);
            rawtime = (time_t)tv_now.tv_sec;

            localtime_r(&rawtime, &tmnow);
            stRet.tmmilliseconds = static_cast<int>(tv_now.tv_usec);
#endif

            stRet.tmyear = tmnow.tm_year + 1900;
            stRet.tmmon = tmnow.tm_mon + 1;
            stRet.tmmday = tmnow.tm_mday;
            stRet.tmhour = tmnow.tm_hour;
            stRet.tmmin = tmnow.tm_min;
            stRet.tmsec = tmnow.tm_sec;
            stRet.tmwday = tmnow.tm_wday;
            stRet.tmyday = tmnow.tm_yday;
            stRet.tmisdst = tmnow.tm_isdst;

            return stRet;
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

        std::string GetProgramRunPath()
        {
            std::string strRet = "";
#if (defined PLATFORM_WINDOWS)
            char szExePath[MAX_PATH] = {0};
            GetModuleFileNameA(NULL, szExePath, MAX_PATH);
            char *pstr = strrchr(szExePath, '\\');
            memset(pstr + 1, '\0', 1);
            strRet = szExePath;
#elif (defined PLATFORM_LINUX)
#define MAX_PATH 0x100
            char szFilePath[MAX_PATH] = {0};
            int cnt = readlink("/proc/self/exe", szFilePath, MAX_PATH);
            if (cnt < 0 || cnt >= MAX_PATH)
            {
                return strRet;
            }

            for (int i = cnt; i >= 0; --i)
            {
                if (szFilePath[i] == '/')
                {
                    szFilePath[i + 1] = '\0';
                    break;
                }
            }
            strRet = szFilePath;
#endif

            return strRet;
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

        int MkDir(const char *path)
        {
            if (NULL == path || strlen(path) <= 0)
            {
                return 1;
            }

            if (IsDirExist(path))
            {
                return 0;
            }

#if (defined PLATFORM_WINDOWS)
            if (!::CreateDirectoryA(path, NULL))
            {
                return 1;
            }
#elif (defined PLATFORM_LINUX)
            if (mkdir(path, 0755) != 0)
            {
                fprintf(stderr, "mkdir %s failed(%s)\n", path, strerror(errno));
                return 1;
            }
#endif

            return 0;
        }

        int MkDirs(const char *path)
        {
            size_t len = 0;
            if (NULL == path || (len = strlen(path)) <= 0)
            {
                return 1;
            }

            char *pTmp = (char *)malloc((len + 3) * sizeof(char));
            if (NULL == pTmp)
            {
                return 1;
            }

            for (int i = 0; i < (int)len; i++)
            {
                if (pTmp[i] != '\\' && pTmp[i] != '/')
                {
                    continue;
                }

                if (0 == i)
                {
                    continue;
                }

                pTmp[i] = '\0';
                if (MkDir(pTmp) != 0)
                {
                    break;
                }

                pTmp[i] = '/';
            }

            free(pTmp);

            return MkDir(path);
        }

        int RmFile(const char *pFileName)
        {
            if (strlen(pFileName) <= 0)
            {
                return 1;
            }

            int iRet;
#if (defined PLATFORM_WINDOWS)
            iRet = DeleteFileA(pFileName);
#elif (defined PLATFORM_LINUX)
            iRet = unlink(pFileName);
#endif
            return iRet;
        }

        int RmDir(const char *pDirName)
        {
            if (!IsDirExist(pDirName))
            {
                fprintf(stderr, "Dir:%s Not Exist!\n", pDirName);
                return 1;
            }

#if (defined PLATFORM_WINDOWS)
            std::string strPath = pDirName;
            if (str_end_with(strPath, "\\") || str_end_with(strPath, "/"))
            {
                strPath += "*";
            }
            else
            {
                strPath += "/*";
            }

            WIN32_FIND_DATAA FindFileData;
            ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
            HANDLE hFile = FindFirstFileA(strPath.c_str(), &FindFileData);
            if (hFile && INVALID_HANDLE_VALUE != hFile)
            {
                BOOL IsFinded = FALSE;
                do
                {
                    if (IsFinded)
                    {
                        if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
                        { //如果不是"." ".."目录
                            std::string strFileName = "";
                            strFileName = strFileName + pDirName + "/" + FindFileData.cFileName;
                            if (IsDirExist(strFileName.c_str()))
                            { //如果是目录，则递归地调用
                                RmDir(strFileName.c_str());
                            }
                            else
                            {
                                RmFile(strFileName.c_str());
                            }
                        }
                    }

                    IsFinded = FindNextFileA(hFile, &FindFileData); //递归搜索其他的文件
                } while (IsFinded);

                FindClose(hFile);
            }
            RemoveDirectoryA(pDirName);
#elif (defined PLATFORM_LINUX)
            DIR *pDir = opendir(pDirName);
            if (pDir)
            {
                struct dirent *dmsg = NULL;
                while ((dmsg = readdir(pDir)) != NULL)
                {
                    if (strcmp(dmsg->d_name, ".") && strcmp(dmsg->d_name, ".."))
                    {
                        std::string strFileName = "";
                        strFileName = strFileName + pDirName + "/" + dmsg->d_name;
                        if (IsDirExist(strFileName.c_str()))
                        { //如果是目录，则递归地调用
                            RmDir(strFileName.c_str());
                        }
                        else
                        {
                            RmFile(strFileName.c_str());
                        }
                    }
                }

                closedir(pDir);
            }

            rmdir(pDirName);
#endif

            return 0;
        }
    } // namespace jlbase
} // namespace jlsvr