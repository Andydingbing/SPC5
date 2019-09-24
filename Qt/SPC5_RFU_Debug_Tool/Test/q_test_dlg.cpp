#include "q_test_dlg.h"
#include "q_rdt_tableview.h"
#include "q_test_freq_res_widget.h"
#include "q_plot.h"

void QTestR1CFreqResDlg::init()
{
    ui->lineEditTXFreqStar->setText(QString("%1M").arg(RF_TX_FREQ_STAR / 1e6));
    ui->lineEditTXFreqStop->setText(QString("%1M").arg(RF_TX_FREQ_STOP / 1e6));
    ui->lineEditTXFreqStep->setText(QString("10M"));

    ui->lineEditRXFreqStar->setText(QString("%1M").arg(RF_RX_FREQ_STAR / 1e6));
    ui->lineEditRXFreqStop->setText(QString("%1M").arg(RF_RX_FREQ_STOP / 1e6));
    ui->lineEditRXFreqStep->setText(QString("10M"));

    addIdleWidget(ui->lineEditTXFreqStar);
    addIdleWidget(ui->lineEditTXFreqStop);
    addIdleWidget(ui->lineEditTXFreqStep);
    addIdleWidget(ui->lineEditRXFreqStar);
    addIdleWidget(ui->lineEditRXFreqStop);
    addIdleWidget(ui->lineEditRXFreqStep);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    widgetResume();
    QTestBaseDlg::init();
}

void QTestR1CFreqResDlg::resetShowWidget(TestBaseParam *param)
{
    ui->widgetData->resetShowWidget(param);
    emit reset();
}

void QTestR1CFreqResDlg::uiToTestParam(TestBaseParam *param)
{
    TestFreqResParam *p = static_cast<TestFreqResParam *>(param);
    p->parent = this;
    p->SP1401 = SP1401;
    p->SP2401 = SP2401;
    p->plotData_0 = ui->widgetData->dataRF_TX;
    p->plotData_1 = ui->widgetData->dataIF_TX;
    p->plotData_2 = ui->widgetData->dataRF_RX;
    p->plotData_3 = ui->widgetData->dataIF_RX;
    p->RF_TXFreqStar = ui->lineEditTXFreqStar->text();
    p->RF_TXFreqStop = ui->lineEditTXFreqStop->text();
    p->RF_TXFreqStep = ui->lineEditTXFreqStep->text();
    p->RF_RXFreqStar = ui->lineEditRXFreqStar->text();
    p->RF_RXFreqStop = ui->lineEditRXFreqStop->text();
    p->RF_RXFreqStep = ui->lineEditRXFreqStep->text();
    p->isTXAuto = ui->comboBoxTX_RF_IF->currentIndex() == 0;
    p->isRXAuto = ui->comboBoxRX_RF_IF->currentIndex() == 0;

    p->isTestRF_TX =
            ui->comboBoxTX_RF_IF->currentIndex() == 0 ||
            ui->comboBoxTX_RF_IF->currentIndex() == 2 ||
            ui->comboBoxTX_RF_IF->currentIndex() == 4;
    p->isTestIF_TX =
            ui->comboBoxTX_RF_IF->currentIndex() == 0 ||
            ui->comboBoxTX_RF_IF->currentIndex() == 3 ||
            ui->comboBoxTX_RF_IF->currentIndex() == 4;
    p->isTestRF_RX =
            ui->comboBoxRX_RF_IF->currentIndex() == 0 ||
            ui->comboBoxRX_RF_IF->currentIndex() == 2 ||
            ui->comboBoxRX_RF_IF->currentIndex() == 4;
    p->isTestIF_RX =
            ui->comboBoxRX_RF_IF->currentIndex() == 0 ||
            ui->comboBoxRX_RF_IF->currentIndex() == 3 ||
            ui->comboBoxRX_RF_IF->currentIndex() == 4;
}

void QTestR1CFreqResDlg::update(const QModelIndex &tl, const QModelIndex &br, test_item_t item, int sec)
{
    Q_UNUSED(tl);
    Q_UNUSED(br);

    if (item == TI_RF_TX_FREQ_RES) {
        ui->widgetData->plotRF->replot();
        ui->resultRF_TX->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_IF_TX_FREQ_RES) {
        ui->widgetData->plotIF->replot();
        ui->resultIF_TX->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_RF_RX_FREQ_RES) {
        ui->widgetData->plotRF->replot();
        ui->resultRF_RX->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_IF_RX_FREQ_RES) {
        ui->widgetData->plotIF->replot();
        ui->resultIF_RX->setColor(sec != 0 ? Qt::green : Qt::red);
    }
}


