#include "q_cal_r1c_rx_att_thread.h"
#include "q_model_rx_att.h"
#include "algorithm.h"

void QCalR1CRXAttThread::run()
{
    RD_CAL_TRY
    CAL_THREAD_START("RX Att Offset",freqRange.freqs.size());
    THREAD_CHECK_BOX("RX<===>Z28<===>Signal Generator");

    QR1CRxAttOPModel *modelOP = dynamic_cast<QR1CRxAttOPModel *>(calParam.model_0);
    QR1CRxAttIOModel *modelIO = dynamic_cast<QR1CRxAttIOModel *>(calParam.model_1);

    CalIOMode calMode = calParam.mode;

    quint64 freq = 0;
    qint32 secBfr = -1;
    qint32 secCur = 0;

    rx_filter_160m_table::data_m_t dataFilter;
    double coefReal[RX_FILTER_ORDER] = {0.0};
    double coefImag[RX_FILTER_ORDER] = {0.0};

    Instr.init();
    Instr.has_pm();
    Instr.has_sg();
    Instr.pm_reset();
    Instr.sg_reset();
    Instr.sg_set_en_mod(false);
    Instr.sg_set_pl(-60.0);
    Instr.sg_set_en_output(true);

    SP1401->set_tx_att(30.0,30.0,30.0,30.0);
    SP1401->set_rx_if_filter_sw(basic_sp1401::_160MHz);
    SP2401->set_dds_src(sp2401_r1a::ALL_0);
    SP2401->set_tx_filter_sw(false);
    SP2401->set_tx_pwr_comp(-5.0);
    SP2401->set_rx_pwr_comp(0);
    SP2401->set_rx_filter_sw(sp2401_r1a::_2I3D);
    SP1401->cf()->set_bw(_160M);

    if (calOP(calMode)) {
        SP1401->set_io_mode(OUTPUT);
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            CAL_THREAD_TEST_CANCEL

            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_rx_filter_160m->get(freq,&dataFilter);
            dataFilter._2double(coefReal,coefImag);
            SP2401->set_rx_filter(coefReal,coefImag);
            SP1401->set_rx_freq(freq);

            attIdx = 0;
            if (RFVer >= R1C && RFVer <= R1E) {
                // Ref   Att0   Att1   Att2   Att3
                //  30	  -20	 -25	-15	   -10
                //  10	  -20	  -5    -15	   -10 // state[0]
                offsetBase = 0;
                calOneSec(OUTPUT,freq,30,11);

                // Ref   Att0   Att1   Att2   Att3
                //  10      0	 -25    -15    -10 // state[1]
                // -10      0	  -5    -15    -10
                // -20      0	  -5     -5    -10
                offsetBase = 0;
                calOneSec(OUTPUT,freq,9,0);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-1,-10);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-11,-19);

                // Ref   Att0   Att1   Att2   Att3
                // -20     14	 -10    -15    -10 // state[2]
                // -30     14	   0    -15    -10
                // -40     14	   0     -5    -10
                offsetBase = 0;
                calOneSec(OUTPUT,freq,-21,-30);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-31,-40);
            } else if (RFVer >= R1F) {
                // Ref   Att0   Att1   Att2
                //  30    -20	 -20    -10
                //  10    -20      0    -10 // state[0]
                offsetBase = 0;
                calOneSec(OUTPUT,freq,30,11);

                // Ref   Att0   Att1   Att2
                //  10      0    -20    -10 // state[1]
                // -10      0	   0	-10
                offsetBase = 0;
                calOneSec(OUTPUT,freq,9,0);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-1,-9);

                // Ref   Att0   Att1   Att2
                // -10     14	 -15    -10 // state[2]
                // -25     14	   0    -10
                // -30     14      0     -5
                // -40     14      0    ...
                offsetBase = 0;
                calOneSec(OUTPUT,freq,-11,-20);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-21,-30);

                offsetBase = dataOP.offset[attIdx - 1];
                calOneSec(OUTPUT,freq,-31,-40);
            }

            dataOP.freq = freq;
            SP1401->get_temp(0,dataOP.temp[0]);
            SP1401->get_temp(1,dataOP.temp[1]);
            SP1401->get_temp(2,dataOP.temp[2]);
            SP1401->get_temp(3,dataOP.temp[3]);
            dataOP.time = getCurTime();

            secCur = freq_section(freq,freqRange);
            for (quint32 j = 0;j < attIdx;j ++) {
                if (secCur != secBfr) {
                    modelOP->iterTable(j)
                            ->at(secCur)
                            ->locate2CalTable(modelOP->calTable()->begin() + i);
                }
                modelOP->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;

            modelOP->calTable()->replace(i,dataOP);
            SP1401->cf()->add(cal_file::RX_ATT_OP,&dataOP);
            emit update(modelOP->index(i * attIdx,0),
                        modelOP->index((i + 1) * attIdx,14),
                        cal_file::RX_ATT_OP,
                        secCur);
            SET_PROG_POS(i + 1);
        }
        SP1401->cf()->w(cal_file::RX_ATT_OP);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        SP1401->set_io_mode(IO);
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            CAL_THREAD_TEST_CANCEL

            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_rx_filter_160m->get(freq,&dataFilter);
            dataFilter._2double(coefReal,coefImag);
            SP2401->set_rx_filter(coefReal,coefImag);
            SP1401->set_rx_freq(freq);

            attIdx = 0;
            if (RFVer >= R1C && RFVer <= R1E) {
                // Ref   Att0   Att1   Att2   Att3
                //  30    -20    -20    -13	   -10
                //  10    -20     -0    -13	   -10 // state[0]
                offsetBase = 0;
                calOneSec(IO,freq,30,11);

                // Ref   Att0   Att1   Att2   Att3
                //  10      0    -20    -13	   -10 // state[1]
                // -10      0      0    -13	   -10
                // -20      0      0     -3	   -10
                offsetBase = 0;
                calOneSec(IO,freq,9,0);

                offsetBase = dataIO.offset[attIdx - 1];
                calOneSec(IO,freq,-1,-10);

                offsetBase = dataIO.offset[attIdx - 1];
                calOneSec(IO,freq,-11,-19);

                // Ref   Att0   Att1   Att2   Att3
                // -20     14     -4    -13	   -10 // state[2]
                // -24     14      0    -13	   -10
                // -30     14      0     -7	   -10
                offsetBase = 0;
                calOneSec(IO,freq,-21,-30);
            } else if (RFVer >= R1F) {
                // Ref   Att0   Att1   Att2
                //  30      0    -30    -13
                //  10      0    -10    -13 // state[0]
                offsetBase = 0;
                calOneSec(IO,freq,30,11);

                //  10      0    -10    -13 // state[1] (=state[0])
                //   0      0      0    -13
                offsetBase = 0;
                calOneSec(IO,freq,9,1);

                //   0     14    -15    -13 // state[2]
                // -15     14      0    -13
                // -20     14      0     -8
                // -30     14      0    ...
                offsetBase = 0;
                calOneSec(IO,freq,-1,-10);

                offsetBase = dataIO.offset[attIdx - 1];
                calOneSec(IO,freq,-11,-20);

                offsetBase = dataIO.offset[attIdx - 1];
                calOneSec(IO,freq,-21,-30);
            }

            dataIO.freq = freq;
            SP1401->get_temp(0,dataIO.temp[0]);
            SP1401->get_temp(1,dataIO.temp[1]);
            SP1401->get_temp(2,dataIO.temp[2]);
            SP1401->get_temp(3,dataIO.temp[3]);
            dataIO.time = getCurTime();

            secCur = freq_section(freq,freqRange);
            for (quint32 j = 0;j < attIdx;j ++) {
                if (secCur != secBfr) {
                    modelIO->iterTable(j)
                            ->at(secCur)
                            ->locate2CalTable(modelIO->calTable()->begin() + i);
                }
                modelIO->iterTable(j)->at(secCur)->addOneData();
            }
            secBfr = secCur;

            modelIO->calTable()->replace(i,dataIO);
            SP1401->cf()->add(cal_file::RX_ATT_IO,&dataIO);
            emit update(modelIO->index(i * attIdx,0),
                        modelIO->index((i + 1) * attIdx,14),
                        cal_file::RX_ATT_IO,
                        secCur);
            SET_PROG_POS(i + 1);
        }
        SP1401->cf()->w(cal_file::RX_ATT_IO);
    }
    CAL_THREAD_ABOART
    RD_CAL_CATCH
}

