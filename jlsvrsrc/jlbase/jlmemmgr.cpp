#include "jlmemmgr.h"
#include "jlcolordef.h"
#include <cstring>
namespace jlsvr
{
    namespace jlbase
    {
        size_t CJlMemMgr::GetMemLen(void *pData)
        {
            size_t iLen = 0;
            if (pData)
                memcpy(&iLen, (unsigned char *)pData - sizeof(iLen), sizeof(iLen));
            return iLen;
        }

        CJlMemMgr::CJlMemMgr()
        {
            miMalloc = 0;
            miFree = 0;
        }

        void *CJlMemMgr::DoMalloc(size_t iLen)
        {
            if (iLen > 0)
            {
                void *pRet = DEFAULT_JL_MEM_FUNC.pMallocFunc(iLen + sizeof(size_t));
                memcpy((unsigned char *)pRet, &iLen, sizeof(iLen));
                miMalloc += iLen + sizeof(iLen);
                return (void *)((unsigned char *)pRet + sizeof(iLen));
            }
            else
            {
                jlsvr::jlbase::RedPrint("CJlMemMgr::DoMalloc Len Is Zero\n");
                return nullptr;
            }
        }

        void *CJlMemMgr::MemMalloc(size_t iLen)
        {
            CJlMemMgr::Instance()->DoMalloc(iLen);
        }

        void CJlMemMgr::DoFree(void *pData)
        {
            if (pData)
            {
                size_t iLen = 0;
                miFree += GetMemLen(pData) + sizeof(iLen);
                DEFAULT_JL_MEM_FUNC.pFreeFunc((unsigned char *)pData - sizeof(iLen));
            }
            else
            {
                jlsvr::jlbase::RedPrint("CJlMemMgr::DoFree Null\n");
            }
        }

        void CJlMemMgr::MemFree(void *pData)
        {
            CJlMemMgr::Instance()->DoFree(pData);
        }

        void *CJlMemMgr::DoCalloc(size_t count, size_t size)
        {
            void *pRet = NULL;
            size_t iNeedLen = count * size;
            pRet = MemMalloc(iNeedLen);

            return pRet;
        }

        void *CJlMemMgr::MemCalloc(size_t count, size_t size)
        {
            return CJlMemMgr::Instance()->DoCalloc(count, size);
        }

        void *CJlMemMgr::DoRealloc(void *ptr, size_t size)
        {
            void *pRet = NULL;
            size_t iSrcLen = GetMemLen(ptr);
            pRet = DoMalloc(iSrcLen + size);
            memcpy(pRet, ptr, iSrcLen);
            DoFree(ptr);
            return pRet;
        }

        void CJlMemMgr::Print()
        {
            char szBuf[180];
            snprintf(szBuf, 180, "miMalloc:%lld miFree:%lld\n", miMalloc.load(), miFree.load());
            jlsvr::jlbase::MagentaPrint(szBuf);
        }

        void *CJlMemMgr::MemRealloc(void *ptr, size_t size)
        {
            return CJlMemMgr::Instance()->DoRealloc(ptr, size);
        }

        void *CJlClassMemMgr::operator new(size_t size)
        {
            return CJlMemMgr::Instance()->MemMalloc(size);
        }

        void CJlClassMemMgr::operator delete(void *p)
        {
            CJlMemMgr::Instance()->MemFree(p);
        }
    } // namespace jlbase
} // namespace jlsvr
