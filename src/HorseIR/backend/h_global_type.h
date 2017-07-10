
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

#define xp  x->typ
#define xn  x->len

#define xb  x->b
#define xh  x->h
#define xi  x->i32
#define xl  x->i64
#define xf  x->f32
#define xe  x->f64
#define xc  x->c
#define xs  x->s
#define xy  x->y
#define xm  x->m
#define xd  x->d
#define xz  x->z
#define xu  x->u
#define xw  x->w
#define xt  x->t
#define xx  x->x
#define xg  x->g

#define xB(k) ((B*)(xg))[k]
#define xI(k) ((I*)(xg))[k]
#define xL(k) ((L*)(xg))[k]
#define xF(k) ((F*)(xg))[k]
#define xE(k) ((E*)(xg))[k]
#define xS(k) xL(k)
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



