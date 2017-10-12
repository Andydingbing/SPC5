#ifndef QFPGADLG_H
#define QFPGADLG_H

#include <QDialog>
#include "libSP3301.h"

namespace Ui {
class QFPGADlg;
}

class QFPGADlg : public QDialog
{
    Q_OBJECT

public:
    CSP2401R1A *m_pSP2401[2];
public:
    explicit QFPGADlg(QWidget *parent = 0);
    ~QFPGADlg();

private slots:
    void on_m_PBWK7_0_clicked();
    void on_m_PBRK7_0_clicked();
    void on_m_PBWK7_1_clicked();
    void on_m_PBRK7_1_clicked();
    void on_m_PBWS6_BAR0_clicked();
    void on_m_PBRS6_BAR0_clicked();
    void on_m_PBWS6_BAR1_clicked();
    void on_m_PBRS6_BAR1_clicked();
    void on_m_PBSetPathK7_0_clicked();

    void on_m_PBSetPathK7_1_clicked();

    void on_m_PBProgramK7_0_clicked();

private:
    Ui::QFPGADlg *ui;
};

#endif // QFPGADLG_H
