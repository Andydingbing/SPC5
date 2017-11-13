#include "qfpgadlg.h"
#include "ui_qfpgadlg.h"
#include <QtWidgets/QFileDialog>

QFPGADlg::QFPGADlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QFPGADlg)
{
    ui->setupUi(this);
}

QFPGADlg::~QFPGADlg()
{
    delete ui;
}

void QFPGADlg::on_m_PBWK7_0_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetK7_0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = ui->m_LEDataK7_0->text().toUInt(Q_NULLPTR,16);
    iStatus = m_pSP2401[1]->GetK7()->W32(IPCIDev::AS_BAR0,uiOffset<<2,uiData);
    Log.AddRegList(iStatus,m_pSP2401[0]->GetK7()->GetDevName(),uiOffset,uiData);
    if (iStatus)
        ui->m_LEStatusK7_0->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_0->clear();
}

void QFPGADlg::on_m_PBRK7_0_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetK7_0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = 0;
    iStatus = m_pSP2401[1]->GetK7()->R32(IPCIDev::AS_BAR0,uiOffset<<2,&uiData);
    ui->m_LEDataK7_0->setText(QString("%1").arg(uiData,8,16,QLatin1Char('0')));
    Log.AddRegList(iStatus,m_pSP2401[1]->GetK7()->GetDevName(),uiOffset,-1,uiData);
    if (iStatus)
        ui->m_LEStatusK7_0->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_0->clear();
}

void QFPGADlg::on_m_PBWK7_1_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetK7_1->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = ui->m_LEDataK7_1->text().toUInt(Q_NULLPTR,16);
    iStatus = m_pSP2401[0]->GetK7()->W32(IPCIDev::AS_BAR0,uiOffset<<2,uiData);
    Log.AddRegList(iStatus,m_pSP2401[1]->GetK7()->GetDevName(),uiOffset,uiData);
    if (iStatus)
        ui->m_LEStatusK7_1->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_1->clear();
}

void QFPGADlg::on_m_PBRK7_1_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetK7_1->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = 0;
    iStatus = m_pSP2401[0]->GetK7()->R32(IPCIDev::AS_BAR0,uiOffset<<2,&uiData);
    ui->m_LEDataK7_1->setText(QString("%1").arg(uiData,8,16,QLatin1Char('0')));
    Log.AddRegList(iStatus,m_pSP2401[0]->GetK7()->GetDevName(),uiOffset,-1,uiData);
    if (iStatus)
        ui->m_LEStatusK7_1->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_1->clear();
}

void QFPGADlg::on_m_PBWS6_BAR0_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetS6_BAR0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = ui->m_LEDataS6_BAR0->text().toUInt(Q_NULLPTR,16);
    iStatus = m_pSP2401[0]->GetS6()->W32(IPCIDev::AS_BAR0,uiOffset<<2,uiData);
    Log.AddRegList(iStatus,m_pSP2401[0]->GetS6()->GetDevName(),uiOffset,uiData);
    if (iStatus)
        ui->m_LEStatusS6_BAR0->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR0->clear();
}

void QFPGADlg::on_m_PBRS6_BAR0_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetS6_BAR0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = 0;
    iStatus = m_pSP2401[0]->GetS6()->R32(IPCIDev::AS_BAR0,uiOffset<<2,&uiData);
    ui->m_LEDataS6_BAR0->setText(QString("%1").arg(uiData,8,16,QLatin1Char('0')));
    Log.AddRegList(iStatus,m_pSP2401[0]->GetS6()->GetDevName(),uiOffset,-1,uiData);
    if (iStatus)
        ui->m_LEStatusS6_BAR0->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR0->clear();
}

void QFPGADlg::on_m_PBWS6_BAR1_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetS6_BAR0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = ui->m_LEDataS6_BAR0->text().toUInt(Q_NULLPTR,16);
    iStatus = m_pSP2401[0]->GetS6()->W32(IPCIDev::AS_BAR1,uiOffset<<2,uiData);
    Log.AddRegList(iStatus,m_pSP2401[0]->GetS6()->GetDevName(),uiOffset,uiData);
    if (iStatus)
        ui->m_LEStatusS6_BAR1->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR1->clear();
}

void QFPGADlg::on_m_PBRS6_BAR1_clicked()
{
    int32_t iStatus = 0;
    uint32_t uiOffset = ui->m_LEOffsetS6_BAR0->text().toUInt(Q_NULLPTR,16);
    uint32_t uiData = 0;
    iStatus = m_pSP2401[0]->GetS6()->R32(IPCIDev::AS_BAR1,uiOffset<<2,&uiData);
    ui->m_LEDataS6_BAR1->setText(QString("%1").arg(uiData,8,16,QLatin1Char('0')));
    Log.AddRegList(iStatus,m_pSP2401[0]->GetS6()->GetDevName(),uiOffset,-1,uiData);
    if (iStatus)
        ui->m_LEStatusS6_BAR1->setText(QString("0x%1").arg(iStatus,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR1->clear();
}

void QFPGADlg::on_m_PBSetPathK7_0_clicked()
{
    QString strPath = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!strPath.isEmpty())
        ui->m_LEPathK7_0->setText(strPath);
}

void QFPGADlg::on_m_PBSetPathK7_1_clicked()
{
    QString strPath = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!strPath.isEmpty())
        ui->m_LEPathK7_1->setText(strPath);
}

void QFPGADlg::on_m_PBProgramK7_0_clicked()
{
    m_pSP2401[1]->SetFpgaBit(ui->m_LEPathK7_0->toPlainText().toStdString().c_str());
}
