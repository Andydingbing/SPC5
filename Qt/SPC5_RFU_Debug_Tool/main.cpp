#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>

void addMsgListCallback() {g_MainW->addMsgListCallback();}
void addRegListCallback() {g_MainW->addRegListCallback();}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log.init(addMsgListCallback, addRegListCallback);
    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("SPC5_RFU_Debug_Tool"));
    g_MainW->show();
    g_MainW->setWindowState(Qt::WindowMaximized);

//    QApplication::setStyle(QStyleFactory::create("windowsvista"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
