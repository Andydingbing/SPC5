#include "arb_reader.h"
#include "liblog.h"
#include <string>

using namespace sp_rd;

int arb_reader::load(const char *path)
{
    if (m_fp) {
        fclose(m_fp);
        m_fp = nullptr;
	}

    m_fp = fopen(path,"rb");
    if (nullptr == m_fp) {
        fclose(m_fp);
        Log.set_last_err("can't open %s",path);
		return -1;
	}
	return 0;
}

FILE *arb_reader::load(param_t *param,vector<data_vector> *data)
{
    int file_size = 0;
    int total_samples = 0;
    file_header_t file_header;
    list<seg_header_t> seg_header;
    list<seg_header_t>::iterator iter_seg_header;
    list<arb_seg_t>	arb_seg;
    list<marker_list> marker;
    int marker_idx = 0;

    fseek(m_fp,0L,SEEK_END);
    file_size = ftell(m_fp);
    fseek(m_fp,0L,SEEK_SET);

    fread(&file_header,sizeof(file_header_t),1,m_fp);
    show(&file_header);

    seg_header.clear();
    arb_seg.clear();
    marker.clear();

    for (int i = 0;i < file_header.segments;i ++) {
        seg_header_t each_segment;
        fread(&each_segment,sizeof(seg_header_t),1,m_fp);
        seg_header.push_back(each_segment);
        show(&each_segment);

        total_samples += each_segment.samples;

        int num_markers = each_segment.marker_bytes / sizeof(marker_t);
        marker_t each_marker;
        marker_list each_seg_marker_list;
        arb_seg_t each_seg_arb_seg;

        each_seg_arb_seg.inter_factor   = uint32_t(245.76e6 / each_segment.sr);
        each_seg_arb_seg.seg_idx        = i;
        each_seg_arb_seg.samples        = each_segment.samples;
        each_seg_arb_seg.seg_adr        = i;
        each_seg_arb_seg.sr             = uint32_t(each_segment.sr);

        for (int j = 0;j < num_markers;j ++) {
            fread(&each_marker,sizeof(marker_t),1,m_fp);
            each_seg_marker_list.push_back(each_marker);

            marker_idx = each_marker.idx - 1;
            if (arb_reader::PERIOD == each_marker.type) {
                each_seg_arb_seg.markers[marker_idx].type = 0;
                each_seg_arb_seg.markers[marker_idx].marker.period.start  = each_marker.period.start_pos;
                each_seg_arb_seg.markers[marker_idx].marker.period.period = each_marker.period.period;
                each_seg_arb_seg.markers[marker_idx].marker.period.keep   = each_marker.period.duration;
			}
            else if (arb_reader::UNPERIOD == each_marker.type) {
                each_seg_arb_seg.markers[marker_idx].type = 1;
                each_seg_arb_seg.markers[marker_idx].marker.unperiod.cnt = each_marker.unperiod.cnt;
                for (int i =0;i < each_marker.unperiod.cnt;i ++){
                    each_seg_arb_seg.markers[marker_idx].marker.unperiod.start[i] = each_marker.unperiod.start_pos[i];
                    each_seg_arb_seg.markers[marker_idx].marker.unperiod.keep[i]  = each_marker.unperiod.duration[i];
				}
			}
		}
        arb_seg.push_back(each_seg_arb_seg);
        show(each_seg_marker_list);
        marker.push_back(each_seg_marker_list);
	}

    if (param) {
        param->file_size = file_size;
        param->total_samples = total_samples;
        param->file_header = file_header;
        param->seg_header = seg_header;
        param->arb_seg = arb_seg;
        param->marker = marker;
	}

    if (data) {
        iq_data_t iq_data;
		int i = 0;
        for (iter_seg_header = seg_header.begin();iter_seg_header != seg_header.end();iter_seg_header ++) {
            for (int j = 0;j < iter_seg_header->samples;j ++) {
                fread(&iq_data,sizeof(iq_data_t),1,m_fp);
                data->at(i).push_back(iq_data);
			}
			i ++;
		}
	}
    return m_fp;
}

