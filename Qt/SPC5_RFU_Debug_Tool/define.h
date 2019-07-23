#ifndef DEFINE_H
#define DEFINE_H

#include "exception.hpp"
#include "sleep_common.h"
#include "libbd.h"
#include "liblog.h"
#include "libinstr.h"
#include "sp3301.h"
#include "sp3501.h"
#include <QString>
#include <QColor>
#include <QAbstractItemModel>
#include <QDialog>

using namespace sp_rd;
using namespace sp_rd::sp1401;

#define CLR_PROTECT_EYE QColor(199,237,204)
#define CLR_ERROR       QColor(255,190,190)
//#define CLR_SA_BLGD     QColor(63,49,44)
#define CLR_SA_BLGD     QColor(0,0,0)

#define STRING_FREQ_TIPS QString("e.g. 2G,2g,2000M,2000000000")

#define INIT_PROG(name,pts) emit(initProg(name,quint32(pts)))
#define SET_PROG_POS(pos) emit(setProgPos(pos))

#define DYNAMIC_SP1401_R1CE_SP1401(sp1401)          \
    ((R1E == sp1401->get_hw_ver()) ?                \
    ((sp1401_r1e *)(sp1401)) : ((sp1401_r1c *)(sp1401)))

#define DYNAMIC_SP1401_R1CE_CAL                     \
    ((R1E == calParam._sp1401->get_hw_ver()) ?      \
    ((sp1401_r1e *)(calParam._sp1401)) : ((sp1401_r1c *)(calParam._sp1401)))

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
        if (order - 1 == idxCoef)
            break;
    }
    coef[idxCoef] = strCoef.mid(idxStar).toDouble();
}

inline QString Coef2QString(const int16_t *coef,int32_t order)
{
    QString buf;
    for (int32_t i = 0;i < order;i ++)
        buf.append(QString("%1,").arg(coef[i]));
    return buf;
}

#ifdef RD_C_MSC
#ifdef _DEBUG
#pragma comment(lib, "qwtd.lib")
#else
#pragma comment(lib, "qwt.lib")
#endif
#endif

#endif // DEFINE_H