void QCalR1CRXAttThread::calOneSec(io_mode_t mode, quint64 freq, qint32 refStar, qint32 refStop)
{
    rx_ref_table_r1cd::data_m_t dataRef;
    rx_ref_table_r1cd::rx_state_m_t state;

    qint64 adBase = 0;
    qint64 ad = 0;
    qint64 offset = 0;
    qint32 sgPwrTarget = refStop > 10 ? 10 : refStop;

    if (mode == OUTPUT) {
        SP1401->cf()->m_rx_ref_op->get_base(freq,&dataRef);
    } else if (mode == IO) {
        SP1401->cf()->m_rx_ref_io->get_base(freq,&dataRef);
    }

    // If the section is alongside one of the base states.
    // Then first set RX to that base state.
    // If not,the state before is the base state.
    if (RFVer >= R1C && RFVer <= R1E) {
        if (refStop >= 10) {
            setRXState(dataRef.state[0]);
        } else if (refStar < 10 && refStop >= 0) {
            setRXState(dataRef.state[1]);
        } else if (refStar < -20 && refStop >= -30) {
            setRXState(dataRef.state[2]);
        }
    } else if (RFVer >= R1F) {
        if (mode == OUTPUT) {
            if (refStop >= 10) {
                setRXState(dataRef.state[0]);
            } else if (refStar < 10 && refStop >= 0) {
                setRXState(dataRef.state[1]);
            } else if (refStar < -10 && refStop >= -20) {
                setRXState(dataRef.state[2]);
            }
        } else if (mode == IO) {
            if (refStop >= 10) {
                setRXState(dataRef.state[0]);
            } else if (refStar < 10 && refStop >= 0) {
                setRXState(dataRef.state[1]);
            } else if (refStar < 0 && refStop >= -10) {
                setRXState(dataRef.state[2]);
            }
        }
    }

    ajustSG(freq,sgPwrTarget);
    getADS5474(SP1401,adBase);

    if (refStar < 10) {
        adBase = dBc2ad(adBase,sgPwrTarget - (refStar + 1)) + offsetBase;
    }

    for (qint32 ref = refStar;ref >= refStop;ref += R1C_RX_REF_STEP) {
        CAL_THREAD_TEST_CANCEL

        if (mode == OUTPUT) {
            SP1401->cf()->m_rx_ref_op->get(RFVer,freq,double(ref),&state);
        } else if (mode == IO) {
            SP1401->cf()->m_rx_ref_io->get(RFVer,freq,double(ref),&state);
        }

        setRXState(state);
        getADS5474(SP1401,ad);
        offset = adBase - dBc2ad(ad,double(sgPwrTarget - ref));

        if (mode == OUTPUT) {
            dataOP.offset[attIdx] = offset;
        } else if (mode == IO) {
            dataIO.offset[attIdx] = offset;
        }
        attIdx ++;
    }
}

