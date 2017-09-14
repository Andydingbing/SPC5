#ifndef __E4443A_H__
#define __E4443A_H__

#include "N9030A.h"

class E4443A : public N9030A
{
public:
	E4443A(void);
	virtual ~E4443A(void);
	virtual char* GetDes();
};

#endif