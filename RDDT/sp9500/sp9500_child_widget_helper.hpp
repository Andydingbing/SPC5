#ifndef SP9500_CHILD_WIDGET_HELPER_HPP
#define SP9500_CHILD_WIDGET_HELPER_HPP

#include "child_widget_helper.h"
#include "rf_container_widget.h"
#include "sp1401_r1a_adv_widget.h"
#include "sp1401_r1c_adv_widget.h"
#include "sp1401_r1f_adv_widget.h"
#include "sp1401_r1a_widget.h"
#include "sp1401_r1c_widget.h"
#include "sp1401_r1f_widget.h"
#include "sp2401_r1a_widget.h"
#include "arb_widget.h"
#include "sp9500_iq_cap_config_widget.h"
#include "signal_analyzer_widget.h"
#include "sp9500_fpga_widget.h"
#include "sp3103.h"

typedef Q_SP1401_Container_Widget<Q_SP1401_R1A_Widget> Q_SP1401_R1A_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_SP1401_R1C_Widget> Q_SP1401_R1C_Container_Widget;
typedef Q_SP1401_Container_Widget<Q_SP1401_R1F_Widget> Q_SP1401_R1F_Container_Widget;

namespace NS_SP9500 {

class ChildWidgets : public ChildWidgetHelper
{
    Q_OBJECT

public:
    explicit ChildWidgets(MainWindow *parent) :
        ChildWidgetHelper(parent) {}

    void initMenu()
    {
        // Device
        actionInit = new QAction("Init(&I)",parent);
        actionInit->setStatusTip("Init Device");
        connect(actionInit,SIGNAL(triggered(bool)),parent,SLOT(deviceInit()));

        actionPCIE_Restart = new QAction("PCIE Restart(&P)",parent);
        actionPCIE_Restart->setStatusTip("Restart PCIE Devices");

        actionTuneOCXO = new QAction("Tune OCXO(&T)",parent);
        actionTuneOCXO->setStatusTip("Tune OCXO");

        actionExit = new QAction("Exit(&X)",parent);
        actionExit->setStatusTip("Exit");
        connect(actionExit,SIGNAL(triggered(bool)),parent,SLOT(exit()));

        menuDevice = new QMenu("Device(&D)",ui->menuBar);
        menuDevice->addAction(actionInit);
        menuDevice->addAction(actionPCIE_Restart);
        menuDevice->addAction(actionTuneOCXO);
        menuDevice->addAction(actionExit);

        // RFU
        actionRFU0 = new QAction("RFU0(&0)",parent);
        actionRFU0->setCheckable(true);

        actionRFU1 = new QAction("RFU1(&1)",parent);
        actionRFU1->setCheckable(true);

        actionRFU2 = new QAction("RFU2(&2)",parent);
        actionRFU2->setCheckable(true);

        actionRFU3 = new QAction("RFU3(&3)",parent);
        actionRFU3->setCheckable(true);

        actionRFU4 = new QAction("RFU4(&4)",parent);
        actionRFU4->setCheckable(true);

        connect(actionRFU0,SIGNAL(triggered(bool)),this,SLOT(selSP3301_0()));
        connect(actionRFU1,SIGNAL(triggered(bool)),this,SLOT(selSP3301_1()));
        connect(actionRFU2,SIGNAL(triggered(bool)),this,SLOT(selSP3301_2()));
        connect(actionRFU3,SIGNAL(triggered(bool)),this,SLOT(selSP3301_3()));
        connect(actionRFU4,SIGNAL(triggered(bool)),this,SLOT(selSP3301_4()));

        actionRFU2->setChecked(true);

        menuRFU = new QMenu("RFU(&R)",ui->menuBar);
        menuRFU->addAction(actionRFU0);
        menuRFU->addAction(actionRFU1);
        menuRFU->addAction(actionRFU2);
        menuRFU->addAction(actionRFU3);
        menuRFU->addAction(actionRFU4);

        // Function
        actionArbViewer = new QAction("Arb Viewer(&A)",parent);

        actionStarAllIQ_Capture = new QAction("Start All IQ Capture(&R)",parent);
        connect(actionStarAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(starAllIQCapture()));

        actionStopAllIQ_Capture = new QAction("Stop All IQ Capture(&O)",parent);
        connect(actionStopAllIQ_Capture,SIGNAL(triggered(bool)),parent,SLOT(stopAllIQCapture()));

        actionLogConfig = new QAction("Log Config(&L)",parent);
        actionLogViewer = new QAction("Log Viewer(&V)",parent);

        menuFunction = new QMenu("Function(&F)",ui->menuBar);
        menuFunction->addAction(actionArbViewer);
        menuFunction->addAction(actionStarAllIQ_Capture);
        menuFunction->addAction(actionStopAllIQ_Capture);
        menuFunction->addAction(actionLogConfig);
        menuFunction->addAction(actionLogViewer);

        // Test
        actionProgramFPGAbit = new QAction("Program FPGA bit(&P)",parent);
        actionAlgorithm_5355 = new QAction("Algorithm 5355(&A)",parent);
        actionFittingInterpolation = new QAction("Fitting & Interpolation(&F)",parent);

        menuTest = new QMenu("Test(&T)",ui->menuBar);
        menuTest->addAction(actionProgramFPGAbit);
        menuTest->addAction(actionAlgorithm_5355);
        menuTest->addAction(actionFittingInterpolation);

        // About
        actionAbout = new QAction("About(&A)",parent);

        menuHelp = new QMenu("Help(&H)",ui->menuBar);
        menuHelp->addAction(actionAbout);

        // Menu
        ui->menuBar->addAction(menuDevice->menuAction());
        ui->menuBar->addAction(menuRFU->menuAction());
        ui->menuBar->addAction(menuFunction->menuAction());
        ui->menuBar->addAction(menuTest->menuAction());
        ui->menuBar->addAction(menuHelp->menuAction());
    }

