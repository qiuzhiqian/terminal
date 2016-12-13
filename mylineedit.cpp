#include "mylineedit.h"

MyLineEdit::MyLineEdit(QTreeWidget *ptw, QTreeWidgetItem *ptwi, QString s):
    QLineEdit()
{
    setText(s);
    tw=ptw;
    twi=ptwi;
}

QTreeWidget *MyLineEdit::getTw()
{
    return tw;
}

QTreeWidgetItem *MyLineEdit::getTwi()
{
    return twi;
}

//void MyLineEdit::slt_click(bool b)
//{
//    emit sgn_click(tw,twi);
//}

//void MyLineEdit::focusInEvent(QFocusEvent *e)
//{
//    tw->setCurrentItem(twi);
//}

void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        tw->setCurrentItem(twi);
    }
    setFocus();         //因为setCurrentItem会使控件失去焦点，从而导致后面的mousePressEvent无法正常工作，所以需要恢复焦点
    QLineEdit::mousePressEvent(event);
}
