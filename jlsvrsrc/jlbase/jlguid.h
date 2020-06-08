/*
@author:lightjia
@date:2020/6/8
@desc:guid定义
*/
#ifndef __JLGUID__H_
#define __JLGUID__H_
#include "jlsingleton.h"
#include <string>
namespace jlsvr
{
    namespace jlbase
    {
        class CJlGuidTool : public jlsvr::jlbase::CJlSingleton<CJlGuidTool>
        {
            SINGLE_CLASS_INITIAL(CJlGuidTool);
        public:
           std::string CreateGuid();
        };
    } // namespace jlbase
} // namespace jlsvr
#endif //__JLGUID__H_