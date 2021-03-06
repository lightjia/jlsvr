/*
@author:lightjia
@date:2020/6/8
@desc:libuv TCP  Client实现
*/
#ifndef __JLUVTCPCLI__H_
#define __JLUVTCPCLI__H_
#include "jluvnetbase.h"
#include "jlmembuffer.h"
#include "jluvmutex.h"
#include <queue>
#include <map>
#include <vector>
#include <string>
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvTcpCli : public CJlUvNetbase
        {
        public:
            CJlUvTcpCli();
            virtual ~CJlUvTcpCli();

        public:
            static void RecvCb(uv_stream_t *pHandle, ssize_t nRead, const uv_buf_t *pBuf);
            static void ConnCb(uv_connect_t *pReq, int iStatus);
            static void SendCb(uv_write_t *pReq, int iStatus);
            static void CloseCb(uv_handle_t *pHandle);
            static void NotifySend(uv_async_t *pHandle);

        public:
            void SetTcpCli(uv_tcp_t *pTcpCli);
            int Connect(std::string strIp, unsigned short sPort);
            int Send(char *pData, ssize_t iLen);
            int Close();
            uint64_t GetTotalSendBytes() { return miTotalSendBytes; }
            uint64_t GetTotalRecvBytes() { return miTotalRecvBytes; }
            uint64_t GetNeedSendBytes() { return miNeedSendBytes; }

        private:
            void ParseIpPort();
            int AfterConn();
            int StartRecv();
            int DoSend();
            void DoConn(int iStatus);
            void DoRecv(ssize_t nRead, const uv_buf_t *pBuf);
            void AfterSend(uv_write_t *pReq, int iStatus);
            void CleanSendQueue();

        protected:
            virtual int OnRecv(ssize_t nRead, const uv_buf_t *pBuf) = 0;
            virtual int OnConn(int iStatus) = 0;
            virtual int OnClose() = 0;
            virtual int OnSend(int iStatus) = 0;

        protected:
            uv_tcp_t *mpTcpCli;
            uv_connect_t *mpUvConn;

        private:
            int miTcpCliState;
            uint64_t miTotalRecvBytes;
            uint64_t miTotalSendBytes;
            uint64_t miNeedSendBytes;
            uv_async_t mstUvSendAsync;
            jlsvr::jlbase::CJlMemBuffer *mpSendBuf;
            CJlUvMutex mcSendMutex;
            std::map<uv_write_t *, tagJlUvBufArray> mmapSend;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLTCPCLI__H_