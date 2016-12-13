#include "sendbutton.h"

SendButton::SendButton(QTreeWidget *ptw, QTreeWidgetItem *ptwi, QString s):
    QPushButton()
{
    setText(s);
    tw=ptw;
    twi=ptwi;

//    connect(this,SIGNAL(clicked(bool))
}

QTreeWidget *SendButton::getTw()
{
    return tw;
}

QTreeWidgetItem *SendButton::getTwi()
{
    return twi;
}

//void SendButton::slt_click(bool b)
//{
//    emit sgn_click(tw,twi);
//}

//void SendButton::focusInEvent(QFocusEvent *e)
//{
//    tw->setCurrentItem(twi);
//}

void SendButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        tw->setCurrentItem(twi);
    }
    setFocus();         //因为setCurrentItem会使控件失去焦点，从而导致后面的mousePressEvent无法正常工作，所以需要恢复焦点

    QPushButton::mousePressEvent(event);
}

