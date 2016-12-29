#ifndef ISP_WIDGET_H
#define ISP_WIDGET_H

#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

#include <QScrollArea>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QFileDialog>
#include <QTimer>
#include <QEventLoop>

class ISP_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit ISP_Widget(QWidget *parent = 0);
    //~ISP_Widget();
    void setPort(QSerialPort *sp);      //设置串口

    void sendHandle();

    void ISP_Start();
    void ISP_Get();
    void ISP_GetV();
    void ISP_GetID();
    void ISP_ReadMEM(quint32 address,quint8 len);
    void ISP_Go(quint32 address);
    void ISP_WriteMEM();
    void ISP_EraseMEM();
    void ISP_ExEraseMEM();
    void ISP_WriteP();
    void ISP_WriteUP();
    void ISP_ReadP();
    void ISP_ReadUP();

private:
    QVBoxLayout *widLayout;
    QHBoxLayout *loadlayout;
    QLineEdit *let_loadfile;

    QPushButton *btn_open;
    QPushButton *btn_down;
    //QTextEdit *ted_display;
    QLabel *lab_display;
    QScrollArea *sa;

    QSerialPort *serialp;
    QByteArray recvData;

    QByteArray fileData;

    QTimer *tm_timeout;     //超时控制

    int state;

    bool ackwait(int to);

    quint8 xorcheck(QByteArray ba);

signals:
    void sgn_ispend();

    void sgn_recvACK();

private slots:
    void slt_receiveData();
    void slt_fileopen();
    void slt_filedown();

protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // ISP_WIDGET_H
