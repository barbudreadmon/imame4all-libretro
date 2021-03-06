
#ifndef __OSINLINE__
#define __OSINLINE__

/* What goes herein depends heavily on the OS. */

#define DIRTY_H 256
#define DIRTY_V 1600/16

extern char *dirty_new;
#define osd_mark_vector_dirty(x,y) dirty_new[(y)/16 * DIRTY_H + (x)/16] = 1

#if (IS_ARM)
#define vec_mult _vec_mult
INLINE int _vec_mult(int x, int y)
{
    int res_hi, res_lo;

    __asm__ __volatile__
    ("smull\t%1,%0,%2,%3"
    : "=r"(res_hi), "=r"(res_lo)
    : "r"(x), "r"(y)
    );

    return res_hi;
}
#endif

#include "minimal.h"

//dst=(src>>1)+(dst>>1);
#if (IS_ARM)
#define mix_sample _mix_sample
#define _mix_sample(dst,src) \
	__asm__ __volatile__ \
	( " mov %2, %2, asr #1 \n" \
	" add %0, %1, %2 , asr #1 \n" \
	: "=r" (dst) \
	: "r"  (src),"r" (dst) \
	)
#endif

//if (x < -32768)
//  x = -32768;
//else if (x > 32767)
//  x = 32767;
#define clip_short _clip_short
#define _clip_short(x) { int sign = x >> 31; if (sign != (x >> 15)) x = sign ^ ((1 << 15) - 1); }

#define clip_short_ret _clip_short_ret
INLINE int _clip_short_ret(int x) { _clip_short(x); return x; }


#endif /* __OSINLINE__ */
