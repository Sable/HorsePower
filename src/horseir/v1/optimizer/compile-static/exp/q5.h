/*==== FE: Loop fusion with elementwise functions ====*/
/* num_func = 3, targ = t73 */
L q5_loopfusion_0(V z, V *x){
    // z -> t73
    V x0 = x[0]; // t66
    initV(z,H_B,vn(x0));
    DOP(vn(x0), vB(z,i)=AND(GEQ(vD(x0,i),19940101),LT(vD(x0,i),19950101))) R 0;
}
/* num_func = 2, targ = t226 */
L q5_loopfusion_1(V z, V *x){
    // z -> t226
    V x0 = x[0]; // t214
    V x1 = x[1]; // t215
    initV(z,H_E,vn(x0));
    DOP(vn(x0), vE(z,i)=MUL(vE(x0,i),MINUS(1,vE(x1,i)))) R 0;
}
/*==== FP: Loop fusion with patterns ====*/
L q5_peephole_0(V *z, V *x, V y){
    L r0 = vn(y);// y -> t230
    V z0 = z[0]; // t234
    V x0 = x[0]; // t226
    initV(z0, H_E, r0);
    /* L v_min = 9999999, v_max = -1;
    DOI(r0, {V t=vV(y,i); \
            if(vn(t)<v_min)v_min=vn(t); \
            if(vn(t)>v_max)v_max=vn(t);})
    P("max = %lld, min = %lld\n", v_max, v_min); getchar(); */
    DOJ(r0, {V t=vV(y,j);  E c0=0; \
        DOP(vn(t), {L k=vL(t,i); c0+=vE(x0,k);},\
        reduction(+:c0)) \
        vE(z0,j)=c0; \
    })
    R 0;
}
// Pattern FP3
// Pattern FP4
// Pattern FP5

// i = 59, j = 0, val = 66
// i = 60, j = 0, val = 66
//static void temp_func0(V x, V y){
//    P("loop size: %lld x %lld\n", vn(x),vn(y)); L tot = 0;
//    DOI(vn(x), DOJ(vn(y), if(vL(x,i)==vL(y,j) && vL(x,i)==66){P("i = %lld, j = %lld, val = %lld\n",i,j,vL(x,i));tot++; if(tot>1)R ;})) P("not found\n");
//}
//
//static L fetchIndexList(HN x, L *val){
//    if(x){
//        //P(" entering ...\n");
//        L c = 1;
//        val[0] = hD(x);
//        //P(" step 1\n");
//        HI t0 = hT(x);
//        //P(" step 2\n");
//        while(t0){val[c++]=t0->ival; t0=t0->inext; P("c = %lld\n",c);}
//        //P(" step 3\n");
//        if(c>=99) EP("error when c >= 99, it is %lld\n", c);
//        //if(c!=1){ EP("c is not equal 1: %lld\n",c); }
//        R c;
//    }
//    else R 0;
//}

// static void printDebugHashInfo(){
// #define SPACE "  "
// if(debug_hash_count1 != 0) {
//     //P(" find1/total1 = %.2lf %% (%lld/%lld)\n", debug_hash_find1*100.0/debug_hash_total1, debug_hash_find1, debug_hash_total1);
//     //P(" find2/total2 = %.2lf %% (%lld/%lld)\n", debug_hash_find2*100.0/debug_hash_total2, debug_hash_find2, debug_hash_total2);
//     P(SPACE "max1 = %lld, max2 = %lld\n", debug_hash_max1, debug_hash_max2);
//     P(SPACE "toal_op1/queries = %.2lf (%lld/%lld)\n", debug_hash_op_total1*1.0/debug_hash_count1, debug_hash_op_total1, debug_hash_count1);
//     P(SPACE "toal_op2/queries = %.2lf (%lld/%lld)\n", debug_hash_op_total2*1.0/debug_hash_count2, debug_hash_op_total2, debug_hash_count2);
//     P(SPACE "need_search1/queries = %.2lf %% (%lld/%lld)\n", debug_hash_need_search1*100.0/debug_hash_count1, debug_hash_need_search1, debug_hash_count1);
//     P(SPACE "need_search2/queries = %.2lf %% (%lld/%lld)\n", debug_hash_need_search2*100.0/debug_hash_count2, debug_hash_need_search2, debug_hash_count2);
// }
//     printHashInfo();
//     P("\n"); 
// }

