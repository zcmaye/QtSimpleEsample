#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget wid;
    MayeTableView w(&wid);
    wid.show();
    return a.exec();
}
