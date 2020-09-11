#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include "algo_chip_pll.inl.hpp"

#include "q_plot.inline.hpp"

void addMsgListCallback() {g_MainW->addMsgListCallback();}
void addRegListCallback() {g_MainW->addRegListCallback();}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log.init(addMsgListCallback, addRegListCallback);
    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("RF_Driver_Debug_Tool"));
    g_MainW->setWindowState(Qt::WindowMaximized);
    g_MainW->show();

//    QApplication::setStyle(QStyleFactory::create("windows"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
