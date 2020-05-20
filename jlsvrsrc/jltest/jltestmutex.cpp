#include "jltest.h"
#include "jlmutex.h"

class M : public jlsvr::jlbase::CJlMutex
{
public:
    M()
    {
        std::cout << "enter M\n";
    }

    ~M()
    {
        std::cout << "enter ~M\n";
    }
    void Lock() override
    {
        std::cout << "enter Lock\n";
    }

    int TryLock() override
    {
        return 0;
    }

    void UnLock() override
    {
        std::cout << "enter UnLock\n";
    }
};

void TestMutex()
{
    
    std::shared_ptr<M> m(new M());
    jlsvr::jlbase::CJlAutoMutex autoM(m.get());
}