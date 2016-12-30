#include "isp_widget.h"
#include <QDebug>

#define CMD_Start       (0x7F)
#define CMD_Get         (0x00)
#define CMD_Get_V_PS    (0x01)
#define CMD_Get_ID      (0x02)
#define CMD_Read_MEM    (0x11)
#define CMD_Go          (0x21)
#define CMD_Write_MEM   (0x31)
#define CMD_Erase       (0x43)
#define CMD_Ex_Erase    (0x44)
#define CMD_Write_P     (0x63)
#define CMD_Write_UP    (0x73)
#define CMD_Read_P      (0x82)
#define CMD_Read_UP     (0x92)

#define CMD_ACK         (0x79)
#define CMD_NACK        (0x1F)

ISP_Widget::ISP_Widget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setAttribute(Qt::WA_QuitOnClose);
    this->setMinimumSize(400,200);
    //this->setBaseSize(400,200);
    this->setWindowTitle(tr("ISP下载"));

    widLayout=new QVBoxLayout();
    loadlayout=new QHBoxLayout();

    let_loadfile=new QLineEdit();
    btn_open=new QPushButton(tr("打开"));
    btn_down=new QPushButton(tr("下载"));

    //ted_display=new QTextEdit();
    lab_display=new QLabel();
    lab_display->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    //lab_display->setStyleSheet("QLabel{border-width:1px;border-style:solid;border-color:rgb(255, 170, 0);}");    //设置边框

    sa=new QScrollArea();
    sa->setWidget(lab_display);
    sa->setWidgetResizable(true);

    loadlayout->addWidget(let_loadfile);
    loadlayout->addWidget(btn_open);
    loadlayout->addWidget(btn_down);

    widLayout->addLayout(loadlayout);
    widLayout->addWidget(sa);

    this->setLayout(widLayout);

    connect(btn_open,SIGNAL(clicked(bool)),this,SLOT(slt_fileopen()));
    connect(btn_down,SIGNAL(clicked(bool)),this,SLOT(slt_filedown()));

    state=1;
}

void ISP_Widget::setPort(QSerialPort *sp)
{
    serialp=sp;
    connect(serialp,SIGNAL(readyRead()),this,SLOT(slt_receiveData()));
}

void ISP_Widget::slt_receiveData()
{
    QByteArray da;
    switch(state)
    {
    case 1:
        da=serialp->readAll();
        recvData.append(da);
        lab_display->setText(recvData);
        break;
    default:
        break;
    }

}

void ISP_Widget::closeEvent(QCloseEvent *event) //关闭事件，完成串口的交接
{
    disconnect(serialp,SIGNAL(readyRead()),this,SLOT(slt_receiveData()));
    sgn_ispend();
    QWidget::closeEvent(event);
}

void ISP_Widget::slt_fileopen()
{
    //打开传输的文件
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("HEX File(*.hex);;BIN File(*.bin);;ALL File (*.*)"));
    if (fileName.isEmpty())
        return;

    QFile *ispfile=new QFile(fileName);
    if (!ispfile->open(QFile::ReadOnly)){     //不能带QFile::Text，因为文本格式是不会去读0x0D的，导致读取到的数据会将0x0D去除掉
        qDebug()<<"Open file err";
    }
    fileData=ispfile->readAll();
    ispfile->close();

    let_loadfile->setText(fileName);
}

void ISP_Widget::slt_filedown()
{
    sendHandle();
}

void ISP_Widget::ISP_Start()
{
    QByteArray tempdata;
    bool sta=0;
    tempdata.append(CMD_Start);      //发送0x7F，然后等待响应ACK(0x79)或者NACK(0x1F)
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    else{

    }
}

void ISP_Widget::ISP_Get()
{
    QByteArray tempdata;
    bool sta=0;
    quint8 cmd=CMD_Get;
    tempdata.append(cmd);      //发送CMD_Get，然后等待响应ACK(0x79)或者NACK(0x1F)
    tempdata.append(~cmd);
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    else{

    }
}

void ISP_Widget::ISP_GetV()
{
    QByteArray tempdata;
    bool sta=0;
    quint8 cmd=CMD_Get_V_PS;
    tempdata.append(cmd);      //发送CMD_Get_V_PS，然后等待响应ACK(0x79)或者NACK(0x1F)
    tempdata.append(~cmd);
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    else{

    }
}

void ISP_Widget::ISP_GetID()
{
    QByteArray tempdata;
    bool sta=0;
    quint8 cmd=CMD_Get_ID;
    tempdata.append(cmd);      //发送CMD_Get_V_PS，然后等待响应ACK(0x79)或者NACK(0x1F)
    tempdata.append(~cmd);
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    else{

    }
}

void ISP_Widget::ISP_ReadMEM(quint32 address, quint8 len)
{
    QByteArray tempdata;
    quint8 checkval;
    bool sta=0;
    quint8 cmd=CMD_Read_MEM;
    tempdata.append(cmd);      //发送CMD_Get_V_PS，然后等待响应ACK(0x79)或者NACK(0x1F)
    tempdata.append(~cmd);
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    tempdata.clear();
    tempdata.append((quint8)(address>>24));         //b3
    tempdata.append((quint8)(address>>16));         //b4
    tempdata.append((quint8)(address>>8));          //b3
    tempdata.append((quint8)address);               //b4

    checkval=xorcheck(tempdata);
    tempdata.append(checkval);                  //b3
    serialp->write(tempdata);
    sta=ackwait(1000);
    if(sta==false){
        //timeout
    }
    tempdata.clear();
    tempdata.append(len);         //b3
    checkval=xorcheck(tempdata);
    tempdata.append(checkval);                  //b3
    serialp->write(tempdata);
}

void ISP_Widget::ISP_Go(quint32 address)
{

}

void ISP_Widget::ISP_WriteMEM()
{

}

void ISP_Widget::ISP_EraseMEM()
{

}

void ISP_Widget::ISP_ExEraseMEM()
{

}

void ISP_Widget::ISP_WriteP()
{

}

void ISP_Widget::ISP_WriteUP()
{

}

void ISP_Widget::ISP_ReadP()
{

}

void ISP_Widget::ISP_ReadUP()
{

}

void ISP_Widget::sendHandle()
{
    ISP_Start();
    ISP_Get();
    ISP_GetV();
}

bool ISP_Widget::ackwait(int to)
{
    QEventLoop loop;
    QTimer timeout_t;
    timeout_t.setSingleShot(true);
    connect(&timeout_t,SIGNAL(timeout()),&loop,SLOT(quit()));       //超时打断
    connect(this,SIGNAL(sgn_recvACK()),&loop,SLOT(quit()));         //接收到ack打断
    timeout_t.start(to);
    loop.exec();
    if(!timeout_t.isActive())
    {
        //超时
        qDebug()<<"timeout";
        return false;
    }
    qDebug()<<"ack ok";
    return true;
}

quint8 ISP_Widget::xorcheck(QByteArray ba)
{
    quint8 res=0;
    quint8 len=ba.length();
    for(int i=0;i<len;i++)
    {
        res=res^ba.at(i);
    }
    return res;
}
