#include "a.h"

#include <QDebug>

A::A()
{
    a = 30;
    qDebug() << "A()";
}

A::~A()
{
    qDebug() << "~A()";
}

void A::run()
{
    qDebug() << "A::run()";
    qDebug() << a;
}

void A::call()
{
    qDebug() << "A::call()";
}
