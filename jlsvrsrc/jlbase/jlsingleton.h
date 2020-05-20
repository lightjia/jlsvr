/*
@author:lightjia
@date:2020/5/20
@desc:单例类公共基类模块
*/

#ifndef __JL_SINGLETON__H_
#define __JL_SINGLETON__H_
#include <mutex>
#define SINGLE_CLASS_INITIAL(TypeName)     \
private:                                   \
    TypeName();                            \
    TypeName(const TypeName &) = delete;            \
    TypeName &operator=(const TypeName &) = delete; \
    friend class CJlSingleton<TypeName>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &);            \
    TypeName &operator=(const TypeName &)
namespace jlsvr
{
    namespace jlbase
    {
        template <typename T>
        class CJlSingleton
        {
        public:
            static T *Instance();
            virtual ~CJlSingleton(){};

        protected:
            CJlSingleton(){};

        private:
            class CGarbo //它的唯一工作就是在析构函数中删除CJlSingleton的实例
            {
            public:
                ~CGarbo()
                {
                    if (nullptr != CJlSingleton::mpInstance)
                    {
                        DODELETE(CJlSingleton::mpInstance);
                    }
                }
            };

        private:
            DISALLOW_COPY_AND_ASSIGN(CJlSingleton);
            static T *mpInstance;
            static CGarbo mcGarbo; //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数
            static std::mutex mcStaticMutex;
        };

        template <typename T>
        T *CJlSingleton<T>::mpInstance = nullptr;

        template <typename T>
        std::mutex CJlSingleton<T>::mcStaticMutex;

        //template <typename T>
        //CJlSingleton<typename>::CGarbo CJlSingleton<T>::mcGarbo;

        template <typename T>
        T *CJlSingleton<T>::Instance()
        {
            if (nullptr == mpInstance)
            {
                mcStaticMutex.lock();
                while (nullptr == mpInstance)
                {
                    mpInstance = new T();
                }
                mcStaticMutex.unlock();
            }

            return mpInstance;
        }
    } // namespace jlbase
} // namespace jlsvr
#endif //__JL_SINGLETON__H_