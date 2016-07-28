#include "b.h"

#include <QDebug>

B::B()
{
    a = 0;
    qDebug() << "B()";
}

B::~B()
{
    qDebug() << "~B()";
}

void B::run()
{
    qDebug() << "B::run()";
    qDebug() << a;
    qDebug() << A::a;
}