void QTestR1CRFDlg::init()
{
    ui->checkBoxTX->setChecked(true);

    addIdleWidget(ui->checkBoxTX);
    addIdleWidget(ui->checkBoxTX_IOSw);
    addIdleWidget(ui->checkBoxTX_PwrModSw);
    addIdleWidget(ui->checkBoxTX_FilterSw);
    addIdleWidget(ui->checkBoxTX_LOLD);
    addIdleWidget(ui->checkBoxTX_PhaseNoise);
    addIdleWidget(ui->checkBoxTX_NoiseFloor);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    widgetResume();
    QTestBaseDlg::init();
}

void QTestR1CRFDlg::resetShowWidget(TestBaseParam *param)
{
    TestRFParam *p = static_cast<TestRFParam *>(param);
}

void QTestR1CRFDlg::uiToTestParam(TestBaseParam *param)
{
    TestRFParam *p = static_cast<TestRFParam *>(param);
    p->parent = this;
    p->SP1401 = SP1401;
    p->SP2401 = SP2401;
    p->isTestTX_IOSw       = ui->checkBoxTX_IOSw->isChecked();
    p->isTestTX_PwrModSw   = ui->checkBoxTX_PwrModSw->isChecked();
    p->isTestTX_FilterSw   = ui->checkBoxTX_FilterSw->isChecked();
    p->isTestTX_LOLD       = ui->checkBoxTX_LOLD->isChecked();
    p->isTestTX_PhaseNoise = ui->checkBoxTX_PhaseNoise->isChecked();
    p->isTestTX_NoiseFloor = ui->checkBoxTX_NoiseFloor->isChecked();
}

void QTestR1CRFDlg::update(const QModelIndex &tl, const QModelIndex &br, test_item_t item, int sec)
{
    Q_UNUSED(tl);
    Q_UNUSED(br);

    if (item == TI_TX_IO_SW) {
        ui->resultTX_IOSw->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_TX_PWR_MOD_SW) {
        ui->resultTX_PwrModSw->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_TX_FILTER_SW) {
        ui->resultTX_FilterSw->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_TX_LO_LD) {
        ui->resultTX_LOLD->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_TX_PHASE_NOISE) {
        ui->resultTX_PhaseNoise->setColor(sec != 0 ? Qt::green : Qt::red);
    }
    if (item == TI_TX_NOISE_FLOOR) {
        ui->resultTX_NoiseFloor->setColor(sec != 0 ? Qt::green : Qt::red);
    }
}

void QTestR1CTempPwrDlg::init()
{
    addIdleWidget(ui->lineEditFreqStar);
    addIdleWidget(ui->lineEditFreqStop);
    addIdleWidget(ui->lineEditFreqStep);
    addIdleWidget(ui->pushButtonStar);
    addRunningWidget(ui->pushButtonStop);
    widgetResume();
    QTestBaseDlg::init();
}

void QTestR1CTempPwrDlg::resetShowWidget(TestBaseParam *param)
{
    ui->tabWidget->clear();

    TestTempPwrParam *p = static_cast<TestTempPwrParam *>(param);

    range_freq_string freqString;
    range_freq<quint64> freqRange;
    freqString.star = p->FreqStar.toStdString();
    freqString.stop = p->FreqStop.toStdString();
    freqString.step = p->FreqStep.toStdString();

    parse_range_freq_string(freqString,freqRange);

    p->data.clear();
    for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
        QTestTempPwrWidget *childWidget = new QTestTempPwrWidget;
        QString tabName = QString::fromStdString(freq_string_from(freqRange.freqs.at(i)));
        childWidget->resetShowWidget(param);
        ui->tabWidget->addTab(childWidget,tabName);
        p->data.push_back(&childWidget->data);
    }
}

void QTestR1CTempPwrDlg::uiToTestParam(TestBaseParam *param)
{
    TestTempPwrParam *p = static_cast<TestTempPwrParam *>(param);
    p->parent = this;
    p->SP1401 = SP1401;
    p->SP2401 = SP2401;
    p->SP3301 = SP3301;
    p->FreqStar = ui->lineEditFreqStar->text();
    p->FreqStop = ui->lineEditFreqStop->text();
    p->FreqStep = ui->lineEditFreqStep->text();
}

void QTestR1CTempPwrDlg::update(const QModelIndex &tl, const QModelIndex &br, test_item_t item, int sec)
{
    Q_UNUSED(tl);
    Q_UNUSED(br);
    Q_UNUSED(sec);
    int tabIdx = int(item);
    ui->tabWidget->setCurrentIndex(tabIdx);
    dynamic_cast<QTestTempPwrWidget *>(ui->tabWidget->widget(tabIdx))->plot->replot();
}