    void initMainTreeWidget()
    {
        QStringList str;

        DECL_TREE_ITEM(str,"RF-R1F" << "Overview",widget_SP1401_R1F_Container);
        DECL_TREE_ITEM(str,"RF-R1F" << "Advance",widget_SP1401_R1F_Adv);
        DECL_TREE_ITEM(str,"RF-R1F" << "Test",widget_Test_R1C);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration",widget_Cal_R1C);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-LOLeakage",widget_Cal_R1C_TX_LOLeak);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-Sideband",widget_Cal_R1C_TX_SB);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-Filter",widget_Cal_R1C_TX_Filter);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-BasePower",widget_Cal_R1C_TX_Pwr);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-Attenuation",widget_Cal_R1C_TX_Att);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-FilterOffset",widget_Cal_R1C_TX_FilterOffset);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "RX-Filter",widget_Cal_R1C_RX_Filter);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "RX-Reference",widget_Cal_R1C_RX_Ref);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "RX-Attenuation",widget_Cal_R1C_RX_Att);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "RX-FilterOffset",widget_Cal_R1C_RX_FilterOffset);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "TX-TempComp",widget_Cal_R1C_TX_TempComp);
        DECL_TREE_ITEM(str,"RF-R1F" << "Calibration" << "RX-TempComp",widget_Cal_R1C_RX_TempComp);

        DECL_TREE_ITEM(str,"BB-Debug" << "OverView",widget_SP2401_R1A);
        DECL_TREE_ITEM(str,"BB-Debug" << "Arb",widget_ARB);
        DECL_TREE_ITEM(str,"BB-Debug" << "IQ-Capture",widget_IQ_Cap);
        DECL_TREE_ITEM(str,"BB-Debug" << "FPGA",widget_FPGA);

        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Overview",widget_SP1401_R1C_Container);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Advance",widget_SP1401_R1C_Adv);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Test",widget_Test_R1C);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-LOLeakage",widget_Cal_R1C_TX_LOLeak);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-Sideband",widget_Cal_R1C_TX_SB);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-Filter",widget_Cal_R1C_TX_Filter);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-BasePower",widget_Cal_R1C_TX_Pwr);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-Attenuation",widget_Cal_R1C_TX_Att);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-FilterOffset",widget_Cal_R1C_TX_FilterOffset);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "RX-Filter",widget_Cal_R1C_RX_Filter);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "RX-Reference",widget_Cal_R1C_RX_Ref);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "RX-Attenuation",widget_Cal_R1C_RX_Att);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "RX-FilterOffset",widget_Cal_R1C_RX_FilterOffset);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "TX-TempComp",widget_Cal_R1C_TX_TempComp);
        DECL_TREE_ITEM(str,"RF-R1C/D/E" << "Calibration" << "RX-TempComp",widget_Cal_R1C_RX_TempComp);

        DECL_TREE_ITEM(str,"RF-R1A/B" << "Overview",widget_SP1401_R1A_Container);
        DECL_TREE_ITEM(str,"RF-R1A/B" << "Advance",widget_SP1401_R1A_Adv);

        setMainTree(_items);
    }

    void addChildWidgets()
    {
        ADD_CHILD_WIDGET(widget_SP1401_R1A_Container,Q_SP1401_R1A_Container_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP1401_R1C_Container,Q_SP1401_R1C_Container_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP1401_R1F_Container,Q_SP1401_R1F_Container_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP1401_R1A_Adv,Q_SP1401_R1A_Adv_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP1401_R1C_Adv,Q_SP1401_R1C_Adv_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP1401_R1F_Adv,Q_SP1401_R1F_Adv_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_SP2401_R1A,Q_SP2401_R1A_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_ARB,Q_ARB_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_IQ_Cap,Q_SA_Widget,ns_sp9500::g_max_rf);
        ADD_CHILD_WIDGET(widget_FPGA,NS_SP9500::Q_FPGA_Widget,1);

        ADD_CHILD_WIDGET(widget_Cal_R1C,QCalR1CDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_LOLeak,QCalR1CTXLOLeakDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_SB,QCalR1CTXSBDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Filter,QCalR1CTXFilterDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Pwr,QCalR1CTXPwrDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_Att,QCalR1CTXAttDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_FilterOffset,QCalR1CTXFilterOffsetDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Filter,QCalR1CRXFilterDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Ref,QCalR1CRXRefDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_Att,QCalR1CRXAttDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_FilterOffset,QCalR1CRXFilterOffsetDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_TX_TempComp,QCalR1CTxTempCompDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Cal_R1C_RX_TempComp,QCalR1CRXTempCompDlg,ns_sp9500::g_max_rf)
        ADD_CHILD_WIDGET(widget_Test_R1C,QTestR1CTabWidget,ns_sp9500::g_max_rf)
    }

    void updatePtr()
    {
        if (SP3301 != nullptr) {
            RFUIdx = SP3301->get_rfu_idx();
            SP1401 = SP3301->get_sp1401(RFIdx);
            SP1401_R1A = SP3301->get_sp1401_r1a(RFIdx);
            SP1401_R1B = SP3301->get_sp1401_r1b(RFIdx);
            SP1401_R1C = SP3301->get_sp1401_r1c(RFIdx);
            SP1401_R1D = SP3301->get_sp1401_r1d(RFIdx);
            SP1401_R1E = SP3301->get_sp1401_r1e(RFIdx);
            SP1401_R1F = SP3301->get_sp1401_r1f(RFIdx);
            SP2401 = SP3301->get_sp2401(RFIdx);
            widget_IQ_Cap[RFIdx]->_data_I._sequence = complexSequence;
            widget_IQ_Cap[RFIdx]->_data_Q._sequence = complexSequence;
            widget_IQ_Cap[RFIdx]->_data_DFT._sequence = complexSequence;
        }
    }

    void mainTabCurrentChanged(int index)
    {
        if (complexSequence == nullptr) {
            return;
        }

        complexSequence->_new(widget_IQ_Cap[index]->widget_Config[project]->samples());

        for (quint8 i = 0;i < ns_sp9500::g_max_rf;++i) {
            if (i != index) {
                SP3301->set_iq_cap_buffer(i,nullptr,nullptr);
            } else {
                SP3301->set_iq_cap_buffer(i,complexSequence->i(),complexSequence->q());
            }
        }
    }

