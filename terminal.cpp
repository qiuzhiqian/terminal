#include "terminal.h"
#include "ui_terminal.h"
#include <QDebug>
#include <QMetaEnum>

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

    ProjectMenu=new QMenu(tr("工程"));
    act_project_creat=new QAction(tr("新建"));
    act_project_open=new QAction(tr("打开"));
    act_project_save=new QAction(tr("保存"));
    act_project_close=new QAction(tr("关闭"));
    ProjectMenu->addAction(act_project_creat);
    ProjectMenu->addAction(act_project_open);
    ProjectMenu->addAction(act_project_save);
    ProjectMenu->addAction(act_project_close);

    SetMenu=new QMenu(tr("设置"));

    FTPMenu=new QMenu(tr("文件传输"));
    XModemMenu=new QMenu(tr("XModem"));
    act_ftp_xsend=new QAction(tr("发送"));
    act_ftp_xreceive=new QAction(tr("接收"));
    XModemMenu->addAction(act_ftp_xsend);
    XModemMenu->addAction(act_ftp_xreceive);
    YModemMenu=new QMenu(tr("YModem"));
    act_ftp_ysend=new QAction(tr("发送"));
    act_ftp_yreceive=new QAction(tr("接收"));
    YModemMenu->addAction(act_ftp_ysend);
    YModemMenu->addAction(act_ftp_yreceive);
    ZModemMenu=new QMenu(tr("ZModem"));
    act_ftp_zsend=new QAction(tr("发送"));
    act_ftp_zreceive=new QAction(tr("接收"));
    ZModemMenu->addAction(act_ftp_zsend);
    ZModemMenu->addAction(act_ftp_zreceive);
    FTPMenu->addMenu(XModemMenu);
    FTPMenu->addMenu(YModemMenu);
    FTPMenu->addMenu(ZModemMenu);

    act_exit=new QAction(tr("退出"));

    menu_file->addMenu(ProjectMenu);
    menu_file->addMenu(SetMenu);
    menu_file->addMenu(FTPMenu);
    menu_file->addAction(act_exit);

    ui->btn_file->setMenu(menu_file);

    connect(act_project_creat,SIGNAL(triggered(bool)),this,SLOT(slt_xml_new()));
    connect(act_project_open,SIGNAL(triggered(bool)),this,SLOT(slt_xml_open()));
    connect(act_project_save,SIGNAL(triggered(bool)),this,SLOT(slt_xml_save()));
    connect(act_project_close,SIGNAL(triggered(bool)),this,SLOT(slt_xml_close()));

    connect(act_ftp_ysend,SIGNAL(triggered(bool)),this,SLOT(slt_ftp_start()));

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
    cbx_databit->setCurrentText(tr("8位"));
    cbx_parity->addItems(QStringList()<<tr("无校验")<<tr("奇校验")<<tr("偶校验"));
    cbx_stopbit->addItems(QStringList()<<tr("1位")<<tr("1.5位")<<tr("2位"));
    cbx_flowctrl->addItems(QStringList()<<tr("无流控")<<tr("硬件流")<<tr("软件流"));

    btn_hexwatch=new QPushButton("HEX");
    hexEnable=false;
    btn_hexwatch->setStyleSheet("background-color:rgb(36,240,67);");
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
    tet_Rec=new QTextEdit();
    tet_Rec->setReadOnly(true);
    spt_sendTree->addWidget(tet_Rec);

    ui->hl_down->addWidget(spt_sendTree);

    connect(act_addgroup,SIGNAL(triggered(bool)),this,SLOT(slt_xml_addGroup()));
    connect(act_addnode,SIGNAL(triggered(bool)),this,SLOT(slt_xml_addNode()));

    //串口部分初始化
    QListView *view = new QListView(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *item;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name         : " << info.portName();
        qDebug() << "Description  : " << info.description();
        qDebug() << "Manufacturer : " << info.manufacturer();
        qDebug() << "Serial number: " << info.serialNumber();

        item = new QStandardItem(info.portName());
        QString comtips="Name:"+info.portName()+"\r\n"+
                "Description:"+info.description()+"\r\n"+
                "Manufacturer:"+info.manufacturer()+"\r\n"+
                "Serial number:"+info.serialNumber();
        item->setToolTip(comtips);                                  //设置提示信息
        model->appendRow(item);
        //cbx_com->addItem(info.portName());
    }
    cbx_com->setView(view);
    cbx_com->setModel(model);

    cbx_com->setCurrentIndex(0);

    my_port=new QSerialPort();

    connect(btn_open,SIGNAL(clicked(bool)),this,SLOT(slt_com_open()));
    connect(btn_recvclear,SIGNAL(clicked(bool)),this,SLOT(slt_com_recClr()));
    connect(btn_hexwatch,SIGNAL(clicked(bool)),this,SLOT(slt_com_hexWatch()));
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
    if (root.tagName() != "TMSignalRegistry") {
        qDebug("The file is not an TMP file.");
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
                qDebug() << "    " << qPrintable(nameele.toElement().tagName()) << qPrintable(nameele.toElement().text());
                qDebug() << "    " << qPrintable(typeele.toElement().tagName()) << qPrintable(typeele.toElement().text());
                qDebug() << "    " << qPrintable(dataele.toElement().tagName()) << qPrintable(dataele.toElement().text());
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

    QDomElement	root_elem = domdoc.createElement( "TMSignalRegistry" );
    root_elem.setAttribute( "version", "1.0" );
    domdoc.appendChild( root_elem );

    QDomElement itemName = domdoc.createElement( "Type" );
    root_elem.appendChild( itemName );

    QDomText itemText= domdoc.createTextNode("TMPFile");
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
            QDomElement itemNode = domdoc.createElement( "TMSignal" );        //向groupDom中添加Node
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
        treeWidget=NULL;
    }
    treeWidget=new QTreeWidget();
    spt_sendTree->addWidget(treeWidget);


    if(tv!=NULL)
    {
        delete tv;
        tv=NULL;
        disconnect(tv,SIGNAL(sgn_send(QTreeWidgetItem*)),this,SLOT(slt_com_senddata(QTreeWidgetItem*)));
    }
    tv=new TreeView(this,treeWidget);
    connect(tv,SIGNAL(sgn_send(QTreeWidgetItem*)),this,SLOT(slt_com_senddata(QTreeWidgetItem*)));
}

