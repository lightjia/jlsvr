#include "jluvnetbase.h"
#include "jlguid.h"
namespace jlsvr
{
    namespace jlthrid
    {
        CJlUvNetbase::CJlUvNetbase()
        {
            mstrIp = "";
            musPort = 0;
            BZERO(&mstLocalAddr);
            mstrNetId = jlsvr::jlbase::CJlGuidTool::Instance()->CreateGuid();
        }

        CJlUvNetbase::~CJlUvNetbase()
        {
        }

        int CJlUvNetbase::SetNetParam(const char *pIp, unsigned short iPort)
        {
            if (NULL != pIp && strlen(pIp) > 0)
            {
                mstrIp = pIp;
            }

            musPort = iPort;
            return uv_ip4_addr(mstrIp.c_str(), iPort, &mstLocalAddr);
        }
    }; // namespace jlthrid
};     // namespace jlsvr