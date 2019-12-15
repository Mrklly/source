#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

#include <stdint.h>
#include "base_types.h"
#include "hc32l13x.h"
#include "system_hc32l13x.h"
#include "gpio.h"


typedef enum _knob_direction_t_ {
    DIRECTION_NONE   = 0,
    DIRECTION_JITTER = 1,
    DIRECTION_CW_JIT = 2,
    DIRECTION_CCW_JIT= 3,
    DIRECTION_CW     = 4,
    DIRECTION_CCW    = 5
} knob_direction;


extern volatile knob_direction g_knob_direction;


#ifndef size_of
#   define size_of(_obj)                (sizeof(_obj)/sizeof((_obj)[0]))
#endif

#ifndef top_of
#   define top_of(_obj)                 (&(_obj)[size_of(_obj)-1])
#endif


/* 以大端格式读取数值内存数值 */
#ifndef GET_BE_UINT16
#   define GET_BE_UINT16(pt)            ( ((uint16)__vat(pt,0) <<  8) \
                                        | ((uint8 )__vat(pt,1) <<  0) )

#   define GET_BE_UINT24(pt)            ( ((uint32)__vat(pt,0) << 16) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint8 )__vat(pt,2) <<  0) )

#   define GET_BE_UINT32(pt)            ( ((uint32)__vat(pt,0) << 24) \
                                        | ((uint32)__vat(pt,1) << 16) \
                                        | ((uint16)__vat(pt,2) <<  8) \
                                        | ((uint8 )__vat(pt,3) <<  0) )

#endif /* #ifndef GET_BE_UINT16 */
#ifndef __vat
#   define __vat(_base,_offset)         (((uint8*)(_base))[_offset])
#endif
/* 以大端格式填充内存 */
#ifndef PUT_BE_UINT16
#   define PUT_BE_UINT16(pt,val)    do{ __vat(pt,0) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,1) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#   define PUT_BE_UINT24(pt,val)    do{ __vat(pt,0) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#   define PUT_BE_UINT32(pt,val)    do{ __vat(pt,0) = (uint8)((uint32)(val) >> 24); \
                                        __vat(pt,1) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,2) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,3) = (uint8)((uint8 )(val) >>  0); \
                                       }while(0)

#endif /* #ifndef PUT_BE_UINT16 */

/* 以小端格式读取数值内存数值 */
#ifndef GET_LE_UINT16
#   define GET_LE_UINT16(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) )

#   define GET_LE_UINT24(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint32)__vat(pt,2) << 16) )

#   define GET_LE_UINT32(pt)            ( ((uint8 )__vat(pt,0) <<  0) \
                                        | ((uint16)__vat(pt,1) <<  8) \
                                        | ((uint32)__vat(pt,2) << 16) \
                                        | ((uint32)__vat(pt,3) << 24) )


#endif /* #ifndef GET_LE_UINT16 */

/* 以小端格式填充内存数据 */
#ifndef PUT_LE_UINT16
#   define PUT_LE_UINT16(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                       }while(0)

#   define PUT_LE_UINT24(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint32)(val) >> 16); \
                                       }while(0)

#   define PUT_LE_UINT32(pt,val)    do{ __vat(pt,0) = (uint8)((uint8 )(val) >>  0); \
                                        __vat(pt,1) = (uint8)((uint16)(val) >>  8); \
                                        __vat(pt,2) = (uint8)((uint32)(val) >> 16); \
                                        __vat(pt,3) = (uint8)((uint32)(val) >> 24); \
                                       }while(0)
#endif



#endif

