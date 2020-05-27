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
        }

        CJlMemBuffer::~CJlMemBuffer()
        {
            miBufferLen = 0;
            miBufferUseLen = 0;
        }

        CJlMemBuffer::CJlMemBuffer(const CJlMemBuffer &cMemBuffer)
        {
            if (this != &cMemBuffer)
            {
                SetBuffer(cMemBuffer.mBuff, cMemBuffer.miBufferUseLen);
            }
        }

        CJlMemBuffer &CJlMemBuffer::operator=(const CJlMemBuffer &cMemBuffer)
        {
            if (this != &cMemBuffer)
            {
                SetBuffer(cMemBuffer.mBuff, cMemBuffer.miBufferUseLen);
            }

            return *this;
        }

        int CJlMemBuffer::SetBuffer(const std::unique_ptr<jlsvr::jlbase::_u8, std::function<void(void *)>> &upBuff, const size_t iDataLen)
        {
            if (iDataLen <= 0)
            {
                return 1;
            }

            miBufferUseLen = 0;
            miBufferLen = 0;
            miAppendNum = 0;
            mBuff.reset();
            Append(upBuff.get(), iDataLen);
            return 0;
        }

        void *CJlMemBuffer::GetBuffer(size_t iIndex)
        {
            if (iIndex > miBufferUseLen)
            {
                return nullptr;
            }

            jlsvr::jlbase::_u8 *pTmp = (jlsvr::jlbase::_u8 *)mBuff.get();
            pTmp += iIndex;
            return (void *)pTmp;
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
                return nullptr;
            }

            jlsvr::jlbase::_u8 *pBuffer = (jlsvr::jlbase::_u8 *)mBuff.get();
            if (!pBuffer)
            {
                if (iLen < MEM_BUFFER_DEFAULT_LEN)
                {
                    miBufferLen = MEM_BUFFER_DEFAULT_LEN;
                }
                else
                {
                    miBufferLen = iLen;
                }

                pBuffer = (jlsvr::jlbase::_u8 *)JL_MEMMGR_MEM_FUNC.pMallocFunc(miBufferLen + 1);
                std::unique_ptr<jlsvr::jlbase::_u8, std::function<void(void *)>> up(pBuffer, JL_MEMMGR_MEM_FUNC.pFreeFunc);
                mBuff = std::move(up);
            }
            else
            {
                if (iLen + miBufferUseLen >= miBufferLen)
                {
                    miAppendNum++;
                    miBufferLen += miAppendNum*MEM_BUFFER_DEFAULT_LEN;
                    pBuffer = (jlsvr::jlbase::_u8 *)JL_MEMMGR_MEM_FUNC.pReallocFunc(pBuffer, miBufferLen);
                    mBuff.release();
                    mBuff.reset(pBuffer);
                }
            }

            return pBuffer;
        }

        void CJlMemBuffer::AppendNul()
        {
            jlsvr::jlbase::_u8 *pBuffer = (jlsvr::jlbase::_u8 *)mBuff.get();
            if (pBuffer)
            {
                char *pTmp = (char *)pBuffer;
                pTmp[miBufferUseLen] = '\0';
            }
        }

        void CJlMemBuffer::Append(const void *pData, const size_t iLen)
        {
            if (pData && iLen > 0)
            {
                 jlsvr::jlbase::_u8 *pBuffer = (jlsvr::jlbase::_u8 *)mBuff.get();
                 size_t iNeedLen = 0;
                if (!pBuffer || iLen + miBufferUseLen > miBufferLen)
                {
                    pBuffer = (jlsvr::jlbase::_u8 *)AllocBuffer(iLen);
                }

                memcpy((char *)pBuffer + miBufferUseLen, pData, iLen);
                miBufferUseLen += iLen;
            }
            else
            {
                jlsvr::jlbase::RedPrint("Error CJlMemBuffer::Append");
            }
        }
    } // namespace jlbase
} // namespace jlsvr