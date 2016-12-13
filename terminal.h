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

namespace Ui {
class Terminal;
}

class Terminal : public QMainWindow
{
    Q_OBJECT

public:
    explicit Terminal(QWidget *parent = 0);
    ~Terminal();

    bool xml_read(QIODevice *device);
    void xml_write(QIODevice *device);

private slots:
    void slt_xml_new();
    void slt_xml_open();
    void slt_xml_save();
    void slt_xml_close();
    void slt_xml_exit();

    void slt_xml_addGroup();
    void slt_xml_addNode();
    void slt_xml_del();

    void slt_toolExtand();



private:
    Ui::Terminal *ui;

    QMenu *menu_file;
    QAction *act_creat;
    QAction *act_open;
    QAction *act_save;
    QAction *act_close;
    QAction *act_exit;

    QWidget *toolWidget;
    QGridLayout *toolLayout;

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

    QSplitter *spt_sendTree;
    QTextEdit *let_Rec;

    TreeView *tv;
    QFile *xmlfile;
//    QDomDocument *domDocument;       //定义DOM对象
    QTreeWidget *treeWidget;

};

#endif // TERMINAL_H
