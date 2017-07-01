#pragma once

/* macros */

#define AL(typ,n) hMalloc(typ, n)

/* methods */

G hMalloc(H_T typ, L n);
L getTypeSize(H_T typ);
