#ifndef FTPHANDLE_H
#define FTPHANDLE_H

#include <QObject>
#include <QWidget>
#include <QTimer>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class FtpHandle : public QObject
{
    Q_OBJECT
public:
    explicit FtpHandle(QObject *parent = 0);

    void SetAttr(QString filePath,QByteArray fileData);

    void SetPort(QSerialPort *P);       //初始化串口端口

    qint16 GetCrc(QByteArray Data);

private:
    QSerialPort *Port;
    QSerialPort *mPort;

    QString FilePath;
    QString *recString;

    QByteArray TxData;
    QByteArray RxData;
    qint32 PackNumber;      //发送文件的包数
    qint32 PackCnt;         //已发送的包数
    qint32 PackIndex;       //包序号
    qint32 PackSize;        //1包字节数
    qint32 SendLen;         //发送一帧数据的实际长度

    QTimer *tm_timeout;

    qint8 Step;

signals:
    sgn_sendData();
    sgn_sendedProgress(int opt,int pec);        //opt=0传输开始,opt=1正在传输,发送进度pec(0~100),opt=2传输结束

private slots:
    void slt_start();
    void slt_rec_Handle();      //接收处理
    void slt_send_Handle(int sta);     //发送处理
    void slt_rec_timeout();     //超时处理

public slots:
};

#endif // FTPHANDLE_H
