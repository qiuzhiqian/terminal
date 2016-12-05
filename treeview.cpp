#include "treeview.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent,QTreeWidget *tw) :
    QWidget(parent)
{
    treeW=tw;
    treeW->setColumnCount(4);
    treeW->setHeaderLabels(QStringList()<<tr("发送")<<tr("名称")<<tr("格式")<<tr("内容"));

    //connect(treeW,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slt_send(QTreeWidgetItem*,int)));
}

TreeView::~TreeView()
{
}

QTreeWidgetItem * TreeView::AddTreeRoot(int index,QString name)
{
    QTreeWidgetItem * item=new QTreeWidgetItem();
    //treeW->addTopLevelItem(item);
    treeW->insertTopLevelItem(index,item);
    treeW->setItemWidget(item,0,new QLineEdit(name));

    qDebug()<<"add a root";
    return item;
}

QTreeWidgetItem * TreeView::AddTreeNode(QTreeWidgetItem *parent,int index,QString name,int cmb_index,QString text)
{
    QTreeWidgetItem * item=new QTreeWidgetItem();
    SendButton *btn=new SendButton(item,QString::number(index));
    QLineEdit *letn=new QLineEdit(name);
    QComboBox *cmb=new QComboBox();
    cmb->addItem(tr("十六进制"));
    cmb->addItem(tr("字符串"));
    cmb->addItem(tr("文件"));
    cmb->setCurrentIndex(cmb_index);
    QLineEdit *lett=new QLineEdit(text);


    //parent->addChild(item);
    parent->insertChild(index,item);

    //item->setText(0,QString::number(index));
    treeW->setItemWidget(item,0,btn);
    treeW->setItemWidget(item,1,letn);
    treeW->setItemWidget(item,2,cmb);
    treeW->setItemWidget(item,3,lett);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(slt_send()));
    qDebug()<<"add a node";
    return item;
}

void TreeView::DelTreeNode()
{
    QTreeWidgetItem * currentItem = treeW->currentItem();

    if(currentItem==Q_NULLPTR)
    {
        return;
    }
    //如果没有父节点就直接删除
    if(currentItem->parent()==Q_NULLPTR)
    {
        delete treeW->takeTopLevelItem(treeW->currentIndex().row());
    }
    else
    {
        //如果有父节点就要用父节点的takeChild删除节点
        delete currentItem->parent()->takeChild(treeW->currentIndex().row());
    }
}

void TreeView::slt_tv_Source_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current==Q_NULLPTR)return;
    if(previous!=Q_NULLPTR)
    {
        previous->setBackground(0,Qt::transparent);
        previous->setBackground(1,Qt::transparent);
        previous->setTextColor(0,Qt::black);
        previous->setTextColor(1,Qt::black);
    }
    current->setTextColor(0,Qt::blue);
    current->setTextColor(1,Qt::blue);
    current->setBackground(0,Qt::red);
    current->setBackground(1,Qt::red);
}

//void TreeView::slt_btn_Add_clicked()
//{
//    QTreeWidgetItem * item= ui->tv_Source->currentItem();
//    if(item!=Q_NULLPTR)
//    {
//        AddTreeNode(ui->tv_Source,item,"new","new");
//    }
//    else
//    {
//        AddTreeRoot(ui->tv_Source,"new","new");
//    }
//    qDebug()<<"add a node";
//}

//void TreeView::slt_btn_AddRoot_clicked()
//{
//    AddTreeRoot(ui->tv_Source,"new","new");
//    qDebug()<<"add a root";
//}

//void TreeView::slt_btn_Del_clicked()
//{
//    DelTreeNode(ui->tv_Source);
//}

//void TreeView::slt_btn_Modify_clicked()
//{
//    QTreeWidgetItem * currentItem = ui->tv_Source->currentItem();

//    if(currentItem==Q_NULLPTR)
//    {
//        return;
//    }
//    for(int i=0;i<currentItem->columnCount();i++)
//    {
//        currentItem->setText(i,tr("Modify")+QString::number(i));
//    }
//}

//void TreeView::slt_btn_Load_clicked()
//{
//    QString fileName =
//            QFileDialog::getOpenFileName(this, tr("Open BSP File"),
//                                         QDir::currentPath(),
//                                         tr("XML Files (*.xbel *.xml)"));
//    if (fileName.isEmpty())
//        return;

//    xmlfile=new QFile(fileName);
//    if (!xmlfile->open(QFile::ReadOnly | QFile::Text)) {
//        qDebug("Cannot read file %s:\n%s.",fileName,xmlfile->errorString());
//        return;
//    }
//}

void TreeView::slt_send()
{
    SendButton *tempSButton=(SendButton *)sender();
    QTreeWidgetItem * currentItem = tempSButton->getTwi();
    QLineEdit *tempNLet=(QLineEdit *)treeW->itemWidget(currentItem,1);
    QComboBox *tempTCmb=(QComboBox *)treeW->itemWidget(currentItem,2);
    QLineEdit *tempDLet=(QLineEdit *)treeW->itemWidget(currentItem,3);
    qDebug()<<tempSButton->text()<<" "<<tempNLet->text()<<" "<<tempTCmb->currentText()<<"" <<tempDLet->text();
    //qDebug()<<sender()->parent()->metaObject()->className();
    //if(index==0)
//    {
//        qDebug()<<twi->text(index);
//    }
    sgn_send(currentItem);
    //qDebug()<<((QPushButton *)(treeW->itemWidget(currentItem,0)))->text();
}
