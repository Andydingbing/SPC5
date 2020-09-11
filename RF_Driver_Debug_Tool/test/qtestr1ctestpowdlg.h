#ifndef QTESTR1CTESTPOWDLG_H
#define QTESTR1CTESTPOWDLG_H

#include "qtestbasedlg.h"
#include "qr1ctestpowmodel.h"
#include "global.h"

namespace Ui {
class QTestR1CTestPowDlg;
}

class QTestR1CTestPowDlg : public QTestR1CBaseDlg
{
    Q_OBJECT
    DECLARE_TEST_R1C_DLG

public:
    explicit QTestR1CTestPowDlg(QWidget *parent = 0);
    ~QTestR1CTestPowDlg();
    qint8 tabIdxRf;
public slots:
     void updateCalUI(QList<QString> list);
private:
     Ui::QTestR1CTestPowDlg *ui;
};

#endif // QTESTR1CTESTPOWDLG_H