public slots:
    void initChildWidgets()
    {
        updatePtr();
        for (quint8 i = 0;i < ns_sp9500::g_max_rf;++i) {
            widget_SP1401_R1A_Container[i]->widget->init();
            widget_SP1401_R1C_Container[i]->widget->init();
            widget_SP1401_R1F_Container[i]->widget->init();
            widget_ARB[i]->init();
            widget_IQ_Cap[i]->init();
        }
        updatePtr();
        mainTabCurrentChanged(0);
    }

    void selSP3301_0()
    {
        SP3301 = &SP3301_0;
        updatePtr();
        actionRFU0->setChecked(true);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU0");
    }

    void selSP3301_1()
    {
        SP3301 = &SP3301_1;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(true);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU1");
    }

    void selSP3301_2()
    {
        SP3301 = &SP3301_2;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(true);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU2");
    }

    void selSP3301_3()
    {
        SP3301 = &SP3301_3;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(true);
        actionRFU4->setChecked(false);
        Log.add_msg(0,"Working on RFU3");
    }

    void selSP3301_4()
    {
        SP3301 = &SP3301_4;
        updatePtr();
        actionRFU0->setChecked(false);
        actionRFU1->setChecked(false);
        actionRFU2->setChecked(false);
        actionRFU3->setChecked(false);
        actionRFU4->setChecked(true);
        Log.add_msg(0,"Working on RFU4");
    }

