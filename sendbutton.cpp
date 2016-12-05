#include "sendbutton.h"

SendButton::SendButton(QTreeWidgetItem *ptwi,QString s):
    QPushButton()
{
    setText(s);
    twi=ptwi;
}

QTreeWidgetItem *SendButton::getTwi()
{
    return twi;
}

