#ifndef SENDBUTTON_H
#define SENDBUTTON_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QPushButton>
#include <QMouseEvent>

class SendButton : public QPushButton
{
public:
    SendButton(QTreeWidget *ptw,QTreeWidgetItem *ptwi,QString s);
    QTreeWidget *getTw();
    QTreeWidgetItem *getTwi();
private:
    QTreeWidget *tw;
    QTreeWidgetItem *twi;

//signals:
//    sgn_click(QTreeWidget *tw,QTreeWidgetItem *twi);

//private slots:
//    slt_click(bool);

protected:
//    virtual void focusInEvent(QFocusEvent *e);
   virtual void mousePressEvent(QMouseEvent *event);
};

#endif // SENDBUTTON_H