public:
    QList<Q_SP1401_R1A_Container_Widget *> widget_SP1401_R1A_Container;
    QList<Q_SP1401_R1A_Adv_Widget *>       widget_SP1401_R1A_Adv;
    QList<Q_SP1401_R1C_Container_Widget *> widget_SP1401_R1C_Container;
    QList<Q_SP1401_R1C_Adv_Widget *>       widget_SP1401_R1C_Adv;
    QList<Q_SP1401_R1F_Container_Widget *> widget_SP1401_R1F_Container;
    QList<Q_SP1401_R1F_Adv_Widget *>       widget_SP1401_R1F_Adv;

    QList<Q_SP2401_R1A_Widget *>      widget_SP2401_R1A;
    QList<Q_ARB_Widget *>             widget_ARB;
    QList<Q_SA_Widget *>              widget_IQ_Cap;
    QList<NS_SP9500::Q_FPGA_Widget *> widget_FPGA;

    QList<QCalR1CDlg *>               widget_Cal_R1C;
    QList<QCalR1CTXLOLeakDlg *>       widget_Cal_R1C_TX_LOLeak;
    QList<QCalR1CTXSBDlg *>           widget_Cal_R1C_TX_SB;
    QList<QCalR1CTXFilterDlg *>       widget_Cal_R1C_TX_Filter;
    QList<QCalR1CTXPwrDlg *>          widget_Cal_R1C_TX_Pwr;
    QList<QCalR1CTXAttDlg *>          widget_Cal_R1C_TX_Att;
    QList<QCalR1CTXFilterOffsetDlg *> widget_Cal_R1C_TX_FilterOffset;
    QList<QCalR1CRXFilterDlg *>       widget_Cal_R1C_RX_Filter;
    QList<QCalR1CRXRefDlg *>          widget_Cal_R1C_RX_Ref;
    QList<QCalR1CRXAttDlg *>          widget_Cal_R1C_RX_Att;
    QList<QCalR1CRXFilterOffsetDlg *> widget_Cal_R1C_RX_FilterOffset;
    QList<QCalR1CTxTempCompDlg *>     widget_Cal_R1C_TX_TempComp;
    QList<QCalR1CRXTempCompDlg *>     widget_Cal_R1C_RX_TempComp;
    QList<QTestR1CTabWidget *>        widget_Test_R1C;

private:
    QMenu *menuDevice;
    QAction *actionInit;
    QAction *actionPCIE_Restart;
    QAction *actionTuneOCXO;
    QAction *actionExit;

    QMenu *menuRFU;
    QAction *actionRFU0;
    QAction *actionRFU1;
    QAction *actionRFU2;
    QAction *actionRFU3;
    QAction *actionRFU4;

    QMenu *menuFunction;
    QAction *actionArbViewer;
    QAction *actionStarAllIQ_Capture;
    QAction *actionStopAllIQ_Capture;
    QAction *actionLogConfig;
    QAction *actionLogViewer;

    QMenu *menuTest;
    QAction *actionProgramFPGAbit;
    QAction *actionAlgorithm_5355;
    QAction *actionFittingInterpolation;

    QMenu *menuHelp;
    QAction *actionAbout;
};

} // namespace NS_SP9500

#endif // SP9500_CHILD_WIDGET_HELPER_HPP
