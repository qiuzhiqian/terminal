#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QComboBox>
#include <QMouseEvent>

class MyComboBox : public QComboBox
{
public:
    MyComboBox(QTreeWidget *ptw,QTreeWidgetItem *ptwi);
    QTreeWidget *getTw();
    QTreeWidgetItem *getTwi();
private:
    QTreeWidget *tw;
    QTreeWidgetItem *twi;

protected:
//    virtual void focusInEvent(QFocusEvent *e);
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // MYCOMBOBOX_H
