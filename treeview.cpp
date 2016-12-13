#include "treeview.h"
#include <QDebug>

TreeView::TreeView(QWidget *parent,QTreeWidget *tw) :
    QWidget(parent)
{
    //domDocument=NULL;
    treeW=tw;
    treeW->setColumnCount(4);
    treeW->setHeaderLabels(QStringList()<<tr("发送")<<tr("名称")<<tr("格式")<<tr("内容"));

    //connect(treeW,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slt_send(QTreeWidgetItem*,int)));
}

TreeView::~TreeView()
{
}

QTreeWidgetItem * TreeView::AddTreeRoot(QString name)
{
    QTreeWidgetItem * newitem=new QTreeWidgetItem();
    treeW->addTopLevelItem(newitem);
    //treeW->insertTopLevelItem(index,item);
    treeW->setItemWidget(newitem,0,new MyLineEdit(treeW,newitem,name));

    qDebug()<<"add a root";
    return newitem;
}

QTreeWidgetItem * TreeView::InsertTreeRoot(int index,QString name)
{
    int groupcnt=treeW->children().length();
    if(index>groupcnt)
    {
        index=groupcnt;
    }
    QTreeWidgetItem * newitem=new QTreeWidgetItem();
    //treeW->addTopLevelItem(newitem);
    treeW->insertTopLevelItem(index,newitem);
    treeW->setItemWidget(newitem,0,new MyLineEdit(treeW,newitem,name));

    qDebug()<<"add a root";
    return newitem;
}

QTreeWidgetItem * TreeView::NewTreeRoot(QString name)
{
    QTreeWidgetItem * newitem=new QTreeWidgetItem();


    QTreeWidgetItem *citem=treeW->currentItem();

    if(citem!=Q_NULLPTR)                //有节点被选中
    {
        if(citem->parent()!=Q_NULLPTR)  //被选中的是子节点
        {
            //在当前节点所在组下方添加组
            int groupnum=treeW->indexOfTopLevelItem(citem->parent());       //获取当前组的索引
            qDebug()<<"groupnum="<<groupnum;
            treeW->insertTopLevelItem(groupnum+1,newitem);
        }
        else                            //被选中的是根节点
        {
            //直接在当前组下方添加组
            int groupnum=treeW->indexOfTopLevelItem(citem);       //获取当前组的索引
            qDebug()<<"groupnum="<<groupnum;
            treeW->insertTopLevelItem(groupnum+1,newitem);
        }
    }
    else                                //没有节点被选中
    {
        qDebug()<<"add groupnum";
        treeW->addTopLevelItem(newitem);
    }
    treeW->setItemWidget(newitem,0,new MyLineEdit(treeW,newitem,name));
    return newitem;
}

QTreeWidgetItem * TreeView::AddTreeNode(QTreeWidgetItem *parent,QString name,int cmb_index,QString text)
{
    int nodecnt=parent->childCount();
//    QTreeWidgetItem * item=new QTreeWidgetItem();
//    SendButton *btn=new SendButton(treeW,item,"S");
//    QLineEdit *letn=new QLineEdit(name);
//    QComboBox *cmb=new QComboBox();
//    cmb->addItem("Hex");
//    cmb->addItem("String");
//    cmb->addItem("File");
//    cmb->setCurrentIndex(cmb_index);
//    QLineEdit *lett=new QLineEdit(text);
    QTreeWidgetItem * newitem=new QTreeWidgetItem();
    SendButton *btn=new SendButton(treeW,newitem,"S");
    MyLineEdit *letn=new MyLineEdit(treeW,newitem,name);
    MyComboBox *cmb=new MyComboBox(treeW,newitem);
    cmb->addItem("Hex");
    cmb->addItem("String");
    cmb->addItem("File");
    cmb->setCurrentIndex(cmb_index);
    MyLineEdit *lett=new MyLineEdit(treeW,newitem,text);


    parent->addChild(newitem);
    //parent->insertChild(index,newitem);

    //item->setText(0,QString::number(index));
    treeW->setItemWidget(newitem,0,btn);
    treeW->setItemWidget(newitem,1,letn);
    treeW->setItemWidget(newitem,2,cmb);
    treeW->setItemWidget(newitem,3,lett);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(slt_send()));
    //connect(letn,SIGNAL())
    qDebug()<<"add a node";
    return newitem;
}

