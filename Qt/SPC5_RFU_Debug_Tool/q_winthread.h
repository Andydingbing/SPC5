#ifndef Q_WINTHREAD_H
#define Q_WINTHREAD_H

#include "define.h"
#include <QThread>
#include <QMutex>
#include "freq_string.hpp"
#include "q_r1c_temp_ctrl_model.h"
#include "test_data.hpp"

#define THREAD_TEST_CANCEL  \
{   if (QWinThread::g_threadStop) { \
        emit done(); \
        return; \
    } \
}

#define THREAD_ABORT \
{   emit done(); \
    return; \
}

#define THREAD_CHECK_BOX(msg) \
{   emit threadCheckBox(msg); \
    if (QWinThread::g_threadStop) { \
        emit result(CTR_MANUAL_STOPED); \
        emit done(); \
        return; \
    } \
}

#define THREAD_ERROR_BOX(msg)       \
{   emit threadErrorBox(msg);       \
}

// Calibration/Test thread stop support.
#define CAL_THREAD_TEST_CANCEL \
{   if (QWinThread::g_threadStop) { \
        Instr.close(); \
        emit result(CTR_MANUAL_STOPED); \
        emit done(); \
        return; \
    } \
}

#define TEST_THREAD_TEST_CANCEL \
    CAL_THREAD_TEST_CANCEL

#define CAL_THREAD_TEST_CANCEL_S(post) \
{   if (QWinThread::g_threadStop) { \
        post; \
        Instr.close(); \
        emit result(CTR_MANUAL_STOPED); \
        emit done(); \
        return; \
    } \
}

// Calibration thread pause/continue support.
#define CAL_THREAD_TEST_PAUSE_S \
{   threadLock.lock();

#define CAL_THREAD_TEST_PAUSE_E \
    threadLock.unlock(); }


#define CAL_THREAD_START(name,pts) \
    CAL_THREAD_TEST_CANCEL; \
    THREAD_CHECK_BOX(QString("Prepare Calibrating %1").arg(name)); \
    INIT_PROG(QString("Calibrating %1").arg(name),pts);

#define TEST_THREAD_START(name,pts) \
    TEST_THREAD_TEST_CANCEL; \
    THREAD_CHECK_BOX(QString("Prepare Testing %1").arg(name));  \
    INIT_PROG(QString("Testing %1").arg(name),pts);

#define CAL_THREAD_ABOART \
{   threadLock.lock(); \
    threadLock.unlock(); \
    Instr.close(); \
    emit result(CTR_COMPLETE_PASS); \
    emit done(); \
    return; \
}

#define TEST_THREAD_ABOART \
    CAL_THREAD_ABOART

#define CAL_THREAD_ABOART_S(post) \
{   threadLock.lock(); \
    threadLock.unlock(); \
    post; \
    Instr.close(); \
    emit done(); \
    emit result(CTR_COMPLETE_PASS); \
    return; \
}

// Calibration/Test thread exception support.
#ifdef RD_EXCEPTION
    #define RD_CAL_TRY try {

    #define RD_CAL_CATCH \
        } catch (std::exception &e) { \
            THREAD_ERROR_BOX(QString("%1").arg(e.what())); \
            CAL_THREAD_ABOART \
        }
#else
    #define RD_CAL_TRY
    #define RD_CAL_CATCH
#endif

#define RD_TEST_TRY RD_CAL_TRY
#define RD_TEST_CATCH RD_CAL_CATCH


class QCalBaseThread;
class QCalBaseModel;
class QwtCalData;


typedef enum CalResult {
    CTR_COMPLETE_PASS = 0,
    CTR_COMPLETE_FAIL,
    CTR_MANUAL_STOPED
} CalResult, TestResult;

typedef enum CalIOMode {
    CAL_IO = 0,
    TEST_IO = 0,
    CAL_OP = 1,
    TEST_OP = 1,
    CAL_IO_OP = 2,
    TEST_IO_OP = 2
} CalIOMode, TestIOMode;

typedef enum LOLeak_Method {
    M_Spectrum = 0,
    M_Loopback = 1
} LOLeak_Method, SB_Method;


