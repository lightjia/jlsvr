#include "jluvtcpcli.h"
#include "jllog.h"
namespace jlsvr
{
    namespace jlthrid
    {
        enum UV_TCP_CLI_STATE
        {
            UV_TCP_CLI_STATE_NONE,
            UV_TCP_CLI_STATE_ESTAB,
            UV_TCP_CLI_STATE_CLOSE,
        };

        CJlUvTcpCli::CJlUvTcpCli()
        {
            mpTcpCli = NULL;
            mpUvConn = NULL;
            mpSendBuf = NULL;
            miTotalSendBytes = 0;
            miNeedSendBytes = 0;
            miTotalRecvBytes = 0;
            miTcpCliState = UV_TCP_CLI_STATE_NONE;
        }

        CJlUvTcpCli::~CJlUvTcpCli()
        {
            CleanSendQueue();
        }

        void CJlUvTcpCli::DoRecv(ssize_t nRead, const uv_buf_t *pBuf)
        {
            if (nRead == 0)
            {
                LOG_DBG("%s recv 0 bytes", GetNetId().c_str());
            }
            else if (nRead < 0)
            {
                Close();
            }
            else if (nRead > 0)
            {
                miTotalRecvBytes += (unsigned long)nRead;
                OnRecv(nRead, pBuf);
            }
        }

        void CJlUvTcpCli::RecvCb(uv_stream_t *pHandle, ssize_t nRead, const uv_buf_t *pBuf)
        {
            CJlUvTcpCli *pTcpCli = (CJlUvTcpCli *)uv_handle_get_data((uv_handle_t *)pHandle);
            ASSERT_RET(pTcpCli);
            pTcpCli->DoRecv(nRead, pBuf);
        }

        void CJlUvTcpCli::ParseIpPort()
        {
            ASSERT_RET(mpTcpCli);
            struct sockaddr stSock;
            int iSockNameLen = (int)sizeof(stSock);
            uv_tcp_getpeername(mpTcpCli, &stSock, &iSockNameLen);

            struct sockaddr_in *pSock = (struct sockaddr_in *)&stSock;
            musPort = ntohs(pSock->sin_port);
#if (defined PLATFORM_WINDOWS)
            mstrIp = inet_ntoa(pSock->sin_addr);
#elif (defined PLATFORM_LINUX)
            struct in_addr in = pSock->sin_addr;
            char str[INET_ADDRSTRLEN]; //INET_ADDRSTRLENÕâ¸öºêÏµÍ³Ä¬ÈÏ¶¨Òå 16
            inet_ntop(AF_INET, &in, str, sizeof(str));
            mstrIp = str;
#endif
        }

        void CJlUvTcpCli::SetTcpCli(uv_tcp_t *pTcpCli)
        {
            ASSERT_RET(NULL != mpUvLoop);
            mpTcpCli = pTcpCli;
            ParseIpPort();
            uv_handle_set_data((uv_handle_t *)mpTcpCli, (void *)this);
            AfterConn();
        }

        int CJlUvTcpCli::AfterConn()
        {
            StartRecv();

            mcSendMutex.Lock();
            uv_handle_set_data((uv_handle_t *)&mstUvSendAsync, (void *)this);
            uv_async_init(mpUvLoop, &mstUvSendAsync, CJlUvTcpCli::NotifySend);
            miTcpCliState = UV_TCP_CLI_STATE_ESTAB;
            mcSendMutex.UnLock();

            return OnConn(0);
        }

        void CJlUvTcpCli::DoConn(int iStatus)
        {
            if (!iStatus)
            {
                AfterConn();
                DoSend();
            }
            else
            {
                LOG_ERR("uv_tcp_connect error:%s %s", uv_strerror(iStatus), uv_err_name(iStatus));
                OnConn(iStatus);
                uv_close((uv_handle_t *)mpTcpCli, NULL);
            }
        }

