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

FtpHandle::FtpHandle(QObject *parent) : QObject(parent)
{

}

void FtpHandle::SetAttr(QString filePath,QByteArray fileData)
{
    FilePath=filePath;
    TxData=fileData;
    PackSize=1024;
    PackNumber = TxData.size() / PackSize;//计算包数
    if(TxData.size() % PackSize)              //多出的按一包算
     PackNumber += 1;
    qDebug()<<"File size="<<TxData.size()<<" PackNumber="<<PackNumber;
}

void FtpHandle::slt_rec_Handle()
{
    RxData = Port->readAll();
    if(RxData.size()<0) return;

    switch(Step){
    case 0:                                  //等待收方的请求
    {
        if(RxData.at(0) != 'C')
            break;

        tm_timeout->stop();
        qDebug()<<"get ack";

        sgn_sendedProgress(0,0);        //传输开始
        RxData.clear();
        Step=1;
        slt_send_Handle(0);
        break;
    }
    case 1:                                //等待响应
    {
        if(RxData.size()==1)
        {
            if(RxData.at(0)=='C')       //只有C
            {
                tm_timeout->stop();
                qDebug()<<"only C";

                sgn_sendedProgress(1,0);                    //正式传输文件
                RxData.clear();
                Step=3;
                slt_send_Handle(1);        //直接发送数据
            }
            else if(RxData.at(0)==ACK)      //只有ACK
            {
                tm_timeout->stop();
                qDebug()<<"only ACK";

                RxData.clear();
                Step=2;
                tm_timeout->start(1000);
            }
        }
        else
        {
            if(RxData.at(0)==ACK&&RxData.at(1)=='C')        //同时ACK C
            {
                tm_timeout->stop();
                qDebug()<<"get ack and C";

                sgn_sendedProgress(1,0);                    //正式传输文件
                RxData.clear();
                Step=3;
                slt_send_Handle(1);        //直接发送数据
            }
        }
        break;
    }
    case 2:                                //第二次等待对方的请求
    {
        if(RxData.at(0) != 'C')
            break;

        tm_timeout->stop();

        sgn_sendedProgress(1,0);                    //正式传输文件
        RxData.clear();
        Step=3;
        slt_send_Handle(1);        //直接发送数据

        break;
    }
    case 3:                                //发送文件数据包
    {
        if(RxData.at(0) != ACK)
            break;

        tm_timeout->stop();
        sgn_sendedProgress(1,PackCnt*100/PackNumber);                    //正式传输文件进度

        TxData.remove(0, SendLen);          //回复正确删除已发的长度

        if(PackCnt >= PackNumber){         //发送完成
            RxData.clear();
            Step=4;
            slt_send_Handle(2);             //发送结束符
        }
        else
        {
            RxData.clear();
            slt_send_Handle(1);             //继续发送文件
        }
        break;
    }
    case 4:                                //等待ACK C响应
    {
        qDebug("receive a data 0x%02X",RxData.at(0));

        if(RxData.size()==1)
        {
            if(RxData.at(0)=='C')       //只有C
            {
                tm_timeout->stop();
                qDebug()<<"only C";

                RxData.clear();
                Step=6;
                slt_send_Handle(3);        //直接发送空包
            }
            else if(RxData.at(0)==ACK)      //只有ACK
            {
                tm_timeout->stop();
                qDebug()<<"only ACK";

                RxData.clear();
                Step=5;
                tm_timeout->start(3000);
            }
            else if(RxData.at(0)==NAK)
            {
                RxData.clear();
                slt_send_Handle(2);             //再次发送结束符
            }
        }
        else
        {
            if(RxData.at(0)==ACK&&RxData.at(1)=='C')        //同时ACK C
            {
                tm_timeout->stop();
                qDebug()<<"get ack and C";

                RxData.clear();
                Step=6;
                slt_send_Handle(3);        //直接发送数据
            }
        }
        break;
    }
    case 5:                                //第三次等待C字符
    {
        if(RxData.at(0) != 'C')
            break;

        tm_timeout->stop();
        qDebug()<<"get C";

        RxData.clear();
        Step=6;
        slt_send_Handle(3);        //直接发送数据
        break;
    }
    case 6:                                //第三次等待C字符
    {
        if(RxData.at(0) != ACK)
            break;

        qDebug()<<"get the null pkg ack,ftp end";
        RxData.clear();
        tm_timeout->stop();

        sgn_sendedProgress(2,0);                    //结束文件传输
        Port->close();                              //关闭串口
        break;
    }

    }
}