//static L q5_chf_0(V z, V *x){
//    P("======= q5_chf_0 =======\n");
//    V x0 = x[0]; // l_orderkey
//    V x1 = x[1]; // o_orderkey
//    V x2 = x[2]; // l_suppkey
//    V x3 = x[3]; // n_nationkey
//    V x4 = x[4]; // c_nationkey
//    V x5 = x[5]; // s_suppkey
//    V x6 = x[6]; // s_nationkey
//    L total = 0;
//    L hash1len, hash2len;
//    V *hash1set = (V []){x1};
//    V *hash2set = (V []){x5,x6,x6};
//    L indx1[99], indx2[99];
//    E elapsed=0;
//    //HN hash1 = createHashWithV(1,hash1set,&hash1len);
//    //HN hash2 = createHashWithV(3,hash2set,&hash2len);
//    HN hash1 = create_hash_multiply(x1);
//    HN hash2 = create_hash_multiply(x5);
//    //temp_func0(x0, x1); getchar();
//    //DOI(200, P("%lld ",vL(x1,i))) P("\n"); getchar();
//    tic;
//    for(L i=0,i2=vn(x0); i<i2;i++){
//        HN h1=find_hash_value_single(hash1,hash1set,hash1len,1,vL(x0,i)); 
//        L n1 = fetchIndexList(h1, indx1);
//        // if(n1 > 1) { P("n1 = %lld\n", n1); isRepeat1 = true; }
//        for(L j=0;j<n1;j++){
//            L j2=indx1[j];
//            //P(" indx1[%lld] = %lld\n",j,indx1[j]);
//            HN h2=find_hash_value_compound(hash2,hash2set,hash2len,3,vL(x2,i),vL(x3,j2),vL(x4,j2)); 
//            //P(" step 21\n");
//            L n2 = fetchIndexList(h2, indx2);
//            //P(" step 22\n");
//            //if(n2 > 1) { P("n2 = %lld\n", n2); isRepeat2 = true; }
//            for(L k=0;k<n2;k++){
//            //P(" step 23\n");
//                L k2=indx2[k];
//                // P("a[%lld] = %lld, b[%lld] = %lld\n", i,vL(x0,i),j2,vL(x1,j2));
//                // P("c[%lld] = %lld,%lld,%lld\n", i,vL(x2,i),vL(x3,j2),vL(x4,j2));
//                // P("d[%lld] = %lld,%lld,%lld\n", i,vL(x5,k2),vL(x6,k2),vL(x6,k2)); getchar();
//                //if(total < 20) P("i = %lld\n", i);
//                total++;
//            }
//        }
//    }
//    toc;
//    P("total0 = %lld\n", total); //printDebugHashInfo(); //getchar();
//    R 0;
//}

// static L q5_chf_1(V z, V *x){
//     P("======= q5_chf_1 =======\n");
//     V x0 = x[0]; // l_orderkey
//     V x1 = x[1]; // o_orderkey
//     V x2 = x[2]; // l_suppkey
//     V x3 = x[3]; // n_nationkey
//     V x4 = x[4]; // c_nationkey
//     V x5 = x[5]; // s_suppkey
//     V x6 = x[6]; // s_nationkey
//     L total = 0;
//     L hash1len, hash2len;
//     V *hash1set = (V []){x1};
//     V *hash2set = (V []){x5,x6,x6};
//     L indx1[99], indx2[99];
//     E elapsed=0;
//     THash th1 = createHashWithV2(1,hash1set,&hash1len);
//     THash th2 = createHashWithV2(3,hash2set,&hash2len);
//     profileHashTable(th1); getchar();
// //    P("profiling x1\n"); profileDupDataV(x1); getchar();
//     //profileHashTable(th2); getchar();
//     HN *hash1 = th1.hashTable;
//     HN *hash2 = th2.hashTable;
//     L  *hash1Size = th1.hashSize;
//     L  *hash2Size = th2.hashSize;
//     //temp_func0(x0, x1); getchar();
//     //DOI(200, P("%lld ",vL(x1,i))) P("\n"); getchar();
//     //B isRepeat1 = false;
//     //B isRepeat2 = false;
//     /*
//     P("profiling x1\n"); profileDupDataV(x1); getchar();
//     P("profiling x5\n"); profileDupDataV(x5); getchar();
//     P("profiling x6\n"); profileDupDataV(x6); getchar();
//     (" -------\n");
//     P("profiling x0\n"); profileDupDataV(x0); getchar();
//     P("profiling x2\n"); profileDupDataV(x2); getchar();
//     P("profiling x3\n"); profileDupDataV(x3); getchar();
//     P("profiling x4\n"); profileDupDataV(x4); getchar();
//     */
//     tic;
//     for(L i=0,i2=vn(x0); i<i2;i++){
//         HN h1=findValueFromHash2(hash1,hash1set,hash1Size,1,vL(x0,i)); 
//         L n1 = fetchIndexList(h1, indx1);
//         //if(n1 > 1) { P("n1 = %lld\n", n1); isRepeat1 = true; }
//         for(L j=0;j<n1;j++){
//             L j2=indx1[j];
//             //P(" indx1[%lld] = %lld\n",j,indx1[j]);
//             HN h2=findValueFromHash2(hash2,hash2set,hash2Size,3,vL(x2,i),vL(x3,j2),vL(x4,j2)); 
//             //P(" step 21\n");
//             L n2 = fetchIndexList(h2, indx2);
//             //P(" step 22\n");
//             //if(n2 > 1) { P("n2 = %lld\n", n2); isRepeat2 = true; }
//             for(L k=0;k<n2;k++){
//             //P(" step 23\n");
//                 L k2=indx2[k];
//                 // P("a[%lld] = %lld, b[%lld] = %lld\n", i,vL(x0,i),j2,vL(x1,j2));
//                 // P("c[%lld] = %lld,%lld,%lld\n", i,vL(x2,i),vL(x3,j2),vL(x4,j2));
//                 // P("d[%lld] = %lld,%lld,%lld\n", i,vL(x5,k2),vL(x6,k2),vL(x6,k2)); getchar();
//                 //if(total < 20) P("i = %lld\n", i);
//                 total++;
//             }
//         }
//     }
//     toc;
//     //if(isRepeat1) P("duplicated data found in loop1\n"); else P("no duplicated in loop1\n");  // no duplicate
//     //if(isRepeat2) P("duplicated data found in loop2\n"); else P("no duplicated in loop2\n");  // no duplicate
//     P("total1 = %lld\n", total); printDebugHashInfo(); getchar();
//     //testFunc1(); getchar();
//     R 0;
// }

