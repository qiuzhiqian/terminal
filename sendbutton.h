#ifndef SENDBUTTON_H
#define SENDBUTTON_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QPushButton>

class SendButton : public QPushButton
{
public:
    SendButton(QTreeWidgetItem *ptwi,QString s);
    QTreeWidgetItem *getTwi();
private:
    QTreeWidgetItem *twi;
};

#endif // SENDBUTTON_H
