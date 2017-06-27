#pragma once

/* macros */

#define AL(typ,n) h_malloc(typ, n)

/* methods */

G h_malloc(H_T typ, L n);