void QCalR1CRXAttThread::setRXState(rx_ref_op_table_r1cd::rx_state_m_t state)
{
    SP1401->set_rx_lna_att_sw(sp1401::rx_lna_att_t(state.lna_att));
    SP1401->set_rx_att_019_sw(sp1401::rx_att_019_t(state.att_019));
    SP1401->set_rx_att(double(state.att1),double(state.att2),double(state.att3));
    SP2401->set_rx_pwr_comp(state.ad_offset);
    msleep(10);
}

void QCalR1CRXAttThread::ajustSG(quint64 freq, qint32 pwr)
{
    double pmPwr = 0.0;
    Instr.sg_set_cw(double(freq));
    Instr.sg_set_pl(pwr + pmIL);
    Instr.pm_set_freq(double(freq));
    msleep(10);
    for (qint32 i = 0;i < 10;i ++) {
        Instr.pm_get_pwr(pmPwr);
        if (abs(pwr - pmPwr) < 0.05) {
            break;
        }
        Instr.sg_set_pl(pwr + pmIL + (pwr - pmPwr));
        msleep(10);
        pmIL += (pwr - pmPwr);
    }
    msleep(10);
    sgPwr = pwr;
}


void QExpR1CRXAttThread::run()
{
    INIT_PROG("Exporting RX Att",100);

    CalIOMode calMode = calParam.mode;
    quint64 freq = 0;
    int secBfr = -1;
    int secCur = 0;

    if(calOP(calMode)) {
        rx_att_op_table_r1cd::data_f_t data;
        QR1CRxAttOPModel *model = (QR1CRxAttOPModel *)(calParam.model_0);
        const quint32 offsetCnt = ARRAY_SIZE(data.offset);

        SP1401->cf()->map2buf(cal_file::RX_ATT_OP);
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_rx_att_op->get(freq,&data);

            secCur = freq_section(freq,freqRange);
            for (quint32 j = 0;j < offsetCnt;j ++) {
                if (secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }

            model->calTable()->replace(i,data);
            secBfr = secCur;
        }

        emit update(model->index(0,0),
                    model->index(freqRange.freqs.size() * offsetCnt,14),
                    cal_file::RX_ATT_OP,
                    secCur);
    }

    secBfr = -1;
    secCur = 0;

    if (calIO(calMode)) {
        rx_att_io_table_r1cd::data_f_t data;
        QR1CRxAttIOModel *model = (QR1CRxAttIOModel *)(calParam.model_1);
        const qint32 offsetCnt = ARRAY_SIZE(data.offset);

        SP1401->cf()->map2buf(cal_file::RX_ATT_IO);
        for (quint32 i = 0;i < freqRange.freqs.size();i ++) {
            freq = freqRange.freqs.at(i);
            SP1401->cf()->m_rx_att_io->get(freq,&data);

            secCur = freq_section(freq,freqRange);
            for (quint32 j = 0;j < offsetCnt;j ++) {
                if (secCur != secBfr)
                    model->iterTable(j)->at(secCur)->locate2CalTable(model->calTable()->begin() + i);
                model->iterTable(j)->at(secCur)->addOneData();
            }

            model->calTable()->replace(i,data);
            secBfr = secCur;
        }

        emit update(model->index(0,0),
                    model->index(freqRange.freqs.size() * offsetCnt,14),
                    cal_file::RX_ATT_IO,
                    secCur);
    }

    SET_PROG_POS(100);
    THREAD_ABORT
}

