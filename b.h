#ifndef B_H
#define B_H

#include "a.h"

class B : public A
{
public:
    B();
    ~B();

    void run();

private:
    int a;
};

#endif // B_H
