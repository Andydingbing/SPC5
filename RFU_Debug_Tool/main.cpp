#include "mainwindow.h"
#include <QApplication>

void addMsgListCallback() {g_pMainW->addMsgListCallback();}
void addRegListCallback() {g_pMainW->addRegListCallback();}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log->Init(addMsgListCallback,addRegListCallback);
    Log->SetEnable(L_REG,true);
    g_pMainW = new MainWindow;
    g_pMainW->setWindowTitle(QString("SPC5 RFU Debug Tool"));
    g_pMainW->show();
    g_pMainW->setWindowState(Qt::WindowMaximized);

    return a.exec();
}
