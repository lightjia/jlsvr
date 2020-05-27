/*
@author:lightjia
@date:2020/5/27
@desc:内存缓存模块
*/
#ifndef __JLMEMBUFFER__H_
#define __JLMEMBUFFER__H_
#include "jlmemmgr.h"
#include <memory>
namespace jlsvr
{
    namespace jlbase
    {
        class CJlMemBuffer : public CJlClassMemMgr
        {
        public:
            CJlMemBuffer();
            ~CJlMemBuffer();
            CJlMemBuffer(const CJlMemBuffer &cMemBuffer);
            CJlMemBuffer &operator=(const CJlMemBuffer &cMemBuffer);

        public:
            void Append(const void *pData, const size_t iLen);
            void AppendNul();
            void *GetBuffer(size_t iIndex = 0);
            const size_t GetBuffLen() { return miBufferUseLen; }
            void SetBuffLen(const size_t iLen);
            void *AllocBuffer(size_t iLen);

        private:
            int SetBuffer(const std::unique_ptr<jlsvr::jlbase::_u8, std::function<void (void *)>>& upBuff, const size_t iDataLen);

        private:
            std::unique_ptr<jlsvr::jlbase::_u8, std::function<void (void *)>> mBuff;
            int miAppendNum;
            size_t miBufferLen;
            size_t miBufferUseLen;
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLMEMBUFFER__H_