#include "global.h"
#include <QWidget>


QRectF Qwt_Data::boundingRect() const
{
    if (d_boundingRect.width() < 0) {
        d_boundingRect = qwtBoundingRect(*this);
    }
    return d_boundingRect;
}


// Common
Project project = SP9500;

quint32 RFIdx = 0;
quint32 RFUIdx = 0;

complex_sequence *complexSequence = nullptr;

// SP9500
sp3301 *SP3301 = nullptr;

sp1401     *SP1401     = nullptr;
sp1401_r1a *SP1401_R1A = nullptr;
sp1401_r1b *SP1401_R1B = nullptr;
sp1401_r1c *SP1401_R1C = nullptr;
sp1401_r1d *SP1401_R1D = nullptr;
sp1401_r1e *SP1401_R1E = nullptr;
sp1401_r1f *SP1401_R1F = nullptr;
sp2401_r1a *SP2401     = nullptr;

// DT3308

// SP9500X

namespace NS_SP9500X {

ns_sp9500x::sp3103 *SP3103 = nullptr;

ns_sp9500x::sp1403 *SP1403 = nullptr;
ns_sp9500x::sp1403_r1a *SP1403_R1A = nullptr;
ns_sp9500x::sp1403_r1b *SP1403_R1B = nullptr;
ns_sp9500x::rrh *SP9500X_RRH = nullptr;
ns_sp9500x::sp2406 *SP2406 = nullptr;

}

rd::sp3103     *SP3103     = nullptr;
rd::sp1403     *SP1403     = nullptr;
rd::sp1403_r1a *SP1403_R1A = nullptr;
rd::sp1403_r1b *SP1403_R1B = nullptr;

