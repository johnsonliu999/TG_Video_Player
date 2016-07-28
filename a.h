#ifndef A_H
#define A_H

#include <QObject>

class A : public QObject
{
    Q_OBJECT
public:
    A();
    ~A();

    virtual void run();

protected:
    int a;

private:
    virtual void call();
};

#endif // A_H
