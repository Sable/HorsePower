/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t17 */
L q10_loopfusion_0(V z, V *x){
    // z -> t17
    V x0 = x[0]; // t10
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19931001),LT(vD(x0,i),19940101))) R 0;
}
/* num_func = 2, targ = t159 */
L q10_loopfusion_1(V z, V *x){
    // z -> t159
    V x0 = x[0]; // t147
    V x1 = x[1]; // t148
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q10_peephole_0(V *z, V y, V *x){
    V z0 = z[0]; // t101
    V z1 = z[1]; // t106
    V z2 = z[2]; // t107
    V x0 = x[0]; // k0
    V x1 = x[1]; // t89
    V x2 = x[2]; // t90
    L len = vn(y), k = 0;
    L lenZ = 0, parZ[H_CORE], offset[H_CORE];
    DOI(H_CORE, parZ[i]=offset[i]=0)
    CHECKE(getNumOfNonZero(y,parZ));
    DOI(H_CORE, lenZ += parZ[i])
    DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
    initV(z0,vp(x0),lenZ);
    initV(z1,vp(x1),lenZ);
    initV(z2,vp(x2),lenZ);
    DOT(len, if(vB(y,i)){L c=offset[tid]++;vLL(z0,c)=vLL(x0,i);vE(z1,c)=vE(x1,i);vE(z2,c)=vE(x2,i);}) R 0;
}
L q10_peephole_1(V *z, V *x, V y){
    L r0 = vn(y);// y -> t163
    V z0 = z[0]; // t173
    V x0 = x[0]; // t159
    initV(z0, H_E, r0);
    /* slow: 37967, max = 17, min = 1
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    }) */
    // fast: simple addition
    DOP(r0, {V t=vV(y,i);  E c0=0; \
        DOJ(vn(t), c0+=vE(x0,vL(t,j));) vE(z0,i)=c0; })
    R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5
E compiled_main(){
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t2   = allocNode(); V t5   = allocNode(); 
    V t6   = allocNode(); V t7   = allocNode(); V t10  = allocNode(); V t15  = allocNode(); 
    V t16  = allocNode(); V t17  = allocNode(); V t27  = allocNode(); V t28  = allocNode(); 
    V t29  = allocNode(); V t30  = allocNode(); V t31  = allocNode(); V t32  = allocNode(); 
    V t33  = allocNode(); V t35  = allocNode(); V t36  = allocNode(); V t37  = allocNode(); 
    V t38  = allocNode(); V t39  = allocNode(); V t48  = allocNode(); V t49  = allocNode(); 
    V t50  = allocNode(); V t51  = allocNode(); V t52  = allocNode(); V t53  = allocNode(); 
    V t54  = allocNode(); V t56  = allocNode(); V t57  = allocNode(); V t58  = allocNode(); 
    V t59  = allocNode(); V t61  = allocNode(); V t65  = allocNode(); V t75  = allocNode(); 
    V t76  = allocNode(); V t77  = allocNode(); V t79  = allocNode(); V t80  = allocNode(); 
    V t82  = allocNode(); V t83  = allocNode(); V t84  = allocNode(); V t89  = allocNode(); 
    V t90  = allocNode(); V t92  = allocNode(); V k0   = allocNode(); V t100 = allocNode(); 
    V t101 = allocNode(); V t106 = allocNode(); V t107 = allocNode(); V t118 = allocNode(); 
    V t119 = allocNode(); V t120 = allocNode(); V t122 = allocNode(); V t134 = allocNode(); 
    V t135 = allocNode(); V t136 = allocNode(); V t138 = allocNode(); V t139 = allocNode(); 
    V t141 = allocNode(); V t147 = allocNode(); V t148 = allocNode(); V t158 = allocNode(); 
    V t159 = allocNode(); V t160 = allocNode(); V t161 = allocNode(); V t162 = allocNode(); 
    V t163 = allocNode(); V t164 = allocNode(); V t165 = allocNode(); V t166 = allocNode(); 
    V t167 = allocNode(); V t168 = allocNode(); V t169 = allocNode(); V t170 = allocNode(); 
    V t171 = allocNode(); V t172 = allocNode(); V t173 = allocNode(); V t174 = allocNode(); 
    V t175 = allocNode(); V t176 = allocNode(); V t177 = allocNode(); V t178 = allocNode(); 
    V t179 = allocNode(); V t180 = allocNode(); V t181 = allocNode(); V t182 = allocNode(); 
    V t183 = allocNode(); V t184 = allocNode(); V t185 = allocNode(); V t186 = allocNode(); 
    tic;
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"nation")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"n_nationkey")));
    PROFILE(  2, t2 , pfnColumnValue(t2, t0, initLiteralSym((S)"n_name")));
    PROFILE(  3, t5 , pfnLoadTable(t5, initLiteralSym((S)"orders")));
    PROFILE(  4, t6 , pfnColumnValue(t6, t5, initLiteralSym((S)"o_orderkey")));
    PROFILE(  5, t7 , pfnColumnValue(t7, t5, initLiteralSym((S)"o_custkey")));
    PROFILE(  6, t10, pfnColumnValue(t10, t5, initLiteralSym((S)"o_orderdate")));
    PROFILE(  7, t17, q10_loopfusion_0(t17,(V []){t10}));
    PROFILE(  8, t27, pfnLoadTable(t27, initLiteralSym((S)"customer")));
    PROFILE(  9, t28, pfnColumnValue(t28, t27, initLiteralSym((S)"c_custkey")));
    PROFILE( 10, t29, pfnColumnValue(t29, t27, initLiteralSym((S)"c_name")));
    PROFILE( 11, t30, pfnColumnValue(t30, t27, initLiteralSym((S)"c_address")));
    PROFILE( 12, t31, pfnColumnValue(t31, t27, initLiteralSym((S)"c_nationkey")));
    PROFILE( 13, t32, pfnColumnValue(t32, t27, initLiteralSym((S)"c_phone")));
    PROFILE( 14, t33, pfnColumnValue(t33, t27, initLiteralSym((S)"c_acctbal")));
    PROFILE( 15, t35, pfnColumnValue(t35, t27, initLiteralSym((S)"c_comment")));
    PROFILE( 16, t36, pfnValues(t36, t7));
    PROFILE( 17, t37, pfnCompress(t37, t17, t36));
    PROFILE( 18, t38, pfnWhere(t38, t17));
    PROFILE( 19, t39, pfnIndex(t39, t6, t38));
    PROFILE( 20, t48, pfnIndex(t48, t28, t37));
    PROFILE( 21, t49, pfnIndex(t49, t29, t37));
    PROFILE( 22, t50, pfnIndex(t50, t30, t37));
    PROFILE( 23, t51, pfnFetch(t51, t31));
    PROFILE( 24, t52, pfnIndex(t52, t51, t37));
    PROFILE( 25, t53, pfnIndex(t53, t32, t37));
    PROFILE( 26, t54, pfnIndex(t54, t33, t37));
    PROFILE( 27, t56, pfnIndex(t56, t35, t37));
    PROFILE( 28, t57, pfnJoinIndex(t57,t1,t52,initLiteralSym((S)"eq")));
    PROFILE( 29, t58, pfnIndex(t58, t57, initLiteralI64(0)));
    PROFILE( 30, t59, pfnIndex(t59, t57, initLiteralI64(1)));
    PROFILE( 31, t61, pfnIndex(t61, t2, t58));
    PROFILE( 32, t65, pfnIndex(t65, t39, t59));
    PROFILE( 33, t75, pfnIndex(t75, t48, t59));
    PROFILE( 34, t76, pfnIndex(t76, t49, t59));
    PROFILE( 35, t77, pfnIndex(t77, t50, t59));
    PROFILE( 36, t79, pfnIndex(t79, t53, t59));
    PROFILE( 37, t80, pfnIndex(t80, t54, t59));
    PROFILE( 38, t82, pfnIndex(t82, t56, t59));
    PROFILE( 39, t83, pfnLoadTable(t83, initLiteralSym((S)"lineitem")));
    PROFILE( 40, t84, pfnColumnValue(t84, t83, initLiteralSym((S)"l_orderkey")));
    PROFILE( 41, t89, pfnColumnValue(t89, t83, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 42, t90, pfnColumnValue(t90, t83, initLiteralSym((S)"l_discount")));
    PROFILE( 43, t92, pfnColumnValue(t92, t83, initLiteralSym((S)"l_returnflag")));
    PROFILE( 44, k0 , pfnFetch(k0, t84));
    PROFILE( 45, t100, pfnEq(t100, t92, initLiteralChar('R')));
    PROFILE( 46, t107, q10_peephole_0((V []){t101,t106,t107},t100,(V []){k0,t89,t90}));
    //getInfoVar(t65); getInfoVar(t101); getchar();
    // Variable t65 has type H_L and len 57069
    // Variable t101 has type H_L and len 1478870
    PROFILE( 47, t118, pfnJoinIndex(t118,t65,t101,initLiteralSym((S)"eq")));
    PROFILE( 48, t119, pfnIndex(t119, t118, initLiteralI64(0)));
    PROFILE( 49, t120, pfnIndex(t120, t118, initLiteralI64(1)));
    PROFILE( 50, t122, pfnIndex(t122, t61, t119));
    PROFILE( 51, t134, pfnIndex(t134, t75, t119));
    PROFILE( 52, t135, pfnIndex(t135, t76, t119));
    PROFILE( 53, t136, pfnIndex(t136, t77, t119));
    PROFILE( 54, t138, pfnIndex(t138, t79, t119));
    PROFILE( 55, t139, pfnIndex(t139, t80, t119));
    PROFILE( 56, t141, pfnIndex(t141, t82, t119));
    PROFILE( 57, t147, pfnIndex(t147, t106, t120));
    PROFILE( 58, t148, pfnIndex(t148, t107, t120));
    PROFILE( 59, t159, q10_loopfusion_1(t159,(V []){t147,t148}));
    PROFILE( 60, t160, pfnList(t160, 7, (V []){t134 ,t135 ,t139 ,t138 ,t122 ,t136 ,t141}));
    //getInfoVar(t134); getchar();
    // Variable t134 has type H_L and len 114705
    PROFILE( 61, t161, pfnGroup(t161, t160));
    PROFILE( 62, t162, pfnKeys(t162, t161));
    PROFILE( 63, t163, pfnValues(t163, t161));
    PROFILE( 64, t164, pfnIndex(t164, t134, t162));
    PROFILE( 65, t165, pfnIndex(t165, t135, t162));
    PROFILE( 66, t166, pfnIndex(t166, t139, t162));
    PROFILE( 67, t167, pfnIndex(t167, t138, t162));
    PROFILE( 68, t168, pfnIndex(t168, t122, t162));
    PROFILE( 69, t169, pfnIndex(t169, t136, t162));
    PROFILE( 70, t170, pfnIndex(t170, t141, t162));
    PROFILE( 71, t173, q10_peephole_1((V []){t173},(V []){t159},t163));
    PROFILE( 72, t174, pfnList(t174, 1, (V []){t173}));
    PROFILE( 73, t175, pfnOrderBy(t175, t174, initLiteralBool(0)));
    PROFILE( 74, t176, pfnIndex(t176, t164, t175));
    PROFILE( 75, t177, pfnIndex(t177, t165, t175));
    PROFILE( 76, t178, pfnIndex(t178, t173, t175));
    PROFILE( 77, t179, pfnIndex(t179, t166, t175));
    PROFILE( 78, t180, pfnIndex(t180, t168, t175));
    PROFILE( 79, t181, pfnIndex(t181, t169, t175));
    PROFILE( 80, t182, pfnIndex(t182, t167, t175));
    PROFILE( 81, t183, pfnIndex(t183, t170, t175));
    PROFILE( 82, t184, copyV(t184, initLiteralSymVector(8, (S []){"c_custkey","c_name","revenue","c_acctbal","n_name","c_address","c_phone","c_comment"})));
    PROFILE( 83, t185, pfnList(t185, 8, (V []){t176 ,t177 ,t178 ,t179 ,t180 ,t181 ,t182 ,t183}));
    PROFILE( 84, t186, pfnTable(t186, t184, t185));
    toc;
    printV2(t186,20);
    R elapsed;
}
