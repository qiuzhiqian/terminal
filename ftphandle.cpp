#include "ftphandle.h"
#include <QDebug>

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

FtpHandle::FtpHandle(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    this->setMinimumHeight(80);
    this->setMaximumHeight(80);
    this->setMinimumWidth(400);
    this->setWindowTitle(tr("YModem Send"));

    ftpLayout=new QVBoxLayout();

    fileLayout=new QHBoxLayout();

    led_filePath=new QLineEdit();
    btn_fileOpen=new QPushButton(tr("打开"));
    btn_fileSend=new QPushButton(tr("发送"));

    fileLayout->addWidget(led_filePath);
    fileLayout->addWidget(btn_fileOpen);
    fileLayout->addWidget(btn_fileSend);

    pb_ftpProgress=new QProgressBar();
    pb_ftpProgress->setRange(0,100);
    pb_ftpProgress->setValue(0);

    ftpLayout->addLayout(fileLayout);
    ftpLayout->addWidget(pb_ftpProgress);

    this->setLayout(ftpLayout);

    connect(btn_fileOpen,SIGNAL(clicked(bool)),this,SLOT(slt_fileOpen()));
    connect(btn_fileSend,SIGNAL(clicked(bool)),this,SLOT(slt_send_Handle()));

}

void FtpHandle::slt_rec_Handle()
{
    QByteArray temp=Port->readAll();
    RxData.append(temp);
    if(RxData.size()<0) return;
    qDebug("receive a data 0x%02X",RxData.at(0));

    switch(Step){
    case 0:                                  //等待收方的请求
    {
        if(RxData.at(0) != 'C')
            break;

        //tm_timeout->stop();
        qDebug()<<"get ack";

        AckFlag='C';
        sgn_recvACK();        //传输开始

        Step=1;
        break;
    }
    case 1:                                //等待响应
    {
        if(RxData.size()==1)
        {
            if(RxData.at(0)=='C')       //只有C
            {
                qDebug()<<"only C";

                AckFlag='C';
                sgn_recvACK();
                Step=3;
            }
            else if(RxData.at(0)==ACK)      //只有ACK
            {
                qDebug()<<"only ACK";
            }
        }
        else
        {
            if(RxData.at(0)==ACK&&RxData.at(1)=='C')        //同时ACK C
            {
                qDebug()<<"get ack and C";

                AckFlag='C';
                sgn_recvACK();
                Step=3;
            }
        }
        break;
    }
    case 2:                                //第二次等待对方的请求
    {
        if(RxData.at(0) != 'C')
            break;

        AckFlag='C';
        sgn_recvACK();
        Step=3;

        break;
    }
    case 3:                                //发送文件数据包
    {
        if(RxData.at(0) != ACK)
            break;

        pb_ftpProgress->setValue(PackCnt*100/PackNumber);                    //正式传输文件进度

        TxData.remove(0, SendLen);          //回复正确删除已发的长度

        if(PackCnt >= PackNumber){         //发送完成
            Step=4;
        }
        sgn_recvACK();
        break;
    }
    case 4:                                //等待ACK C响应
    {
        //qDebug("receive a data 0x%02X",RxData.at(0));

        if(RxData.size()==1)
        {
            if(RxData.at(0)=='C')       //只有C
            {
                qDebug()<<"only C";

                sgn_recvACK();
                Step=6;
                //slt_send_Handle(3);        //直接发送空包
            }
            else if(RxData.at(0)==ACK)      //只有ACK
            {
                qDebug()<<"only ACK";
            }
            else if(RxData.at(0)==NAK)
            {
                sgn_recvACK();
            }
        }
        else
        {
            if(RxData.at(0)==ACK&&RxData.at(1)=='C')        //同时ACK C
            {
                qDebug()<<"get ack and C";

                sgn_recvACK();
                Step=6;
            }
        }
        break;
    }
    case 5:                                //第三次等待C字符
    {
        if(RxData.at(0) != 'C')
            break;

        qDebug()<<"get C";
        Step=6;
        break;
    }
    case 6:                                //
    {
        if(RxData.at(0) != ACK)
            break;

        qDebug()<<"get the null pkg ack,ftp end";
        sgn_recvACK();

        break;
    }

    }
}

