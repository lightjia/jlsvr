/*
@author:lightjia
@date:2020/5/21
@desc:C++11锁模块
*/
#ifndef __JL11MUTEX_H__
#define __JL11MUTEX_H__
#include "jlmutex.h"
#include <mutex>
namespace jlsvr
{
    namespace jlplus11
    {
        class CPlus11Mutex : public jlsvr::jlbase::CJlMutex
        {
        public:
            void Lock() override
            {
                mMutex.lock();
            }

            void UnLock() override
            {
                mMutex.unlock();
            }

            int TryLock() override
            {
                return mMutex.try_lock();
            }

        private:
            std::mutex mMutex;
        };
    } // namespace jlplus11
} // namespace jlsvr
#endif //__JL11MUTEX_H__