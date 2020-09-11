#ifndef Q_SP1401_R1F_ADV_WIDGET_H
#define Q_SP1401_R1F_ADV_WIDGET_H

#include "global.h"

namespace Ui {
class Q_SP1401_R1F_Adv_Widget;
}

class Q_ADF5355_Dlg;

class Q_SP1401_R1F_Adv_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_SP1401_R1F_Adv_Widget(QWidget *parent = nullptr);
    ~Q_SP1401_R1F_Adv_Widget();

private slots:
    void on_comboBoxTXIOSw2_activated(int index);

    void on_comboBoxTXIOSw1_activated(int index);

    void on_comboBoxTXAtt2_activated(int index);

    void on_comboBoxTXAtt1_activated(int index);

    void on_comboBoxTXFilterSw1_activated(int index);

    void on_comboBoxTXFilterSw2_activated(int index);

    void on_comboBoxTXAtt0_activated(int index);

    void on_pushButtonModEn_clicked();

    void on_pushButtonPowerOn_clicked();

    void on_comboBoxLoopSw_activated(int index);

    void on_pushButtonSetTXLO3_clicked();

    void on_pushButtonSetTXLO2_clicked();

    void on_pushButtonSetTXLO1_clicked();

    void on_pushButtonSetRXLO1_clicked();

    void on_pushButtonAdvTXLO3_clicked();

    void on_pushButtonAdvTXLO2_clicked();

    void on_pushButtonAdvTXLO1_clicked();

    void on_pushButtonAdvRXLO1_clicked();

    void on_comboBoxIOMode_activated(int index);

    void on_checkBoxTXLED_clicked(bool checked);

    void on_checkBoxRXLED_clicked(bool checked);

    void on_pushButtonSetTXFreq_clicked();

    void on_pushButtonSetRXFreq_clicked();

    void on_pushButtonGetAllTemp_clicked();

    void on_pushButtonGetAllDet_clicked();

    void on_comboBoxRXIOSw1_activated(int index);

    void on_comboBoxRXIOSw2_activated(int index);

    void on_comboBoxRXIOSw3_activated(int index);

    void on_comboBoxLNAAttSw_activated(int index);

    void on_comboBoxAtt019Sw_activated(int index);

    void on_comboBoxRXAtt1_activated(int index);

    void on_comboBoxRXAtt2_activated(int index);

    void on_comboBoxIFFilterSw_activated(int index);

    void on_lineEditTXFreq_textChanged(const QString &arg1);

    void on_lineEditRXFreq_textChanged(const QString &arg1);

private:
    Q_ADF5355_Dlg *dlg_ADF5355[3];
    Ui::Q_SP1401_R1F_Adv_Widget *ui;
};

#endif // Q_SP1401_R1F_ADV_WIDGET_H
