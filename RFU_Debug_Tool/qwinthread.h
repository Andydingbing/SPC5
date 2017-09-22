#ifndef QWINTHREAD_H
#define QWINTHREAD_H

#include <QThread>
#include "define.h"

#define CAL_THREAD_TEST_CANCEL()    \
    if (g_bStop) {                  \
        Instrument.Close();         \
        emit done();                \
        return;                     \
    }

#define CAL_THREAD_TEST_CANCEL_S(post)  \
    if (g_bStop) {                      \
        post;                           \
        Instrument.Close();             \
        emit done();                    \
        return;                         \
    }

#define CAL_THREAD_ABOART() \
    Instrument.Close();     \
    emit done();            \
    return;

#define CAL_THREAD_ABOART_S(post)   \
    post;                           \
    Instrument.Close();             \
    emit done();                    \
    return;

#define THREAD_CHECK_BOX(msg)       \
    emit threadCheckBox(msg);       \
    if (QWinThread::g_bStop) {      \
        emit done();                \
        return;                     \
    }
#define THREAD_ERROR_BOX(msg)       \
    emit threadErrorBox(msg);

class QCalBaseModel;

typedef struct CalParam {
public:
    enum LOLeak_Method {
        M_Spectrum = 0,
        M_Loopback = 1
    };

    QDialog *m_pParent;
    QCalBaseModel *m_pModel;
    ISP1401 *m_pSP1401;
    CSP2401R1A *m_pSP2401;
    CSP3501 *m_pSP3501;
    QString m_strRfFreqStar;
    QString m_strRfFreqStop;
    QString m_strRfFreqStep;
    QString m_strIfFreqStar;
    QString m_strIfFreqStop;
    QString m_strIfFreqStep;
    bool m_bCalX9119;
    LOLeak_Method m_LOLeakMethod;
}CalParam;

class QWinThread : public QThread
{
    Q_OBJECT

public:
    QWinThread(QObject *parent = Q_NULLPTR);
signals:
    void initProg(const QString strName,int iPts);
    void setProgPos(int iPos);
    void done();
    void threadCheckBox(QString strMsg);
    void threadErrorBox(QString strmsg);
public:
    int32_t m_iFuncIdx;
    static bool g_bStop;
    static QString  g_strProc;
    static QWinThread *g_pThread;
};

class QCalBaseThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QCalBaseThread(CalParam *pParam,QObject *parent = Q_NULLPTR);
signals:
    void update(const QModelIndex topleft,const QModelIndex rightbottom);
protected:
    CalParam m_CalParam;
};

class QExportBaseThread : public QWinThread
{
    Q_OBJECT

public:
    explicit QExportBaseThread(CalParam *pParam,QObject *parent = Q_NULLPTR);
signals:
    void update(const QModelIndex topleft,const QModelIndex rightbottom);
protected:
    CalParam m_CalParam;
};

void threadCheckBox(const char *format,...);
void threadErrorBox(const char *format,...);

int freq2array(QString pin1,QString pin2,QString step,int64_t *pout = NULL,int64_t *pfreqstar = NULL,int64_t *pfreqstop = NULL,int64_t *pmax = NULL,int64_t *pmin = NULL);

extern QWinThread *g_pThread;
#endif // QWINTHREAD_H
