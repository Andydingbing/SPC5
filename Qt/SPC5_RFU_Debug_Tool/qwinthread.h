#ifndef QWINTHREAD_H
#define QWINTHREAD_H

#include "define.h"
#include <QThread>
#include <QMutex>

#define THREAD_TEST_CANCEL          \
{   if (QWinThread::g_threadStop) { \
        emit done();                \
        return;                     \
    }                               \
}

#define THREAD_ABORT    \
{   emit done();        \
    return;             \
}

#define THREAD_CHECK_BOX(msg)       \
{   emit threadCheckBox(msg);       \
    if (QWinThread::g_threadStop) { \
        emit done();                \
        return;                     \
    }                               \
}

#define THREAD_ERROR_BOX(msg)       \
{   emit threadErrorBox(msg);       \
}

#define CAL_THREAD_TEST_CANCEL      \
{   if (QWinThread::g_threadStop) { \
        Instr.close();              \
        emit done();                \
        return;                     \
    }                               \
}

#define CAL_THREAD_TEST_CANCEL_S(post)  \
{   if (QWinThread::g_threadStop) {     \
        post;                           \
        Instr.close();                  \
        emit done();                    \
        return;                         \
    }                                   \
}

#define CAL_THREAD_TEST_PAUSE_S \
{   threadLock.lock();

#define CAL_THREAD_TEST_PAUSE_E \
    threadLock.unlock(); }

#define CAL_THREAD_START(name,pts)  \
    CAL_THREAD_TEST_CANCEL;         \
    THREAD_CHECK_BOX(QString("Prepare Calibrating %1").arg(name));  \
    INIT_PROG(QString("Calibrating %1").arg(name),pts);

#define CAL_THREAD_ABOART   \
{   threadLock.lock();      \
    threadLock.unlock();    \
    Instr.close();          \
    emit done();            \
    return;                 \
}

#define CAL_THREAD_ABOART_S(post)   \
{   threadLock.lock();              \
    threadLock.unlock();            \
    post;                           \
    Instr.close();                  \
    emit done();                    \
    return;                         \
}

class QCalBaseDlg;
class QCalBaseModel;
class QwtCalData;

typedef struct CalParam {
public:
    enum LOLeak_Method {
        M_Spectrum = 0,
        M_Loopback = 1
    };
    enum CalIOMode {
        CAL_IO = 0,
        CAL_OP,
        CAL_IO_OP
    };

    typedef LOLeak_Method SB_Method;

    QCalBaseDlg     *parent;
    CalIOMode       mode;
    QCalBaseModel   *model_0;
    QCalBaseModel   *model_1;
    QwtCalData      *plotData_0;
    QwtCalData      *plotData_1;
    QwtCalData      *plotData_2;
    basic_sp1401    *_sp1401;
    sp2401_r1a      *_sp2401;
    sp3501          *_sp3501;
    QString         rfFreqStar;
    QString         rfFreqStop;
    QString         rfFreqStep;
    QString         ifFreqStar;
    QString         ifFreqStop;
    QString         ifFreqStep;
    bool            calX9119;
    LOLeak_Method   methodLOLeak;
    SB_Method       methodSB;
    bool            justRebuildCoef;
public:
    CalParam();
}CalParam;

class QWinThread : public QThread
{
    Q_OBJECT

public:
    explicit QWinThread(QObject *parent = 0);
signals:
    void initProg(const QString name,int pts);
    void setProgPos(int pos);
    void done(bool success = true);
    void threadCheckBox(QString msg);
    void threadErrorBox(QString msg);
public:
    int32_t funcIdx;
    QMutex threadLock;
    static bool g_threadStop;
    static QString  g_threadName;
    static QWinThread *g_threadThread;
};

class QCalBaseThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QCalBaseThread(CalParam *param,QObject *parent = 0);
    tm getCurTime();
signals:
    // tl : top left model index
    // br : bottom right model index
    // sec : which section is currently calling frequency in
    // item : cal item
    void update(const QModelIndex &tl,const QModelIndex &br,cal_file::cal_item_t item = cal_file::TX_LOL,int sec = 0);
protected:
    CalParam calParam;
};

typedef class QCalBaseThread QExportBaseThread;

bool calOP(CalParam::CalIOMode mode);
bool calIO(CalParam::CalIOMode mode);

int exeFirProcess(char *path);
void getADS5474(basic_sp1401 *sp1401,qint64 &ad,qint32 avgTime = 3);

void threadCheckBox(const char *format,...);
void threadErrorBox(const char *format,...);

int freq2array(QString in1,QString in2,QString step,int64_t *out = NULL,int64_t *freqstar = NULL,int64_t *freqstop = NULL,int64_t *max = NULL,int64_t *min = NULL);

#endif // QWINTHREAD_H
