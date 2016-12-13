#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QLineEdit>
#include <QMouseEvent>

class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit(QTreeWidget *ptw,QTreeWidgetItem *ptwi,QString s);
    QTreeWidget *getTw();
    QTreeWidgetItem *getTwi();
private:
    QTreeWidget *tw;
    QTreeWidgetItem *twi;

protected:
//    virtual void focusInEvent(QFocusEvent *e);
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // MYLINEEDIT_H
