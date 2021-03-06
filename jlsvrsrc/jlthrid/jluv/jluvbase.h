/*
@author:lightjia
@date:2020/5/29
@desc:libuv 基类定义
*/
#ifndef __JLUVBASE__H_
#define __JLUVBASE__H_
#include "jllog.h"
#include "jlutil.h"
#include "jlmemmgr.h"
#include "jluvdef.h"
namespace jlsvr
{
    namespace jlthrid
    {
        class CJlUvBase
        {
        public:
            CJlUvBase();
            virtual ~CJlUvBase();

        public:
            void SetUvLoop(uv_loop_t *const pUvLoop) { mpUvLoop = pUvLoop; }
            virtual uv_loop_t *GetUvLoop() { return mpUvLoop; }
            int Init(ssize_t iBufSize = UV_DEFAULT_BUF_SIZE);
            int StartBaseTimer(uint64_t iTimeout, uint64_t iRepeat);
            int StopBaseTimer();

        protected:
            static void UvBufAlloc(uv_handle_t *pStream, size_t iSize, uv_buf_t *pBuf);
            static void BaseTimerOut(uv_timer_t *pHandler);

        protected:
            virtual int OnInit() = 0;
            virtual void OnBaseTimer();

        protected:
            uv_loop_t *mpUvLoop;
            uv_timer_t mstUvTimer;
            tagJlUvBuf mstUvBuf;
        };
    }; // namespace jlthrid
};     // namespace jlsvr
#endif //__JLUVBASE__H_