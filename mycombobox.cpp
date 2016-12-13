#include "mycombobox.h"

MyComboBox::MyComboBox(QTreeWidget *ptw, QTreeWidgetItem *ptwi):
    QComboBox()
{
    tw=ptw;
    twi=ptwi;
}

QTreeWidget *MyComboBox::getTw()
{
    return tw;
}

QTreeWidgetItem *MyComboBox::getTwi()
{
    return twi;
}

//void MyComboBox::slt_click(bool b)
//{
//    emit sgn_click(tw,twi);
//}

//void MyComboBox::focusInEvent(QFocusEvent *e)
//{
//    tw->setCurrentItem(twi);
//}

void MyComboBox::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        tw->setCurrentItem(twi);
    }
    setFocus();         //因为setCurrentItem会使控件失去焦点，从而导致后面的mousePressEvent无法正常工作，所以需要恢复焦点
    QComboBox::mousePressEvent(event);
}

