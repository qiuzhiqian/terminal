#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QWidget>
#include<QTreeWidgetItem>
//#include "senditem.h"
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include "sendbutton.h"


class TreeView : public QWidget
{
    Q_OBJECT

public:
    explicit TreeView(QWidget *parent = 0,QTreeWidget *tw=0);
    ~TreeView();

    QTreeWidgetItem * AddTreeRoot(int index=0, QString name="group");
    QTreeWidgetItem * AddTreeNode(QTreeWidgetItem *parent=0,int index=0,QString name="node",int cmb_index=0,QString text="string");
    void DelTreeNode();

signals:
    void sgn_send(QTreeWidgetItem *twi);


private slots:
    void slt_tv_Source_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void slt_send();

//    void slt_btn_Add_clicked();

//    void slt_btn_AddRoot_clicked();

//    void slt_btn_Del_clicked();

//    void slt_btn_Modify_clicked();
    //void slt_btn_Load_clicked();


private:
    QTreeWidget *treeW;


};

#endif // TREEVIEW_H
