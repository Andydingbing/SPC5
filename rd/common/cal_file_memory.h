#ifndef RD_COMMON_CAL_FILE_MEMORY_H
#define RD_COMMON_CAL_FILE_MEMORY_H

#include "rd.h"
#include <boost/noncopyable.hpp>

#define convert_buf convert_buf_t::instance()

namespace rd {

class RD_API convert_buf_t : boost::noncopyable
{
public:
    void     *buf;
    char     sn[64];
    int32_t  table;
    uint32_t size;
    uint32_t size_table;

    convert_buf_t() : buf(nullptr),table(-1),size(0),size_table(0) { ZERO_ARRAY(sn); }
    ~convert_buf_t() { if (size > 0) { delete []static_cast<char *>(buf); } }
    static convert_buf_t &instance();

    void allocate(uint32_t new_size);
};

} // namespace rd

#endif // RD_COMMON_CAL_FILE_MEMORY_H
