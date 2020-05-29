/*
@author:lightjia
@date:2020/5/29
@desc:libuv 动态库操作
*/
#ifndef __JLUVDL__H_
#define __JLUVDL__H_
#include "jluvdef.h"
#include "jlutil.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvDl
        {
        public:
            CJlUvDl() { BZERO(&mstLib); }
            ~CJlUvDl() { Close(); }

        public:
            int Open(const char *pFileName)
            {
                return uv_dlopen(pFileName, &mstLib);
            }

            void Close()
            {
                uv_dlclose(&mstLib);
            }

            int GetFunc(const char *pFuncName, void **pFuncPtr)
            {
                return uv_dlsym(&mstLib, pFuncName, pFuncPtr);
            }
            
            const char *GetErr()
            {
                return uv_dlerror(&mstLib);
            }

        private:
            uv_lib_t mstLib;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVDL__H_