
#define H_B 0
#define H_H 1
#define H_I 2
#define H_L 3
#define H_F 4
#define H_E 5
#define H_C 6
#define H_S 7
#define H_Y 8
#define H_M 9
#define H_D 10
#define H_Z 11
#define H_U 12
#define H_W 13
#define H_T 14
#define H_X 15
#define H_G 16 //list
#define H_N 17 //dict
#define H_A 18 //table
#define H_K 19 //ktable
#define H_V 20 //value item

#define caseB case H_B:
#define caseH case H_H:
#define caseI case H_I:
#define caseL case H_L:
#define caseF case H_F:
#define caseE case H_E:
#define caseC case H_C:
#define caseS case H_S:
#define caseY case H_Y:
#define caseM case H_M:
#define caseD case H_D:
#define caseZ case H_Z:
#define caseU case H_U:
#define caseW case H_W:
#define caseT case H_T:
#define caseX case H_X:
#define caseG case H_G:
#define caseN case H_N:
#define caseA case H_A:
#define caseK case H_K:
#define caseV case H_V:

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
#define sH(x) ((H*)(vg(x)))
#define sI(x) ((I*)(vg(x)))
#define sL(x) ((L*)(vg(x)))
#define sE(x) ((E*)(vg(x)))
#define sF(x) ((F*)(vg(x)))
#define sS(x) ((L*)(vg(x)))
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

/* special x */

#define xp  vp(x)
#define xn  vn(x)

#define xb  vb(x)
#define xh  vh(x)
#define xi  vi(x)
#define xl  vl(x)
#define xf  vf(x)
#define xe  ve(x)
#define xc  vc(x)
#define xs  vs(x)
#define xy  vy(x)
#define xm  vm(x)
#define xd  vd(x)
#define xz  vz(x)
#define xu  vu(x)
#define xw  vw(x)
#define xt  vt(x)
#define xx  vx(x)
#define xg  vg(x)

#define xB(k) vB(x,k)
#define xH(k) vH(x,k)
#define xI(k) vI(x,k)
#define xL(k) vL(x,k)
#define xF(k) vF(x,k)
#define xE(k) vE(x,k)
#define xS(k) vS(x,k)
#define xG(k) ((V)(xg)+k)
#define xV(k) ((V)(xg)+k)
#define xN(k) xV(k)

/* save pointer to a slot */
#define setXV(k, val) {V t=((V)xg)+(k); P("t = %lld\n",t); t=val; P("after: xg = %lld, t = %lld\n",xg,t);}

#define isB() (H_B==xp)
#define isH() (H_H==xp)
#define isA() (H_A==xp)
#define isN() (H_N==xp)

#define VDexl(x, k) (((V)((x)->g))+(k))



