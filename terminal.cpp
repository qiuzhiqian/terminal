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

    int threeLength=75;

    menu_file=new QMenu();
    act_creat=new QAction(tr("新建"));
    act_open=new QAction(tr("打开"));
    act_close=new QAction(tr("关闭"));
    act_exit=new QAction(tr("退出"));

    menu_file->addAction(act_creat);
    menu_file->addAction(act_open);
    menu_file->addAction(act_close);
    menu_file->addAction(act_exit);
    //menu_file->addAction(act_creat);

    ui->btn_file->setMenu(menu_file);

    connect(act_creat,SIGNAL(triggered(bool)),this,SLOT(slt_xml_new()));
    connect(act_open,SIGNAL(triggered(bool)),this,SLOT(slt_xml_open()));
    connect(act_close,SIGNAL(triggered(bool)),this,SLOT(slt_xml_save()));
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

    domDocument=new QDomDocument();

    if (!domDocument->setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        qDebug("Parse error at line %d, column %d:\n%s",errorLine,errorColumn,errorStr);
        return false;
    }

    QDomElement root = domDocument->documentElement();
    if (root.tagName() != "BSSignalRegistry") {
        qDebug("The file is not an BSP file.");
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        qDebug("The file is not an XBEL version 1.0");
        return false;
    }

    //clear();

    QDomElement child = root.firstChildElement("Name");
    qDebug() << "Root name " << child.text();
    child = child.nextSiblingElement("Group");;
    while (!child.isNull()) {
        qDebug()<<child.attributeNode("Name").value();
        QTreeWidgetItem *gp=tv->AddTreeRoot(groupcnt++,child.attributeNode("Name").value());

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

                if(typeele.text()==tr("Hex"))   cmb_index=0;
                else if(typeele.text()==tr("String"))   cmb_index=1;
                else if(typeele.text()==tr("File"))   cmb_index=2;
                else    cmb_index=0;

                tv->AddTreeNode(gp,i,nameele.text(),cmb_index,dataele.text());
                qDebug() << "    " << qPrintable(nodechild.toElement().tagName()) << qPrintable(nodechild.toElement().text());
            }
        }

        child = child.nextSiblingElement("Group");
    }

    return true;
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
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open BSP File"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xbel *.xml)"));
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
}

void Terminal::slt_xml_save()
{
    if(xmlfile==NULL)
    {
        QString filename = QFileDialog::getSaveFileName( this, "Save", "", "*.xml" );
        xmlfile=new QFile( filename );

        if( !xmlfile->open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            return;
        }
    }

    QDomDocument document;

    QString strHeader( "version=\"1.0\"" );
    document.appendChild( document.createProcessingInstruction("xml", strHeader) );

    QDomElement	root_elem = document.createElement( "BSSignalRegistry" );
    root_elem.setAttribute( "version", "1.0" );
    document.appendChild( root_elem );

    QDomElement itemName = document.createElement( "Name" );
    root_elem.appendChild( itemName );

    QDomText itemText= document.createTextNode("BSPFile");
    itemName.appendChild(itemText);

    for(int i=0;i<2;i++)
    {
        QDomElement itemGroup1 = document.createElement( "Group" );
        itemGroup1.setAttribute( "Name", "AT" );
        root_elem.appendChild( itemGroup1 );

        for(int j=0;j<3;j++)
        {
            QDomElement itemG1BSS1 = document.createElement( "BSSignal" );
            itemGroup1.appendChild( itemG1BSS1 );

            QDomElement itemGBName = document.createElement( "Name" );
            itemText= document.createTextNode("AT Test");
            itemGBName.appendChild(itemText);
            itemG1BSS1.appendChild( itemGBName );

            QDomElement itemGBType = document.createElement( "DataFormate" );
            itemText= document.createTextNode("String");
            itemGBType.appendChild(itemText);
            itemG1BSS1.appendChild( itemGBType );

            QDomElement itemGBData = document.createElement( "Data" );
            itemText= document.createTextNode("ATI\\r\\n");
            itemGBData.appendChild(itemText);
            itemG1BSS1.appendChild( itemGBData );
        }
    }

    QTextStream out( xmlfile );
    document.save( out, 4 );
    xmlfile->close();
}

void Terminal::slt_xml_exit()
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
