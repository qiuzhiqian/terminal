#include "terminal.h"
#include "ui_terminal.h"
#include <QDebug>

Terminal::Terminal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Terminal)
{
    ui->setupUi(this);

    treeWidget=NULL;
    tv=NULL;
    xmlfile=NULL;

    int threeLength=75;

    menu_file=new QMenu();
    act_creat=new QAction(tr("新建"));
    act_open=new QAction(tr("打开"));
    act_save=new QAction(tr("保存"));
    act_close=new QAction(tr("关闭"));
    act_exit=new QAction(tr("退出"));

    menu_file->addAction(act_creat);
    menu_file->addAction(act_open);
    menu_file->addAction(act_save);
    menu_file->addAction(act_close);
    menu_file->addAction(act_exit);

    ui->btn_file->setMenu(menu_file);

    connect(act_creat,SIGNAL(triggered(bool)),this,SLOT(slt_xml_new()));
    connect(act_open,SIGNAL(triggered(bool)),this,SLOT(slt_xml_open()));
    connect(act_save,SIGNAL(triggered(bool)),this,SLOT(slt_xml_save()));
    connect(act_close,SIGNAL(triggered(bool)),this,SLOT(slt_xml_close()));
    connect(act_exit,SIGNAL(triggered(bool)),this,SLOT(slt_xml_exit()));


    toolWidget=new QWidget();
    toolLayout=new QGridLayout();
    toolWidget->setLayout(toolLayout);
    toolWidget->setStyleSheet("background-color:rgb(200,200,200);");   //rgb(55,100,255)

    btn_open=new QPushButton(tr("打开"));
    btn_open->setStyleSheet("background-color:rgb(36,240,67);");


    lb_com=new QLabel(tr("串口号"));
    lb_bdrate=new QLabel(tr("波特率"));
    lb_databit=new QLabel(tr("数据位"));
    lb_parity=new QLabel(tr("校验位"));
    lb_stopbit=new QLabel(tr("停止位"));
    lb_flowctrl=new QLabel(tr("流控制"));

    cbx_com=new QComboBox();
    cbx_bdrate=new QComboBox();
    cbx_databit=new QComboBox();
    cbx_parity=new QComboBox();
    cbx_stopbit=new QComboBox();
    cbx_flowctrl=new QComboBox();


    cbx_bdrate->addItems(QStringList() <<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200"<<"128000"<<"256000");
    cbx_bdrate->setCurrentText("115200");
    cbx_databit->addItems(QStringList()<<tr("5位")<<tr("6位")<<tr("7位")<<tr("8位"));
    cbx_parity->addItems(QStringList()<<tr("无校验")<<tr("奇校验")<<tr("偶校验"));
    cbx_stopbit->addItems(QStringList()<<tr("1位")<<tr("1.5位")<<tr("2位"));

    btn_hexwatch=new QPushButton("HEX");
    btn_recvclear=new QPushButton(tr("清除"));
    btn_wintop=new QPushButton(tr("置顶"));

    btn_treeitemadd=new QPushButton(tr("添加"));
    menu_treeadd=new QMenu();
    act_addgroup=new QAction(tr("添加组"));
    act_addnode=new QAction(tr("添加项"));
    menu_treeadd->addAction(act_addgroup);
    menu_treeadd->addAction(act_addnode);
    btn_treeitemadd->setMenu(menu_treeadd);

    toolLayout->addWidget(btn_open,0,0,3,1);
    btn_open->setMinimumHeight(threeLength);     //设置打开按键的大小

    toolLayout->addWidget(lb_com,0,1);
    toolLayout->addWidget(lb_bdrate,1,1);
    toolLayout->addWidget(lb_databit,2,1);

    toolLayout->addWidget(cbx_com,0,2);
    toolLayout->addWidget(cbx_bdrate,1,2);
    toolLayout->addWidget(cbx_databit,2,2);

    toolLayout->addWidget(lb_parity,0,3);
    toolLayout->addWidget(lb_stopbit,1,3);
    toolLayout->addWidget(lb_flowctrl,2,3);

    toolLayout->addWidget(cbx_parity,0,4);
    toolLayout->addWidget(cbx_stopbit,1,4);
    toolLayout->addWidget(cbx_flowctrl,2,4);

    toolLayout->addWidget(btn_hexwatch,0,5,3,1);
    btn_hexwatch->setMinimumHeight(threeLength);     //设置打开按键的大小

    toolLayout->addWidget(btn_recvclear,0,6);
    toolLayout->addWidget(btn_wintop,1,6);

    toolLayout->addWidget(btn_treeitemadd,0,7,3,1);
    btn_treeitemadd->setMinimumHeight(threeLength);     //设置打开按键的大小

    QHBoxLayout * hb = new QHBoxLayout();
    hb->addStretch();
    toolLayout->addLayout(hb,0,8);

    ui->hl_tool->addWidget(toolWidget);
    toolWidget->setVisible(false);

    connect(ui->btn_tool,SIGNAL(clicked(bool)),this,SLOT(slt_toolExtand()));

    spt_sendTree = new QSplitter(Qt::Horizontal, 0); //新建主分割窗口，水平分割
    let_Rec=new QTextEdit();
    spt_sendTree->addWidget(let_Rec);

    ui->hl_down->addWidget(spt_sendTree);

    connect(act_addgroup,SIGNAL(triggered(bool)),this,SLOT(slt_xml_addGroup()));
    connect(act_addnode,SIGNAL(triggered(bool)),this,SLOT(slt_xml_addNode()));
}

Terminal::~Terminal()
{
    delete ui;
}

bool Terminal::xml_read(QIODevice *device)
{
    QString errorStr;
    int errorLine;
    int errorColumn;
    int groupcnt=0;

    QDomDocument domdoc;

    if (!domdoc.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug("Parse error at line %d, column %d:\n%s",errorLine,errorColumn,errorStr);
        return false;
    }

    QDomElement root = domdoc.documentElement();
    if (root.tagName() != "BSSignalRegistry") {
        qDebug("The file is not an BSP file.");
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        qDebug("The file is not an XBEL version 1.0");
        return false;
    }

    //clear();

    QDomElement child = root.firstChildElement("Type"); //自定义类型--保留
    qDebug() << "Root name " << child.text();
    child = child.nextSiblingElement("Group");;
    while (!child.isNull()) {
        qDebug()<<child.attributeNode("Name").value();
        QTreeWidgetItem *gp=tv->AddTreeRoot(child.attributeNode("Name").value());

        QDomNodeList list = child.childNodes();
        for(int i = 0;i < list.count();i++)
        {
            QDomNode nodechild = list.at(i);
            if(nodechild.isElement())
            {
                int cmb_index=0;
                QDomElement nameele=nodechild.firstChildElement("Name");
                QDomElement typeele=nodechild.firstChildElement("DataFormat");
                QDomElement dataele=nodechild.firstChildElement("Data");

                if(typeele.text()=="Hex")   cmb_index=0;
                else if(typeele.text()=="String")   cmb_index=1;
                else if(typeele.text()=="File")   cmb_index=2;
                else    cmb_index=0;

                tv->AddTreeNode(gp,nameele.text(),cmb_index,dataele.text());
                qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
            }
        }

        child = child.nextSiblingElement("Group");
    }

    return true;
}

void Terminal::xml_write(QIODevice *device)
{
    QDomDocument domdoc;

    QString strHeader( "version=\"1.0\"" );
    domdoc.appendChild( domdoc.createProcessingInstruction("xml", strHeader) );

    QDomElement	root_elem = domdoc.createElement( "BSSignalRegistry" );
    root_elem.setAttribute( "version", "1.0" );
    domdoc.appendChild( root_elem );

    QDomElement itemName = domdoc.createElement( "Type" );
    root_elem.appendChild( itemName );

    QDomText itemText= domdoc.createTextNode("BSPFile");
    itemName.appendChild(itemText);

    int groupcnt=treeWidget->topLevelItemCount();
    for(int i=0;i<groupcnt;i++)                         //遍历组
    {
        QTreeWidgetItem *groupitem=treeWidget->topLevelItem(i);
        QLineEdit *groupedit=(QLineEdit *)(treeWidget->itemWidget(groupitem,0));       //获取组元素

        //添加dom组
        QDomElement itemGroup = domdoc.createElement( "Group" );
        itemGroup.setAttribute( "Name", groupedit->text());
        root_elem.appendChild( itemGroup );

        int nodecnt=groupitem->childCount();
        for(int j=0;j<nodecnt;j++)                      //遍历节点
        {
            QTreeWidgetItem *nodeitem=groupitem->child(j);

            SendButton *sbtn=(SendButton *)(treeWidget->itemWidget(nodeitem,0));        //获取节点元素
            QLineEdit *nameedit=(QLineEdit *)(treeWidget->itemWidget(nodeitem,1));
            QComboBox *typebox=(QComboBox *)(treeWidget->itemWidget(nodeitem,2));
            QLineEdit *textedit=(QLineEdit *)(treeWidget->itemWidget(nodeitem,3));

            //添加dom节点
            QDomElement itemNode = domdoc.createElement( "BSSignal" );        //向groupDom中添加Node
            itemGroup.appendChild( itemNode );

            //向itemNode中添加元素
            QDomElement itemGBName = domdoc.createElement( "Name" );
            itemText= domdoc.createTextNode(nameedit->text());
            itemGBName.appendChild(itemText);
            itemNode.appendChild( itemGBName );

            QDomElement itemGBType = domdoc.createElement( "DataFormat" );
            itemText= domdoc.createTextNode(typebox->currentText());
            itemGBType.appendChild(itemText);
            itemNode.appendChild( itemGBType );

            QDomElement itemGBData = domdoc.createElement( "Data" );
            itemText= domdoc.createTextNode(textedit->text());
            itemGBData.appendChild(itemText);
            itemNode.appendChild( itemGBData );
        }
    }

    //遍历完成，导出dom
    QTextStream out( device );
    domdoc.save( out, 4 );
}

void Terminal::slt_xml_new()
{
    if(treeWidget!=NULL)
    {
        delete treeWidget;
    }
    treeWidget=new QTreeWidget();
    spt_sendTree->addWidget(treeWidget);


    if(tv!=NULL)
    {
        delete tv;
    }
    tv=new TreeView(this,treeWidget);
}

void Terminal::slt_xml_open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open BSP File"),QDir::currentPath(),tr("XML Files (*.xml *.xbel)"));
    if (fileName.isEmpty())
        return;

    xmlfile=new QFile(fileName);
    if (!xmlfile->open(QFile::ReadOnly | QFile::Text)) {
        qDebug("Cannot read file %s:\n%s.",fileName,xmlfile->errorString());
        return;
    }

    if(treeWidget!=NULL)
    {
        delete treeWidget;
    }
    treeWidget=new QTreeWidget();
    spt_sendTree->addWidget(treeWidget);

    if(tv!=NULL)
    {
        delete tv;
    }
    tv=new TreeView(this,treeWidget);

    xml_read(xmlfile);
    xmlfile->close();
}