#if defined(DO_HASH_V1) || defined(DO_HASH_V2)

static L q5_chf_2(V *z, V *x, V *r){
#ifdef DO_HASH_V1
    printBanner("q5_chf_2 with " DO_HASH_V1);
#else
    printBanner("q5_chf_2 with " DO_HASH_V2);
#endif
    V x0 = x[0]; // l_orderkey
    V x1 = x[1]; // o_orderkey
    V x2 = x[2]; // l_suppkey
    V x3 = x[3]; // n_nationkey
    V x4 = x[4]; // c_nationkey
    V x5 = x[5]; // s_suppkey
    V x6 = x[6]; // s_nationkey
    // added
    V r0 = r[0]; // n_name
    V r1 = r[1]; // l_extendedprice
    V r2 = r[2]; // l_discount
    V z0 = z[0]; // n_name
    V z1 = z[1]; // l_extendedprice
    V z2 = z[2]; // l_discount
    L total = 0;
    L hash1len, hash2len;
    //getInfoVar(x1); getInfoVar(x5); getInfoVar(x6); getchar();
    V *hash1set = (V []){x1};
    V *hash2set = (V []){x5,x6,x6};
    L indx1[99], indx2[99];
    time_tic();
    TBucket th1 = create_hash_multiply(hash1set[0]);
    TBucket th2 = create_hash_multiply(hash2set[0]);
    time_toc();
//    profileHashBucket(th1); getchar();
//    profileHashBucket(th2); getchar();
//    P("profiling x1\n"); profileDupDataV(x1); getchar();
    //profileHashTable(th2); getchar();
   HB *hash1 = th1.hashBucket;
   HB *hash2 = th2.hashBucket;
   L  *hash1Size = th1.hashSize;
   L  *hash2Size = th2.hashSize;
   //temp_func0(x0, x1); getchar();
   //DOI(200, P("%lld ",vL(x1,i))) P("\n"); getchar();
   //B isRepeat1 = false;
   //B isRepeat2 = false;
   /*
   P("profiling x1\n"); profileDupDataV(x1); getchar();
   P("profiling x5\n"); profileDupDataV(x5); getchar();
   P("profiling x6\n"); profileDupDataV(x6); getchar();
   (" -------\n");
   P("profiling x0\n"); profileDupDataV(x0); getchar();
   P("profiling x2\n"); profileDupDataV(x2); getchar();
   P("profiling x3\n"); profileDupDataV(x3); getchar();
   P("profiling x4\n"); profileDupDataV(x4); getchar();
   */

   /*
   L tot_fetch = 0;
   tic;
   for(L i=0,i2=vn(x0); i<i2;i++){
       HN h1=findValueFromHash2(hash1,hash1set,hash1Size,1,vL(x0,i)); 
       L n1 = fetchIndexList(h1, indx1);
       tot_fetch += n1;
       //if(n1 > 1) { P("n1 = %lld\n", n1); isRepeat1 = true; }
       for(L j=0;j<n1;j++){
           L j2=indx1[j];
           //P(" indx1[%lld] = %lld\n",j,indx1[j]);
           HN h2=findValueFromHash2(hash2,hash2set,hash2Size,3,vL(x2,i),vL(x3,j2),vL(x4,j2)); 
           //P(" step 21\n");
           L n2 = fetchIndexList(h2, indx2);
           tot_fetch += n2;
           //P(" step 22\n");
           //if(n2 > 1) { P("n2 = %lld\n", n2); isRepeat2 = true; }
           for(L k=0;k<n2;k++){
           //P(" step 23\n");
               L k2=indx2[k];
               // P("a[%lld] = %lld, b[%lld] = %lld\n", i,vL(x0,i),j2,vL(x1,j2));
               // P("c[%lld] = %lld,%lld,%lld\n", i,vL(x2,i),vL(x3,j2),vL(x4,j2));
               // P("d[%lld] = %lld,%lld,%lld\n", i,vL(x5,k2),vL(x6,k2),vL(x6,k2)); getchar();
               //if(total < 20) P("i = %lld\n", i);
               total++;
           }
       }
   }
   toc;
   */

   // v1
   // time_tic();
   // for(L i=0,i2=vn(x0); i<i2;i++){
   //     //P("i = %lld\n",i);
   //     HN h1=findValueFromHash2(hash1,hash1set,hash1Size,1,vLL(x0,i)); 
   //     L n1 = h1?h1->h_num:0;
   //     if(n1>0){
   //         //P("n1 = %lld\n", n1); getchar();
   //         tot_fetch1 += n1;
   //         L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
   //         for(L j=0;j<n1;j++){
   //             L  j2 = d1[j];
   //             HN h2 = findValueFromHash2(hash2,hash2set,hash2Size,3,vLL(x2,i),vLL(x3,j2),vLL(x4,j2)); 
   //             L  n2 = h2?h2->h_num:0;
   //             if(n2>0){
   //                 tot_fetch2 += n2;
   //                 L *d2 = n2==1?(L*)&(h2->h_other):n2>1?(L*)h2->h_other:NULL;
   //                 for(L k=0;k<n2;k++){
   //                     L k2=d2[k];
   //                     total++;
   //                 }
   //             }
   //         }
   //     }
   // }
   // time_toc();
   
   L tot_fetch1 = 0, tot_fetch2 = 0;
   ///P("outer loop iteration: %lld\n",vn(x0));
   const LL setN = setT_CONSTANT-1;
   //time_tic();
   //exp_join_q5_v2_c2(x0); // 18 ms
   //time_toc();
   //getchar();

   HA res1 = newHorseArray(), arr1 = res1;
   HA res2 = newHorseArray(), arr2 = res2;
   time_tic();
   for(L i=0,i2=vn(x0); i<i2;i++){
       //P("i = %lld\n",i);
       //HN h1=findValueFromHash2(hash1,hash1set,hash1Size,1,vLL(x0,i)); 
       LL id1 = vLL(x0,i)&setN;
       HN h1  = find_hash_value_single(hash1[id1],hash1Size[id1],vLL(x0,i));
       L  n1  = h1?h1->h_num:0;
       if(n1>0){
           //P("n1 = %lld\n", n1); getchar();
           tot_fetch1 += n1;
           if(true){  // <---- true / false
           L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
           for(L j=0;j<n1;j++){
               L   j2 = d1[j];
               HN h2 = find_hash_value_compound(hash2,hash2set,hash2Size,3,vLL(x2,i),vLL(x3,j2),vLL(x4,j2)); 
               L  n2 = h2?h2->h_num:0;
               //P("n2 = %lld\n", n2); getchar();
               if(n2>0){
                   tot_fetch2 += n2;
                   L *d2 = n2==1?(L*)&(h2->h_other):n2>1?(L*)h2->h_other:NULL;
                   for(L k=0;k<n2;k++){
                       L k2=d2[k];
                       arr1 = appendToHorseArray(arr1, j2);
                       arr2 = appendToHorseArray(arr2, i);
                       total++;
                   }
               }
           }
           }
       }
   }
   time_toc();
   //if(isRepeat1) P("duplicated data found in loop1\n"); else P("no duplicated in loop1\n");  // no duplicate
   //if(isRepeat2) P("duplicated data found in loop2\n"); else P("no duplicated in loop2\n");  // no duplicate
   //  total: fetch1 = 184082, fetch2 = 7243
   ///P("fetch1 = %lld, fetch2 = %lld\n", tot_fetch1,tot_fetch2);
   ///P("total = %lld, array size = %lld\n", total,countHorseArray(res1));
   //printDebugHashInfo(); //getchar();
   time_tic();
   initV(z0, vp(r0), total);
   initV(z1, vp(r1), total);
   initV(z2, vp(r2), total);
   indexWithHorseArray(z0,r0,res1);
   indexWithHorseArray(z1,r1,res2);
   indexWithHorseArray(z2,r2,res2);
   time_toc();
   time_tic();
   freeHorseArray(res1);
   freeHorseArray(res2);
   time_toc();
   //getchar();
   //testFunc1(); getchar();
    R 0;
}
#endif

