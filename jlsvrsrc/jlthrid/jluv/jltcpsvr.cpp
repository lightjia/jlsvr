#include "jltcpsvr.h"
#include "jllog.h"
namespace jlsvr
{
    namespace jlthrid
    {
        CJlUvTcpSvr::CJlUvTcpSvr()
        {
            mpTcpSvr = NULL;
        }

        CJlUvTcpSvr::~CJlUvTcpSvr()
        {
        }

        uv_tcp_t *CJlUvTcpSvr::AllocTcpCli()
        {
            uv_tcp_t *pTcpCli = (uv_tcp_t *)jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pMallocFunc(sizeof(uv_tcp_t));
            //Can Use Another Loop In SubClass
            uv_tcp_init(GetUvLoop(), pTcpCli);
            return pTcpCli;
        }

        int CJlUvTcpSvr::FreeTcpCli(uv_tcp_t *pTcpCli)
        {
            ASSERT_RET_VALUE(pTcpCli && mpUvLoop, 1);
            uv_close((uv_handle_t *)pTcpCli, NULL);
            jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(pTcpCli);
            return 0;
        }

        void CJlUvTcpSvr::DoConn(int iStatus)
        {
            if (iStatus)
            {
                LOG_ERR("uv_conn error:%s %s", uv_strerror(iStatus), uv_err_name(iStatus));
                return;
            }

            uv_tcp_t *pTcpCli = AllocTcpCli();
            ASSERT_RET(NULL != pTcpCli);
            int iRet = uv_accept((uv_stream_t *)mpTcpSvr, (uv_stream_t *)pTcpCli);
            if (iRet)
            {
                LOG_ERR("uv_accept error:%s %s", uv_strerror(iRet), uv_err_name(iRet));
                FreeTcpCli(pTcpCli);
                return;
            }

            if (OnAccept(pTcpCli))
            {
                FreeTcpCli(pTcpCli);
            }
        }

        void CJlUvTcpSvr::ConnCb(uv_stream_t *pHandle, int iStatus)
        {
            CJlUvTcpSvr *pTcpSvr = (CJlUvTcpSvr *)uv_handle_get_data((uv_handle_t *)pHandle);
            ASSERT_RET(pTcpSvr);
            pTcpSvr->DoConn(iStatus);
        }

        int CJlUvTcpSvr::Listen(int iBackLog)
        {
            int iRet = 1;
            ASSERT_RET_VALUE(mpUvLoop && mpTcpSvr, iRet);

            uv_handle_set_data((uv_handle_t *)mpTcpSvr, (void *)this);
            uv_tcp_init(mpUvLoop, mpTcpSvr);
            iRet = uv_tcp_bind(mpTcpSvr, (struct sockaddr *)&mstLocalAddr, SO_REUSEADDR);
            if (iRet)
            {
                LOG_ERR("uv_tcp_bind error:%s %s", uv_strerror(iRet), uv_err_name(iRet));
                uv_close((uv_handle_t *)mpTcpSvr, NULL);
                return iRet;
            }

            iRet = uv_listen((uv_stream_t *)mpTcpSvr, iBackLog, CJlUvTcpSvr::ConnCb);
            if (iRet)
            {
                LOG_ERR("uv_listen error:%s %s", uv_strerror(iRet), uv_err_name(iRet));
                uv_close((uv_handle_t *)mpTcpSvr, NULL);
            }

            return iRet;
        }
    }; // namespace jlthrid
};     // namespace jlsvr