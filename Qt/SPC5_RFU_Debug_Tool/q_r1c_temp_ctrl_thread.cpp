#include "q_r1c_temp_ctrl_thread.h"
#include "q_temp_ctrl_dlg.h"

QR1CTempCtrlThread::QR1CTempCtrlThread(TempParam *param,QObject *parent) :QWinThread(parent)
{
    this->param = *param;
//    if(TempTimer->isActive() == false)
//    {

//         connect(TempTimer, SIGNAL(timeout()), this, SLOT(updatetemper()));
//    }
//TempTimer->start(1000);
}

void QR1CTempCtrlThread::run()
{
//   THREAD_CHECK_BOX(QString("Prepare temperature record"));
   sp1401_r1c *pSP1401 = (sp1401_r1c *)(param._sp1401);
   QwtTempCtrlData *model_0 = ((QwtTempCtrlData *)param.plot_0);
   QwtTempCtrlData *model_1 = ((QwtTempCtrlData *)param.plot_1);
   QwtTempCtrlData *model_2 = ((QwtTempCtrlData *)param.plot_2);
   QwtTempCtrlData *model_3 = ((QwtTempCtrlData *)param.plot_3);
   QwtTempCtrlData *model_4 = ((QwtTempCtrlData *)param.plot_4);
   QwtTempCtrlData *model_5 = ((QwtTempCtrlData *)param.plot_5);
   QwtTempCtrlData *model_6 = ((QwtTempCtrlData *)param.plot_6);
   QwtTempCtrlData *model_7 = ((QwtTempCtrlData *)param.plot_7);
   double temp[10] = {0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8};

   tempdata_t data;
   qint64 idxOffsetStar = 0;
   qint64 idxOffset = 0;


   idxOffset = idxOffsetStar;
    if (1) {
        while (1) {
 //           CAL_THREAD_TEST_CANCEL
              sleep_ms(1000);
            temp[2] = 0;
            pSP1401->get_temp(0,data.temp[0]);
            temp[0] = data.temp[0];
            temp[1] = idxOffset;
            temp[2] += data.temp[0];
            data.i = idxOffset;
            model_0->calTable()->append(data);
            emit update(temp);

            pSP1401->get_temp(1,data.temp[0]);
            model_1->calTable()->append(data);
            temp[2] += data.temp[0];
            emit update(temp);

            pSP1401->get_temp(2,data.temp[0]);
            model_2->calTable()->append(data);
            emit update(temp);

            pSP1401->get_temp(3,data.temp[0]);
            model_3->calTable()->append(data);
            temp[2] += data.temp[0];
            emit update(temp);

            pSP1401->get_temp(4,data.temp[0]);
            model_4->calTable()->append(data);
            temp[2] += data.temp[0];
            emit update(temp);

            pSP1401->get_temp(5,data.temp[0]);
            model_5->calTable()->append(data);
            temp[2] += data.temp[0];
            emit update(temp);

            pSP1401->get_temp(6,data.temp[0]);
            model_6->calTable()->append(data);
            temp[2] += data.temp[0];
            emit update(temp);

            pSP1401->get_temp(7,data.temp[0]);
            model_7->calTable()->append(data);
            temp[2] += data.temp[0];
            temp[2] = temp[2] / 8;
            emit update(temp);

           idxOffset++;
        }


    }
    //   CAL_THREAD_ABOART
}



void QR1CTempCtrlThread::updatetemper(int x)
{
    double temp[10] = {0.0,1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8};
      for(int i=0;i<8;i++)
      {
          temp[i] = temp[i] * x;
      }
    //emit update(temp[]);
}
