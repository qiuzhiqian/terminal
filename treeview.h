#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QWidget>
#include<QTreeWidgetItem>
#include <QDomDocument>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include "sendbutton.h"
#include "mylineedit.h"
#include "mycombobox.h"


class TreeView : public QWidget
{
    Q_OBJECT

public:
    explicit TreeView(QWidget *parent = 0,QTreeWidget *tw=0);
    ~TreeView();

    QTreeWidgetItem * AddTreeRoot(QString name="GROUP");
    QTreeWidgetItem * InsertTreeRoot(int index=0,QString name="GROUP");
    QTreeWidgetItem * NewTreeRoot(QString name="GROUP");
    QTreeWidgetItem * AddTreeNode(QTreeWidgetItem *parent=0,QString name="node",int cmb_index=0,QString text="string");
    QTreeWidgetItem * InsertTreeNode(QTreeWidgetItem *parent=0,int index=0,QString name="node",int cmb_index=0,QString text="string");
    QTreeWidgetItem * NewTreeNode(QString name="node",int cmb_index=0,QString text="string");
    void DelTreeNode();

signals:
    void sgn_send(QTreeWidgetItem *twi);


private slots:
    void slt_tv_Source_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void slt_send();

private:
    QTreeWidget *treeW;


};

#endif // TREEVIEW_H
