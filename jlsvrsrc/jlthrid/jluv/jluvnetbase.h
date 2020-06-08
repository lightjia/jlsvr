/*
@author:lightjia
@date:2020/6/8
@desc:libuv pipe通信基类定义
*/
#ifndef __JLUVNETBASE__H_
#define __JLUVNETBASE__H_
#include "jluvbase.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvNetbase : public CJlUvBase
        {
        public:
            CJlUvNetbase();
            virtual ~CJlUvNetbase();

        public:
            int SetNetParam(const char *pIp, unsigned short iPort);
            unsigned short GetPort() { return musPort; }
            std::string &GetIp() { return mstrIp; }
            const std::string &GetNetId() { return mstrNetId; };

        protected:
            unsigned short musPort;
            std::string mstrIp;
            struct sockaddr_in mstLocalAddr;

        private:
            std::string mstrNetId;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVNETBASE__H_