/********************************************************************************
** Form generated from reading UI file 'qfpgadlg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QFPGADLG_H
#define UI_QFPGADLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QFPGADlg
{
public:
    QHBoxLayout *horizontalLayout_13;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLineEdit *m_LEOffsetK7_0;
    QPushButton *m_PBWK7_0;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *m_LEDataK7_0;
    QPushButton *m_PBRK7_0;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QLineEdit *m_LEStatusK7_0;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_5;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *m_LEOffsetK7_1;
    QPushButton *m_PBWK7_1;
    QVBoxLayout *verticalLayout_9;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *m_LEDataK7_1;
    QPushButton *m_PBRK7_1;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_6;
    QLineEdit *m_LEStatusK7_1;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_12;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_7;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_13;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *m_LEOffsetS6_BAR0;
    QPushButton *m_PBWS6_BAR0;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_6;
    QLineEdit *m_LEDataS6_BAR0;
    QPushButton *m_PBRS6_BAR0;
    QVBoxLayout *verticalLayout_15;
    QLabel *label_9;
    QLineEdit *m_LEStatusS6_BAR0;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_9;
    QLineEdit *lineEdit_13;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_10;
    QComboBox *comboBox;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_21;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_16;
    QSpacerItem *verticalSpacer_5;
    QVBoxLayout *verticalLayout_18;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_7;
    QLineEdit *m_LEOffsetS6_BAR1;
    QPushButton *m_PBWS6_BAR1;
    QVBoxLayout *verticalLayout_19;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *m_LEDataS6_BAR1;
    QPushButton *m_PBRS6_BAR1;
    QVBoxLayout *verticalLayout_20;
    QLabel *label_12;
    QLineEdit *m_LEStatusS6_BAR1;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_17;
    QHBoxLayout *horizontalLayout_11;
    QTextEdit *m_LEPathK7_0;
    QVBoxLayout *verticalLayout_22;
    QPushButton *m_PBSetPathK7_0;
    QPushButton *m_PBProgramK7_0;
    QHBoxLayout *horizontalLayout_12;
    QTextEdit *m_LEPathK7_1;
    QVBoxLayout *verticalLayout_23;
    QPushButton *m_PBSetPathK7_1;
    QPushButton *m_PBProgramK7_1;

    void setupUi(QDialog *QFPGADlg)
    {
        if (QFPGADlg->objectName().isEmpty())
            QFPGADlg->setObjectName(QStringLiteral("QFPGADlg"));
        QFPGADlg->resize(913, 522);
        horizontalLayout_13 = new QHBoxLayout(QFPGADlg);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBox = new QGroupBox(QFPGADlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalSpacer_2 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_LEOffsetK7_0 = new QLineEdit(groupBox);
        m_LEOffsetK7_0->setObjectName(QStringLiteral("m_LEOffsetK7_0"));

        horizontalLayout->addWidget(m_LEOffsetK7_0);

        m_PBWK7_0 = new QPushButton(groupBox);
        m_PBWK7_0->setObjectName(QStringLiteral("m_PBWK7_0"));

        horizontalLayout->addWidget(m_PBWK7_0);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_LEDataK7_0 = new QLineEdit(groupBox);
        m_LEDataK7_0->setObjectName(QStringLiteral("m_LEDataK7_0"));

        horizontalLayout_2->addWidget(m_LEDataK7_0);

        m_PBRK7_0 = new QPushButton(groupBox);
        m_PBRK7_0->setObjectName(QStringLiteral("m_PBRK7_0"));

        horizontalLayout_2->addWidget(m_PBRK7_0);

        horizontalLayout_2->setStretch(0, 5);
        horizontalLayout_2->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        m_LEStatusK7_0 = new QLineEdit(groupBox);
        m_LEStatusK7_0->setObjectName(QStringLiteral("m_LEStatusK7_0"));
        m_LEStatusK7_0->setReadOnly(true);

        verticalLayout_3->addWidget(m_LEStatusK7_0);


        verticalLayout_4->addLayout(verticalLayout_3);


        verticalLayout_6->addWidget(groupBox);

        groupBox_2 = new QGroupBox(QFPGADlg);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalSpacer_3 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(0);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_8->addWidget(label_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        m_LEOffsetK7_1 = new QLineEdit(groupBox_2);
        m_LEOffsetK7_1->setObjectName(QStringLiteral("m_LEOffsetK7_1"));

        horizontalLayout_3->addWidget(m_LEOffsetK7_1);

        m_PBWK7_1 = new QPushButton(groupBox_2);
        m_PBWK7_1->setObjectName(QStringLiteral("m_PBWK7_1"));

        horizontalLayout_3->addWidget(m_PBWK7_1);

        horizontalLayout_3->setStretch(0, 5);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_8->addLayout(horizontalLayout_3);


        verticalLayout_5->addLayout(verticalLayout_8);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_9->addWidget(label_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        m_LEDataK7_1 = new QLineEdit(groupBox_2);
        m_LEDataK7_1->setObjectName(QStringLiteral("m_LEDataK7_1"));

        horizontalLayout_4->addWidget(m_LEDataK7_1);

        m_PBRK7_1 = new QPushButton(groupBox_2);
        m_PBRK7_1->setObjectName(QStringLiteral("m_PBRK7_1"));

        horizontalLayout_4->addWidget(m_PBRK7_1);

        horizontalLayout_4->setStretch(0, 5);
        horizontalLayout_4->setStretch(1, 1);

        verticalLayout_9->addLayout(horizontalLayout_4);


        verticalLayout_5->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(0);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_10->addWidget(label_6);

        m_LEStatusK7_1 = new QLineEdit(groupBox_2);
        m_LEStatusK7_1->setObjectName(QStringLiteral("m_LEStatusK7_1"));
        m_LEStatusK7_1->setReadOnly(true);

        verticalLayout_10->addWidget(m_LEStatusK7_1);


        verticalLayout_5->addLayout(verticalLayout_10);


        verticalLayout_6->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);

        verticalLayout_6->setStretch(2, 1);

        horizontalLayout_13->addLayout(verticalLayout_6);

        horizontalSpacer = new QSpacerItem(24, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer);

        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        groupBox_3 = new QGroupBox(QFPGADlg);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_7 = new QVBoxLayout(groupBox_3);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalSpacer_4 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_4);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_13->addWidget(label_7);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        m_LEOffsetS6_BAR0 = new QLineEdit(groupBox_3);
        m_LEOffsetS6_BAR0->setObjectName(QStringLiteral("m_LEOffsetS6_BAR0"));

        horizontalLayout_5->addWidget(m_LEOffsetS6_BAR0);

        m_PBWS6_BAR0 = new QPushButton(groupBox_3);
        m_PBWS6_BAR0->setObjectName(QStringLiteral("m_PBWS6_BAR0"));

        horizontalLayout_5->addWidget(m_PBWS6_BAR0);

        horizontalLayout_5->setStretch(0, 5);
        horizontalLayout_5->setStretch(1, 1);

        verticalLayout_13->addLayout(horizontalLayout_5);


        verticalLayout_7->addLayout(verticalLayout_13);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_14->addWidget(label_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(0);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        m_LEDataS6_BAR0 = new QLineEdit(groupBox_3);
        m_LEDataS6_BAR0->setObjectName(QStringLiteral("m_LEDataS6_BAR0"));

        horizontalLayout_6->addWidget(m_LEDataS6_BAR0);

        m_PBRS6_BAR0 = new QPushButton(groupBox_3);
        m_PBRS6_BAR0->setObjectName(QStringLiteral("m_PBRS6_BAR0"));

        horizontalLayout_6->addWidget(m_PBRS6_BAR0);

        horizontalLayout_6->setStretch(0, 5);
        horizontalLayout_6->setStretch(1, 1);

        verticalLayout_14->addLayout(horizontalLayout_6);


        verticalLayout_7->addLayout(verticalLayout_14);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(0);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout_15->addWidget(label_9);

        m_LEStatusS6_BAR0 = new QLineEdit(groupBox_3);
        m_LEStatusS6_BAR0->setObjectName(QStringLiteral("m_LEStatusS6_BAR0"));
        m_LEStatusS6_BAR0->setReadOnly(true);

        verticalLayout_15->addWidget(m_LEStatusS6_BAR0);


        verticalLayout_7->addLayout(verticalLayout_15);


        verticalLayout_12->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(QFPGADlg);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_11 = new QVBoxLayout(groupBox_4);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(0);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        lineEdit_13 = new QLineEdit(groupBox_4);
        lineEdit_13->setObjectName(QStringLiteral("lineEdit_13"));

        horizontalLayout_9->addWidget(lineEdit_13);

        pushButton = new QPushButton(groupBox_4);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_9->addWidget(pushButton);

        horizontalLayout_9->setStretch(0, 1);

        verticalLayout_11->addLayout(horizontalLayout_9);

        textEdit = new QTextEdit(groupBox_4);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setReadOnly(true);

        verticalLayout_11->addWidget(textEdit);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        comboBox = new QComboBox(groupBox_4);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_10->addWidget(comboBox);

        pushButton_2 = new QPushButton(groupBox_4);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_10->addWidget(pushButton_2);

        horizontalLayout_10->setStretch(0, 1);

        verticalLayout_11->addLayout(horizontalLayout_10);


        verticalLayout_12->addWidget(groupBox_4);

        verticalLayout_12->setStretch(1, 1);

        horizontalLayout_13->addLayout(verticalLayout_12);

        horizontalSpacer_2 = new QSpacerItem(24, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_2);

        verticalLayout_21 = new QVBoxLayout();
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        groupBox_5 = new QGroupBox(QFPGADlg);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_16 = new QVBoxLayout(groupBox_5);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        verticalSpacer_5 = new QSpacerItem(20, 6, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_16->addItem(verticalSpacer_5);

        verticalLayout_18 = new QVBoxLayout();
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout_18->addWidget(label_10);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        m_LEOffsetS6_BAR1 = new QLineEdit(groupBox_5);
        m_LEOffsetS6_BAR1->setObjectName(QStringLiteral("m_LEOffsetS6_BAR1"));

        horizontalLayout_7->addWidget(m_LEOffsetS6_BAR1);

        m_PBWS6_BAR1 = new QPushButton(groupBox_5);
        m_PBWS6_BAR1->setObjectName(QStringLiteral("m_PBWS6_BAR1"));

        horizontalLayout_7->addWidget(m_PBWS6_BAR1);

        horizontalLayout_7->setStretch(0, 5);
        horizontalLayout_7->setStretch(1, 1);

        verticalLayout_18->addLayout(horizontalLayout_7);


        verticalLayout_16->addLayout(verticalLayout_18);

        verticalLayout_19 = new QVBoxLayout();
        verticalLayout_19->setSpacing(0);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QStringLiteral("label_11"));

        verticalLayout_19->addWidget(label_11);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        m_LEDataS6_BAR1 = new QLineEdit(groupBox_5);
        m_LEDataS6_BAR1->setObjectName(QStringLiteral("m_LEDataS6_BAR1"));

        horizontalLayout_8->addWidget(m_LEDataS6_BAR1);

        m_PBRS6_BAR1 = new QPushButton(groupBox_5);
        m_PBRS6_BAR1->setObjectName(QStringLiteral("m_PBRS6_BAR1"));

        horizontalLayout_8->addWidget(m_PBRS6_BAR1);

        horizontalLayout_8->setStretch(0, 5);
        horizontalLayout_8->setStretch(1, 1);

        verticalLayout_19->addLayout(horizontalLayout_8);


        verticalLayout_16->addLayout(verticalLayout_19);

        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(0);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        label_12 = new QLabel(groupBox_5);
        label_12->setObjectName(QStringLiteral("label_12"));

        verticalLayout_20->addWidget(label_12);

        m_LEStatusS6_BAR1 = new QLineEdit(groupBox_5);
        m_LEStatusS6_BAR1->setObjectName(QStringLiteral("m_LEStatusS6_BAR1"));
        m_LEStatusS6_BAR1->setReadOnly(true);

        verticalLayout_20->addWidget(m_LEStatusS6_BAR1);


        verticalLayout_16->addLayout(verticalLayout_20);


        verticalLayout_21->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(QFPGADlg);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setStyleSheet(QLatin1String("QGroupBox{ border:1px solid black; margin-top:0.5ex; }\n"
"QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left; top:-2px; left:10px; margin-left: 0px; padding:0 1px; }"));
        verticalLayout_17 = new QVBoxLayout(groupBox_6);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        m_LEPathK7_0 = new QTextEdit(groupBox_6);
        m_LEPathK7_0->setObjectName(QStringLiteral("m_LEPathK7_0"));

        horizontalLayout_11->addWidget(m_LEPathK7_0);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        m_PBSetPathK7_0 = new QPushButton(groupBox_6);
        m_PBSetPathK7_0->setObjectName(QStringLiteral("m_PBSetPathK7_0"));

        verticalLayout_22->addWidget(m_PBSetPathK7_0);

        m_PBProgramK7_0 = new QPushButton(groupBox_6);
        m_PBProgramK7_0->setObjectName(QStringLiteral("m_PBProgramK7_0"));

        verticalLayout_22->addWidget(m_PBProgramK7_0);


        horizontalLayout_11->addLayout(verticalLayout_22);


        verticalLayout_17->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        m_LEPathK7_1 = new QTextEdit(groupBox_6);
        m_LEPathK7_1->setObjectName(QStringLiteral("m_LEPathK7_1"));

        horizontalLayout_12->addWidget(m_LEPathK7_1);

        verticalLayout_23 = new QVBoxLayout();
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        m_PBSetPathK7_1 = new QPushButton(groupBox_6);
        m_PBSetPathK7_1->setObjectName(QStringLiteral("m_PBSetPathK7_1"));

        verticalLayout_23->addWidget(m_PBSetPathK7_1);

        m_PBProgramK7_1 = new QPushButton(groupBox_6);
        m_PBProgramK7_1->setObjectName(QStringLiteral("m_PBProgramK7_1"));

        verticalLayout_23->addWidget(m_PBProgramK7_1);


        horizontalLayout_12->addLayout(verticalLayout_23);


        verticalLayout_17->addLayout(horizontalLayout_12);


        verticalLayout_21->addWidget(groupBox_6);

        verticalLayout_21->setStretch(1, 1);

        horizontalLayout_13->addLayout(verticalLayout_21);

        horizontalLayout_13->setStretch(0, 10);
        horizontalLayout_13->setStretch(1, 1);
        horizontalLayout_13->setStretch(2, 10);
        horizontalLayout_13->setStretch(3, 1);
        horizontalLayout_13->setStretch(4, 10);
        groupBox->raise();
        label_4->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();
        groupBox_5->raise();
        groupBox_6->raise();

        retranslateUi(QFPGADlg);

        QMetaObject::connectSlotsByName(QFPGADlg);
    } // setupUi

    void retranslateUi(QDialog *QFPGADlg)
    {
        QFPGADlg->setWindowTitle(QApplication::translate("QFPGADlg", "Dialog", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("QFPGADlg", "K7_0", Q_NULLPTR));
        label->setText(QApplication::translate("QFPGADlg", "Offset(hex)", Q_NULLPTR));
        m_PBWK7_0->setText(QApplication::translate("QFPGADlg", "Write", Q_NULLPTR));
        label_2->setText(QApplication::translate("QFPGADlg", "Data(hex)", Q_NULLPTR));
        m_PBRK7_0->setText(QApplication::translate("QFPGADlg", "Read", Q_NULLPTR));
        label_3->setText(QApplication::translate("QFPGADlg", "Status", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("QFPGADlg", "K7_1", Q_NULLPTR));
        label_4->setText(QApplication::translate("QFPGADlg", "Offset(hex)", Q_NULLPTR));
        m_PBWK7_1->setText(QApplication::translate("QFPGADlg", "Write", Q_NULLPTR));
        label_5->setText(QApplication::translate("QFPGADlg", "Data(hex)", Q_NULLPTR));
        m_PBRK7_1->setText(QApplication::translate("QFPGADlg", "Read", Q_NULLPTR));
        label_6->setText(QApplication::translate("QFPGADlg", "Status", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("QFPGADlg", "S6_BAR0", Q_NULLPTR));
        label_7->setText(QApplication::translate("QFPGADlg", "Offset(hex)", Q_NULLPTR));
        m_PBWS6_BAR0->setText(QApplication::translate("QFPGADlg", "Write", Q_NULLPTR));
        label_8->setText(QApplication::translate("QFPGADlg", "Data(hex)", Q_NULLPTR));
        m_PBRS6_BAR0->setText(QApplication::translate("QFPGADlg", "Read", Q_NULLPTR));
        label_9->setText(QApplication::translate("QFPGADlg", "Status", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("QFPGADlg", "Write From File", Q_NULLPTR));
        pushButton->setText(QApplication::translate("QFPGADlg", "...", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("QFPGADlg", "Execute", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("QFPGADlg", "S6_BAR1", Q_NULLPTR));
        label_10->setText(QApplication::translate("QFPGADlg", "Offset(hex)", Q_NULLPTR));
        m_PBWS6_BAR1->setText(QApplication::translate("QFPGADlg", "Write", Q_NULLPTR));
        label_11->setText(QApplication::translate("QFPGADlg", "Data(hex)", Q_NULLPTR));
        m_PBRS6_BAR1->setText(QApplication::translate("QFPGADlg", "Read", Q_NULLPTR));
        label_12->setText(QApplication::translate("QFPGADlg", "Status", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("QFPGADlg", "Program FPGA", Q_NULLPTR));
        m_PBSetPathK7_0->setText(QApplication::translate("QFPGADlg", "K7_0_bit", Q_NULLPTR));
        m_PBProgramK7_0->setText(QApplication::translate("QFPGADlg", "Program", Q_NULLPTR));
        m_PBSetPathK7_1->setText(QApplication::translate("QFPGADlg", "K7_1_bit", Q_NULLPTR));
        m_PBProgramK7_1->setText(QApplication::translate("QFPGADlg", "Program", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QFPGADlg: public Ui_QFPGADlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFPGADLG_H
