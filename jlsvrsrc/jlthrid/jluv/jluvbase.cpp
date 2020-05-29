#include "jluvbase.h"

namespace jlsvr
{
    namespace jlthrid
    {
        CJlUvBase::CJlUvBase()
        {
            mpUvLoop = nullptr;
            BZERO(&mstUvBuf);
        }

        CJlUvBase::~CJlUvBase()
        {
            jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(mstUvBuf.pBuf);
            mstUvBuf.pBuf = NULL;
        }

        void CJlUvBase::UvBufAlloc(uv_handle_t *pStream, size_t iSize, uv_buf_t *pBuf)
        {
            CJlUvBase *pUvBase = (CJlUvBase *)uv_handle_get_data((uv_handle_t *)pStream);
            ASSERT_RET(pUvBase && pUvBase->mstUvBuf.pBuf && pUvBase->mstUvBuf.iLen > 0 && pUvBase->mstUvBuf.iLen > pUvBase->mstUvBuf.iUse);
            *pBuf = uv_buf_init(pUvBase->mstUvBuf.pBuf + pUvBase->mstUvBuf.iUse, (unsigned int)(pUvBase->mstUvBuf.iLen - pUvBase->mstUvBuf.iUse));
        }

        int CJlUvBase::Init(ssize_t iBufSize)
        {
            ASSERT_RET_VALUE(NULL != mpUvLoop, 1);
            if (iBufSize > 0)
            {
                mstUvBuf.pBuf = (char *)jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pMallocFunc(iBufSize * sizeof(char));
            }

            mstUvBuf.iLen = iBufSize;
            return OnInit();
        }

        void CJlUvBase::BaseTimerOut(uv_timer_t *pHandler)
        {
            CJlUvBase *pUvBase = (CJlUvBase *)uv_handle_get_data((uv_handle_t *)pHandler);
            if (NULL != pUvBase)
            {
                pUvBase->OnBaseTimer();
            }
        }

        void CJlUvBase::OnBaseTimer()
        {
        }

        int CJlUvBase::StopBaseTimer()
        {
            uv_close((uv_handle_t *)&mstUvTimer, NULL);
            return 0;
        }

        int CJlUvBase::StartBaseTimer(uint64_t iTimeout, uint64_t iRepeat)
        {
            ASSERT_RET_VALUE(NULL != mpUvLoop, 1);
            uv_timer_init(mpUvLoop, &mstUvTimer);
            uv_handle_set_data((uv_handle_t *)&mstUvTimer, (void *)this);
            return uv_timer_start(&mstUvTimer, CJlUvBase::BaseTimerOut, iTimeout, iRepeat);
        }
    }; // namespace jlthrid
};     // namespace jlsvr