#if defined(DO_HASH_V3)
static L q5_chf_2(V *z, V *x, V *r){
    printBanner("q5_chf_2 with " DO_HASH_V3);
    V x0 = x[0]; // l_orderkey
    V x1 = x[1]; // o_orderkey
    V x2 = x[2]; // l_suppkey
    V x3 = x[3]; // n_nationkey
    V x4 = x[4]; // c_nationkey
    V x5 = x[5]; // s_suppkey
    V x6 = x[6]; // s_nationkey
    // added
    V r0 = r[0]; // n_name
    V r1 = r[1]; // l_extendedprice
    V r2 = r[2]; // l_discount
    V z0 = z[0]; // n_name
    V z1 = z[1]; // l_extendedprice
    V z2 = z[2]; // l_discount
    L total = 0;
    L hash1len, hash2len;
    //getInfoVar(x1); getInfoVar(x5); getInfoVar(x6); getchar();
    V *hash1set = (V []){x1};
    V *hash2set = (V []){x5,x6,x6};
    L indx1[99], indx2[99];
    time_tic();
    TB th1 = create_hash_multiply(hash1set[0]);
    TB th2 = create_hash_multiply(hash2set[0]);
    HC **hash1 = th1.hashCell;
    HC **hash2 = th2.hashCell;
    L *hash1Mask = th1.hashMask;
    L *hash2Mask = th2.hashMask;
    //profile_hash_join_v3(hash1, hash1Size);
    //profile_hash_join_v3(hash2, hash2Size);
    time_toc();

    L tot_fetch1 = 0, tot_fetch2 = 0;
    if(H_DEBUG) P("outer loop iteration: %lld\n",vn(x0));
    const LL setN = setT_CONSTANT-1;

    HA res1 = newHorseArray(), arr1 = res1;
    HA res2 = newHorseArray(), arr2 = res2;
    LL temp3[3];
time_tic();
    for(L i=0,i2=vn(x0); i<i2;i++){
        //P("i = %lld\n",i);
        //HN h1=findValueFromHash2(hash1,hash1set,hash1Size,1,vLL(x0,i)); 
        LL id1 = vLL(x0,i)&setN;
#ifndef FUNC_INLINE
        HN h1  = find_hash_value_single(hash1[id1],hash1Mask[id1],vLL(x0,i));
#else
        //LL hashId = hash_LL(vLL(x0,i)) & hash1Mask[id1];
#endif
        L  n1  = h1?h1->h_num:0;
        if(n1>0){
            //P("n1 = %lld\n", n1); getchar();
            tot_fetch1 += n1;
            //if(true){  // <---- true / false
                L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
                for(L j=0;j<n1;j++){
                    L j2 = d1[j];
                    //HN h2 = find_hash_value_compound(hash2,hash2set,hash2Size,3,vLL(x2,i),vLL(x3,j2),vLL(x4,j2)); 
                    temp3[0] = vLL(x2,i); temp3[1] = vLL(x3,j2); temp3[2] = vLL(x4,j2);
                    LL id2 = temp3[0]&setN;
                    HN h2 = find_hash_value_compound(hash2[id2],hash2set,hash2Mask[id2],3,temp3);
                    L  n2 = h2?h2->h_num:0;
                    //P("n2 = %lld\n", n2); getchar();
                    if(n2>0){
                        ///tot_fetch2 += n2;
                        L *d2 = n2==1?(L*)&(h2->h_other):n2>1?(L*)h2->h_other:NULL;
                        for(L k=0;k<n2;k++){
                            L k2=d2[k];
                            arr1 = appendToHorseArray(arr1, j2);
                            arr2 = appendToHorseArray(arr2, i);
                            total++;
                        }
                    }
                }
            //}
        }
    }
time_toc();
    if(H_DEBUG){
        P("total: fetch1 = %lld, fetch2 = %lld\n", tot_fetch1,tot_fetch2);
        ///P("total = %lld, array size = %lld\n", total,countHorseArray(res1));
        ///P("total = %lld\n", total);
        ///getchar();
    }
time_tic();
    initV(z0, vp(r0), total);
    initV(z1, vp(r1), total);
    initV(z2, vp(r2), total);
    indexWithHorseArray(z0,r0,res1);
    indexWithHorseArray(z1,r1,res2);
    indexWithHorseArray(z2,r2,res2);
time_toc();
time_tic();
    freeHorseArray(res1);
    freeHorseArray(res2);
time_toc();
    R 0;
}
#endif

