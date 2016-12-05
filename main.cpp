#include "terminal.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Terminal w;
    w.show();

    return a.exec();
}
