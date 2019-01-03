#include "qframeslottrigwidget.h"
#include "ui_qframeslottrigwidget.h"

QFrameSlotTrigWidget::QFrameSlotTrigWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QFrameSlotTrigWidget)
{
    ui->setupUi(this);

    ui->comboBoxFrameTrigMode->setCurrentIndex(sp2401_r1a::TRIGGER_IN);
    ui->lineEditFrameTrigInOffset->setText("0");
    ui->checkBoxFrameTrigInOffset->setChecked(false);
    ui->lineEditFrameTrigOutOffset->setText("0");
    ui->checkBoxFrameTrigOutOffset->setChecked(false);
    ui->lineEditFrameTime->setText("10");
    ui->comboBoxHyperFrameTrigMode->setCurrentIndex(sp2401_r1a::TRIGGER_IN);
    ui->lineEditHyperFrameTrigInOffset->setText("0");
    ui->checkBoxHyperFrameTrigInOffset->setChecked(false);
    ui->lineEditHyperFrameTrigOutOffset->setText("0");
    ui->checkBoxHyperFrameTrigOutOffset->setChecked(false);
    ui->lineEditHyperFramePeriod->setText("1024");
    ui->lineEditSlotTime->setText("0.5");
    ui->comboBoxFrameTrigSrc->setCurrentIndex(sp2401_r1a::NEXT_FRAME);
    ui->lineEditTrigFrame->setText("0");
    ui->lineEditTrigSlot->setText("0");
}

QFrameSlotTrigWidget::~QFrameSlotTrigWidget()
{
    delete ui;
}

void QFrameSlotTrigWidget::setAll(sp2401_r1a *sp2401)
{
    sp2401_r1a::frame_trig_mode_t frameTrigMode = (sp2401_r1a::frame_trig_mode_t)(ui->comboBoxFrameTrigMode->currentIndex());
    INT_CHECKV(sp2401->set_frame_trig_mode(frameTrigMode));

    bool frameTrigInOffsetEn = ui->checkBoxFrameTrigInOffset->isChecked();
    double frameTrigInOffset = ui->lineEditFrameTrigInOffset->text().toDouble();
    bool frameTrigOutOffsetEn = ui->checkBoxFrameTrigOutOffset->isChecked();
    double frameTrigOutOffset = ui->lineEditFrameTrigOutOffset->text().toDouble();
    INT_CHECKV(sp2401->set_frame_trig_in_offset(frameTrigInOffsetEn, frameTrigInOffset));
    INT_CHECKV(sp2401->set_frame_trig_out_offset(frameTrigOutOffsetEn, frameTrigOutOffset));

    double frameTime = ui->lineEditFrameTime->text().toDouble();
    INT_CHECKV(sp2401->set_frame_time(frameTime));

    sp2401_r1a::hyper_frame_trig_mode_t hyperFrameTrigMode = (sp2401_r1a::hyper_frame_trig_mode_t)(ui->comboBoxHyperFrameTrigMode->currentIndex());
    INT_CHECKV(sp2401->set_hyper_frame_trig_mode(hyperFrameTrigMode));

    bool hyperFrameTrigInOffsetEn = ui->checkBoxHyperFrameTrigInOffset->isChecked();
    double hyperFrameTrigInOffset = ui->lineEditHyperFrameTrigInOffset->text().toDouble();
    bool hyperFrameTrigOutOffsetEn = ui->checkBoxHyperFrameTrigOutOffset->isChecked();
    double hyperFrameTrigOutOffset = ui->lineEditHyperFrameTrigOutOffset->text().toDouble();
    INT_CHECKV(sp2401->set_hyper_frame_trig_in_offset(hyperFrameTrigInOffsetEn, hyperFrameTrigInOffset));
    INT_CHECKV(sp2401->set_hyper_frame_trig_out_offset(hyperFrameTrigOutOffsetEn, hyperFrameTrigOutOffset));

    quint32 hyperFramePeriod = ui->lineEditHyperFramePeriod->text().toUInt();
    INT_CHECKV(sp2401->set_hyper_frame_period(hyperFramePeriod));

    double slotTime = ui->lineEditSlotTime->text().toDouble();
    INT_CHECKV(sp2401->set_slot_time(slotTime));

    sp2401_r1a::frame_trig_src_t frameTrigSrc = (sp2401_r1a::frame_trig_src_t)(ui->comboBoxFrameTrigSrc->currentIndex());
    INT_CHECKV(sp2401->set_frame_trig_src(frameTrigSrc));

    quint32 trigFrame = ui->lineEditTrigFrame->text().toUInt();
    INT_CHECKV(sp2401->set_trig_frame(trigFrame));

    quint32 trigSlot = ui->lineEditTrigSlot->text().toUInt();
    INT_CHECKV(sp2401->set_trig_slot(trigSlot));
}
