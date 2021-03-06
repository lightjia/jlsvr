/*
@author:lightjia
@date:2020/5/20
@desc:公共定义模块
*/

#ifndef __JLCOMMON__H_
#define __JLCOMMON__H_
#include "jldef.h"
#include <iostream>
#include <memory>
#include <sys/stat.h>
#include <string>
#include <cstring>
#if (defined PLATFORM_WINDOWS)
#elif (defined PLATFORM_LINUX)
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#endif
#endif //__JLCOMMON__H_