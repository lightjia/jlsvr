/*
@author:lightjia
@date:2020/5/20
@desc:随机数模块
*/
#ifndef __JL_RAND__H_
#define __JL_RAND__H_
#include "jlsingleton.h"
namespace jlsvr
{
    namespace jlbase
    {
        class CJlRand : public jlsvr::jlbase::CJlSingleton<CJlRand>
        {
            SINGLE_CLASS_INITIAL(CJlRand);

        public:
            ~CJlRand();

            //设置随机种子
            void SetSeed(uint32_t dwSeedVal);
            uint32_t GetSeed() { return m_dwRandSeed; }

            uint32_t RandInt(uint32_t min, uint32_t max);

            float RandFloat(); //产生[0, 1f]闭区间的随机数

        private:
            void init_genrand(uint32_t s);
            uint32_t genrand_int32(void);

        private:
            static const int N = 624;
            static const int M = 397;

            uint64_t mt[N];
            int mti;

            uint32_t m_dwRandSeed;
        };
    } // namespace jlbase
} // namespace jlsvr

#endif //__JL_RAND__H_