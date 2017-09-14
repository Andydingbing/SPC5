/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionInitialization;
    QAction *actionPCIE_Restart_P;
    QAction *actionTune_OCXO_T;
    QAction *actionCXU_Restart_R;
    QAction *actionCXU_Shutdown_S;
    QAction *actionExit_X;
    QAction *actionRFU0_0;
    QAction *actionRFU1_1;
    QAction *actionRFU2_2;
    QAction *actionRFU3_3;
    QAction *actionRFU4_4;
    QAction *actionArb_Viewer_A;
    QAction *actionStart_All_IQ_Capture_R;
    QAction *actionStop_All_IQ_Capture_O;
    QAction *actionLog_Config_L;
    QAction *actionLog_Viewer_V;
    QAction *actionInstrument_Control_I;
    QAction *actionCalculator_C;
    QAction *actionNI_IO_Trace_N;
    QAction *actionProgram_FPGAbit_P;
    QAction *actionAlgorithm_5355_A;
    QAction *actionFitting_Interpolation_F;
    QAction *actionAbout_A;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QTreeWidget *m_pMainTree;
    QTabWidget *m_pMainTab;
    QHBoxLayout *horizontalLayout;
    QListView *listView;
    QListView *listView_2;
    QMenuBar *menuBar;
    QMenu *menuDevice;
    QMenu *menuRFU_R;
    QMenu *menuFunction_F;
    QMenu *menuTest_T;
    QMenu *menuHelp_H;
    QStatusBar *m_pStatusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(797, 554);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionInitialization = new QAction(MainWindow);
        actionInitialization->setObjectName(QStringLiteral("actionInitialization"));
        actionPCIE_Restart_P = new QAction(MainWindow);
        actionPCIE_Restart_P->setObjectName(QStringLiteral("actionPCIE_Restart_P"));
        actionTune_OCXO_T = new QAction(MainWindow);
        actionTune_OCXO_T->setObjectName(QStringLiteral("actionTune_OCXO_T"));
        actionCXU_Restart_R = new QAction(MainWindow);
        actionCXU_Restart_R->setObjectName(QStringLiteral("actionCXU_Restart_R"));
        actionCXU_Shutdown_S = new QAction(MainWindow);
        actionCXU_Shutdown_S->setObjectName(QStringLiteral("actionCXU_Shutdown_S"));
        actionExit_X = new QAction(MainWindow);
        actionExit_X->setObjectName(QStringLiteral("actionExit_X"));
        actionRFU0_0 = new QAction(MainWindow);
        actionRFU0_0->setObjectName(QStringLiteral("actionRFU0_0"));
        actionRFU1_1 = new QAction(MainWindow);
        actionRFU1_1->setObjectName(QStringLiteral("actionRFU1_1"));
        actionRFU2_2 = new QAction(MainWindow);
        actionRFU2_2->setObjectName(QStringLiteral("actionRFU2_2"));
        actionRFU3_3 = new QAction(MainWindow);
        actionRFU3_3->setObjectName(QStringLiteral("actionRFU3_3"));
        actionRFU4_4 = new QAction(MainWindow);
        actionRFU4_4->setObjectName(QStringLiteral("actionRFU4_4"));
        actionArb_Viewer_A = new QAction(MainWindow);
        actionArb_Viewer_A->setObjectName(QStringLiteral("actionArb_Viewer_A"));
        actionStart_All_IQ_Capture_R = new QAction(MainWindow);
        actionStart_All_IQ_Capture_R->setObjectName(QStringLiteral("actionStart_All_IQ_Capture_R"));
        actionStop_All_IQ_Capture_O = new QAction(MainWindow);
        actionStop_All_IQ_Capture_O->setObjectName(QStringLiteral("actionStop_All_IQ_Capture_O"));
        actionLog_Config_L = new QAction(MainWindow);
        actionLog_Config_L->setObjectName(QStringLiteral("actionLog_Config_L"));
        actionLog_Viewer_V = new QAction(MainWindow);
        actionLog_Viewer_V->setObjectName(QStringLiteral("actionLog_Viewer_V"));
        actionInstrument_Control_I = new QAction(MainWindow);
        actionInstrument_Control_I->setObjectName(QStringLiteral("actionInstrument_Control_I"));
        actionCalculator_C = new QAction(MainWindow);
        actionCalculator_C->setObjectName(QStringLiteral("actionCalculator_C"));
        actionNI_IO_Trace_N = new QAction(MainWindow);
        actionNI_IO_Trace_N->setObjectName(QStringLiteral("actionNI_IO_Trace_N"));
        actionProgram_FPGAbit_P = new QAction(MainWindow);
        actionProgram_FPGAbit_P->setObjectName(QStringLiteral("actionProgram_FPGAbit_P"));
        actionAlgorithm_5355_A = new QAction(MainWindow);
        actionAlgorithm_5355_A->setObjectName(QStringLiteral("actionAlgorithm_5355_A"));
        actionFitting_Interpolation_F = new QAction(MainWindow);
        actionFitting_Interpolation_F->setObjectName(QStringLiteral("actionFitting_Interpolation_F"));
        actionAbout_A = new QAction(MainWindow);
        actionAbout_A->setObjectName(QStringLiteral("actionAbout_A"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_pMainTree = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        m_pMainTree->setHeaderItem(__qtreewidgetitem);
        m_pMainTree->setObjectName(QStringLiteral("m_pMainTree"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(5);
        sizePolicy1.setHeightForWidth(m_pMainTree->sizePolicy().hasHeightForWidth());
        m_pMainTree->setSizePolicy(sizePolicy1);
        m_pMainTree->setMaximumSize(QSize(201, 16777215));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        m_pMainTree->setFont(font);

        horizontalLayout_2->addWidget(m_pMainTree);

        m_pMainTab = new QTabWidget(centralWidget);
        m_pMainTab->setObjectName(QStringLiteral("m_pMainTab"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(5);
        sizePolicy2.setHeightForWidth(m_pMainTab->sizePolicy().hasHeightForWidth());
        m_pMainTab->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(m_pMainTab);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        listView = new QListView(centralWidget);
        listView->setObjectName(QStringLiteral("listView"));
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(listView);

        listView_2 = new QListView(centralWidget);
        listView_2->setObjectName(QStringLiteral("listView_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(1);
        sizePolicy3.setHeightForWidth(listView_2->sizePolicy().hasHeightForWidth());
        listView_2->setSizePolicy(sizePolicy3);
        listView_2->setMinimumSize(QSize(321, 0));

        horizontalLayout->addWidget(listView_2);

        horizontalLayout->setStretch(0, 1);

        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 1);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 797, 23));
        menuDevice = new QMenu(menuBar);
        menuDevice->setObjectName(QStringLiteral("menuDevice"));
        menuRFU_R = new QMenu(menuBar);
        menuRFU_R->setObjectName(QStringLiteral("menuRFU_R"));
        menuFunction_F = new QMenu(menuBar);
        menuFunction_F->setObjectName(QStringLiteral("menuFunction_F"));
        menuTest_T = new QMenu(menuBar);
        menuTest_T->setObjectName(QStringLiteral("menuTest_T"));
        menuHelp_H = new QMenu(menuBar);
        menuHelp_H->setObjectName(QStringLiteral("menuHelp_H"));
        MainWindow->setMenuBar(menuBar);
        m_pStatusBar = new QStatusBar(MainWindow);
        m_pStatusBar->setObjectName(QStringLiteral("m_pStatusBar"));
        MainWindow->setStatusBar(m_pStatusBar);

        menuBar->addAction(menuDevice->menuAction());
        menuBar->addAction(menuRFU_R->menuAction());
        menuBar->addAction(menuFunction_F->menuAction());
        menuBar->addAction(menuTest_T->menuAction());
        menuBar->addAction(menuHelp_H->menuAction());
        menuDevice->addAction(actionInitialization);
        menuDevice->addAction(actionPCIE_Restart_P);
        menuDevice->addSeparator();
        menuDevice->addAction(actionTune_OCXO_T);
        menuDevice->addSeparator();
        menuDevice->addAction(actionCXU_Restart_R);
        menuDevice->addAction(actionCXU_Shutdown_S);
        menuDevice->addSeparator();
        menuDevice->addAction(actionExit_X);
        menuRFU_R->addAction(actionRFU0_0);
        menuRFU_R->addAction(actionRFU1_1);
        menuRFU_R->addAction(actionRFU2_2);
        menuRFU_R->addAction(actionRFU3_3);
        menuRFU_R->addAction(actionRFU4_4);
        menuFunction_F->addAction(actionArb_Viewer_A);
        menuFunction_F->addSeparator();
        menuFunction_F->addAction(actionStart_All_IQ_Capture_R);
        menuFunction_F->addAction(actionStop_All_IQ_Capture_O);
        menuFunction_F->addSeparator();
        menuFunction_F->addAction(actionLog_Config_L);
        menuFunction_F->addAction(actionLog_Viewer_V);
        menuFunction_F->addSeparator();
        menuFunction_F->addAction(actionInstrument_Control_I);
        menuFunction_F->addAction(actionCalculator_C);
        menuFunction_F->addAction(actionNI_IO_Trace_N);
        menuTest_T->addAction(actionProgram_FPGAbit_P);
        menuTest_T->addAction(actionAlgorithm_5355_A);
        menuTest_T->addAction(actionFitting_Interpolation_F);
        menuHelp_H->addAction(actionAbout_A);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionInitialization->setText(QApplication::translate("MainWindow", "Initialization(&I)", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionInitialization->setStatusTip(QApplication::translate("MainWindow", "Init Device in Menu RFU", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionPCIE_Restart_P->setText(QApplication::translate("MainWindow", "PCIE Restart(&P)", Q_NULLPTR));
        actionTune_OCXO_T->setText(QApplication::translate("MainWindow", "Tune OCXO(&T)", Q_NULLPTR));
        actionCXU_Restart_R->setText(QApplication::translate("MainWindow", "CXU Restart(&R)", Q_NULLPTR));
        actionCXU_Shutdown_S->setText(QApplication::translate("MainWindow", "CXU Shutdown(&S)", Q_NULLPTR));
        actionExit_X->setText(QApplication::translate("MainWindow", "Exit(&X)", Q_NULLPTR));
        actionRFU0_0->setText(QApplication::translate("MainWindow", "RFU0(&0)", Q_NULLPTR));
        actionRFU1_1->setText(QApplication::translate("MainWindow", "RFU1(&1)", Q_NULLPTR));
        actionRFU2_2->setText(QApplication::translate("MainWindow", "RFU2(&2)", Q_NULLPTR));
        actionRFU3_3->setText(QApplication::translate("MainWindow", "RFU3(&3)", Q_NULLPTR));
        actionRFU4_4->setText(QApplication::translate("MainWindow", "RFU4(&4)", Q_NULLPTR));
        actionArb_Viewer_A->setText(QApplication::translate("MainWindow", "Arb Viewer(&A)", Q_NULLPTR));
        actionStart_All_IQ_Capture_R->setText(QApplication::translate("MainWindow", "Start All IQ Capture(&R)", Q_NULLPTR));
        actionStop_All_IQ_Capture_O->setText(QApplication::translate("MainWindow", "Stop All IQ Capture(&O)", Q_NULLPTR));
        actionLog_Config_L->setText(QApplication::translate("MainWindow", "Log Config(&L)", Q_NULLPTR));
        actionLog_Viewer_V->setText(QApplication::translate("MainWindow", "Log Viewer(&V)", Q_NULLPTR));
        actionInstrument_Control_I->setText(QApplication::translate("MainWindow", "Instrument Control(&I)", Q_NULLPTR));
        actionCalculator_C->setText(QApplication::translate("MainWindow", "Calculator(&C)", Q_NULLPTR));
        actionNI_IO_Trace_N->setText(QApplication::translate("MainWindow", "NI IO Trace(&N)", Q_NULLPTR));
        actionProgram_FPGAbit_P->setText(QApplication::translate("MainWindow", "Program FPGA bit(&P)", Q_NULLPTR));
        actionAlgorithm_5355_A->setText(QApplication::translate("MainWindow", "Algorithm 5355(&A)", Q_NULLPTR));
        actionFitting_Interpolation_F->setText(QApplication::translate("MainWindow", "Fitting & Interpolation(&F)", Q_NULLPTR));
        actionAbout_A->setText(QApplication::translate("MainWindow", "About(&A)", Q_NULLPTR));
        menuDevice->setTitle(QApplication::translate("MainWindow", "Device(&D)", Q_NULLPTR));
        menuRFU_R->setTitle(QApplication::translate("MainWindow", "RFU(&R)", Q_NULLPTR));
        menuFunction_F->setTitle(QApplication::translate("MainWindow", "Function(&F)", Q_NULLPTR));
        menuTest_T->setTitle(QApplication::translate("MainWindow", "Test(&T)", Q_NULLPTR));
        menuHelp_H->setTitle(QApplication::translate("MainWindow", "Help(&H)", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
