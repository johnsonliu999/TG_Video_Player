#ifndef IPLINEEDIT_H
#define IPLINEEDIT_H

#include <QLineEdit>

class QWidget;

class SegIPEdit : public QLineEdit
{
    Q_OBJECT

public:
    SegIPEdit(QWidget* parent = 0);
    ~SegIPEdit();
};

class IPLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IPLineEdit(QWidget* parent = 0);
    ~IPLineEdit();
    void setGeometry(int x, int y, int w, int h);
    void setGeometry(const QRect&);
    QString text() const;

private:
    void paintEvent(QPaintEvent* E);

    SegIPEdit* mp_segIPEdit[4];

};

#endif // IPLINEEDIT_H