void Terminal::slt_xml_save()
{
    if(xmlfile==NULL)
    {
        QString filename = QFileDialog::getSaveFileName( this, tr("Save BSP File"), "", tr("XML Files (*.xml *.xbel)"));
        if (filename.isEmpty())
            return;

        xmlfile=new QFile( filename );
    }

    if( !xmlfile->open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        return;
    }

    xml_write(xmlfile);

    xmlfile->close();
}

void Terminal::slt_xml_close()
{

}

void Terminal::slt_xml_exit()
{

}

void Terminal::slt_xml_addGroup()
{
    if(treeWidget==NULL)    return;
    if(tv==NULL)    return;

    tv->NewTreeRoot("GROUP");
}

void Terminal::slt_xml_addNode()
{
    if(treeWidget==NULL)    return;
    if(tv==NULL)    return;

    if(treeWidget->topLevelItemCount()==0)  return; //没有根节点，无法添加子节点
    tv->NewTreeNode("Name",0,"TEXT");
}

void Terminal::slt_xml_del()
{

}

void Terminal::slt_toolExtand()
{
    if(ui->btn_tool->text()==">")
    {
        ui->btn_tool->setText("<");

        toolWidget->setVisible(true);
    }
    else
    {
        ui->btn_tool->setText(">");

        toolWidget->setVisible(false);

    }
}
