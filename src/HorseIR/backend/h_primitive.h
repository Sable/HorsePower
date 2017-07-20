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

#define sB(x) ((B*)(vg(x)))
#define sH(x) ((B*)(vg(x)))
#define sI(x) ((B*)(vg(x)))
#define sL(x) ((L*)(vg(x)))
#define sE(x) ((L*)(vg(x)))
#define sF(x) ((L*)(vg(x)))
#define sS(x) ((S*)(vg(x)))
#define sX(x) ((X*)(vg(x)))
#define sV(x) ((V )(vg(x)))

#define vB(x,i) sB(x)[i]
#define vH(x,i) sH(x)[i]
#define vI(x,i) sI(x)[i]
#define vL(x,i) sL(x)[i]
#define vE(x,i) sE(x)[i]
#define vF(x,i) sF(x)[i]
#define vS(x,i) sS(x)[i]
#define vX(x,i) sX(x)[i]
#define vV(x,i) sV(x)[i]

#define xReal(x) ((x).real)
#define xImag(x) ((x).imag)

#define isBool(x)      (H_B==vp(x))
#define isSymbol(x)    (H_S==vp(x))
#define isInteger(x)   (H_L==vp(x))
#define isComplex(x)   (H_X==vp(x))
#define isOneSymbol(x) (isSymbol(x) && vn(x)==1)

L pfnColumnValue   (V z, V x, V y);
L pfnIndexOf       (V z, V x, V y);
L pfnFindValidIndex(V z, V x, V y);
L pfnFindValidItem (V z, V x, V y);
L pfnIndex         (V z, V x, V y);
L pfnList          (V z, L n, ...);
L pfnDict          (V z, V x, V y);
L pfnTable         (V z, V x);


/* Implement in order */

L pfnAbs           (V z, V x);
L pfnNeg           (V z, V x);
L pfnCeil          (V z, V x);
L pfnFloor         (V z, V x);
L pfnRound         (V z, V x);

L pfnConj          (V z, V x);
L pfnRecip         (V z, V x);
L pfnSignum        (V z, V x);
L pfnPi            (V z, V x);
L pfnNot           (V z, V x);

#ifdef	__cplusplus
}
#endif


