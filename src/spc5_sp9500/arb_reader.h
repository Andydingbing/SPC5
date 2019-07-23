#ifndef ARB_READER_H
#define ARB_READER_H

#include "rd.h"
#include <stdio.h>
#include <list>
#include <vector>
#include <boost/noncopyable.hpp>

using namespace std;

namespace sp_rd {

struct arb_seg_t
{
    uint32_t seg_idx;
	uint32_t samples;
	uint32_t seg_adr;
    uint32_t inter_factor;
	uint32_t sr;
    struct marker_t {
		int type;
        union {
            struct {
				int start;
				int keep;
				int period;
			} period;
            struct {
                int cnt;
				int start[1024];
				int keep[1024];
			} unperiod;
		} marker;
    } markers[4];
};

union as_reg_t
{
	struct {unsigned sample			:32;} adr0;
	struct {unsigned seg_adr		:32;} adr1;
    struct {unsigned inter_factor	:32;} adr2;
	struct {unsigned mkr1_start		:32;} adr3;
	struct {unsigned mkr1_keep		:32;} adr4;
	struct {unsigned mkr1_period	:32;} adr5;
	struct {unsigned mkr2_start		:32;} adr6;
	struct {unsigned mkr2_keep		:32;} adr7;
	struct {unsigned mkr2_period	:32;} adr8;
	struct {unsigned mkr3_start		:32;} adr9;
	struct {unsigned mkr3_keep		:32;} adr10;
	struct {unsigned mkr3_period	:32;} adr11;
	struct {unsigned mkr4_start		:32;} adr12;
	struct {unsigned mkr4_keep		:32;} adr13;
	struct {unsigned mkr4_period	:32;} adr14;
	struct {unsigned sr				:32;} adr16;
	struct {
		unsigned mkr1_type  : 1;
		unsigned mkr2_type	: 1;
		unsigned mkr3_type	: 1;
		unsigned mkr4_type  : 1;
		unsigned mkr1_onoff : 1;
		unsigned mkr2_onoff : 1;
		unsigned mkr3_onoff : 1;
		unsigned mkr4_onoff : 1;
	}adr17;
	uint32_t data;
};

#define AS_REG(addr) as_reg[addr].adr##addr

class RD_API arb_reader : boost::noncopyable
{
public:
    arb_reader() : m_fp(nullptr) {}
    ~arb_reader() { SAFE_CLOSE_FP(m_fp) }

public:
    enum marker_type_t {
        PERIOD,
        UNPERIOD
	};

    struct file_header_t {
        char file_name[512];
        char date[512];
        char version[512];
        int  segments;
        int  checksum;
    };

    struct seg_header_t {
        int   id;
        int   samples;
        int   duration;
        float level_offset;
        float peak_offset;
        char  name[512];
        char  auth_info[32];
        int   marker_bytes;
        float sr;
    };

    struct marker_t {
        int idx;
        int type;
        union {
            struct {
                int start_pos;
                int duration;
                int period;
            } period;
			struct {
                int cnt;
                int start_pos[1024];
                int duration[1024];
            } unperiod;
        };
    };

    typedef list<marker_t> marker_list;
    typedef vector<iq_data_t> data_vector;

    typedef struct param_t {
        int                 file_size;
        int                 total_samples;
        file_header_t       file_header;
        list<seg_header_t>  seg_header;
        list<arb_seg_t>     arb_seg;
        list<marker_list>   marker;
    }param_t;
public:
    int load(const char *path);
    FILE* load(param_t *param = nullptr,vector<data_vector> *data = nullptr);
    bool is_loaded() { return (m_fp == nullptr ? false : true); }
    int get_file_info(char *buf);
private:
    void show(const file_header_t *p);
    void show(const seg_header_t *p);
    void show(marker_list &marker);
    void show(const iq_data_t *p);
public:
    FILE *m_fp;
};

} //namespace sp_rd

#endif // ARB_READER_H


