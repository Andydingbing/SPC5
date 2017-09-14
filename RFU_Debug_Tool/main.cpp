#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    g_pMainW = new MainWindow;
    g_pMainW->setWindowTitle(QString("SPC5 RFU Debug Tool"));
    g_pMainW->show();
    g_pMainW->setWindowState(Qt::WindowMaximized);

    return a.exec();
}
