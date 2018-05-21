#include "singleton.hpp"
#include <vector>

static std::vector<SingletonFinalizer::FinalizerFunc> __finalizers;

void SingletonFinalizer::addFinalizer(FinalizerFunc func)
{
    __finalizers.push_back(func);
}

void SingletonFinalizer::finalize()
{
    // WARN: thread-safe 하지 않으므로 호출에 주의할 것
    for (auto finalizerFunc : __finalizers)
    {
        finalizerFunc();
    }
}

