#ifndef Q_WINTHREAD_H
#define Q_WINTHREAD_H

#include "global.h"
#include <QThread>
#include <QMutex>
#include "freq_string.hpp"
#include "q_r1c_temp_ctrl_model.h"
#include "test_data.hpp"

#define THREAD_CHECK_BOX(msg) \
{   emit threadCheckBox(msg); \
    if (QWinThread::g_threadStop) { \
        emit threadProcess(END_MANUAL); \
        emit done(); \
        return; \
    } \
}

#define THREAD_ERROR_BOX(msg) { emit threadErrorBox(msg); }

#define THREAD_TEST_CANCEL \
{   if (QWinThread::g_threadStop) { \
        Instr.close(); \
        emit threadProcess(END_MANUAL); \
        emit done(); \
        return; \
    } \
}

#define THREAD_TEST_CANCEL_S(post) \
{   if (QWinThread::g_threadStop) { \
        post; \
        Instr.close(); \
        emit threadProcess(END_MANUAL); \
        emit done(); \
        return; \
    } \
}

// Thread pause/continue support.
#define THREAD_TEST_PAUSE_S { threadLock.lock();
#define THREAD_TEST_PAUSE_E   threadLock.unlock(); }


#define CAL_THREAD_START(name,pts) \
    THREAD_TEST_CANCEL; \
    emit threadProcess(PREPARE); \
    THREAD_CHECK_BOX(QString("Prepare Calibrating %1").arg(name)); \
    emit threadProcess(STARTED); \
    initProgress(QString("Calibrating %1").arg(name),pts);

#define TEST_THREAD_START(name,pts) \
    THREAD_TEST_CANCEL; \
    emit threadProcess(PREPARE); \
    THREAD_CHECK_BOX(QString("Prepare Testing %1").arg(name)); \
    emit threadProcess(STARTED); \
    initProgress(QString("Testing %1").arg(name),pts);

#define THREAD_ENDED \
{   threadLock.lock(); \
    threadLock.unlock(); \
    Instr.close(); \
    emit threadProcess(END_NORMAL); \
    emit done(); \
    return; \
}

#define THREAD_ENDED_S(POST) \
{   threadLock.lock(); \
    threadLock.unlock(); \
    POST \
    Instr.close(); \
    emit threadProcess(END_NORMAL); \
    emit done(); \
    return; \
}

#define THREAD_CHECK_TEMP \
{   if (!checkTemp()) { \
        THREAD_ERROR_BOX("Pay Attention to The Temperature!"); \
        THREAD_ENDED; \
    } \
}

// Calibration/Test thread exception support.
#ifdef RD_EXCEPTION
    #define RD_CAL_TRY try {

    #define RD_CAL_CATCH \
        } catch (std::exception &e) { \
            THREAD_ERROR_BOX(QString("%1").arg(e.what())); \
            threadLock.lock(); \
            threadLock.unlock(); \
            Instr.close(); \
            emit threadProcess(END_EXCEPT); \
            emit done(); \
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
    rd::sp1401     *SP1401;
    rd::sp2401_r1a *SP2401;
    rd::sp3301     *SP3301;
    QObject       *parent;
    CalIOMode     mode;
    QCalBaseModel *model_0;
    QCalBaseModel *model_1;
    QwtCalData    *plotData_0;
    QwtCalData    *plotData_1;
    QwtCalData    *plotData_2;
    QString       rfFreqStar;
    QString       rfFreqStop;
    QString       rfFreqStep;
    range_freq_string freqStringCheck;
    bool          calX9119;
    LOLeak_Method methodLOLeak;
    SB_Method     methodSB;
    bool          justRebuildCoef;

    CalParam() :
        cal(false),
        check(false),
        SP1401(nullptr),
        SP2401(nullptr),
        SP3301(nullptr),
        parent(nullptr),
        model_0(nullptr),
        model_1(nullptr),
        calX9119(true),
        methodLOLeak(M_Spectrum),
        methodSB(M_Spectrum),
        justRebuildCoef(false)
    {
        rfFreqStar = QString("%1").arg(rd::ns_sp1401::tx_freq_star);
        rfFreqStop = QString("%1").arg(rd::ns_sp1401::tx_freq_stop);
        rfFreqStep = QString("%1").arg(rd::ns_sp1401::tx_freq_step_called);
    }
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

    CalR1CParam() :
        txMode(CAL_IO_OP),
        rxMode(CAL_IO_OP) {}
};


/*
 * But test parameters could be very fickle.
 * So I just put some common parameters here.
 */
class QTestBaseDlg;

struct TestBaseParam
{
public:
    QTestBaseDlg *parent;
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

    TestBaseParam() :
        parent(nullptr),
        model_0(nullptr),
        model_1(nullptr) {}
};