void FtpHandle::slt_send_Handle()
{
    quint16 ValidLen = 0;
    QByteArray Pack, Data;
    quint8 cnt = 0;                               //xModem 包号从1开始
    quint16 Crc;
    int ackval=0;

    if(FilePath.isEmpty())
    {
        QMessageBox *msgBox=new QMessageBox(this);                     //串口未打开，创建一个消息提醒框
        msgBox->setWindowTitle(tr("Notice"));
        msgBox->setText(tr("Please Select a file!"));
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

    Step=0;
    PackCnt=0;
    PackIndex=0;

/***********************************************************************/
    RxData.clear();
    qDebug()<<"start";
    ackval=ackwait(20000);
    if(ackval<=0)   {RxData.clear();return;}
    else{
        if(RxData.at(0)!='C')   {RxData.clear();return;}
    }
/***********************************************************************/
    qDebug()<<"send file info";
    PackIndex=0;                             //设置包序号
    Pack[0] = SOH;
    Pack[1] = PackIndex;                      //包号
    Pack[2] = ~PackIndex;                     //包号取反
    Pack+=FilePath.section('/',-1).toLocal8Bit();      //添加文件名
    Pack+='\0';
    Pack+=QString::number(TxData.size());
    int len=Pack.length()-3;   //出去头部和序列号的长度
    if(len<128)
    {
       QByteArray zero(128 - len, 0x00); //不足部分填充0x00
       qDebug()<<"creat zero section";
       Pack+=zero;
    }
    Crc = GetCrc(Pack.mid(3));                 //CRC
    Pack += (quint8)(Crc >> 8);         //先发高位
    Pack += (quint8)(Crc & 0xff);       //后发低位

    qDebug()<<"write data to port";
    Port->write(Pack, 128 + 5);    //发送数据
    qDebug()<<"write end";
    qDebug()<<"send file info end";
    Pack.clear();                       //清buf
    Data.clear();                       //清buf

    RxData.clear();
    ackval=ackwait(2000);            //超时2s
    if(ackval<=0)   {RxData.clear();return;}
    else{
        if(RxData.size()==1)
        {
            if(RxData.at(0)!='C')   {RxData.clear();return;}
        }
        if(RxData.size()>1)
        {
            if(RxData.at(0)!=ACK||RxData.at(1)!='C')   {RxData.clear();return;}
        }
    }
/***********************************************************************/
    for(int i=0;i<PackNumber;i++)
    {
        PackIndex++;
        qDebug()<<"PackIndex="<<PackIndex;
        Pack[0] = STX;                     //目前只用1024byte类型
        Pack[1] = PackIndex;                      //包号
        Pack[2] = ~PackIndex;                     //包号取反
        ValidLen = TxData.size();           //有效数据长度
        if(ValidLen >= PackSize){           //大于包长
            Data = TxData.left(PackSize);   //从TxData的左侧取出PackSize长
            SendLen = PackSize;             //已发长度
        }else{
            QByteArray zero(PackSize - ValidLen, 0x1A); //不足部分填充0x1A
            Data = TxData.left(ValidLen);   //不足一包
            Data += zero;                   //填充其它数据
            SendLen = ValidLen;             //已发长度
        }
        Crc = GetCrc(Data);                 //CRC
        Pack += Data;                       //填入数据
        Pack += (quint8)(Crc >> 8);         //先发高位
        Pack += (quint8)(Crc & 0xff);       //后发低位
        Port->write(Pack, PackSize + 5);    //发送数据

        Pack.clear();                       //清buf
        Data.clear();                       //清buf

        PackCnt++;
        qDebug()<<"PackCnt="<<PackCnt;

        RxData.clear();
        ackval=ackwait(1000);            //超时1s
        if(ackval<=0)   {RxData.clear();return;}
        else{
            if(RxData.at(0)!=ACK)   {RxData.clear();return;}
        }
    }
/***********************************************************************/
    while(1)
    {
        qDebug()<<"send EOT";
        Pack.clear();                      //清除buf
        Pack += EOT;                      //发送结束符
        Port->write(Pack, 1);              //写
        Pack.clear();

        RxData.clear();
        ackval=ackwait(3000);            //超时1s
       if(ackval<=0)   {RxData.clear();return;}
        else{
           if(RxData.size()==1)
           {
                if(RxData.at(0)=='C')   {RxData.clear();break;}
                else if(RxData.at(0)!=NAK)  {RxData.clear();return;}
           }
           else if(RxData.size()>1)
           {
                if(RxData.at(0)==ACK&&RxData.at(1)=='C')    {RxData.clear();break;}
           }
        }
    }

/***********************************************************************/
    qDebug()<<"send null pkg to end";
    cnt = 0;
    Pack[0] = SOH;                      //目前只用128byte类型
    Pack[1] = cnt;                      //包号
    Pack[2] = ~cnt;                     //包号取反
    QByteArray zero(128, 0x00);         //填充0x00
    Crc = GetCrc(zero);                 //CRC
    Pack += zero;                       //填入数据
    Pack += (quint8)(Crc >> 8);         //先发高位
    Pack += (quint8)(Crc & 0xff);       //后发低位
    Port->write(Pack, 128 + 5);         //发送数据
    Pack.clear();                       //清buf
    Data.clear();                       //清buf

    RxData.clear();
    ackval=ackwait(1000);               //超时1s
    if(ackval<=0)   {RxData.clear();return;}
    else{
        if(RxData.at(0)!=ACK)   {RxData.clear();return;}
    }
/***********************************************************************/
    this->close();
}

void FtpHandle::SetPort(QSerialPort *P)
{
    Port=P;
    connect(Port,SIGNAL(readyRead()),this,SLOT(slt_rec_Handle()));
}

//多项是为0x1021的CRC算法
qint16 FtpHandle::GetCrc(QByteArray Data)
{
   quint16 mCrc = 0;
   quint16 sLen = Data.size();
   for(quint32 j = 0; j < sLen; j++){
       mCrc = mCrc^(quint16)(Data.at(j)) << 8;
       for (quint32 i=8; i!=0; i--){
           if (mCrc & 0x8000)
               mCrc = mCrc << 1 ^ 0x1021;
           else
               mCrc = mCrc << 1;
       }
   }
   return mCrc;
}


int FtpHandle::ackwait(int to)
{
    QEventLoop loop;
    QTimer timeout_t;
    timeout_t.setSingleShot(true);
    connect(&timeout_t,SIGNAL(timeout()),&loop,SLOT(quit()));
    connect(this,SIGNAL(sgn_recvACK()),&loop,SLOT(quit()));
    timeout_t.start(to);
    loop.exec();
//    qDebug()<<"wait end";
//    qDebug()<<timeout_t.remainingTime();

    if(timeout_t.isActive())
    {
        qDebug()<<"ack ok";
        return 1;
    }
    else
    {
        qDebug()<<"timeout";
        return -1;
    }
}

void FtpHandle::slt_fileOpen()
{
    //打开传输的文件
    FilePath = QFileDialog::getOpenFileName(this, tr("Open File"),QDir::currentPath(),tr("ALL Files (*.*)"));
    if (FilePath.isEmpty())
        return;

    QFile *ftpfile=new QFile(FilePath);
    if (!ftpfile->open(QFile::ReadOnly)){     //不能带QFile::Text，因为文本格式是不会去读0x0D的，导致读取到的数据会将0x0D去除掉
        qDebug()<<"Open file err";
    }
    TxData=ftpfile->readAll();
    ftpfile->close();

    PackSize=1024;
    PackNumber = TxData.size() / PackSize;//计算包数
    if(TxData.size() % PackSize)              //多出的按一包算
     PackNumber += 1;
    qDebug()<<"File size="<<TxData.size()<<" PackNumber="<<PackNumber;

    led_filePath->setText(FilePath);
}

void FtpHandle::closeEvent(QCloseEvent *event) //子窗口关闭事件
{
    disconnect(Port,SIGNAL(readyRead()),this,SLOT(slt_rec_Handle()));
    sgn_ftpEnd();
    QWidget::closeEvent(event);
}