        void CJlUvTcpCli::ConnCb(uv_connect_t *pReq, int iStatus)
        {
            CJlUvTcpCli *pTcpCli = (CJlUvTcpCli *)uv_handle_get_data((uv_handle_t *)pReq);
            ASSERT_RET(pTcpCli);
            pTcpCli->DoConn(iStatus);
        }

        int CJlUvTcpCli::Connect(std::string strIp, unsigned short sPort)
        {
            int iRet = 1;
            ASSERT_RET_VALUE(mpTcpCli && mpUvLoop && mpUvConn, iRet);

            uv_handle_set_data((uv_handle_t *)mpTcpCli, (void *)this);
            uv_tcp_init(mpUvLoop, mpTcpCli);
            if (musPort > 0)
            {
                iRet = uv_tcp_bind(mpTcpCli, (struct sockaddr *)&mstLocalAddr, SO_REUSEADDR);
                if (iRet)
                {
                    LOG_ERR("uv_tcp_bind error:%s %s", uv_strerror(iRet), uv_err_name(iRet));
                    return iRet;
                }
            }

            struct sockaddr_in stRemoteAddr;
            uv_ip4_addr(strIp.c_str(), sPort, &stRemoteAddr);
            uv_handle_set_data((uv_handle_t *)mpUvConn, (void *)this);
            iRet = uv_tcp_connect(mpUvConn, mpTcpCli, (struct sockaddr *)&stRemoteAddr, CJlUvTcpCli::ConnCb);
            if (iRet)
            {
                LOG_ERR("uv_tcp_connect error:%s %s", uv_strerror(iRet), uv_err_name(iRet));
            }

            return iRet;
        }

        int CJlUvTcpCli::StartRecv()
        {
            ASSERT_RET_VALUE(mpTcpCli && mpUvLoop, 1);

            int iSockBufLen = (int)(mstUvBuf.iLen - mstUvBuf.iUse);
            uv_recv_buffer_size((uv_handle_t *)mpTcpCli, &iSockBufLen);
            return uv_read_start((uv_stream_t *)mpTcpCli, CJlUvBase::UvBufAlloc, CJlUvTcpCli::RecvCb);
        }

        void CJlUvTcpCli::AfterSend(uv_write_t *pReq, int iStatus)
        {
            std::map<uv_write_t *, tagJlUvBufArray>::iterator iter = mmapSend.find(pReq);
            if (iter != mmapSend.end())
            {
                uv_write_t *pWriteReq = iter->first;
                jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(pWriteReq);
                miTotalSendBytes += iter->second.pMemBuf->GetBuffLen();
                DODELETE(iter->second.pMemBuf);
                jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(iter->second.pBufs);
                mmapSend.erase(iter);
            }
            else
            {
                LOG_ERR("Can Not Find The WriteReq");
            }

            OnSend(iStatus);
            if (iStatus)
            {
                if (iStatus == UV_ECANCELED)
                {
                    LOG_ERR("Peer Cancel Send Msg");
                    return;
                }

                LOG_ERR("uv_write:%s %s", uv_strerror(iStatus), uv_err_name(iStatus));
                Close();
            }
            else
            {
                DoSend();
            }
        }

        void CJlUvTcpCli::SendCb(uv_write_t *pReq, int iStatus)
        {
            CJlUvTcpCli *pTcpCli = (CJlUvTcpCli *)uv_handle_get_data((uv_handle_t *)pReq);
            ASSERT_RET(pTcpCli);
            pTcpCli->AfterSend(pReq, iStatus);
        }

        void CJlUvTcpCli::NotifySend(uv_async_t *pHandle)
        {
            CJlUvTcpCli *pTcpCli = (CJlUvTcpCli *)uv_handle_get_data((uv_handle_t *)pHandle);
            if (NULL != pTcpCli)
            {
                pTcpCli->DoSend();
            }
        }

