/*
@author:lightjia
@date:2020/5/29
@desc:libuv公共定义
*/
#ifndef __JLUVDEF__H_
#define __JLUVDEF__H_
namespace jlsvr
{
    namespace jlthrid
    {
#include "uv.h"
#include "jlmemmgr.h"

#define UV_DEFAULT_BUF_SIZE 10240 //default 10KB buffer size
#pragma pack(1)
        struct tagJlUvBuf
        {
            char *pBuf;
            ssize_t iLen;
            ssize_t iUse;
            ssize_t iTotal;
        };

        struct tagJlUvBufArray
        {
            uv_buf_t *pBufs;
            unsigned int iBufNum;
            void *pMemBuf;
        };
#pragma pack()
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVDEF__H_