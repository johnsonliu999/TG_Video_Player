#include "iplineedit.h"

#include <QWidget>
#include <QRegExp>
#include <QValidator>
#include <QPainter>
#include <QDebug>

SegIPEdit::SegIPEdit(QWidget* parent) :
    QLineEdit(parent)
{
    setAlignment(Qt::AlignCenter);
    setMaxLength(3);
    //setMinimumSize(25,18);
    QRegExp rx("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");

    setValidator(new QRegExpValidator(rx, this));
}

SegIPEdit::~SegIPEdit()
{

}

IPLineEdit::IPLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

IPLineEdit::~IPLineEdit()
{

}

void IPLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);

    int segWidth = (width()-5)/4;
    int pointHeight = height()/2;

    QPainter painter(this);
    painter.save();
    painter.setPen(QPen(Qt::black));

    for (int i = 0; i < 3; i++)
        painter.drawPoint((segWidth+1)*(i+1),pointHeight);
    painter.restore();
}

void IPLineEdit::setGeometry(int x, int y, int w, int h)
{
    QLineEdit::setGeometry(x, y, w, h);

    int segWidth = (width()-5)/4;
    int segHeight = height()-2;

    for (int i = 0; i < 4; i++)
    {
        mp_segIPEdit[i] = new SegIPEdit(this);
        mp_segIPEdit[i]->setFrame(false);
        mp_segIPEdit[i]->setGeometry(1+(segWidth+1)*i, 1, segWidth, segHeight);
    }
}

void IPLineEdit::setGeometry(const QRect &r)
{
    QLineEdit::setGeometry(r);

    int segWidth = (width()-5)/4;
    int segHeight = height()-2;

    for (int i = 0; i < 4; i++)
    {
        mp_segIPEdit[i] = new SegIPEdit(this);
        mp_segIPEdit[i]->setFrame(false);
        mp_segIPEdit[i]->setGeometry(1+(segWidth+1)*i, 1, segWidth, segHeight);
    }


}

QString IPLineEdit::text() const
{
    QString text;
    for (int i = 0; i < 3; i++)
    {
        text += mp_segIPEdit[i]->text();
        text += ".";
    }
    text += mp_segIPEdit[3]->text();

    return text;
}
