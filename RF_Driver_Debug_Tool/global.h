#ifndef DEFINE_H
#define DEFINE_H

#include "libbd.h"
#include "liblog.h"
#include "libinstr.h"
#include "sp3301.h"
#include "sp3501.h"
#include "bb_3308_f.h"
#include "rf_3308_f.h"
#include "rd_dt3308.h"
#include "rd_sp9500x.h"
#include "sp3103.h"
#include "freq_string.hpp"
#include "qwt_series_data.h"
#include <QWidget>
#include <QPointF>
#include <QTabWidget>
#include <QTableView>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QAbstractItemModel>
#include <QDialog>
#include <QStandardItemModel>
#include <QStyledItemDelegate>

class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;

using namespace rd;

enum Project {
    SPC5 = 0,
    SP9500 = 0,
    DT3308,
    SP9500X,
    SP9500Pro,
    PROJECTS
};

#define CLR_PROTECT_EYE QColor(199,237,204)

#define SET_PROG_POS(pos) emit(setProgPos(pos));
#define ADD_PROG_POS(off) emit(addProgPos(off));

#define DYNAMIC_SP1401_R1CE_SP1401(sp1401) \
    ((R1E == sp1401->get_hw_ver()) ? \
    ((sp1401_r1e *)(sp1401)) : ((sp1401_r1c *)(sp1401)))

#define DYNAMIC_SP1401_R1CE_CAL \
    ((R1E == calParam.SP1401->get_hw_ver()) ? \
    ((sp1401_r1e *)(calParam.SP1401)) : ((sp1401_r1c *)(calParam.SP1401)))

#define COMBOBOX_MAP_FROM_BETTER_ENUM(box,better_enum) \
for (size_t i = 0;i < better_enum::_size();++i) { \
    box->addItem(QString::fromUtf8(better_enum::_names()[i])); \
}

namespace Ui {
class Q_SP2406_Widget;
}

namespace NS_SP9500 {
class ChildWidgets;
class Q_ARB_Config_Widget_Helper;
}

namespace NS_DT3308 {
class ChildWidgets;
}

namespace NS_SP9500X {
class ChildWidgets;
class Q_ARB_Config_Widget_Helper;
}

class Q_RD_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Q_RD_Widget(QWidget *parent = nullptr) :
        QWidget(parent) {}

public slots:
    // Init child widgets
    virtual void init() {}
};


class Q_Config_Table_View : public QTableView
{
public:
    Q_Config_Table_View(QWidget *parent = nullptr);
};


class Q_Config_Table_Model : public QStandardItemModel
{
public:
    Q_Config_Table_Model(QObject *parent = nullptr) : QStandardItemModel(parent) {}

    int rowCount(const QModelIndex &) const { return _item.size(); }

    int columnCount(const QModelIndex &) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;

protected:
    QStringList _item;
};


class Q_Config_Table_Delegate : public QStyledItemDelegate
{
public:
    Q_Config_Table_Delegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const {}

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {}

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;

    virtual QWidget **first() const = 0;
};


class Qwt_Data : public QwtSeriesData<QPointF>
{
public:
    Qwt_Data() : QwtSeriesData<QPointF>() {}

    QRectF boundingRect() const;
};


inline QString tm2QString(const tm &t)
{
    return QString("%1/%2/%3 %4:%5:%6").arg(t.tm_year + 1900)
                                       .arg(t.tm_mon)
                                       .arg(t.tm_mday)
                                       .arg(t.tm_hour)
                                       .arg(t.tm_min)
                                       .arg(t.tm_sec);
}

inline void QString2Coef(const QString strCoef,double *coef,qint32 order)
{
    int idxStar = 0;
    int idxStop = 0;
    int idxCoef = 0;

    while (-1 != (idxStop = strCoef.indexOf(',',idxStar))) {
        coef[idxCoef] = strCoef.mid(idxStar,idxStop - idxStar).toDouble();
        idxStar = idxStop + 1;
        idxCoef ++;
        if (order - 1 == idxCoef) {
            break;
        }
    }
    coef[idxCoef] = strCoef.mid(idxStar).toDouble();
}

inline void QString2CoefShort(const QString strCoef,short *coef,qint32 order)
{
    int idxStar = 0;
    int idxStop = 0;
    int idxCoef = 0;

    while (-1 != (idxStop = strCoef.indexOf(',',idxStar))) {
        QString temp = strCoef.mid(idxStar,idxStop - idxStar);
        coef[idxCoef] = short(temp.toLong(nullptr,16));
        idxStar = idxStop + 1;
        idxCoef ++;
        if (order - 1 == idxCoef) {
            break;
        }
    }
    coef[idxCoef] = strCoef.mid(idxStar).toShort(nullptr,16);
}

inline QString Coef2QString(const int16_t *coef,int32_t order)
{
    QString buf;
    for (int32_t i = 0;i < order;i ++) {
        buf.append(QString("%1,").arg(coef[i]));
    }
    return buf;
}

template <typename T>
inline QString freqStringFrom(const T &freq, freq_string_unit_priority_t priority = FSU_M)
{ return QString::fromStdString(freq_string_from<T>(freq,priority)); }

inline QString DecimalToHexString(const qint8 dec)
{ return QString("0x%1").arg(dec,2,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint8 dec)
{ return QString("0x%1").arg(dec,2,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const qint16 dec)
{ return QString("0x%1").arg(dec,4,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint16 dec)
{ return QString("0x%1").arg(dec,4,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const qint32 dec)
{ return QString("0x%1").arg(dec,8,16,QLatin1Char('0')); }

inline QString DecimalToHexString(const quint32 dec)
{ return QString("0x%1").arg(dec,8,16,QLatin1Char('0')); }

// Global Variables

// Common
extern Project project;

extern quint32 RFIdx;
extern quint32 RFUIdx;

extern complex_sequence *complexSequence;

// SP9500
extern sp3301 *SP3301;

extern sp1401 *SP1401;
extern sp1401_r1a *SP1401_R1A;
extern sp1401_r1b *SP1401_R1B;
extern sp1401_r1c *SP1401_R1C;
extern sp1401_r1d *SP1401_R1D;
extern sp1401_r1e *SP1401_R1E;
extern sp1401_r1f *SP1401_R1F;
extern sp2401_r1a *SP2401;

// DT3308

// SP9500X
extern sp3103 *SP3103;

extern sp1403 *SP1403;
extern sp1403_r1a *SP1403_R1A;
extern sp1403_r1b *SP1403_R1B;
extern ns_sp9500x::rrh *SP9500X_RRH;
extern sp2406 *SP2406;

#endif // DEFINE_H
