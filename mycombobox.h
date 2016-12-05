#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QComboBox>

class MyComboBox : public QComboBox
{
public:
    MyComboBox(QTreeWidgetItem *ptwi);
    QTreeWidgetItem *getTwi();
private:
    QTreeWidgetItem *twi;
};

#endif // MYCOMBOBOX_H
