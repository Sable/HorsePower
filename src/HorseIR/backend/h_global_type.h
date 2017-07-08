
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

#define caseB case H_B:
#define caseH case H_H:
#define caseI case H_I:
#define caseL case H_L:
#define caseF case H_F:
#define caseE case H_E:
#define caseS case H_S:
#define caseA case H_A:


#define xp x->typ
#define xn x->len
#define xb x->b
#define xh x->h
#define xi x->i32
#define xl x->i64
#define xf x->f32
#define xe x->f64
#define xs x->s
#define xg x->g

#define xB(k) ((B*)(xg))[k]
#define xI(k) ((I*)(xg))[k]
#define xL(k) ((L*)(xg))[k]
#define xF(k) ((F*)(xg))[k]
#define xE(k) ((E*)(xg))[k]
#define xS(k) xL(k)
#define xG(k) ((V)(xg)+k)
