#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "libBusDriver.h"
#include "libLog.h"
#include "libInstr.h"
#include "libSP3301.h"
#include "libSP3501.h"
#include <QString>
#include <QColor>
#include <QAbstractItemModel>
#include <QDialog>

#define CLR_PROTECT_EYE QColor(199,237,204)
#define CLR_ERROR       QColor(255,150,150)
#define CLR_SA_BLGD     QColor(63,49,44)

#define STRING_FREQ_TIPS QString("e.g. 2G,2g,2000M,2000000000")

#define INIT_PROG(name,pts) emit(initProg(name,pts))
#define SET_PROG_POS(pos) emit(setProgPos(pos))

inline QString tm2QString(const tm &t)
{
    return QString("%1/%2/%3 %4:%5:%6").arg(t.tm_year + 1900)
                                       .arg(t.tm_mon)
                                       .arg(t.tm_mday)
                                       .arg(t.tm_hour)
                                       .arg(t.tm_min)
                                       .arg(t.tm_sec);
}

#endif
