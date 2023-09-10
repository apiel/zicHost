// g++ methodptr.cpp -o methodptr

#include <functional>
#include <vector>
#include <stdio.h>

class MyClass1{
public:
    void hello(float value)
    {
        printf("hello %f\n", value);
    }
};

class MyClass2 {
public:
    void world(float value)
    {
        printf("world %f\n", value);
    }
};

using Callback_t = std::function<void(float val)>;

template <typename T>
Callback_t MakeCallback(void (T::*f)(float))
{
    T* p = new T;
    return [=](float val) { (p->*f)(val); };
}

int main()
{
    std::vector<Callback_t> vetcor = {
        MakeCallback(&MyClass1::hello),
        MakeCallback(&MyClass2::world)
    };

    for (std::size_t i = 0; i < vetcor.size(); i++) {
        vetcor[i](0.456);
    }
    return 0;
}