class QWinThread : public QThread
{
    Q_OBJECT

public:
    enum Process {
        PREPARE = 0,     // The thread is prepared,but not started yet.
        STARTED,         // The thread has been started and running.
        RUNNING_NORMAL,  // Running normally.
        RUNNING_EXCEPT,  // Some exceptions happened or something failed,but still running.
        PAUSED,          // The thread has been paused.
        END_NORMAL,      // The thread is normally ended.
        END_MANUAL,      // The thread is manually ended.
        END_EXCEPT       // Some exceptions are throwed and the thread is ended.
    };

public:
    explicit QWinThread(QObject *parent = nullptr);
    static int registerMetaType() { return qRegisterMetaType<Process>("Process"); }
    void initProgress(const QString name) { emit initProg(name); }
    void initProgress(const QString name, quint32 pts) { emit initProg(name,pts); }
    void setProgressPos(quint32 pos) { emit setProgPos(pos); }
    void addProgressPos(quint32 off) { emit addProgPos(off); }

signals:
    void initProg(const QString name);
    void initProg(const QString name, quint32 pts);
    void setProgPos(quint32 pos);
    void addProgPos(quint32 off);
    void done(bool success = true);
    void threadCheckBox(QString msg);
    void threadErrorBox(QString msg);
    void threadProcess(const Process p);

public:
    QMutex threadLock;
    static bool g_threadStop;
    static bool g_threadPausing;
    static QString  g_threadName;
    static QWinThread *g_threadThread;
};

class QCalTestBaseThread : public QWinThread
{
public:
    explicit QCalTestBaseThread(QObject *parent = nullptr) :
        QWinThread (parent), totalResult(true), totalPts(0) {}

    void updateTotalResult(bool singleResult)
    { totalResult = totalResult ? singleResult : totalResult; }

protected:
    bool totalResult;
    quint32 totalPts;
};

typedef class QCalBaseThread : public QCalTestBaseThread
{
    Q_OBJECT

public:
    explicit QCalBaseThread(const CalParam &param);
    void setTempRange(const double range) { tempRange = range; }

    static tm getCurTime();

    virtual void cal() {}
    virtual void check() {}
    virtual bool checkTemp();

    virtual void run()
    {
        RD_CAL_TRY
        if (calParam.cal) cal();
        if (calParam.check) check();
        THREAD_ENDED
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

    void uiInsert(const int first,const int last,const int cal_table);
    void uiUpdate(const int first,const int last,const int cal_table);

protected:
    sp3301 *SP3301;
    quint32 RFIdx;
    int32_t HWVer;
    double tempCur;
    double tempMin;
    double tempMax;
    double tempRange;
    CalParam calParam;
    range_freq_string freqStringCal;
    range_freq<quint64> freqRangeCal;
    range_freq_string freqStringCheck;
    range_freq<quint64> freqRangeCheck;

    std::string freqString;
    set_helper::range_freq<uint64_t> freqRange;
} QCalBaseThread, QExportBaseThread, QExpBaseThread;

typedef class QCalR1CBaseThread : public QCalBaseThread
{
public:
    explicit QCalR1CBaseThread(const CalParam &param) :
        QCalBaseThread(param)
    {
        SP1401 = dynamic_cast<rd::sp1401_r1c *>(param.SP1401);
        SP2401 = dynamic_cast<rd::sp2401_r1a *>(param.SP2401);

        RFVer = SP1401->get_hw_ver();
        RFIdx = SP1401->get_rf_idx();
    }

    double getRxAvgPwr()
    {
        double pwr = 0.0;
        rd::sp1401::pwr_meas_state_t state = rd::sp1401::PMS_IDLE;

        SP1401->pwr_meas_abort();
        SP1401->pwr_meas_start();

        while (state != rd::sp1401::PMS_DONE) {
            SP1401->get_pwr_meas_state(state);
            if (state == rd::sp1401::PMS_TT) {
                return -100.0;
            }
        }
        SP1401->get_pwr_meas_pwr(pwr);
        return pwr;
    }

protected:
    ns_sp1401::hw_ver_t RFVer;
    rd::sp1401_r1c *SP1401;
    rd::sp2401_r1a *SP2401;
} QCalR1CBaseThread, QExpR1CBaseThread;


class QTestThread : public QCalTestBaseThread
{
    Q_OBJECT

public:
    explicit QTestThread(const TestBaseParam *param);

    ~QTestThread() { SAFE_DEL(Param); }

    double getRxAvgPwr()
    {
        double pwr = 0.0;
        rd::sp1401::pwr_meas_state_t state = rd::sp1401::PMS_IDLE;

        SP1401->pwr_meas_abort();
        SP1401->pwr_meas_start();

        while (state != rd::sp1401::PMS_DONE) {
            SP1401->get_pwr_meas_state(state);
            if (state == rd::sp1401::PMS_TT) {
                return -100.0;
            }
        }
        SP1401->get_pwr_meas_pwr(pwr);
        return pwr;
    }

signals:
    void update(const QModelIndex &tl,
                const QModelIndex &br,
                rd::test_item_t item = rd::TI_RF_TX_FREQ_RES,
                int sec = 0);

protected:
    ns_sp1401::hw_ver_t RFVer;
    TestBaseParam *Param;
};


RD_INLINE bool calOP(const CalIOMode mode) { return (mode == CAL_OP || mode == CAL_IO_OP); }
RD_INLINE bool calIO(const CalIOMode mode) { return (mode == CAL_IO || mode == CAL_IO_OP); }
RD_INLINE bool testOP(const TestIOMode mode) { return calOP(mode); }
RD_INLINE bool testIO(const TestIOMode mode) { return calIO(mode); }

int exeFirProcess(char *path);
void getADS5474(rd::sp1401 *sp1401,qint64 &ad,qint32 avgTime = 3);

void threadCheckBox(const char *format, ...);
void threadErrorBox(const char *format, ...);
bool ftpRetryBox();
void testTXIOSwBox(const QColor TX, const QColor RX, int &exec);

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
