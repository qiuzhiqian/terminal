#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit(QTreeWidgetItem *ptwi,QString s);
    QTreeWidgetItem *getTwi();
private:
    QTreeWidgetItem *twi;
};

#endif // MYLINEEDIT_H