void arb_reader::show(const file_header_t *p)
{
    Log.stdprintf("**************************** file header ****************************\n");
    Log.stdprintf("file_name    = %s\n",p->file_name);
    Log.stdprintf("date         = %s\n",p->date);
    Log.stdprintf("version      = %s\n",p->version);
    Log.stdprintf("segments     = %d\n",p->segments);
    Log.stdprintf("checksum     = 0x%x\n",p->checksum);
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(const seg_header_t *p)
{
    Log.stdprintf("************************** segment header ***************************\n");
    Log.stdprintf("samples      = %d\n",p->samples);
    Log.stdprintf("duration     = %d\n",p->duration);
    Log.stdprintf("level_offset = %f\n",p->level_offset);
    Log.stdprintf("peak_offset  = %f\n",p->peak_offset);
    Log.stdprintf("auth_info    = %s\n",p->auth_info);
    Log.stdprintf("name         = %s\n",p->name);
    Log.stdprintf("marker_bytes = %d\n",p->marker_bytes);
    Log.stdprintf("sr           = %f\n",p->sr);
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(marker_list &marker)
{
    Log.stdprintf("****************************** marker *******************************\n");
    marker_list::const_iterator iter;

    for (iter = marker.begin();iter != marker.end();iter ++) {
        Log.stdprintf("idx = %d\n",iter->idx);
        Log.stdprintf("type = %s\n",arb_reader::PERIOD == iter->type ? "period" : "unperiod");
        if (arb_reader::PERIOD == iter->type) {
            Log.stdprintf("start_pos = %d\n",iter->period.start_pos);
            Log.stdprintf("duration  = %d\n",iter->period.duration);
            Log.stdprintf("period    = %d\n",iter->period.period);
        } else if (arb_reader::UNPERIOD == iter->type) {
            for (int i = 0;i < iter->unperiod.cnt;i ++) {
                Log.stdprintf("start_pos = %d\n",iter->unperiod.start_pos[i]);
                Log.stdprintf("duration  = %d\n",iter->unperiod.duration[i]);
            }
        } else {
            Log.stdprintf("Unknown marker type");
        }
    }
    Log.stdprintf("*********************************************************************\n");
}

void arb_reader::show(const iq_data_t *p)
{
    Log.stdprintf("data.i = %d, data.q = %d\n",p->i,p->q);
}

int arb_reader::get_file_info(char *buf)
{
#define ADD_INFO() strcat(buf,info_buf);strcat(buf,"\r\n");

    char info_buf[576] = {0};

    list<seg_header_t>::iterator iter_seg_header;
    list<marker_list>::iterator iter_marker_list;
    param_t param;
    load(&param);
    iter_seg_header = param.seg_header.begin();
    iter_marker_list = param.marker.begin();

    sprintf(info_buf,"file_size = %d",param.file_size);
	ADD_INFO();

    sprintf(info_buf,"********** file header **********");
    ADD_INFO();	sprintf(info_buf,"file_name = %s",param.file_header.file_name);
    ADD_INFO(); sprintf(info_buf,"date = %s",param.file_header.date);
    ADD_INFO(); sprintf(info_buf,"version = %s",param.file_header.version);
    ADD_INFO(); sprintf(info_buf,"segments = %d",param.file_header.segments);
    ADD_INFO(); sprintf(info_buf,"checksum = %#x",param.file_header.checksum);
	ADD_INFO(); 
    sprintf(info_buf,"*********************************");
	ADD_INFO();

    int seg_idx = 0;
    for (iter_seg_header = param.seg_header.begin();iter_seg_header != param.seg_header.end();iter_seg_header ++) {
        int num_markers = iter_seg_header->marker_bytes / sizeof(marker_t);

        sprintf(info_buf,"segment:%d",seg_idx);
        ADD_INFO();	sprintf(info_buf," samples = %d",iter_seg_header->samples);
        ADD_INFO();	sprintf(info_buf," duration = %d",iter_seg_header->duration);
        ADD_INFO(); sprintf(info_buf," level_offset = %f",iter_seg_header->level_offset);
        ADD_INFO(); sprintf(info_buf," peak_offset = %f",iter_seg_header->peak_offset);
        ADD_INFO(); sprintf(info_buf," auth_info = %s",iter_seg_header->auth_info);
        ADD_INFO(); sprintf(info_buf," name = %s",iter_seg_header->name);
        ADD_INFO(); sprintf(info_buf," marker_bytes = %d,num_makers = %d",iter_seg_header->marker_bytes,num_markers);
        ADD_INFO(); sprintf(info_buf," sr = %f",iter_seg_header->sr);
		ADD_INFO();

        marker_list::const_iterator iter;

        for (iter = iter_marker_list->begin();iter != iter_marker_list->end();iter ++) {
            sprintf(info_buf,"   marker %d:",iter->idx);ADD_INFO();
            sprintf(info_buf,"    type = %s",arb_reader::PERIOD == iter->type ? "period" : "unperiod");ADD_INFO();
            if (arb_reader::PERIOD == iter->type) {
                sprintf(info_buf,"    start_pos = %d",iter->period.start_pos);ADD_INFO();
                sprintf(info_buf,"    duration = %d",iter->period.duration);ADD_INFO();
                sprintf(info_buf,"    period = %d",iter->period.period);ADD_INFO();
			}
            else if (arb_reader::UNPERIOD == iter->type) {
                for (int k = 0;k < iter->unperiod.cnt;k ++) {
                    sprintf(info_buf,"    start_pos = %d",iter->unperiod.start_pos[k]);ADD_INFO();
                    sprintf(info_buf,"    duration = %d",iter->unperiod.duration[k]);ADD_INFO();
				}
			}
			else {
                sprintf(info_buf,"    unknown marker type");
				ADD_INFO();
            }
		}

        seg_idx ++;
        iter_marker_list ++;
	}
	return 0;
}
