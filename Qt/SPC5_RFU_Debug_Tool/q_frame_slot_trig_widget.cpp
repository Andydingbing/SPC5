#include "q_frame_slot_trig_widget.h"
#include "ui_q_frame_slot_trig_widget.h"

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
    ui->comboBoxHFTrigMode->setCurrentIndex(sp2401_r1a::TRIGGER_IN);
    ui->lineEditHFTrigInOffset->setText("0");
    ui->checkBoxHFTrigInOffset->setChecked(false);
    ui->lineEditHFTrigOutOffset->setText("0");
    ui->checkBoxHFTrigOutOffset->setChecked(false);
    ui->lineEditHFPeriod->setText("1024");
    ui->lineEditSlotTime->setText("0.5");
    ui->comboBoxFrameTrigSrc->setCurrentIndex(sp2401_r1a::NEXT_FRAME);
    ui->lineEditTrigFrame->setText("0");
    ui->lineEditTrigSlot->setText("0");
    ui->lineEditTrigX->setText("1");
    ui->lineEditTrigY->setText("1");
}

QFrameSlotTrigWidget::~QFrameSlotTrigWidget()
{
    delete ui;
}

void QFrameSlotTrigWidget::setAll(sp2401_r1a *sp2401)
{
    sp2401_r1a::frame_trig_mode_t frameTrigMode =
            sp2401_r1a::frame_trig_mode_t(ui->comboBoxFrameTrigMode->currentIndex());
    INT_CHECKV(sp2401->set_frame_trig_mode(frameTrigMode));

    bool frameTrigInOffsetEn = ui->checkBoxFrameTrigInOffset->isChecked();
    double frameTrigInOffset = ui->lineEditFrameTrigInOffset->text().toDouble();
    bool frameTrigOutOffsetEn = ui->checkBoxFrameTrigOutOffset->isChecked();
    double frameTrigOutOffset = ui->lineEditFrameTrigOutOffset->text().toDouble();
    INT_CHECKV(sp2401->set_frame_trig_in_offset(frameTrigInOffsetEn,frameTrigInOffset));
    INT_CHECKV(sp2401->set_frame_trig_out_offset(frameTrigOutOffsetEn,frameTrigOutOffset));

    double frameTime = ui->lineEditFrameTime->text().toDouble();
    INT_CHECKV(sp2401->set_frame_time(frameTime));

    sp2401_r1a::hyper_frame_trig_mode_t hyperFrameTrigMode =
            sp2401_r1a::hyper_frame_trig_mode_t(ui->comboBoxHFTrigMode->currentIndex());
    INT_CHECKV(sp2401->set_hyper_frame_trig_mode(hyperFrameTrigMode));

    bool hyperFrameTrigInOffsetEn = ui->checkBoxHFTrigInOffset->isChecked();
    double hyperFrameTrigInOffset = ui->lineEditHFTrigInOffset->text().toDouble();
    bool hyperFrameTrigOutOffsetEn = ui->checkBoxHFTrigOutOffset->isChecked();
    double hyperFrameTrigOutOffset = ui->lineEditHFTrigOutOffset->text().toDouble();
    INT_CHECKV(sp2401->set_hyper_frame_trig_in_offset(hyperFrameTrigInOffsetEn, hyperFrameTrigInOffset));
    INT_CHECKV(sp2401->set_hyper_frame_trig_out_offset(hyperFrameTrigOutOffsetEn, hyperFrameTrigOutOffset));

    quint32 hyperFramePeriod = ui->lineEditHFPeriod->text().toUInt();
    INT_CHECKV(sp2401->set_hyper_frame_period(hyperFramePeriod));

    double slotTime = ui->lineEditSlotTime->text().toDouble();
    INT_CHECKV(sp2401->set_slot_time(slotTime));

    sp2401_r1a::frame_trig_src_t frameTrigSrc =
            sp2401_r1a::frame_trig_src_t(ui->comboBoxFrameTrigSrc->currentIndex());
    INT_CHECKV(sp2401->set_frame_trig_src(frameTrigSrc));

    quint32 trigFrame = ui->lineEditTrigFrame->text().toUInt();
    INT_CHECKV(sp2401->set_trig_frame(trigFrame));

    quint32 trigSlot = ui->lineEditTrigSlot->text().toUInt();
    INT_CHECKV(sp2401->set_trig_slot(trigSlot));

    quint16 trigX = ui->lineEditTrigX->text().toUInt();
    quint16 trigY = ui->lineEditTrigY->text().toUInt();
    INT_CHECKV(sp2401->set_frame_trig_mod_x_y(trigX,trigY));
}
