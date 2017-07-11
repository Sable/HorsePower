#pragma once

#ifdef	__cplusplus
extern "C" {
#endif

#define vp(x)  x->typ
#define vn(x)  x->len

#define vb(x)  x->b
#define vh(x)  x->h
#define vi(x)  x->i32
#define vl(x)  x->i64
#define vf(x)  x->f32
#define ve(x)  x->f64
#define vc(x)  x->c
#define vs(x)  x->s
#define vy(x)  x->y
#define vm(x)  x->m
#define vd(x)  x->d
#define vz(x)  x->z
#define vu(x)  x->u
#define vw(x)  x->w
#define vt(x)  x->t
#define vx(x)  x->x
#define vg(x)  x->g

#define vB(x) (B*)(vg(x))
#define vL(x) (L*)(vg(x))
#define vS(x) (S*)(vg(x))

#define isSymbol(x) (vp(x) == H_S)
#define isOneSymbol(x) (isSymbol(x) && vn(x)==1)

#ifdef	__cplusplus
}
#endif


