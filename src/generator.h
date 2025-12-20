#ifndef __MICROB_GENERATOR_H__
#define __MICROB_GENERATOR_H__

#include "config.h"

enum gmk_type
{
    GMK_TYPE__AVR = 0
};


int mb_generator__makefile(const struct config*, enum gmk_type);


#endif // __MICROB_GENERATOR_H__