/*
 * While calibration parameters are somewhat the same.
 * So I put them all just in one structure.
 */
struct CalParam
{
public:
    bool cal;
    bool check;
    QObject       *parent;
    CalIOMode     mode;
    QCalBaseModel *model_0;
    QCalBaseModel *model_1;
    QwtCalData    *plotData_0;
    QwtCalData    *plotData_1;
    QwtCalData    *plotData_2;
    basic_sp1401  *_sp1401;
    sp2401_r1a    *_sp2401;
    sp3501        *_sp3501;
    QString       rfFreqStar;
    QString       rfFreqStop;
    QString       rfFreqStep;
    bool          calX9119;
    LOLeak_Method methodLOLeak;
    SB_Method     methodSB;
    bool          justRebuildCoef;
public:
    CalParam();
};

struct CalR1CParam
{
public:
    CalIOMode txMode;
    CalIOMode rxMode;
    QString   rfTxFreqStar;
    QString   rfTxFreqStop;
    QString   rfRxFreqStar;
    QString   rfRxFreqStop;
    QVector<QCalBaseThread *> calThreads;
public:
    CalR1CParam();
};


/*
 * But test parameters could be very fickle.
 * So I just put some commen parameters here.
 */
struct TestBaseParam
{
public:
    QObject *parent;
    TestIOMode TXMode;
    TestIOMode RXMode;
    bool isTXAuto;
    bool isRXAuto;
    QTestBaseModel *model_0;
    QTestBaseModel *model_1;
    QwtTestData *plotData_0;
    QwtTestData *plotData_1;
    QwtTestData *plotData_2;
    QwtTestData *plotData_3;
    basic_sp1401 *SP1401;
    sp2401_r1a   *SP2401;
    sp3301       *SP3301;

    TestBaseParam();
};

typedef struct TempParam {
public:
    QwtTempCtrlData *plot_0;
    QwtTempCtrlData *plot_1;
    QwtTempCtrlData *plot_2;
    QwtTempCtrlData *plot_3;
    QwtTempCtrlData *plot_4;
    QwtTempCtrlData *plot_5;
    QwtTempCtrlData *plot_6;
    QwtTempCtrlData *plot_7;
    basic_sp1401    *_sp1401;
    sp2401_r1a      *_sp2401;
    sp3501          *_sp3501;

}TempParam;


class QWinThread : public QThread
{
    Q_OBJECT

public:
    explicit QWinThread(QObject *parent = nullptr);

signals:
    void initProg(const QString name,quint32 pts);
    void setProgPos(quint32 pos);
    void done(bool success = true);
    void threadCheckBox(QString msg);
    void threadErrorBox(QString msg);

public:
    int32_t funcIdx;
    QMutex threadLock;
    static bool g_threadStop;
    static bool g_threadPausing;
    static QString  g_threadName;
    static QWinThread *g_threadThread;
};


typedef class QCalBaseThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QCalBaseThread(const CalParam &param);
    static tm getCurTime();

    virtual void cal() {}
    virtual void check() {}

    virtual void run()
    {
        RD_CAL_TRY
        if (calParam.cal) cal();
        if (calParam.check) check();
        CAL_THREAD_ABOART
        RD_CAL_CATCH
    }

signals:
    // tl : top_left model index.
    // br : bottom_right model index.
    // sec : Section the currently calling frequency in.
    // item : Calibration item.
    void update(const QModelIndex &tl,
                const QModelIndex &br,
                cal_file::cal_item_t item = cal_file::TX_LOL,
                int sec = 0);
    void result(CalResult cr = CTR_COMPLETE_PASS);

protected:
    hw_ver_t RFVer;
    CalParam calParam;
    range_freq_string freqString;
    range_freq<quint64> freqRange;
} QCalBaseThread, QExportBaseThread;

typedef class QCalR1CBaseThread : public QCalBaseThread
{
public:
    explicit QCalR1CBaseThread(const CalParam &param) :
        QCalBaseThread(param)
    {
        SP1401 = (sp1401_r1c *)(param._sp1401);
        SP2401 = (sp2401_r1a *)(param._sp2401);
    }

protected:
    sp1401_r1c *SP1401;
    sp2401_r1a *SP2401;
} QCalR1CBaseThread, QExpR1CBaseThread;


class QTestBaseThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QTestBaseThread(QObject *parent = nullptr) :
        QWinThread(parent) {}

signals:
    void update(const QModelIndex &tl,
                const QModelIndex &br,
                test_item_t item = TI_RF_TX_FREQ_RES,
                int sec = 0);
    void result(TestResult tr = CTR_COMPLETE_PASS);
};

class QTestThread : public QTestBaseThread
{
public:
    explicit QTestThread(const TestBaseParam *param) :
        QTestBaseThread(param->parent),
        totalResult(true)
    {
        RFVer = param->SP1401->get_hw_ver();

        if (this->parent() != nullptr) {
            connect(this,SIGNAL(update(QModelIndex,QModelIndex,test_item_t,int)),
            this->parent(),SLOT(update(QModelIndex,QModelIndex,test_item_t,int)));

            connect(this,SIGNAL(done(bool)),this->parent(),SLOT(done()));
        }

        testParam = const_cast<TestBaseParam *>(param);

        if (testParam != nullptr) {
            if (testParam->model_0 != nullptr) {
                connect(this,SIGNAL(update(QModelIndex,QModelIndex,test_item_t,int)),
            testParam->model_0,SLOT(update(QModelIndex,QModelIndex,test_item_t,int)));
            }
            if (testParam->model_1 != nullptr) {
                connect(this,SIGNAL(update(QModelIndex,QModelIndex,test_item_t,int)),
            testParam->model_1,SLOT(update(QModelIndex,QModelIndex,test_item_t,int)));
            }
        }
    }

    ~QTestThread() { SAFE_DEL(testParam); }
    void updateTotalResult(bool singleResult)
    { totalResult = totalResult ? singleResult : totalResult; }

protected:
    hw_ver_t RFVer;
    TestBaseParam *testParam;
    bool totalResult;
};

typedef class QTestR1CBaseThread : public QTestThread
{
public:
    explicit QTestR1CBaseThread(const TestBaseParam *param) :
        QTestThread(param)
    {
        SP1401 = dynamic_cast<sp1401_r1c *>(param->SP1401);
        SP2401 = dynamic_cast<sp2401_r1a *>(param->SP2401);
    }

    double getRXAvgPwr()
    {
        double pwr = 0.0;
        basic_sp1401::pwr_meas_state_t state = basic_sp1401::PMS_IDLE;

        SP1401->pwr_meas_abort();
        SP1401->pwr_meas_start();

        while (state != basic_sp1401::PMS_DONE) {
            SP1401->get_pwr_meas_state(state);
            if (state == basic_sp1401::PMS_TT)
                return -100.0;
        }
        SP1401->get_pwr_meas_pwr(pwr);
        return pwr;
    }

protected:
    sp1401_r1c *SP1401;
    sp2401_r1a *SP2401;
} QTestR1CBaseThread;

bool calOP(CalIOMode mode);
bool calIO(CalIOMode mode);
Q_ALWAYS_INLINE bool testOP(TestIOMode mode) { return calOP(mode); }
Q_ALWAYS_INLINE bool testIO(TestIOMode mode) { return calIO(mode); }

int exeFirProcess(char *path);
void getADS5474(basic_sp1401 *sp1401,qint64 &ad,qint32 avgTime = 3);

void threadCheckBox(const char *format, ...);
void threadErrorBox(const char *format, ...);
bool ftpRetryBox();

template <typename T>
quint32 parseRangeFreqStringFrom(const CalParam *param, range_freq<T> *freqRange = nullptr)
{
    range_freq_string freqString;
    range_freq<T> tempFreqRange;
    range_freq<T> *parsedPtr = (freqRange == nullptr ? &tempFreqRange : freqRange);

    freqString.star = param->rfFreqStar.toStdString();
    freqString.stop = param->rfFreqStop.toStdString();
    freqString.step = param->rfFreqStep.toStdString();

    return parse_range_freq_string(freqString,*parsedPtr);
}

#endif // Q_WINTHREAD_H
