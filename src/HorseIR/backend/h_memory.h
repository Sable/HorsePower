#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

/* macros */

#define AL(typ,n) hMalloc(typ, n)

/* methods */

G hMalloc(H_Type typ, L n);
L getTypeSize(H_Type typ);

#ifdef	__cplusplus
}
#endif