/*
@author:lightjia
@date:2020/5/20
@desc:公共定义模块
*/

#ifndef __JLDEF__H_
#define __JLDEF__H_
#include <functional>
//Get the platform
#if (defined WIN32) || (defined _WIN32) || (defined WIN64) || (defined _WIN64)
#define PLATFORM_WINDOWS
#elif (defined __linux__)
#define PLATFORM_LINUX
#endif

namespace jlsvr
{
    namespace jlbase
    {
        typedef signed char _i8;
        typedef unsigned char _u8;
        typedef signed short _i16;
        typedef unsigned short _u16;
        typedef signed int _i32;
        typedef unsigned int _u32;
        typedef signed long _l;
        typedef unsigned long _ul;
        typedef signed long long _i64;
        typedef unsigned long long _u64;

#pragma pack(1)
        struct tagJlTime
        {
            int tmsec;          /* seconds after the minute - [0,59] */
            int tmmin;          /* minutes after the hour - [0,59] */
            int tmhour;         /* hours since midnight - [0,23] */
            int tmmday;         /* day of the month - [1,31] */
            int tmmon;          /* months since January - [0,11] */
            int tmyear;         /* years since 1900 */
            int tmwday;         /* days since Sunday - [0,6] */
            int tmyday;         /* days since January 1 - [0,365] */
            int tmisdst;        /* daylight savings time flag */
            int tmmilliseconds; /*milliseconds after the sec[0,1000]*/
        };

        struct jl_mem_oper_func
        {
            std::function<void *(size_t)> pMallocFunc;
            std::function<void(void *)> pFreeFunc;
            std::function<void *(size_t, size_t)> pCallocFunc;
            std::function<void *(void *, size_t)> pReallocFunc;
        };
        const struct jl_mem_oper_func DEFAULT_JL_MEM_FUNC = {malloc, free, calloc, realloc};
#pragma pack()
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLDEF__H_