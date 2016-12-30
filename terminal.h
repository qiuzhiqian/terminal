#ifndef TERMINAL_H
#define TERMINAL_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLayout>
#include <QMenu>
#include <QAction>
#include <QSplitter>

#include <QDomDocument>
#include <QFileDialog>
#include <QTreeWidget>
#include <QTextEdit>
#include "treeview.h"

#include <QCloseEvent>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

#include <QListView>
#include <QStandardItemModel>

#include <QThread>
#include "ftphandle.h"
#include "isp_widget.h"

#ifdef __cplusplus
extern "C" {
#endif
//#include "Ymodem.h"
#ifdef __cplusplus
}
#endif

namespace Ui {
class Terminal;
}

class Terminal : public QMainWindow
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

    void extendShow(int sta);

    bool xml_read(QIODevice *device);
    void xml_write(QIODevice *device);

    //数据转换
    QByteArray HexStingEncode(QByteArray hexString);
    QByteArray HexStingCode(QByteArray rawString);

private slots:
    void slt_xml_new();
    void slt_xml_open();
    void slt_xml_save();
    void slt_xml_close();
    void slt_xml_exit();

    void slt_xml_addGroup();
    void slt_xml_addNode();
    void slt_xml_del();

    void slt_startExtend();
    void slt_toolExtend();
    void slt_extandOpen();

    void slt_tool_calcRun();
    void slt_tool_asciiRun();

    void slt_com_open();
    void slt_com_recClr();
    void slt_com_hexWatch();
    void slt_com_recdata();
    void slt_com_senddata(QTreeWidgetItem *itemtext);

    void slt_winTopSet();

    void slt_ftp_start();
    void slt_ftp_end();

    void slt_ftp_stm32isp();
    void slt_ftp_ispend();

private:
    Ui::Terminal *ui;

    QMenu *menu_file;       //总menu
    QMenu *ProjectMenu;
    QMenu *SetMenu;
    QMenu *FTPMenu;
    QMenu *XModemMenu;
    QMenu *YModemMenu;
    QMenu *ZModemMenu;
    QAction *act_project_creat;
    QAction *act_project_open;
    QAction *act_project_save;
    QAction *act_project_close;
    QAction *act_ftp_xsend;
    QAction *act_ftp_xreceive;
    QAction *act_ftp_ysend;
    QAction *act_ftp_yreceive;
    QAction *act_ftp_zsend;
    QAction *act_ftp_zreceive;
    QAction *act_ftp_stm32isp;      //stm32isp下载
    QAction *act_exit;

    //QPushButton *btn_extStart;  //展开start
    //QPushButton *btn_exTool;    //展开tool

    int extandSelect;
    QWidget *startWidget;
    QGridLayout *startLayout;

    QPushButton *btn_open;

    QLabel *lb_com;
    QLabel *lb_bdrate;
    QLabel *lb_databit;
    QLabel *lb_parity;
    QLabel *lb_stopbit;
    QLabel *lb_flowctrl;

    QComboBox *cbx_com;
    QComboBox *cbx_bdrate;
    QComboBox *cbx_databit;
    QComboBox *cbx_parity;
    QComboBox *cbx_stopbit;
    QComboBox *cbx_flowctrl;

    QPushButton *btn_hexwatch;
    QPushButton *btn_recvclear;
    QPushButton *btn_wintop;

    QPushButton *btn_treeitemadd;
    QMenu *menu_treeadd;
    QAction *act_addgroup;
    QAction *act_addnode;

    QWidget *toolWidget;
    QGridLayout *toolLayout;
    QPushButton *btn_tool_calc;      //计算器
    QPushButton *btn_tool_ascii;        //ascii表

    QSplitter *spt_sendTree;
    QTextEdit *tet_Rec;

    TreeView *tv;
    QFile *xmlfile;
//    QDomDocument *domDocument;       //定义DOM对象
    QTreeWidget *treeWidget;

    //串口定义
    QSerialPort *my_port;
    bool hexEnable;
    QString receString;

    QString filepath;
    QByteArray filedata;
    FtpHandle *ftphd;
    QThread *thd;

    ISP_Widget *ispwidget;
protected:
    virtual void closeEvent(QCloseEvent *event);

};

#endif // TERMINAL_H
