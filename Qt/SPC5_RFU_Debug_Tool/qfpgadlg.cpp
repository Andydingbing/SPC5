#include "qfpgadlg.h"
#include "ui_qfpgadlg.h"
#include <QFileDialog>

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
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetK7_0->text().toUInt(0,16);
    uint32_t data = ui->m_LEDataK7_0->text().toUInt(0,16);
    status = _sp2401[1]->get_k7()->w32(pci_dev::AS_BAR0,offset<<2,data);
    Log.add_reg_list(status,_sp2401[0]->get_k7()->get_des(),offset,data);
    if (status)
        ui->m_LEStatusK7_0->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_0->clear();
}

void QFPGADlg::on_m_PBRK7_0_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetK7_0->text().toUInt(0,16);
    uint32_t data = 0;
    status = _sp2401[1]->get_k7()->r32(pci_dev::AS_BAR0,offset<<2,&data);
    ui->m_LEDataK7_0->setText(QString("%1").arg(data,8,16,QLatin1Char('0')));
    Log.add_reg_list(status,_sp2401[1]->get_k7()->get_des(),offset,-1,data);
    if (status)
        ui->m_LEStatusK7_0->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_0->clear();
}

void QFPGADlg::on_m_PBWK7_1_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetK7_1->text().toUInt(0,16);
    uint32_t data = ui->m_LEDataK7_1->text().toUInt(0,16);
    status = _sp2401[0]->get_k7()->w32(pci_dev::AS_BAR0,offset<<2,data);
    Log.add_reg_list(status,_sp2401[1]->get_k7()->get_des(),offset,data);
    if (status)
        ui->m_LEStatusK7_1->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_1->clear();
}

void QFPGADlg::on_m_PBRK7_1_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetK7_1->text().toUInt(0,16);
    uint32_t data = 0;
    status = _sp2401[0]->get_k7()->r32(pci_dev::AS_BAR0,offset<<2,&data);
    ui->m_LEDataK7_1->setText(QString("%1").arg(data,8,16,QLatin1Char('0')));
    Log.add_reg_list(status,_sp2401[0]->get_k7()->get_des(),offset,-1,data);
    if (status)
        ui->m_LEStatusK7_1->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusK7_1->clear();
}

void QFPGADlg::on_m_PBWS6_BAR0_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetS6_BAR0->text().toUInt(0,16);
    uint32_t data = ui->m_LEDataS6_BAR0->text().toUInt(0,16);
    status = _sp2401[0]->get_s6()->w32(pci_dev::AS_BAR0,offset<<2,data);
    Log.add_reg_list(status,_sp2401[0]->get_s6()->get_des(),offset,data);
    if (status)
        ui->m_LEStatusS6_BAR0->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR0->clear();
}

void QFPGADlg::on_m_PBRS6_BAR0_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetS6_BAR0->text().toUInt(0,16);
    uint32_t data = 0;
    status = _sp2401[0]->get_s6()->r32(pci_dev::AS_BAR0,offset<<2,&data);
    ui->m_LEDataS6_BAR0->setText(QString("%1").arg(data,8,16,QLatin1Char('0')));
    Log.add_reg_list(status,_sp2401[0]->get_s6()->get_des(),offset,-1,data);
    if (status)
        ui->m_LEStatusS6_BAR0->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR0->clear();
}

void QFPGADlg::on_m_PBWS6_BAR1_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetS6_BAR0->text().toUInt(0,16);
    uint32_t data = ui->m_LEDataS6_BAR0->text().toUInt(0,16);
    status = _sp2401[0]->get_s6()->w32(pci_dev::AS_BAR1,offset<<2,data);
    Log.add_reg_list(status,_sp2401[0]->get_s6()->get_des(),offset,data);
    if (status)
        ui->m_LEStatusS6_BAR1->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
    else
        ui->m_LEStatusS6_BAR1->clear();
}

void QFPGADlg::on_m_PBRS6_BAR1_clicked()
{
    int32_t status = 0;
    uint32_t offset = ui->m_LEOffsetS6_BAR0->text().toUInt(0,16);
    uint32_t data = 0;
    status = _sp2401[0]->get_s6()->r32(pci_dev::AS_BAR1,offset<<2,&data);
    ui->m_LEDataS6_BAR1->setText(QString("%1").arg(data,8,16,QLatin1Char('0')));
    Log.add_reg_list(status,_sp2401[0]->get_s6()->get_des(),offset,-1,data);
    if (status)
        ui->m_LEStatusS6_BAR1->setText(QString("0x%1").arg(status,8,16,QLatin1Char('0')));
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
    _sp2401[1]->set_fpga_bit(ui->m_LEPathK7_0->toPlainText().toStdString().c_str());
}

void QFPGADlg::on_m_PBProgramK7_1_clicked()
{
	_sp2401[0]->set_fpga_bit(ui->m_LEPathK7_1->toPlainText().toStdString().c_str());
}
