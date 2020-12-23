#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include "algo_chip_pll.inl.hpp"

#include "rddt_plot.inline.hpp"
#include "config_table.cpp"

void addMsgListCallback() {g_MainW->addMsgListCallback();}
void addRegListCallback() {g_MainW->addRegListCallback();}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log.init(addMsgListCallback, addRegListCallback);
    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("RDDT"));
    g_MainW->show();

//    QApplication::setStyle(QStyleFactory::create("windows"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