        int CJlUvTcpCli::DoSend()
        {
            jlsvr::jlbase::CJlAutoMutex cAutoMutex(&mcSendMutex);
            if (UV_TCP_CLI_STATE_ESTAB == miTcpCliState)
            {
                tagJlUvBufArray stBufArray;
                BZERO(&stBufArray);
                if (mpSendBuf && mpSendBuf->GetBuffLen() > 0)
                {
                    stBufArray.iBufNum = 1;
                    stBufArray.pBufs = (uv_buf_t *)jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pMallocFunc(sizeof(uv_buf_t));
                    stBufArray.pBufs->base = (char *)mpSendBuf->GetBuffer();
                    stBufArray.pBufs->len = (unsigned long)mpSendBuf->GetBuffLen();
                    stBufArray.pMemBuf = mpSendBuf;
                    mpSendBuf = NULL;
                }

                if (stBufArray.iBufNum > 0)
                {
                    uv_write_t *pWriteReq = (uv_write_t *)jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pMallocFunc(sizeof(uv_write_t));
                    uv_handle_set_data((uv_handle_t *)pWriteReq, (void *)this);
                    mmapSend.insert(std::make_pair(pWriteReq, stBufArray));
                    int iWriteRet = uv_write(pWriteReq, (uv_stream_t *)mpTcpCli, stBufArray.pBufs, stBufArray.iBufNum, CJlUvTcpCli::SendCb);
                    if (iWriteRet)
                    {
                        LOG_ERR("uv_write:%s %s", uv_strerror(iWriteRet), uv_err_name(iWriteRet));
                    }
                }
            }

            return 0;
        }

        int CJlUvTcpCli::Send(char *pData, ssize_t iLen)
        {
            ASSERT_RET_VALUE(pData && iLen > 0 && mpTcpCli && mpUvLoop && UV_TCP_CLI_STATE_CLOSE != miTcpCliState, 1);
            jlsvr::jlbase::CJlAutoMutex cAutoMutex(&mcSendMutex);
            if (!mpSendBuf)
            {
                mpSendBuf = new jlsvr::jlbase::CJlMemBuffer();
            }

            ASSERT_RET_VALUE(mpSendBuf, 1);
            miNeedSendBytes += iLen;
            mpSendBuf->Append(pData, iLen);
            if (UV_TCP_CLI_STATE_ESTAB == miTcpCliState)
            {
                uv_async_send(&mstUvSendAsync);
            }

            return 0;
        }

        void CJlUvTcpCli::CleanSendQueue()
        {
            jlsvr::jlbase::CJlAutoMutex cAutoMutex(&mcSendMutex);
            DODELETE(mpSendBuf);

            while (!mmapSend.empty())
            {
                std::map<uv_write_t *, tagJlUvBufArray>::iterator iter = mmapSend.begin();
                uv_write_t *pWriteReq = iter->first;
                jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(pWriteReq);
                DODELETE(iter->second.pMemBuf);
                jlsvr::jlbase::JL_MEMMGR_MEM_FUNC.pFreeFunc(iter->second.pBufs);
                mmapSend.erase(iter);
            }
        }

        void CJlUvTcpCli::CloseCb(uv_handle_t *pHandle)
        {
            CJlUvTcpCli *pTcpCli = (CJlUvTcpCli *)uv_handle_get_data((uv_handle_t *)pHandle);
            if (pTcpCli)
            {
                pTcpCli->OnClose();
                //use uv_is_active to panduan
                uv_close((uv_handle_t *)&pTcpCli->mstUvSendAsync, NULL);
            }
        }

        int CJlUvTcpCli::Close()
        {
            ASSERT_RET_VALUE(mpTcpCli && mpUvLoop, 1);
            jlsvr::jlbase::CJlAutoMutex cAutoMutex(&mcSendMutex);
            if (UV_TCP_CLI_STATE_ESTAB == miTcpCliState)
            {
                uv_close((uv_handle_t *)mpTcpCli, CJlUvTcpCli::CloseCb);
                miTcpCliState = UV_TCP_CLI_STATE_CLOSE;
            }

            return 0;
        }
    }; // namespace jlthrid
};     // namespace jlsvr