QTreeWidgetItem * TreeView::InsertTreeNode(QTreeWidgetItem *parent,int index,QString name,int cmb_index,QString text)
{
    int nodecnt=parent->childCount();
    if(index>nodecnt)
    {
        index=nodecnt;
    }

    QTreeWidgetItem * newitem=new QTreeWidgetItem();
    SendButton *btn=new SendButton(treeW,newitem,"S");
    MyLineEdit *letn=new MyLineEdit(treeW,newitem,name);
    MyComboBox *cmb=new MyComboBox(treeW,newitem);
    cmb->addItem("Hex");
    cmb->addItem("String");
    cmb->addItem("File");
    cmb->setCurrentIndex(cmb_index);
    MyLineEdit *lett=new MyLineEdit(treeW,newitem,text);


    //parent->addChild(newitem);
    parent->insertChild(index,newitem);

    //item->setText(0,QString::number(index));
    treeW->setItemWidget(newitem,0,btn);
    treeW->setItemWidget(newitem,1,letn);
    treeW->setItemWidget(newitem,2,cmb);
    treeW->setItemWidget(newitem,3,lett);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(slt_send()));
    qDebug()<<"add a node";
    return newitem;
}

QTreeWidgetItem * TreeView::NewTreeNode(QString name,int cmb_index,QString text)
{
    QTreeWidgetItem * newitem=new QTreeWidgetItem();

    QTreeWidgetItem *citem=treeW->currentItem();
    int groupnum;
    int nodenum;

    if(citem!=Q_NULLPTR)                //有节点被选中
    {
        if(citem->parent()!=Q_NULLPTR)  //被选中的是子节点
        {
            //在当前子节点下方添加子节点
            int nodeindex=citem->parent()->indexOfChild(citem);     //当前子节点索引
            qDebug()<<"nodeindex="<<nodeindex;
            citem->parent()->insertChild(nodeindex+1,newitem);         //在当前节点后面添加
        }
        else                            //被选中的是根节点
        {
            //直接在当前组最后添加子节点
            citem->addChild(newitem);
        }
    }
    else                                //最后组的最后添加子节点
    {
        groupnum=treeW->topLevelItemCount();
        nodenum=treeW->topLevelItem(groupnum-1)->childCount();
        if(groupnum>0)
        {
            qDebug()<<"groupnum="<<groupnum;
            treeW->topLevelItem(groupnum-1)->addChild(newitem);
        }
        else
        {
            qDebug()<<"No a group";
        }
    }


//    SendButton *btn=new SendButton(treeW,newitem,"S");
//    QLineEdit *letn=new QLineEdit(name);
//    QComboBox *cmb=new QComboBox();
//    cmb->addItem(tr("十六进制"));
//    cmb->addItem(tr("字符串"));
//    cmb->addItem(tr("文件"));
//    cmb->setCurrentIndex(cmb_index);
//    QLineEdit *lett=new QLineEdit(text);
    SendButton *btn=new SendButton(treeW,newitem,"S");
    MyLineEdit *letn=new MyLineEdit(treeW,newitem,name);
    MyComboBox *cmb=new MyComboBox(treeW,newitem);
    cmb->addItem("Hex");
    cmb->addItem("String");
    cmb->addItem("File");
    cmb->setCurrentIndex(cmb_index);
    MyLineEdit *lett=new MyLineEdit(treeW,newitem,text);

    treeW->setItemWidget(newitem,0,btn);
    treeW->setItemWidget(newitem,1,letn);
    treeW->setItemWidget(newitem,2,cmb);
    treeW->setItemWidget(newitem,3,lett);

    connect(btn,SIGNAL(clicked(bool)),this,SLOT(slt_send()));
    return newitem;
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



void TreeView::slt_send()
{
    SendButton *tempSButton=(SendButton *)sender();
    QTreeWidgetItem * currentItem = tempSButton->getTwi();
    QLineEdit *tempNLet=(QLineEdit *)treeW->itemWidget(currentItem,1);
    QComboBox *tempTCmb=(QComboBox *)treeW->itemWidget(currentItem,2);
    QLineEdit *tempDLet=(QLineEdit *)treeW->itemWidget(currentItem,3);
    qDebug()<<tempSButton->text()<<" "<<tempNLet->text()<<" "<<tempTCmb->currentText()<<"" <<tempDLet->text();

    sgn_send(currentItem);
    //qDebug()<<((QPushButton *)(treeW->itemWidget(currentItem,0)))->text();
}
