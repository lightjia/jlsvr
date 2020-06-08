/*
@author:lightjia
@date:2020/6/8
@desc:libuv TCP  Server实现
*/
#ifndef __JLTCPSVR__H_
#define __JLTCPSVR__H_
#include "jluvnetbase.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvTcpSvr : public CJlUvNetbase
        {
        public:
            CJlUvTcpSvr();
            virtual ~CJlUvTcpSvr();

        public:
            static void ConnCb(uv_stream_t *pHandle, int iStatus);

        protected:
            virtual int OnAccept(uv_tcp_t *pUvTcp) = 0; //TODO free pUvTcp
            virtual uv_tcp_t *AllocTcpCli();
            virtual int FreeTcpCli(uv_tcp_t *pTcpCli);

        protected:
            int Listen(int iBackLog = 5);

        private:
            void DoConn(int iStatus);

        protected:
            uv_tcp_t *mpTcpSvr;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLTCPSVR__H_