static I q5_fuse_compress(V *z, V f, V *x){
    V z0 = z[0];
    V z1 = z[1];
    V x0 = x[0];
    V x1 = x[1];
    L c=0;
    DOI(vn(f), if(AND(GEQ(vD(f,i),19940101),LT(vD(f,i),19950101)))c++)
    initV(z0, H_I, c);
    initV(z1, H_I, c);
    c=0;
    DOI(vn(f), if(AND(GEQ(vD(f,i),19940101),LT(vD(f,i),19950101))){vI(z0,c)=vI(x0,i); vI(z1,c)=vI(x1,i); c++;})
    P("writes %lld\n",c);
    R 0;
}

E compiled_main(){
    //P("waiting...\n"); getchar();
    E elapsed=0;
    V t0   = allocNode(); V t1   = allocNode(); V t4   = allocNode(); V t8   = allocNode(); 
    V t10  = allocNode(); V t12  = allocNode(); V t16  = allocNode(); V t17  = allocNode(); 
    V t18  = allocNode(); V t19  = allocNode(); V t21  = allocNode(); V t22  = allocNode(); 
    V t24  = allocNode(); V t28  = allocNode(); V t29  = allocNode(); V t33  = allocNode(); 
    V t34  = allocNode(); V t37  = allocNode(); V t42  = allocNode(); V t43  = allocNode(); 
    V t44  = allocNode(); V t45  = allocNode(); V t49  = allocNode(); V t50  = allocNode(); 
    V t53  = allocNode(); V t56  = allocNode(); V t61  = allocNode(); V t62  = allocNode(); 
    V t63  = allocNode(); V t66  = allocNode(); V t71  = allocNode(); V t72  = allocNode(); 
    V t73  = allocNode(); V t74  = allocNode(); V k0   = allocNode(); V t75  = allocNode(); 
    V t84  = allocNode(); V t85  = allocNode(); V t86  = allocNode(); V t90  = allocNode(); 
    V t91  = allocNode(); V t97  = allocNode(); V t102 = allocNode(); V t111 = allocNode(); 
    V t112 = allocNode(); V t114 = allocNode(); V t117 = allocNode(); V t118 = allocNode(); 
    V t128 = allocNode(); V t129 = allocNode(); V t130 = allocNode(); V t131 = allocNode(); 
    V t135 = allocNode(); V t136 = allocNode(); V t142 = allocNode(); V t158 = allocNode(); 
    V t161 = allocNode(); V t162 = allocNode(); V t172 = allocNode(); V t173 = allocNode(); 
    V t174 = allocNode(); V t175 = allocNode(); V t177 = allocNode(); V t189 = allocNode(); 
    V t214 = allocNode(); V t215 = allocNode(); V t225 = allocNode(); V t226 = allocNode(); 
    V t227 = allocNode(); V t228 = allocNode(); V t229 = allocNode(); V t230 = allocNode(); 
    V t231 = allocNode(); V t232 = allocNode(); V t233 = allocNode(); V t234 = allocNode(); 
    V t235 = allocNode(); V t236 = allocNode(); V t237 = allocNode(); V t238 = allocNode(); 
    V t239 = allocNode(); V t240 = allocNode(); V t241 = allocNode(); 
    V tt0  = allocNode();
    tic;
    time_tic();
    PROFILE(  0, t0 , pfnLoadTable(t0, initLiteralSym((S)"supplier")));
    PROFILE(  1, t1 , pfnColumnValue(t1, t0, initLiteralSym((S)"s_suppkey")));
    PROFILE(  2, t4 , pfnColumnValue(t4, t0, initLiteralSym((S)"s_nationkey")));
    PROFILE(  3, t8 , pfnLoadTable(t8, initLiteralSym((S)"region")));
    PROFILE(  4, t10, pfnColumnValue(t10, t8, initLiteralSym((S)"r_name")));
    PROFILE(  5, t12, pfnEq(t12, t10, initLiteralSym((S)"ASIA")));
    PROFILE(  6, t16, pfnLoadTable(t16, initLiteralSym((S)"nation")));
    PROFILE(  7, t17, pfnColumnValue(t17, t16, initLiteralSym((S)"n_nationkey")));
    PROFILE(  8, t18, pfnColumnValue(t18, t16, initLiteralSym((S)"n_name")));
    PROFILE(  9, t19, pfnColumnValue(t19, t16, initLiteralSym((S)"n_regionkey")));
    PROFILE( 10, t21, pfnValues(t21, t19));
    PROFILE( 11, t22, pfnIndex(t22, t12, t21));
    //getInfoVar(t12); getInfoVar(t21); getInfoVar(t22); getchar();
    PROFILE( 12, t24, pfnWhere(t24, t22));
    PROFILE( 13, t28, pfnIndex(t28, t17, t24));
    PROFILE( 14, t29, pfnIndex(t29, t18, t24));
    PROFILE( 15, t33, pfnLoadTable(t33, initLiteralSym((S)"customer")));
    PROFILE( 16, t34, pfnColumnValue(t34, t33, initLiteralSym((S)"c_custkey")));
    PROFILE( 17, t37, pfnColumnValue(t37, t33, initLiteralSym((S)"c_nationkey")));
    PROFILE( 18, t42, pfnFetch(t42, t37));
    //getInfoVar(t28); getInfoVar(t42); getchar();
    //Variable t28 has type H_L and len 5
    //Variable t42 has type H_L and len 150000
    PROFILE( 19, t43, pfnJoinIndex(t43,t28,t42,initLiteralSym((S)"eq")));  // <-- 10 ms / 3
    //time_toc(); getchar();
    PROFILE( 20, t44, pfnIndex(t44, t43, initLiteralI64(0)));
    PROFILE( 21, t45, pfnIndex(t45, t43, initLiteralI64(1)));
    PROFILE( 22, t49, pfnIndex(t49, t28, t44));
    PROFILE( 23, t50, pfnIndex(t50, t29, t44));
    PROFILE( 24, t53, pfnIndex(t53, t34, t45));
    PROFILE( 25, t56, pfnIndex(t56, t42, t45));
    PROFILE( 26, t61, pfnLoadTable(t61, initLiteralSym((S)"orders")));
    PROFILE( 27, t62, pfnColumnValue(t62, t61, initLiteralSym((S)"o_orderkey")));
    PROFILE( 28, t63, pfnColumnValue(t63, t61, initLiteralSym((S)"o_custkey")));
    PROFILE( 29, t66, pfnColumnValue(t66, t61, initLiteralSym((S)"o_orderdate")));
    //getchar();
    // t73: compress ratio:15.17 % (227597/1500000) compress > fuse
    if(false){
    PROFILE( 30, t73, q5_loopfusion_0(t73,(V []){t66}));                   // <--  5 ms / 5
    PROFILE( 31, t74, pfnCompress(t74, t73, t62));                         // <-- 15 ms / 12
    PROFILE( 32, k0 , pfnFetch(k0, t63));                                  // <-- 19 ms / 16
    PROFILE( 33, t75, pfnCompress(t75, t73, k0));                          // <-- 13 ms / 12
    }
    else {
    PROFILE( 32, k0 , pfnFetch(k0, t63));                                  // <-- 19 ms / 16 / 0 (fixed)
    //     fuse line 30: ~ 12 ms
    // NOT fuse line 30: ~  9 ms
    PROFILE( 99, t75, q5_fuse_compress((V []){t74,t75}, t66, (V []){t62,k0}));
    //getchar();
    }
    //getInfoVar(t53); getInfoVar(t75); getchar();
    //Variable t53 has type H_L and len 30183
    //Variable t75 has type H_L and len 227597
    PROFILE( 34, t84, pfnJoinIndex(t84,t53,t75,initLiteralSym((S)"eq")));  // <-- 35 ms / 24
    //if(false){
    //    PROFILE(99, t84, exp_join_q5_v2_c1(t84, (V []){t53,t73,k0})); // not profitable
    //}
    //getchar();
    PROFILE( 35, t85, pfnIndex(t85, t84, initLiteralI64(0)));
    PROFILE( 36, t86, pfnIndex(t86, t84, initLiteralI64(1)));
    PROFILE( 37, t90, pfnIndex(t90, t49, t85));
    PROFILE( 38, t91, pfnIndex(t91, t50, t85));  // n_name
    PROFILE( 39, t97, pfnIndex(t97, t56, t85));
    PROFILE( 40, t102, pfnIndex(t102, t74, t86));
    //getInfoVar(t74); getInfoVar(t86);
    //Variable t74 has type H_L and len 227597
    //Variable t86 has type H_L and len 46008
    //time_toc(); getchar(); /* almost 100 ms */
    PROFILE( 41, t111, pfnLoadTable(t111, initLiteralSym((S)"lineitem")));
    PROFILE( 42, t112, pfnColumnValue(t112, t111, initLiteralSym((S)"l_orderkey")));
    PROFILE( 43, t114, pfnColumnValue(t114, t111, initLiteralSym((S)"l_suppkey")));
    PROFILE( 44, t117, pfnColumnValue(t117, t111, initLiteralSym((S)"l_extendedprice")));
    PROFILE( 45, t118, pfnColumnValue(t118, t111, initLiteralSym((S)"l_discount")));
    PROFILE( 46, t128, pfnFetch(t128, t112));
    if(false) {
    //getInfoVar(t102); getInfoVar(t128); getchar();
    // Variable t102 has type H_L and len 46008
    // Variable t128 has type H_L and len 6001215
    //-- start (old)
    PROFILE( 47, t129, pfnJoinIndex(t129,t102,t128,initLiteralSym((S)"eq"))); // <-- expensive
    getchar();
    PROFILE( 48, t130, pfnIndex(t130, t129, initLiteralI64(0)));
    PROFILE( 49, t131, pfnIndex(t131, t129, initLiteralI64(1)));
    PROFILE( 50, t135, pfnIndex(t135, t90, t130));   // right, 3, c_nationkey
    PROFILE( 51, t136, pfnIndex(t136, t91, t130));
    PROFILE( 52, t142, pfnIndex(t142, t97, t130));   // right, 2, n_nationkey
    PROFILE( 53, t158, pfnIndex(t158, t114, t131));  // right, 1, l_suppkey
    PROFILE( 54, t161, pfnIndex(t161, t117, t131));
    PROFILE( 55, t162, pfnIndex(t162, t118, t131));
    PROFILE( 56, t172, pfnFetch(t172, t4));
    // s_suppkey (t1), s_nationkey (t172), s_nationkey (t172)
    PROFILE( 57, t173, pfnList(t173, 3, (V []){t1 ,t172 ,t172}));
    PROFILE( 58, t174, pfnList(t174, 3, (V []){t158 ,t142 ,t135}));
    //getInfoVar(t173); getInfoVar(t174); getchar();
    //Variable t173 has type H_G and len 3 (10000)
    //Variable t174 has type H_G and len 3 (184082)
    PROFILE( 59, t175, pfnJoinIndex(t175,t173,t174,initLiteralSym((S)"eq")));
    getchar();
    PROFILE( 60, t177, pfnIndex(t177, t175, initLiteralI64(1)));
    //getInfoVar(t177); getchar();
    //Variable t177 has type H_L and len 7243
    PROFILE( 61, t189, pfnIndex(t189, t136, t177));  // n_name
    PROFILE( 62, t214, pfnIndex(t214, t161, t177));  // l_extendedprice
    PROFILE( 63, t215, pfnIndex(t215, t162, t177));  // l_discount
    }
    //-- end
    else {
    //-- start (new)
    //PROFILE(999, t175, q5_chf_0(tt0,(V []){t128,t102,t114,t97,t90,t1,t172}));
    //PROFILE(999, t175, q5_chf_1(tt0,(V []){t128,t102,t114,t97,t90,t1,t172}));
    PROFILE( 56, t172, pfnFetch(t172, t4));
    PROFILE(999, t175, q5_chf_2(\
                 (V []){t189,t214,t215},\
                 (V []){t128,t102,t114,t97,t90,t1,t172},\
                 (V []){t91,t117,t118}));
    //getchar();
    //-- end
    }
    PROFILE( 64, t226, q5_loopfusion_1(t226,(V []){t214,t215}));
    PROFILE( 65, t227, pfnList(t227, 1, (V []){t189}));
    PROFILE( 66, t228, pfnGroup(t228, t227));
    PROFILE( 67, t229, pfnKeys(t229, t228));
    PROFILE( 68, t230, pfnValues(t230, t228));
    PROFILE( 69, t231, pfnIndex(t231, t189, t229));
    PROFILE( 70, t234, q5_peephole_0((V []){t234},(V []){t226},t230));
    PROFILE( 71, t235, pfnList(t235, 1, (V []){t234}));
    PROFILE( 72, t236, pfnOrderBy(t236, t235, initLiteralBool(0)));
    PROFILE( 73, t237, pfnIndex(t237, t231, t236));
    PROFILE( 74, t238, pfnIndex(t238, t234, t236));
    PROFILE( 75, t239, copyV(t239, initLiteralSymVector(2, (S []){"n_name","revenue"})));
    PROFILE( 76, t240, pfnList(t240, 2, (V []){t237 ,t238}));
    PROFILE( 77, t241, pfnTable(t241, t239, t240));
    toc;
    printV2(t241,20);
    R elapsed;
}
