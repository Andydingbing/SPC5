#ifndef Q_CAL_R1C_RX_ATT_THREAD_H
#define Q_CAL_R1C_RX_ATT_THREAD_H

#include "q_winthread.h"

class QCalR1CRXAttThread : public QCalR1CBaseThread
{
public:
    explicit QCalR1CRXAttThread(const CalParam &param) :
        QCalR1CBaseThread(param),
        attIdx(0), offsetBase(0), sgPwr(-160), pmIL(PM_IL) {}
    void run();
    void setRXState(rx_ref_op_table_r1cd::rx_state_m_t state);
    void calOneSec(io_mode_t mode, quint64 freq, qint32 refStar, qint32 refStop);
    void ajustSG(quint64 freq,qint32 pwr);

private:
    quint32 attIdx;
    qint64 offsetBase;
    qint32 sgPwr;
    double pmIL;
    rx_att_op_table_r1cd::data_f_t dataOP;
    rx_att_io_table_r1cd::data_f_t dataIO;
};


class QExpR1CRXAttThread : public QExpR1CBaseThread
{
public:
    explicit QExpR1CRXAttThread(const CalParam &param) :
        QExpR1CBaseThread(param) {}
    void run();
};

#endif // Q_CAL_R1C_RX_ATT_THREAD_H
