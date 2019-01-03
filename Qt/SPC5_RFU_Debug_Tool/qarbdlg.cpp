#include "qarbdlg.h"
#include "qframeslottrigwidget.h"
#include "ui_qframeslottrigwidget.h"
#include "ui_qarbdlg.h"
#include <QFileDialog>

QArbDlg::QArbDlg(QWidget *parent) :
    QRfDlg(parent),
    ui(new Ui::QArbDlg)
{
    ui->setupUi(this);

    ui->comboBoxTrigSrc->setCurrentIndex(basic_sp1401::TS_MANUAL);
    ui->lineEditFreqOffset->setText("0");
    ui->lineEditCycles->setText("1");
    ui->lineEditTrigDelay->setText("0");
    ui->lineEditAddSamplse->setText("0");
    ui->comboBoxRepMode->setCurrentIndex(basic_sp1401::RM_CONTINUOUS);
    ui->lineEditCurSeg->setText("0");
    ui->lineEditCurSample->setText("0");
    ui->lineEditCurCycle->setText("0");
    ui->lineEditCurTime->setText("0");
    ui->lineEditCurFrame->setText("0");
    ui->lineEditCurSlot->setText("0");
}

QArbDlg::~QArbDlg()
{
    delete ui;
}

void QArbDlg::on_pushButtonSelArbFile_clicked()
{
    PTR_CHECKV(_sp3301);
    QString path = QFileDialog::getOpenFileName(this,tr("Select Arb File"),"",tr("Arb Files (*.wv)"));
    if (!path.isEmpty()) {
        ui->textEditArbFilePath->setText(path);
        ui->pushButtonLoad->setEnabled(true);

        char fileInfo[2048] = {0};
        memset(fileInfo,0,sizeof(fileInfo));
        getSP1401()->get_arb_reader()->load(path.toStdString().c_str());
        getSP1401()->get_arb_reader()->get_file_info(fileInfo);
        ui->textEditArbFileInfo->setText(QString("%1").arg(fileInfo));
    }
}

void QArbDlg::on_pushButtonSetAll_clicked()
{
    PTR_CHECKV(_sp3301);

    bool reTrigger = ui->checkBoxReTrig->isChecked();
    bool autoStart = ui->checkBoxAutoStart->isChecked();
    uint32_t trigSrc = ui->comboBoxTrigSrc->currentIndex();
    uint32_t trigDelay = ui->lineEditTrigDelay->text().toUInt();
    INT_CHECKV(getSP1401()->set_arb_trigger(reTrigger, autoStart, trigSrc, trigDelay));

    uint32_t addSamples = ui->lineEditAddSamplse->text().toUInt();
    uint32_t cycles = ui->lineEditCycles->text().toUInt();
    uint32_t repMode = ui->comboBoxRepMode->currentIndex();
    INT_CHECKV(getSP1401()->set_arb_param(addSamples, cycles, repMode));

    ui->widgetFrameSlotTrig->setAll(getSP2401R1A());
}

void QArbDlg::on_pushButtonArbStart_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP1401()->arb_start());
}

void QArbDlg::on_pushButtonArbStop_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP1401()->arb_stop());
}

void QArbDlg::on_pushButtonManualTrig_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP1401()->set_arb_manual_trigger());
}

void QArbDlg::on_pushButtonLoad_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(_sp3301->arb_load(rfIdx,ui->textEditArbFilePath->toPlainText().toStdString().c_str()));
}

void QArbDlg::on_pushButtonQueryState_clicked()
{
    PTR_CHECKV(_sp3301);
    ui->lineEditCurSeg->setText(QString("%1").arg(getSP1401()->get_arb_current_seg()));
    ui->lineEditCurSample->setText(QString("%1").arg(getSP1401()->get_arb_current_sample()));
    ui->lineEditCurCycle->setText(QString("%1").arg(getSP1401()->get_arb_current_cycle()));
    ui->lineEditCurTime->setText(QString("%1").arg(getSP1401()->get_arb_current_timer()));
}

void QArbDlg::on_pushButtonQueryFrameSlot_clicked()
{
    PTR_CHECKV(_sp3301);
    ui->lineEditCurFrame->setText(QString("%1").arg(getSP2401R1A()->get_cur_frame()));
    ui->lineEditCurSlot->setText(QString("%1").arg(getSP2401R1A()->get_cur_slot()));
}

void QArbDlg::on_pushButtonFrameSlotTrigStart_clicked()
{
    PTR_CHECKV(_sp3301);
    INT_CHECKV(getSP2401R1A()->set_frame_slot_trigger_link(sp2401_r1a::DL));
    INT_CHECKV(getSP2401R1A()->set_frame_slot_trigger_start());
}
