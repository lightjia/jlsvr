#include "jlmembuffer.h"
#include "jlcolordef.h"
namespace jlsvr
{
    namespace jlbase
    {
#define MEM_BUFFER_DEFAULT_LEN 1024

        CJlMemBuffer::CJlMemBuffer()
        {
            miBufferLen = 0;
            miBufferUseLen = 0;
            miAppendNum = 0;
            mBuff = nullptr;
        }

        CJlMemBuffer::~CJlMemBuffer()
        {
            miBufferLen = 0;
            miBufferUseLen = 0;
            if (mBuff)
            {
                JL_MEMMGR_MEM_FUNC.pFreeFunc(mBuff);
                mBuff = nullptr;
            }
        }

        CJlMemBuffer::CJlMemBuffer(const CJlMemBuffer &cMemBuffer)
        {
            if (this != &cMemBuffer)
            {
                SetBuffer(cMemBuffer);
            }
        }

        CJlMemBuffer &CJlMemBuffer::operator=(const CJlMemBuffer &cMemBuffer)
        {
            if (this != &cMemBuffer)
            {
                SetBuffer(cMemBuffer);
            }

            return *this;
        }

        int CJlMemBuffer::SetBuffer(const CJlMemBuffer &cMemBuffer)
        {
            miBufferUseLen = cMemBuffer.miBufferUseLen;
            miBufferLen = cMemBuffer.miBufferLen;
            miAppendNum = cMemBuffer.miAppendNum;
            if (mBuff)
            {
                JL_MEMMGR_MEM_FUNC.pFreeFunc(mBuff);
            }

            mBuff = JL_MEMMGR_MEM_FUNC.pMallocFunc(miBufferLen);
            memcpy(mBuff, cMemBuffer.mBuff, miBufferUseLen);
            return 0;
        }

        void *CJlMemBuffer::GetBuffer()
        {
            return mBuff;
        }

        void CJlMemBuffer::SetBuffLen(const size_t iLen)
        {
            if (iLen > miBufferLen)
            {
                jlsvr::jlbase::RedPrint("Mem Over Limit");
                return;
            }

            miBufferUseLen = iLen;
        }

        void *CJlMemBuffer::AllocBuffer(size_t iLen)
        {
            if (iLen <= 0)
            {
                return mBuff;
            }

            if (!mBuff)
            {
                if (iLen < MEM_BUFFER_DEFAULT_LEN)
                {
                    miBufferLen = MEM_BUFFER_DEFAULT_LEN;
                }
                else
                {
                    miBufferLen = iLen;
                }

                mBuff = JL_MEMMGR_MEM_FUNC.pMallocFunc(miBufferLen + 1);
            }
            else
            {
                if (iLen + miBufferUseLen >= miBufferLen)
                {
                    miAppendNum++;
                    miBufferLen += miAppendNum * MEM_BUFFER_DEFAULT_LEN;
                    mBuff = JL_MEMMGR_MEM_FUNC.pReallocFunc(mBuff, miBufferLen);
                }
            }

            return mBuff;
        }

        void CJlMemBuffer::AppendNul()
        {
            if (mBuff)
            {
                char *pBuff = (char *)mBuff;
                pBuff[miBufferUseLen] = '\0';
            }
        }

        void CJlMemBuffer::Append(const void *pData, const size_t iLen)
        {
            if (pData && iLen > 0)
            {
                if (iLen + miBufferUseLen > miBufferLen)
                {
                    AllocBuffer(iLen);
                }

                memcpy((char *)mBuff + miBufferUseLen, pData, iLen);
                miBufferUseLen += iLen;
            }
            else
            {
                jlsvr::jlbase::RedPrint("Error CJlMemBuffer::Append");
            }
        }
    } // namespace jlbase
} // namespace jlsvr