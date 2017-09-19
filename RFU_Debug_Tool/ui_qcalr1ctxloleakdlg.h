/********************************************************************************
** Form generated from reading UI file 'qcalr1ctxloleakdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QCALR1CTXLOLEAKDLG_H
#define UI_QCALR1CTXLOLEAKDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QCalR1CTxLOLeakDlg
{
public:
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *m_LEFreqStar;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *m_LEFreqStop;
    QTextEdit *m_TEPath;
    QPushButton *m_PBExport;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *m_LEFreqStep;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *m_CBCalRef;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *m_RBSpec;
    QRadioButton *m_RBLoop;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout_4;
    QPushButton *m_PBStar;
    QPushButton *m_PBPaus;
    QVBoxLayout *verticalLayout_5;
    QPushButton *m_PBStop;
    QPushButton *m_PBCont;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *m_LEFreq;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *m_LEDCI;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_4;
    QLineEdit *m_LEDCQ;
    QPushButton *m_PBGetSet;
    QSpacerItem *verticalSpacer_2;
    QTableView *m_TVData;

    void setupUi(QDialog *QCalR1CTxLOLeakDlg)
    {
        if (QCalR1CTxLOLeakDlg->objectName().isEmpty())
            QCalR1CTxLOLeakDlg->setObjectName(QStringLiteral("QCalR1CTxLOLeakDlg"));
        QCalR1CTxLOLeakDlg->resize(759, 479);
        verticalLayout_7 = new QVBoxLayout(QCalR1CTxLOLeakDlg);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(QCalR1CTxLOLeakDlg);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(84, 16));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        m_LEFreqStar = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEFreqStar->setObjectName(QStringLiteral("m_LEFreqStar"));

        horizontalLayout->addWidget(m_LEFreqStar);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(QCalR1CTxLOLeakDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(84, 16));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        m_LEFreqStop = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEFreqStop->setObjectName(QStringLiteral("m_LEFreqStop"));

        horizontalLayout_2->addWidget(m_LEFreqStop);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        m_TEPath = new QTextEdit(QCalR1CTxLOLeakDlg);
        m_TEPath->setObjectName(QStringLiteral("m_TEPath"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_TEPath->sizePolicy().hasHeightForWidth());
        m_TEPath->setSizePolicy(sizePolicy1);
        m_TEPath->setMaximumSize(QSize(16777215, 49));
        m_TEPath->setReadOnly(true);

        horizontalLayout_3->addWidget(m_TEPath);

        m_PBExport = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBExport->setObjectName(QStringLiteral("m_PBExport"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(m_PBExport->sizePolicy().hasHeightForWidth());
        m_PBExport->setSizePolicy(sizePolicy2);

        horizontalLayout_3->addWidget(m_PBExport);

        horizontalLayout_3->setStretch(0, 10);
        horizontalLayout_3->setStretch(1, 3);
        horizontalLayout_3->setStretch(2, 1);

        verticalLayout_7->addLayout(horizontalLayout_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(QCalR1CTxLOLeakDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(84, 16));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        m_LEFreqStep = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEFreqStep->setObjectName(QStringLiteral("m_LEFreqStep"));
        sizePolicy1.setHeightForWidth(m_LEFreqStep->sizePolicy().hasHeightForWidth());
        m_LEFreqStep->setSizePolicy(sizePolicy1);
        m_LEFreqStep->setReadOnly(true);

        horizontalLayout_4->addWidget(m_LEFreqStep);


        verticalLayout_6->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        m_CBCalRef = new QCheckBox(QCalR1CTxLOLeakDlg);
        m_CBCalRef->setObjectName(QStringLiteral("m_CBCalRef"));
        sizePolicy1.setHeightForWidth(m_CBCalRef->sizePolicy().hasHeightForWidth());
        m_CBCalRef->setSizePolicy(sizePolicy1);

        verticalLayout_3->addWidget(m_CBCalRef);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_3->addItem(verticalSpacer);


        horizontalLayout_10->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        m_RBSpec = new QRadioButton(QCalR1CTxLOLeakDlg);
        m_RBSpec->setObjectName(QStringLiteral("m_RBSpec"));
        sizePolicy1.setHeightForWidth(m_RBSpec->sizePolicy().hasHeightForWidth());
        m_RBSpec->setSizePolicy(sizePolicy1);
        m_RBSpec->setChecked(true);

        verticalLayout_2->addWidget(m_RBSpec);

        m_RBLoop = new QRadioButton(QCalR1CTxLOLeakDlg);
        m_RBLoop->setObjectName(QStringLiteral("m_RBLoop"));
        sizePolicy1.setHeightForWidth(m_RBLoop->sizePolicy().hasHeightForWidth());
        m_RBLoop->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(m_RBLoop);


        horizontalLayout_10->addLayout(verticalLayout_2);


        verticalLayout_6->addLayout(horizontalLayout_10);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_3);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        m_PBStar = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBStar->setObjectName(QStringLiteral("m_PBStar"));
        sizePolicy1.setHeightForWidth(m_PBStar->sizePolicy().hasHeightForWidth());
        m_PBStar->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(m_PBStar);

        m_PBPaus = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBPaus->setObjectName(QStringLiteral("m_PBPaus"));
        sizePolicy1.setHeightForWidth(m_PBPaus->sizePolicy().hasHeightForWidth());
        m_PBPaus->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(m_PBPaus);


        horizontalLayout_8->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        m_PBStop = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBStop->setObjectName(QStringLiteral("m_PBStop"));
        sizePolicy1.setHeightForWidth(m_PBStop->sizePolicy().hasHeightForWidth());
        m_PBStop->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(m_PBStop);

        m_PBCont = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBCont->setObjectName(QStringLiteral("m_PBCont"));
        sizePolicy1.setHeightForWidth(m_PBCont->sizePolicy().hasHeightForWidth());
        m_PBCont->setSizePolicy(sizePolicy1);

        verticalLayout_5->addWidget(m_PBCont);


        horizontalLayout_8->addLayout(verticalLayout_5);


        verticalLayout_6->addLayout(horizontalLayout_8);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_6->addItem(verticalSpacer_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_5 = new QLabel(QCalR1CTxLOLeakDlg);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(84, 16));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_5);

        m_LEFreq = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEFreq->setObjectName(QStringLiteral("m_LEFreq"));
        sizePolicy1.setHeightForWidth(m_LEFreq->sizePolicy().hasHeightForWidth());
        m_LEFreq->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(m_LEFreq);


        verticalLayout_6->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_6 = new QLabel(QCalR1CTxLOLeakDlg);
        label_6->setObjectName(QStringLiteral("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(84, 16));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label_6);

        m_LEDCI = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEDCI->setObjectName(QStringLiteral("m_LEDCI"));
        sizePolicy1.setHeightForWidth(m_LEDCI->sizePolicy().hasHeightForWidth());
        m_LEDCI->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(m_LEDCI);


        verticalLayout_6->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_4 = new QLabel(QCalR1CTxLOLeakDlg);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(84, 16));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_4);

        m_LEDCQ = new QLineEdit(QCalR1CTxLOLeakDlg);
        m_LEDCQ->setObjectName(QStringLiteral("m_LEDCQ"));
        sizePolicy1.setHeightForWidth(m_LEDCQ->sizePolicy().hasHeightForWidth());
        m_LEDCQ->setSizePolicy(sizePolicy1);
        m_LEDCQ->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_7->addWidget(m_LEDCQ);


        verticalLayout_6->addLayout(horizontalLayout_7);

        m_PBGetSet = new QPushButton(QCalR1CTxLOLeakDlg);
        m_PBGetSet->setObjectName(QStringLiteral("m_PBGetSet"));
        sizePolicy1.setHeightForWidth(m_PBGetSet->sizePolicy().hasHeightForWidth());
        m_PBGetSet->setSizePolicy(sizePolicy1);

        verticalLayout_6->addWidget(m_PBGetSet);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);


        horizontalLayout_9->addLayout(verticalLayout_6);

        m_TVData = new QTableView(QCalR1CTxLOLeakDlg);
        m_TVData->setObjectName(QStringLiteral("m_TVData"));

        horizontalLayout_9->addWidget(m_TVData);

        horizontalLayout_9->setStretch(0, 1);
        horizontalLayout_9->setStretch(1, 10);

        verticalLayout_7->addLayout(horizontalLayout_9);

        verticalLayout_7->setStretch(1, 1);
#ifndef QT_NO_SHORTCUT
        label->setBuddy(m_LEFreqStar);
        label_2->setBuddy(m_LEFreqStop);
        label_3->setBuddy(m_LEFreqStep);
        label_5->setBuddy(m_LEFreq);
        label_6->setBuddy(m_LEDCI);
        label_4->setBuddy(m_LEDCQ);
#endif // QT_NO_SHORTCUT

        retranslateUi(QCalR1CTxLOLeakDlg);

        QMetaObject::connectSlotsByName(QCalR1CTxLOLeakDlg);
    } // setupUi

    void retranslateUi(QDialog *QCalR1CTxLOLeakDlg)
    {
        QCalR1CTxLOLeakDlg->setWindowTitle(QApplication::translate("QCalR1CTxLOLeakDlg", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Freq Start(Hz)", Q_NULLPTR));
        label_2->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Freq Stop(Hz)", Q_NULLPTR));
        m_PBExport->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Export", Q_NULLPTR));
        label_3->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Freq Step(Hz)", Q_NULLPTR));
        m_CBCalRef->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "10M Ref", Q_NULLPTR));
        m_RBSpec->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Spectrum", Q_NULLPTR));
        m_RBLoop->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Loopback", Q_NULLPTR));
        m_PBStar->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Start", Q_NULLPTR));
        m_PBPaus->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Pause", Q_NULLPTR));
        m_PBStop->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Stop", Q_NULLPTR));
        m_PBCont->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Continue", Q_NULLPTR));
        label_5->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Freq(Hz)", Q_NULLPTR));
        label_6->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "DC Offset I", Q_NULLPTR));
        label_4->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "DC Offset Q", Q_NULLPTR));
        m_PBGetSet->setText(QApplication::translate("QCalR1CTxLOLeakDlg", "Get And Set", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QCalR1CTxLOLeakDlg: public Ui_QCalR1CTxLOLeakDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QCALR1CTXLOLEAKDLG_H