void Terminal::slt_xml_open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open TMP File"),QDir::currentPath(),tr("XML Files (*.xml *.xbel)"));
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
        treeWidget=NULL;
    }
    treeWidget=new QTreeWidget();
    spt_sendTree->addWidget(treeWidget);

    if(tv!=NULL)
    {
        delete tv;
        tv=NULL;
        disconnect(tv,SIGNAL(sgn_send(QTreeWidgetItem*)),this,SLOT(slt_com_senddata(QTreeWidgetItem*)));
    }
    tv=new TreeView(this,treeWidget);
    connect(tv,SIGNAL(sgn_send(QTreeWidgetItem*)),this,SLOT(slt_com_senddata(QTreeWidgetItem*)));

    xml_read(xmlfile);
    xmlfile->close();
}

void Terminal::slt_xml_save()
{
    if(xmlfile==NULL)
    {
        QString filename = QFileDialog::getSaveFileName( this, tr("Save TMP File"), "", tr("XML Files (*.xml *.xbel)"));
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
    if(treeWidget==NULL)    return;
    if(tv==NULL)    return;

    bool chgflag=tv->getChangedFlag();
    if(chgflag==true)
    {                       //此处设置一个QMassgeBox
        QMessageBox *msgBox=new QMessageBox(this);
        msgBox->setText(tr("The document has been modified."));
        msgBox->setInformativeText(tr("Do you want to save your changes?"));
        //msgBox->setDetailedText(tr("Differences here..."));
        msgBox->setStandardButtons(QMessageBox::Save
                                  | QMessageBox::Cancel);
        msgBox->setDefaultButton(QMessageBox::Save);
        int ret = msgBox->exec();
        switch (ret) {
        case QMessageBox::Save:
            qDebug() << "Save document!";
            slt_xml_save();
            break;
        case QMessageBox::Cancel:
            qDebug() << "Close document!";
            break;
        }
    }

    delete tv;
    tv=NULL;
    disconnect(tv,SIGNAL(sgn_send(QTreeWidgetItem*)),this,SLOT(slt_com_senddata(QTreeWidgetItem*)));
    delete treeWidget;
    treeWidget=NULL;
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

void Terminal::closeEvent(QCloseEvent *event)
{
    qDebug()<<"Closing...";
    slt_xml_close();
    QWidget::closeEvent(event);
}

//串口相关函数
void Terminal::slt_com_open()
{
    if(btn_open->text()==tr("打开"))
    {
        if(cbx_com->count()==0) return;

        my_port->setPortName(cbx_com->currentText());
        if(my_port->open(QIODevice::ReadWrite))     //串口打开成功
        {
            my_port->setBaudRate(cbx_bdrate->currentText().toInt());

            QString databitString=cbx_databit->currentText();
            if(databitString==tr("5位"))
            {
                my_port->setDataBits(QSerialPort::Data5);
            }
            else if(databitString==tr("6位"))
            {
                my_port->setDataBits(QSerialPort::Data6);
            }
            else if(databitString==tr("7位"))
            {
                my_port->setDataBits(QSerialPort::Data7);
            }
            else if(databitString==tr("8位"))
            {
                my_port->setDataBits(QSerialPort::Data8);
            }

            QString parityString=cbx_parity->currentText();
            if(parityString==tr("无校验"))
            {
                my_port->setParity(QSerialPort::NoParity);
            }
            else if(parityString==tr("奇校验"))
            {
                my_port->setParity(QSerialPort::OddParity);
            }
            else if(parityString==tr("偶校验"))
            {
                my_port->setParity(QSerialPort::EvenParity);
            }

            QString stopbitString=cbx_stopbit->currentText();
            if(stopbitString==tr("1位"))
            {
                my_port->setStopBits(QSerialPort::OneStop);
            }
            else if(stopbitString==tr("1.5位"))
            {
                my_port->setStopBits(QSerialPort::OneAndHalfStop);
            }
            else if(stopbitString==tr("2位"))
            {
                my_port->setStopBits(QSerialPort::TwoStop);
            }

            QString flowcontrolString=cbx_flowctrl->currentText();

            if(flowcontrolString==tr("无流控"))
            {
                my_port->setFlowControl(QSerialPort::NoFlowControl);
            }
            else if(flowcontrolString==tr("硬件流"))
            {
                my_port->setFlowControl(QSerialPort::HardwareControl);
            }
            else if(flowcontrolString==tr("软件流"))
            {
                my_port->setFlowControl(QSerialPort::SoftwareControl);
            }

            cbx_com->setEnabled(false);
            cbx_bdrate->setEnabled(false);
            cbx_databit->setEnabled(false);
            cbx_parity->setEnabled(false);
            cbx_stopbit->setEnabled(false);
            cbx_flowctrl->setEnabled(false);

            btn_open->setStyleSheet("background-color:red");
            btn_open->setText(tr("关闭"));
            connect(my_port,SIGNAL(readyRead()),this,SLOT(slt_com_recdata()));
            //connect(ui->btn_send,SIGNAL(clicked(bool)),this,SLOT(slt_senddata()));
        }
        else        //串口打开失败
        {
            QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();       //将枚举转化为字符串，方便显示
            const char* s = metaEnum.valueToKey(my_port->error());              //获取错误信息
            qDebug()<<s;
            QMessageBox *msgBox=new QMessageBox(this);                     //串口未打开，创建一个消息提醒框
            msgBox->setWindowTitle(tr("Notice"));
            msgBox->setText(tr("Serialport open error!"));
            //msgBox->setInformativeText();
            msgBox->setDetailedText(s);
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->setDefaultButton(QMessageBox::Ok);
            msgBox->exec();
        }

    }
    else if(btn_open->text()==tr("关闭"))
    {
        //my_port->clear();
        //my_port->deleteLater();
        my_port->close();

        cbx_com->setEnabled(true);
        cbx_bdrate->setEnabled(true);
        cbx_databit->setEnabled(true);
        cbx_parity->setEnabled(true);
        cbx_stopbit->setEnabled(true);
        cbx_flowctrl->setEnabled(true);

        btn_open->setStyleSheet("background-color:rgb(36,240,67)");
        btn_open->setText(tr("打开"));
    }
}

void Terminal::slt_com_recClr()
{
    tet_Rec->clear();
    receString.clear();
}

void Terminal::slt_com_hexWatch()
{
    if(hexEnable==false)
    {
        hexEnable=true;
        btn_hexwatch->setStyleSheet("background-color:red");
    }
    else
    {
        hexEnable=false;
        btn_hexwatch->setStyleSheet("background-color:rgb(36,240,67)");
    }
}

void Terminal::slt_com_recdata()
{
    unsigned int i=0;
    QByteArray temp;
    QString tempString;

    temp = my_port->readAll();


    if(hexEnable==true)
    {
        for(i=0;i<temp.length();i++)
        {
            qDebug("0x%02x ",(unsigned char)temp[i]);

            int val=(int)(temp.at(i));
            if(val<=15)
            {
                tempString.append('0');
            }
            tempString.append(QString::number(val,16).toUpper());       //转化为大写的16进制
            tempString.append(0x20);
        }
    }
    else
    {
        tempString=QString(temp);
    }

    receString.append(tempString);
    tet_Rec->setText(QString(receString));
}

void Terminal::slt_com_senddata(QTreeWidgetItem *itemtext)
{
    if(btn_open->text()==tr("打开"))
    {
        QMessageBox *msgBox=new QMessageBox(this);                     //串口未打开，创建一个消息提醒框
        msgBox->setWindowTitle(tr("Notice"));
        msgBox->setText(tr("Please open a serialport!"));
        //msgBox->setInformativeText(tr("Please open a serialport!"));
        //msgBox->setDetailedText(tr("Differences here..."));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);
        int ret = msgBox->exec();
        if(ret == QMessageBox::Ok)
            return;     //串口未打开，禁止发送
        else
            return;
    }

    QLineEdit *tempNLet=(QLineEdit *)treeWidget->itemWidget(itemtext,1);
    QComboBox *tempTCmb=(QComboBox *)treeWidget->itemWidget(itemtext,2);
    QLineEdit *tempDLet=(QLineEdit *)treeWidget->itemWidget(itemtext,3);
    qDebug()<<tempNLet->text()<<" "<<tempTCmb->currentText()<<" "<<tempDLet->text();

    QString s=tempDLet->text().replace("\\r\\n","\r\n");
    QByteArray tempData=s.toLocal8Bit();       //全部转化为大写

    if(tempTCmb->currentText()=="Hex")
    {
        QByteArray HexData=HexStingEncode(tempData);

        my_port->write(HexData.data(),HexData.length());      //不论是数据还是长度，都要转换成无编码的数据
    }
    else if(tempTCmb->currentText()=="String")
    {
        my_port->write(tempData.data(),tempData.length());      //不论是数据还是长度，都要转换成无编码的数据
    }
    else if(tempTCmb->currentText()=="File")                    //将文本中的数据以字符串形式发送出去
    {

    }
}

QByteArray Terminal::HexStingEncode(QByteArray hexString)
{
    QByteArray rawString;
    for(int i=0;i<hexString.length();i++)
    {
        if((i%3==2)&&(hexString.at(i)!=0x20))
        {
            qDebug()<<"Format error";
            return NULL;
        }
        else if(i%3==1)
        {
            QByteArray ba;
            bool ok;
            ba.append(hexString.at(i-1));
            ba.append(hexString.at(i));
            rawString.append((char)(ba.toInt(&ok,16)&0xFF));
        }
    }
    return rawString;
}

QByteArray Terminal::HexStingCode(QByteArray rawString)
{

}

void Terminal::slt_ftp_start()      //文件传输开始
{
    if(btn_open->text()==tr("打开"))
    {
        QMessageBox *msgBox=new QMessageBox(this);                     //串口未打开，创建一个消息提醒框
        msgBox->setWindowTitle(tr("Notice"));
        msgBox->setText(tr("Please open a serialport!"));
        //msgBox->setInformativeText(tr("Please open a serialport!"));
        //msgBox->setDetailedText(tr("Differences here..."));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);
        int ret = msgBox->exec();
        if(ret == QMessageBox::Ok)
            return;     //串口未打开，禁止发送
        else
            return;
    }

    //打开传输的文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("ALL Files (*.*)"));
    if (fileName.isEmpty())
        return;

    QFile *ftpfile=new QFile(fileName);
    if (!ftpfile->open(QFile::ReadOnly)){     //不能带QFile::Text，因为文本格式是不会去读0x0D的，导致读取到的数据会将0x0D去除掉
        qDebug()<<"Open file err";
    }
    filedata=ftpfile->readAll();
    ftpfile->close();

    qDebug()<<"open file success";
    qDebug()<<"File size="<<filedata.size()<<"Byte";
    disconnect(my_port,SIGNAL(readyRead()),this,SLOT(slt_com_recdata()));       //解除串口读信号槽
    my_port->close();       //关闭串口，为串口交接做准备
    qDebug()<<"close port success";

    //创建线程
    //让线程接管串口功能
    ftphd=new FtpHandle();                                      //创建Object对象
    thd=new QThread();                                          //创建进程
    ftphd->moveToThread(thd);                                   //将Object对象移入进程
    ftphd->SetAttr(fileName,filedata);                          //设置属性
    ftphd->SetPort(my_port);                                    //传递串口参数
    connect(thd,SIGNAL(started()),ftphd,SLOT(slt_start()));     //进程初始化，因为初始化需要在子进程中运行，所以不能用子进程的构造函数代替，而必须要用信号槽触发
    connect(ftphd,SIGNAL(sgn_sendedProgress(int,int)),this,SLOT(slt_ftp_handle(int,int)));      //传输进度同步
    connect(thd,SIGNAL(finished()),this,SLOT(slt_ftp_end()));
    thd->start();    //开启进程

    QString tempstr=QString("\r\nFile Name:%1\r\nFile Size:%2byte\r\n").arg(QString(fileName.section('/',-1).toLocal8Bit())).arg(filedata.size());
    receString.append(tempstr);
    tet_Rec->setText(QString(receString));
}

void Terminal::slt_ftp_handle(int opt,int pec)        //文件传输进度处理
{
    switch(opt)
    {
    case 0:
    {
        qDebug()<<"File Transmit start";
        receString.append("File Transmitting...");
        break;
    }
    case 1:
    {
        qDebug("File Transmitting...%d%%",pec);
        int index=receString.lastIndexOf("...")+3;
        int len=receString.size()-index;
        qDebug()<<index<<" "<<len;
        QString tempstr=QString("%1%").arg(pec,0,10);
        receString.remove(index,len);
        receString.append(tempstr);
        tet_Rec->setText(QString(receString));
        break;
    }
    case 2:                 //文件传输结束
    {
        //销毁线程
        thd->quit();
        qDebug()<<"Fils Transmit end";

        break;
    }
    }

}

void Terminal::slt_ftp_end()
{
    delete thd;
    delete ftphd;

    //恢复串口信号槽
    if(!my_port->open(QIODevice::ReadWrite))
    {
        qDebug()<<"serialport open err";
    }
    connect(my_port,SIGNAL(readyRead()),this,SLOT(slt_com_recdata()));          //恢复串口读信号槽

    receString.append("\r\nTransmit end\r\n");
    tet_Rec->setText(QString(receString));
}
