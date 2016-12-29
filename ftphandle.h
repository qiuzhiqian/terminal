#ifndef FTPHANDLE_H
#define FTPHANDLE_H

#include <QWidget>
#include <QTimer>
#include <QEventLoop>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

#include <QMessageBox>

class FtpHandle : public QWidget
{
    Q_OBJECT
public:
    explicit FtpHandle(QWidget *parent = 0);

    //void SetAttr(QString filePath,QByteArray fileData);

    void SetPort(QSerialPort *P);       //初始化串口端口

    qint16 GetCrc(QByteArray Data);

    int ackwait(int to);

private:
    QSerialPort *Port;

    QString FilePath;
    QString *recString;

    QByteArray TxData;
    QByteArray RxData;
    qint32 PackNumber;      //发送文件的包数
    qint32 PackCnt;         //已发送的包数
    qint32 PackIndex;       //包序号
    qint32 PackSize;        //1包字节数
    qint32 SendLen;         //发送一帧数据的实际长度

    //QTimer *tm_timeout;

    qint8 Step;

    quint8 AckFlag;

    //子窗口
    QVBoxLayout *ftpLayout;
    QHBoxLayout *fileLayout;
    QProgressBar *pb_ftpProgress;

    QLineEdit *led_filePath;
    QPushButton *btn_fileOpen;
    QPushButton *btn_fileSend;

signals:
    sgn_ftpEnd();
    sgn_recvACK();

private slots:
    void slt_rec_Handle();      //接收处理
    void slt_send_Handle();     //发送处理

    void slt_fileOpen();
    //void slt_fileSend();

public slots:

protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // FTPHANDLE_H