void FtpHandle::slt_send_Handle(int sta)
{
    quint16 ValidLen = 0;
    QByteArray Pack, Data;
    quint8 cnt = 0;                               //xModem 包号从1开始
    quint16 Crc;

    switch(sta)
    {
    case 0:
    {
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

        tm_timeout->start(2000);            //超时2s
        break;
    }
    case 1:                                 //发送文件数据包
    {
        PackIndex++;
        qDebug()<<"PackIndex="<<PackIndex;
        Pack[0] = STX;                     //目前只用128byte类型
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

        tm_timeout->start(1000);            //超时1s
        break;
    }
    case 2:                                    //发送结束符
    {
        qDebug()<<"send EOT";
        Pack.clear();                      //清除buf
        Pack += EOT;                      //发送结束符
        Port->write(Pack, 1);              //写
        Pack.clear();

        tm_timeout->start(1000);            //超时1s
        break;
    }
    case 3:                                 //发送空包
    {
        qDebug()<<"send null pkg to end";
        cnt = 0;
        Pack[0] = SOH;                     //目前只用128byte类型
        Pack[1] = cnt;                      //包号
        Pack[2] = ~cnt;                     //包号取反
        QByteArray zero(128, 0x00); //填充0x00
        Crc = GetCrc(zero);                 //CRC
        Pack += zero;                       //填入数据
        Pack += (quint8)(Crc >> 8);         //先发高位
        Pack += (quint8)(Crc & 0xff);       //后发低位
        Port->write(Pack, 128 + 5);    //发送数据
        Pack.clear();                       //清buf
        Data.clear();                       //清buf

        tm_timeout->start(1000);            //超时1s
        break;
    }

    }
}

void FtpHandle::slt_rec_timeout()
{
    switch(Step)
    {
    case 0:
        qDebug()<<"start timeout";
        break;
    case 1:
        qDebug()<<"wait file info ack timeout";
        break;
    case 2:
        qDebug()<<"wait C timeout";
        break;
    case 3:
        qDebug()<<"wait file pkg ack timeout";
        break;
    case 4:
        qDebug()<<"wait end ack timeout";
        break;
    case 5:
        qDebug()<<"wait end C timeout";
        break;
    case 6:
        qDebug()<<"wait null pkg ack timeout";
        break;
    }
}

void FtpHandle::SetPort(QSerialPort *P)
{
    mPort=P;
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

void FtpHandle::slt_start()
{
    PackCnt=0;
    PackIndex=0;

    Step=0;

    tm_timeout=new QTimer();
    connect(tm_timeout,SIGNAL(timeout()),this,SLOT(slt_rec_timeout()));
    tm_timeout->start(10000);       //10s超时

    Port=new QSerialPort();
    Port->setPortName(mPort->portName());
    Port->setBaudRate(mPort->baudRate());
    Port->setDataBits(mPort->dataBits());
    Port->setParity(mPort->parity());
    Port->setStopBits(mPort->stopBits());
    Port->setFlowControl(mPort->flowControl());
    qDebug()<<mPort->baudRate();
    if(!Port->open(QIODevice::ReadWrite))
    {
        qDebug()<<"open err";
    }
    else{
        qDebug()<<"open serialport in thread success";
    }
    //串口打开

    connect(Port,SIGNAL(readyRead()),this,SLOT(slt_rec_Handle()));
}
