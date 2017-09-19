#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "libBusDriver.h"
#include "libLog.h"
#include "libInstr.h"
#include "libSP3301.h"
#include "libSP3501.h"
#include <QColor>
#include <QAbstractItemModel>

#define CLR_PROTECT_EYE QColor(199,237,204)

#define STRING_FREQ_TIPS QString("e.g. 2G,2g,2000M,2000000000")

#define INIT_PROG(name,pts) emit(initProg(name,pts))
#define SET_PROG_POS(pos) emit(setProgPos(pos))

#endif
