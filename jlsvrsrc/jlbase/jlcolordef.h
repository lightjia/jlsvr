/*
@author:lightjia
@date:2020/5/20
@desc:控制台打印模块

控制字符的通用格式如下:
Esc[{attr1};...;{attrn}m

其中:
Esc 是转义字符, 其值为"\033";
[ 是常量的左中括号;
{attr1};...{attrn} 是若干属性, 通常是由一个有特定意义的数字代替, 每个属性之间用分号分隔;
m 就是字面常量字符m;
*/
#ifndef __JL_COLOR_DEF__H_
#define __JL_COLOR_DEF__H_
#include "jlcommoninc.h"

namespace jlsvr
{
    namespace jlbase
    {
        //通用格式控制
        enum ColorControl
        {
            COLOR_CONTROL_RESET = 0,     //重置所有属性
            COLOR_CONTROL_HIGHLIGHT = 1, //高亮加粗
            COLOR_CONTROL_DIM = 2,       //暗淡
            COLOR_CONTROL_UNDERLINE = 4, //下划线
            COLOR_CONTROL_FLICKER = 5,   //闪烁
            COLOR_CONTROL_REVERSE = 7,   //反转
            COLOR_CONTROL_HIDE = 8,      //隐藏
        };

        //前景色
        enum ColorForeground
        {
            COLOR_FOREGROUND_BLACK = 30,   //黑色
            COLOR_FOREGROUND_RED = 31,     //红色
            COLOR_FOREGROUND_GREEN = 32,   //绿色
            COLOR_FOREGROUND_YELLOW = 33,  //黄色
            COLOR_FOREGROUND_BLUE = 34,    //蓝色
            COLOR_FOREGROUND_MAGENTA = 35, //品红
            COLOR_FOREGROUND_CYAN = 36,    //青色
            COLOR_FOREGROUND_WHITE = 37,   //白色
        };

        //前景色
        enum ColorBackground
        {
            COLOR_BACKGROUND_BLACK = 40,   //黑色
            COLOR_BACKGROUND_RED = 41,     //红色
            COLOR_BACKGROUND_GREEN = 42,   //绿色
            COLOR_BACKGROUND_YELLOW = 43,  //黄色
            COLOR_BACKGROUND_BLUE = 44,    //蓝色
            COLOR_BACKGROUND_MAGENTA = 45, //品红
            COLOR_BACKGROUND_CYAN = 46,    //青色
            COLOR_BACKGROUND_WHITE = 47,   //白色
        };

        void RedPrint(const char *pData);
        void MagentaPrint(const char *pData);
        void YellowPrint(const char *pData);
        void GreenPrint(const char *pData);
        void BluePrint(const char *pData);
        void CyanPrint(const char *pData);
        void RandColorPrint(const char *pData);
    }; // namespace jlbase
} // namespace jlsvr
#endif //__JL_COLOR_DEF__H_