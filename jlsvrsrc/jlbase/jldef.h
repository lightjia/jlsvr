/*
@author:lightjia
@date:2020/5/20
@desc:公共定义模块
*/

#ifndef __JLDEF__H_
#define __JLDEF__H_

//Get the platform
#if (defined WIN32) || (defined _WIN32) ||(defined WIN64) || (defined _WIN64)
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
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLDEF__H_