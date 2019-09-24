#ifndef Q_MODEL_TEMP_PWR_H
#define Q_MODEL_TEMP_PWR_H

#include "q_base_model.h"
#include "test_data.hpp"

class QwtTempPwrData : public QwtTestData
{
public:
    struct Point
    {
        common_tx_temp_state_t temp;
        float avg_temp;
        float pwr;
        Point() : avg_temp(0.0), pwr(0.0) {}
    };

    explicit QwtTempPwrData() :
        QwtTestData() {}

    QVector<QList<Point>> *allData() { return &_allData; }

    QVector<Point> *curveData() { return &_curveData; }

    bool add(const float temp, const Point &p)
    {
        int tempIdx = SERIE_INDEX(temp,g_temp_star,g_temp_step);
        QList<Point> *list = &(_allData[tempIdx]);
        QList<Point>::iterator iter;
        int times = list->size();

        if (!list->empty()) {
            for (iter = list->begin();iter != list->end();iter ++) {
                if (abs(iter->pwr - p.pwr) > 0.1f) {
                    return false;
                }
            }
        }
        list->push_back(p);

        _curveData[tempIdx].pwr = (_curveData.at(tempIdx).pwr * times + p.pwr) / (times + 1);
        return true;
    }

    size_t size() const { return size_t(_curveData.size()); }

    QPointF sample(size_t i) const
    { return QPointF(double(SERIE_VALUE(g_temp_star,g_temp_step,i)),double(_curveData[i].pwr)); }

    void reset()
    {
        _allData.clear();
        _curveData.clear();

        for (quint32 i = 0;i < SERIE_SIZE(g_temp_star,g_temp_stop,g_temp_step);i ++) {
            _allData.push_back(QList<Point>());
            _curveData.push_back(Point());
        }
    }

private:
    QVector<QList<Point>> _allData;
    QVector<Point> _curveData;
};

#endif // Q_MODEL_TEMP_PWR_H
