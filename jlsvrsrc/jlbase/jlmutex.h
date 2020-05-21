/*
@author:lightjia
@date:2020/5/20
@desc:锁公共基类定义
*/

#ifndef __JL_MUTEX__H_
#define __JL_MUTEX__H_
#include "jlcommoninc.h"
namespace jlsvr
{
    namespace jlbase
    {
        class CJlMutex
        {
        public:
            virtual void Lock() = 0;
            virtual int TryLock() = 0;
            virtual void UnLock() = 0;
        };

        class CNoneMutex : CJlMutex
        {
        public:
            void Lock() override
            {
            }

            void UnLock() override
            {
            }

            int TryLock() override
            {
                return 0;
            }
        };

        class CJlAutoMutex
        {
        public:
            CJlAutoMutex(CJlMutex *pMutex) : mpJlMutex(pMutex)
            {
                if (mpJlMutex)
                {
                    mpJlMutex->Lock();
                }
            }

            ~CJlAutoMutex()
            {
                if (mpJlMutex)
                {
                    mpJlMutex->UnLock();
                }
            }

        private:
            CJlMutex *mpJlMutex;
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_MUTEX__H_