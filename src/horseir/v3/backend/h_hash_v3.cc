#include "../global.h"

#ifdef DO_HASH_V3

#include "h_hash_common.h" // common hash functions

// same as insert_index_v1
static L insert_index_v3(HN t, L td){
    const L SLOT_SIZE = 1024;
    //P("insert_index_v3 stop here: h_num=%lld\n",t->h_num); getchar();
    if(t->h_num == 0){
        t->h_num = 1;
        t->h_other = td;
    }
    else if(t->h_num == 1){
        L *other = HASH_AL(L, SLOT_SIZE);
        // memset(other, 0, SLOT_SIZE*sizeof(L));
        other[0] = t->h_other;
        other[1] = td;
        t->h_other = (L)other;
        t->h_num = 2;
    }
    else if(t->h_num > 1 && t->h_num < SLOT_SIZE){
        L *other = (L*)(t->h_other);
        other[t->h_num] = td;
        t->h_num++;
    }
    else EP("Not enough slots: %lld", t->h_num); // 64 not enough in q7 -> 1024
    // q14 output too many below
    //if(H_DEBUG) WP("number of slots: %lld\n", t->h_num); //getchar();
    R td;
}

static L insert_hash_int_v3_I(HC *ht, L htMask, LL *src, L srcI, LL id){
#define STORE_INFO_I(x) do{ \
    x->h_index = srcI; \
    x->h_i     = val;  \
    x->h_num   = 1;    \
    x->h_other = srcI; \
  }while(0)
#define createHashCell(n) HASH_AL(HC0, n)
    LL val    = src[srcI];
    L  hashId = hash_LL(val) & htMask;
    HC t      = ht[hashId];
    if(t){
        HC p=t;
        while(t){
            DOI(t->c_total,{HN x=t->c_node+i; if(x->h_i==val) R insert_index_v3(x,srcI);})
            t = t->c_next;
        }
        // if not found in all cells, then
        t=p; while(t->c_next) t=t->c_next;
        if(t->c_total < PRE_SIZE_CELL){
            HN x = t->c_node+t->c_total;
            STORE_INFO_I(x);
            t->c_total++;
        }
        else {
            if(H_DEBUG)
                WP("...... need a new cell: %lld\n",t->c_total);
            HC t0 = createHashCell(1);
            HN x = t0->c_node;
            STORE_INFO_I(x);
            t0->c_total = 1;
            t0->c_next  = NULL;
            t->c_next   = t0;   // link to a new cell
        }
    }
    else {
        HC t0 = createHashCell(1);
        HN x = t0->c_node;
        STORE_INFO_I(x);
        t0->c_total = 1;
        t0->c_next  = NULL;
        ht[hashId]  = t0; // set to the beginning
    }
    R srcI;
}

static L insert_hash_int_v3_E(HC *ht, L htMask, E *src, L srcI, LL id){
#define STORE_INFO_E(x) do{ \
    x->h_index = srcI; \
    x->h_e     = val;  \
    x->h_num   = 1;    \
    x->h_other = srcI; \
  }while(0)
#define createHashCell(n) HASH_AL(HC0, n)
    E val    = src[srcI];
    L hashId = hash_LL(val) & htMask;
    HC t      = ht[hashId];
    if(t){
        HC p=t;
        while(t){
            DOI(t->c_total,{HN x=t->c_node+i; if(x->h_i==val) R insert_index_v3(x,srcI);})
            t = t->c_next;
        }
        // if not found in all cells, then
        t=p; while(t->c_next) t=t->c_next;
        if(t->c_total < PRE_SIZE_CELL){
            HN x = t->c_node+t->c_total;
            STORE_INFO_E(x);
            t->c_total++;
        }
        else {
            if(H_DEBUG)
                WP("...... need a new cell: %lld\n",t->c_total);
            HC t0 = createHashCell(1);
            HN x = t0->c_node;
            STORE_INFO_E(x);
            t0->c_total = 1;
            t0->c_next  = NULL;
            t->c_next   = t0;   // link to a new cell
        }
    }
    else {
        HC t0 = createHashCell(1);
        HN x = t0->c_node;
        STORE_INFO_E(x);
        t0->c_total = 1;
        t0->c_next  = NULL;
        ht[hashId]  = t0; // set to the beginning
    }
    R srcI;
}

TB create_hash_multiply_v3(V x){
    L  *count    = HASH_AL(L , setT);
    L  *hashMask = HASH_AL(L , setT);
    HC**hashCell = HASH_AL(HC*,setT);
    // L  *prefix   = HASH_AL(L , setT);
    // memset(count, 0, sizeof(L)*setT);
    // prefix[0] = 0;
    // if(H_DEBUG) P("v3: //Step 1: scan for basic info\n");
// tic();
    DOI(xn, count[vLL(x,i)&setN]++)
    // DOI(xn, if(count[i]>0) P("count[%lld] = %lld\n", i,count[i])) // show count distribution
    // DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
// toc();
    // if(H_DEBUG) P("v3: //Step 2: create partitions (setT=%lld)\n",(L)setT);
// tic();
    DOI(setT, { \
        L hashLen=count[i]<2?count[i]:getHashTableSize(count[i]); \
        hashCell[i]=(hashLen>0)?HASH_AL(HC,hashLen):NULL; \
        hashMask[i]=hashLen-1;})
// toc();
    //test_hashSize(hashSize);
    //if(H_DEBUG) P("v3: //Step 3: insert items (%lld)\n",xn);
    //P("xn = %lld, setT = %lld\n",xn,setT); getchar();
// tic();
    switch(xp){
        caseI
            DOI(xn, {LL id=vLL(x,i)&setN; \
                    insert_hash_int_v3_I(hashCell[id],hashMask[id],sLL(x),i,id);}) break;
        caseE
            DOI(xn, {LL id=vLL(x,i)&setN; \
                    insert_hash_int_v3_E(hashCell[id],hashMask[id],sE(x),i,id);}) break;
        default: EP("Type not supported: %s", getTypeName(xp));
    }
// toc();
    TB tb;
    tb.hashCell = hashCell;
    tb.hashMask = hashMask;
    R tb;
}

HN find_hash_value_multiple_v3(HC *ht, V *src, L hashMask, L n, LL *args){
    if(ht){
        LL val    = args[0];
        L hashKey = hash_LL(val);
        L hashId  = hashKey & hashMask;
        HC t = ht[hashId];
        while(t){
            DOI(t->c_total, {HN x=t->c_node+i; \
                    if(x->h_i==val && isAllMatch(n,src,hD(x),args))R x;})
                t=t->c_next;
        }
    }
    R 0;
}

HN find_hash_value_multiple_v3_basic(HC *ht, V src, L hashMask, L n, V y0, L k){
    if(ht){
        LL val    = vLL(y0,k);
        L hashKey = hash_LL(val);
        L hashId  = hashKey & hashMask;
        HC t = ht[hashId];
        //if(val == 199478) {
        if(k == 13) {
            P("found k == 13\n"); 
            P("vI(y0,%lld) = %d\n", k,vLL(y0,k)); getchar();
            while(t){
                P("total = %lld\n", t->c_total);
                DOI(t->c_total, {HN x=t->c_node+i; \
                        P("x->h_i = %d, val = %d, h_num = %lld\n", x->h_i,val,x->h_num); \
                        P("match? %d\n", isAllMatchBasic(n,src,hD(x),y0,k)); \
                        if(x->h_i==val && isAllMatchBasic(n,src,hD(x),y0,k))R x;})
                    t=t->c_next;
            }
            P("done\n"); getchar();
        }
        while(t){
            DOI(t->c_total, {HN x=t->c_node+i; \
                    if(x->h_i==val && isAllMatchBasic(n,src,hD(x),y0,k))R x;})
                t=t->c_next;
        }
    }
    R 0;
}

static HN find_hash_value_v3(HC *hashT, V *src, L hashMask, L n, LL *args){
    if(1==n) R find_hash_value_single_v3(hashT, hashMask, args[0]);
    else R find_hash_value_multiple_v3(hashT, src, hashMask, n, args);
}

HN find_hash_value_single_v3(HC *ht, L hashMask, LL val){
    if(ht){
        L hashId = hash_LL(val) & hashMask;
        HC t = ht[hashId];
        while(t){
            DOI(t->c_total, {HN x=t->c_node+i; if(x->h_i==val)R x;})
            t=t->c_next;
        }
    }
    R 0;
}

HN findValueFromHashV3(HC **hashT, V *src, L *hashMask, L n, ...){
    readArgs();
    LL hashSetId  = args[0] & setN;
    R find_hash_value_v3(hashT[hashSetId],src,hashMask[hashSetId],n,args);
}


static I lib_join_radix_hash_v3(V z0, V z1, V x, V y){
    printBanner("Radix-based hash join v3");
    P("left(x) = %lld, right(y) = %lld\n", vn(x),vn(y));
    // getInfoVar(x); getInfoVar(y); //getchar();
    if(vp(x)==H_LL && vp(y)==H_LL){
        HN *tempHN = HASH_AL(HN, vn(y));
    tic();
        TB tb = create_hash_multiply_v3(x);
        HC**hashTable = tb.hashCell;
        L  *hashMask  = tb.hashMask;
    time_toc("> Radix hash, build phase (ms): %g\n", elapsed);
        L c = 0;
    tic();
        if(true){
            DOI(vn(y), {I id=vI(y,i)&setN;
                HN h1=find_hash_value_single_v3(hashTable[id],hashMask[id],vI(y,i));
                tempHN[i]=h1;})
        }
        else {
            DOP(vn(y), {I id=vI(y,i)&setN; \
                HN h1=find_hash_value_single_v3(hashTable[id],hashMask[id],vI(y,i));\
                tempHN[i]=h1; if(h1) c+=h1->h_num;}, reduction(+:c))
        }
        // P("parallel: c = %lld\n", c);
        // DOI(300, if(tempHN[i])P("tempHN[%lld] = %lld\n",i,(L)tempHN[i]));
        // c = 0;
        //// upon (parallel), below (serial)
        // DOI(vn(y), {I id=vI(y,i)&setN; \
        //     HN h1=find_hash_value_single_v3(hashTable[id],hashMask[id],vI(y,i));\
        //     if(h1) c+=h1->h_num; tempHN[i]=h1;})
        // P("serial: c = %lld\n", c);
        // DOI(300, if(tempHN[i])P("tempHN[%lld] = %lld\n",i,(L)tempHN[i]));
        // getchar();
    time_toc("> Radix hash, probe phase (ms): %g\n", elapsed);
    tic();
        L parZ[H_CORE], offset[H_CORE];
        memset(parZ  , 0, sizeof(L)*H_CORE);
        memset(offset, 0, sizeof(L)*H_CORE);
        DOT(vn(y), if(tempHN[i]) parZ[tid]+=tempHN[i]->h_num)
        DOI(H_CORE, c+=parZ[i])
        DOIa(H_CORE, offset[i]=parZ[i-1]+offset[i-1])
        // P("// Total writes = %lld\n", c);
        // DOI(H_CORE, P("offset[%lld] = %lld\n", i, offset[i]))
        // getchar();
        initV(z0, H_L, c);
        initV(z1, H_L, c);
    time_toc("> Radix hash, prepare write (ms): %g\n", elapsed);
    tic();
//         c = 0;
//// very slow code below
//         { L i2=vn(y);
// #pragma omp parallel for
//         for(L i=0;i<i2;i++){
//             HN h1=tempHN[i];
//             if(h1){ L n1=h1->h_num;
//                 L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
// #pragma omp critical
//                 {
//                     DOJ(n1, {vL(z0,c)=d1[j]; vL(z1,c)=i; c++;})
//                 }
//             }
//         }}
        if(true){ // parallel
            DOT(vn(y), {HN h1=tempHN[i];
                if(h1){ L n1=h1->h_num;
                  L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
                  DOJ(n1, {L k=offset[tid]++; vL(z0,k)=d1[j]; vL(z1,k)=i;})}}, reduction(+:c))
        }
        else { // serial
            c = 0;
            DOJ(vn(y), {HN h1=tempHN[j]; \
                if(h1){ L n1=h1->h_num; \
                  L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL; \
                  /*P("[%lld][%lld] with size %lld\n",d1[0],j,n1); getchar();*/ \
                  DOI(n1, {vL(z0,c)=d1[i]; vL(z1,c)=j; c++;})}})
        }
    time_toc("> Radix hash, write phase (ms): %g\n", elapsed);
        P("// Total elements write = %lld\n", c); // expected for Q5 3rd join: 46008
    // printV2(z0, vn(z0));
    // P("-------------\n");
    // printV2(z1, vn(z1));
    // getchar();
        R 0;
    }
    else {EP("Type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));R 1;}
}

static I lib_join_radix_hash(V z0, V z1, V x, V y){
    L hashCur = getHashHeap();
    I status = lib_join_radix_hash_v3(z0,z1,x,y);
    setHashHeap(hashCur);
    R status;
}

static L* r2_createHashTable(V x, L size){
    L *ht = HASH_AL(L, size);
    // parallel memset
    DOTa(size, memset(ht+sid, -1, sizeof(L)*slen))
    // memset(ht, -1, sizeof(L)*size);
    DOI(xn, ht[vI(x,i)]=i)
    R ht;
}

static void r2_probeHashTable(V z0, V z1, V x, L *ht, I minX, I maxX){
    // 1. allocate with a large number
    initV(z0, H_L, xn);
    initV(z1, H_L, xn);
    // 2. find all matched indices
    L offset[H_CORE], part = xn/H_CORE;
    DOI(H_CORE, offset[i]=i*part)
    tic();
    DOT(xn, {I v=vI(x,i);if(v>=minX && v<=maxX){
            L k = ht[v];
            if(k>=0) { vL(z0,offset[tid]) = k; vL(z1,offset[tid]) = i;
                offset[tid]++;}}
            })
    toc();
    L c = offset[0];
    if(H_CORE > 1){
        L *t_z0 = sL(z0)+c, *t_z1 = sL(z1)+c;
        DOIa(H_CORE, {L pos=i*part;  L seg=offset[i]-pos;
                memcpy(t_z0, sL(z0)+pos, sizeof(L)*seg);
                memcpy(t_z1, sL(z1)+pos, sizeof(L)*seg);
                t_z0 += seg;
                t_z1 += seg;
                c    += seg;
                })
    }
    // L c = 0;
    // DOI(xn, {I v=vI(x,i);
    //     if(v>=minX && v<=maxX){
    //     L k = ht[v]; if(k>=0) { vL(z0,c)=k; vL(z1,c)=i; c++; }
    //     }})
    // 3. update length (shorter)
    vn(z0) = vn(z1) = c;
    P("// r2: result c = %lld\n", c);
}

static I lib_join_table_hash_r2(V z0, V z1, V x, V y, I minX, I maxX){
    printBanner("Table-based hash join r2");
    P("// r2: left(x) = %lld, right(y) = %lld\n", vn(x),vn(y));
    P("// r2: min = %d, max = %d\n", minX, maxX);
    if(vp(x)==H_I && vp(y)==H_I){
        if(minX >= 0){
            tic();
            L *ht = r2_createHashTable(x, maxX+1);
            time_toc("r2: build time (ms): %g\n", elapsed);
            tic();
            r2_probeHashTable(z0, z1, y, ht, minX, maxX);
            time_toc("r2: probe + write time (ms): %g\n", elapsed);
            R 0;
        }
        else {
            TODO("r2: minX = %d, maxX = %d\n", minX, maxX);
        }
    }
    else {EP("Type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));R 1;}
}

static I lib_join_table_hash(V z0, V z1, V x, V y, I minX, I maxX){
    L hashCur = getHashHeap();
    I status = lib_join_table_hash_r2(z0,z1,x,y,minX,maxX);
    setHashHeap(hashCur);
    R status;
}


typedef struct r6_dup_node{
    L maxCap;
    L *index;
}r6_D0, *r6_D;

typedef struct r6_table_node{
    L num;
    union{
        L index;
        struct r6_dup_node *ptr;
    };
}r6_T0,*r6_T;

static r6_D r6_newDupNode(){
    r6_D d = HASH_AL(r6_D0, 1);
    d->maxCap = 16;
    d->index = HASH_AL(L, d->maxCap);
    R d;
}

static r6_T r6_createHashTable(V x, L size){
    r6_T ht = HASH_AL(r6_T0, size);
    // parallel memset
    DOTa(size, memset(ht+sid, 0, sizeof(r6_T0)*slen))
    // memset(ht, -1, sizeof(L)*size);
    DOI(xn, { r6_T t = ht + vI(x,i);
            if(t->num == 0){
                t->num = 1;
                t->index = i;
            }
            else if(t->num == 1){
                r6_D d = r6_newDupNode();
                d->index[0] = t->index;
                d->index[1] = i;
                t->ptr = d;
                t->num++;
            }
            else {
                r6_D d = t->ptr;
                if(t->num < d->maxCap){
                    d->index[t->num++] = i;
                }
                else {
                    L newSize = d->maxCap*2;
                    L *newIndex = HASH_AL(L, newSize);
                    memcpy(newIndex, d->index, sizeof(L)*d->maxCap);
                    d->index  = newIndex;
                    d->maxCap = newSize;
                    d->index[t->num++] = i;
                }
            }
        })
    R ht;
}

static void r6_probeHashTable(V z0, V z1, V x, r6_T ht, I minX, I maxX){
    // 1. scan to determine size
    L count[H_CORE];
    DOI(H_CORE, count[i]=0)
    DOT(xn, {I v=vI(x,i);if(v>=minX && v<=maxX){
            r6_T k = ht + v;
            if(k->num > 0) {
                count[tid] += k->num;
            }}
        })
    L lenZ = 0;
    DOI(H_CORE, lenZ+=count[i])
    initV(z0, H_L, lenZ);
    initV(z1, H_L, lenZ);
    // 2. find all matched indices
    L offset[H_CORE];
    offset[0]=0;
    DOIa(H_CORE, offset[i]=offset[i-1]+count[i-1])
    DOT(xn, {I v=vI(x,i);if(v>=minX && v<=maxX){
            r6_T k = ht + v;
            if(k->num == 1) {
                vL(z0,offset[tid]) = k->index;
                vL(z1,offset[tid]) = i;
                offset[tid]++;
            }
            else if(k->num > 1){
                r6_D d = k->ptr;
                DOJ(k->num, {
                    vL(z0,offset[tid]) = d->index[j];
                    vL(z1,offset[tid]) = i;
                    offset[tid]++;})
                }
            }
        })
    P("// r6: result c = %lld\n", lenZ);
}

static I lib_join_table_hash_r6(V z0, V z1, V x, V y, I minX, I maxX){
    printBanner("Table-based hash join r6");
    P("// r6: left(x) = %lld, right(y) = %lld\n", vn(x),vn(y));
    P("// r6: min = %d, max = %d\n", minX, maxX);
    if(vp(x)==H_I && vp(y)==H_I){
        tic();
        r6_T ht = r6_createHashTable(x, maxX+1);
        time_toc("r6: build time (ms): %g\n", elapsed);
        tic();
        r6_probeHashTable(z0, z1, y, ht, minX, maxX);
        time_toc("r6: probe + write time (ms): %g\n", elapsed);
        R 0;
    }
    else {EP("Type not supported: %s,%s",getTypeName(vp(x)),getTypeName(vp(y)));R 1;}
}

static I lib_join_table_hash_dup(V z0, V z1, V x, V y, I minX, I maxX){
    L hashCur = getHashHeap();
    I status = lib_join_table_hash_r6(z0,z1,x,y,minX,maxX);
    setHashHeap(hashCur);
    R status;
}



static B isStrictOrderUp(V x){
    DOIa(xn, if(vI(x,i)<=vI(x,i-1))R 0) R 1;
    // DOIa(xn, if(vI(x,i)<=vI(x,i-1)){P("v1=%d,v0=%d\n",vI(x,i),vI(x,i-1)); getchar(); R 0;}) R 1;
}

static B isStrictOrderDown(V x){
    DOIa(xn, if(vI(x,i)>=vI(x,i-1))R 0) R 1;
}

static B isStrictOrderEqual(V x){
    DOIa(xn, if(vI(x,i)==vI(x,i-1))R 0) R 1;
}

static B isStrictOrder(V x, I *minX, I *maxX){
    if(isStrictOrderUp(x)){
        *minX = vI(x,0);
        *maxX = vI(x,xn-1);
        R 1;
    }
    else if(isStrictOrderDown(x)){
        *minX = vI(x,xn-1);
        *maxX = vI(x,0);
        R 1;
    }
    R 0;
}

static B isLessStrictOrderUp(V x){
    DOIa(xn, if(vI(x,i)<vI(x,i-1))R 0) R 1;
}

static B isLessStrictOrderDown(V x){
    DOIa(xn, if(vI(x,i)>vI(x,i-1))R 0) R 1;
}

static I r2_simple_cmp(const void * a, const void * b) {
    return ( *(I*)a - *(I*)b );
}

B isStrictUnique(V x, I *minX, I *maxX){
    if(isStrictOrder(x,minX,maxX)){
        R 1;
    }
    else if(!isStrictOrderEqual(x)){
        R 0;
    }
    else{
        WP("sorting while checking unique items");
        I *temp = HASH_AL(I, xn);
        memcpy(temp, sI(x), sizeof(I)*xn);
        qsort(temp, xn, sizeof(I), r2_simple_cmp);
        V0 t0; V t = &t0;
        vn(t) = xn;
        vp(t) = H_I;
        vg(t) = (S)temp;
        // printV2(t, 20);
        R isStrictOrder(t,minX,maxX);
    }
}

static B checkValueRange(V x, I *minX, I *maxX){
    if(isLessStrictOrderUp(x)){
        *minX = vI(x,0);
        *maxX = vI(x,xn-1);
    }
    else if(isLessStrictOrderDown(x)){
        *minX = vI(x,xn-1);
        *maxX = vI(x,0);
    }
    else {
        *minX = *maxX = vI(x,0);
        DOIa(xn, {
                if(vI(x,i)<*minX){*minX=vI(x,i);}
                if(vI(x,i)>*maxX){*maxX=vI(x,i);}
                })
    }
    const L maxSize = 1LL << 24;
    if(*minX > 0 && *maxX < maxSize){
        R 1;
    }
    else R 0;
}



I lib_join_index_hash(V z0, V z1, V x, V y, B isEq){
    //lib_join_dummy(x,y); getchar();
    if(isEq) {
        I minX, maxX;
        tic();
        B doTableHash = isStrictUnique(x,&minX,&maxX);
        time_toc("r2: pre-probe (ms): %g\n", elapsed);
        if(doTableHash){
            R lib_join_table_hash(z0,z1,x,y,minX,maxX);
        }
        tic();
        B doCheckValue = checkValueRange(x,&minX,&maxX);
        time_toc("r6: pre-probe (ms): %g\n", elapsed);
        if(doCheckValue){
        // if(checkValueRange(x,&minX,&maxX)){
            R lib_join_table_hash_dup(z0,z1,x,y,minX,maxX);
        }
        else {
            if(vn(x) > vn(y)){
                R lib_join_radix_hash(z1,z0,y,x);
            }
            else R lib_join_radix_hash(z0,z1,x,y);
        }
    }
    else {
        TODO("error when op for join is not equal\n");
        R lib_join_index_basic(z0,z1,x,y,isEq);
    }
}

static L profile_hash_join_cell(HC x){
    L c=0; while(x) { x=x->c_next; c++; } R c;
}

static L profile_hash_join_group(HC *ht, L size, L groupId){
    //DOI(size, P("[%lld] %lld",groupId,profile_hash_join_cell(ht[i])))
    L subTotal = 0;
    DOI(size, subTotal+=profile_hash_join_cell(ht[i]))
    //EP("[%3lld] %lld\n", groupId,subTotal);
    WP("--- Group Id: %lld ---\n",groupId);
    DOI(size, P("[%lld] %lld\n",groupId,profile_hash_join_cell(ht[i])))
    //DOI(size, {L t=profile_hash_join_cell(ht[i]); \
            if(t>1)EP("[%3lld] i=%lld, t=%lld\n", groupId,i,t);} )
    //EP("No len(cell) > 2 found\n");
    R subTotal;
}

static L profile_total_buckets(L *hashMask){
    L c=0; DOI(setT, c+=hashMask[i]+1) R c;
}

void profile_hash_join_v3(HC **ht, L *hashMask){
    L total_cell=0, total_bucket=profile_total_buckets(hashMask);
    DOI(setT, total_cell+=profile_hash_join_group(ht[i],hashMask[i],i))
    WP("percent: %.2lf%% (%lld/%lld)\n", \
            percent(total_cell,total_bucket), total_cell, total_bucket);
}

// xlist -> x (input list)
static L insert_hash_int_v3_G(HC *ht, L htMask, LL *src, L srcI, LL id, V xlist){
#define STORE_INFO_G(x) do{ \
    x->h_index = srcI;    \
    x->h_v     = hashVal; \
    x->h_num   = 1;       \
    x->h_other = srcI;    \
  }while(0)
#define createHashCell(n) HASH_AL(HC0, n)
    LL hashVal= src[srcI];  // hash value
    L  hashId = hashVal & htMask;
    HC t      = ht[hashId];
    if(t){
        HC p=t;
        while(t){
            DOI(t->c_total,{HN x=t->c_node+i; \
                if(x->h_v==hashVal && isAllMatchList(xlist,xlist,hD(x),srcI)) \
                    R insert_index_v3(x,srcI);}) // insert the same items (records)
            t = t->c_next;
        }
        // if not found in all cells, then
        t=p; while(t->c_next) t=t->c_next;
        if(t->c_total < PRE_SIZE_CELL){
            HN x = t->c_node+t->c_total;
            STORE_INFO_G(x);
            t->c_total++;
        }
        else {
            //if(H_DEBUG) P("...... need a new cell: %lld\n",t->c_total);
            HC t0 = createHashCell(1);
            HN x = t0->c_node;
            STORE_INFO_G(x);
            t0->c_total = 1;
            t0->c_next  = NULL;
            t->c_next   = t0;   // link to a new cell
        }
    }
    else {
        HC t0 = createHashCell(1);
        HN x = t0->c_node;
        STORE_INFO_G(x);
        t0->c_total = 1;
        t0->c_next  = NULL;
        ht[hashId]  = t0; // set to the beginning
    }
    R srcI;
}


TB create_hash_multiply_v3_list(V x){
    if(!isList(x))
        EP("Domain error: must be a list, but found: %s\n",getTypeName(xp));
    L numRow = vn(x)>0?vn(vV(x,0)):0;
    L  *count      = HASH_AL(L , setT);
    L  *prefix     = HASH_AL(L , setT);
    L  *hashMask   = HASH_AL(L , setT);
    HC**hashCell   = HASH_AL(HC*,setT);
    I  *hashTemp   = HASH_AL(I , numRow);  // hash value I -> L?
    // memset(count, 0, setT*sizeof(L));
    // prefix[0] = 0;
    if(H_DEBUG) WP("v3-list: //Step 1: scan for basic info\n");
tic();
    DOP(numRow, hashTemp[i]=hash_list(x,i))
    DOI(numRow, count[hashTemp[i]&setN]++)
    DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
toc();
    if(H_DEBUG) WP("v3-list: //Step 2: create partitions (setT=%lld)\n",(L)setT);
tic();
    DOI(setT, { \
        L hashLen=count[i]==0?0:getHashTableSize(count[i]); \
        hashCell[i]=(hashLen>0)?HASH_AL(HC,hashLen):NULL; \
        hashMask[i]=hashLen-1;})
toc();
    if(H_DEBUG) WP("v3-list //Step 3: insert items (%lld)\n",numRow);
tic();
    DOI(numRow, {LL id=hashTemp[i]&setN; \
        insert_hash_int_v3_G(hashCell[id],hashMask[id],hashTemp,i,id,x);})
toc();
    TB tb;
    tb.hashCell = hashCell;
    tb.hashMask = hashMask;
    R tb;
}

static HN find_hash_value_multiple_v3_list(HC *ht, L hashMask, V x, V y, L k){
    // x,y : list
    if(ht){
        L hashKey = hash_list(y,k);
        L hashId  = hashKey & hashMask;
        HC t = ht[hashId];
        //if(k==13){
        //    P("t->c_total = %lld\n",t->c_total);
        //    DOI(t->c_total, {HN t2=t->c_node+i; \
        //            P("res[%lld] = %d\n",i,isAllMatchList(x,y,hD(t2),k));\
        //            if(isAllMatchList(x,y,hD(t2),k))R t2;})
        //    P("done.\n"); getchar();
        //}
        while(t){
            DOI(t->c_total, {HN t2=t->c_node+i; \
                    if(isAllMatchList(x,y,hD(t2),k))R t2;})
            t=t->c_next;
        }
    }
    //if(k==13){
    //    P("ht = %lld\n",(L)ht); getchar();
    //}
    R 0;
}

static I lib_join_special_1(V z0, V z1, V x, V y, B isEq){
    printBanner("Basic hash join v3: special (I,I) join (I,I)");
    V x0=vV(x,0), x1=vV(x,1);
    V y0=vV(y,0), y1=vV(y,1);
tic();
    V p = allocNode();
    initV(p, H_L, 0);
    vn(p) = vn(x0);
    vg(p) = (G)NEWL(L, vn(p));
    DOI(vn(x0), vL(p,i)=((((L)0)|vI(x0,i))<<32)|vI(x1,i))
        V q = allocNode();
    initV(q, H_L, 0);
    vn(q) = vn(y0);
    vg(q) = (G)NEWL(L, vn(y));
    DOI(vn(y0), vL(q,i)=((((L)0)|vI(y0,i))<<32)|vI(y1,i))
toc();
tic();
    I e = lib_join_radix_hash(z0,z1,p,q); // need to support L
toc();
    free(vg(p));
    free(vg(q));
    R e;
}

I lib_join_index_basic_v3(V z0, V z1, V x, V y, B isEq){
    if(H_DEBUG){
        printBanner("Basic hash join v3");
        getInfoVar(x); 
        getInfoVar(y);
        //lib_join_dummy2(x,y);
    }
    if(false && vn(x) == 2 && isInt(vV(x,0)) && isInt(vV(x,1))){
        // pending ...
        lib_join_special_1(z0,z1,x,y,isEq);
    }
    V x0 = sV(x); V y0 = sV(y);
    //V x1 = vV(x,1), y1 = vV(y,1); // debug
    //if(vp(x0) != H_I) EP("Current x0 must be H_I\n");
    //TB th1 = create_hash_multiply_v3(x0);
    TB th1 = create_hash_multiply_v3_list(x);
    HC **hash1     = th1.hashCell;
    L   *hash1Mask = th1.hashMask;
    HA res0 = newHorseArray(), arr0 = res0;
    HA res1 = newHorseArray(), arr1 = res1;
    L total = 0;
    for(L i=0,i2=vn(y0); i<i2;i++){
        //LL id1 = vLL(y0,i)&setN;
        //HN h1  = find_hash_value_multiple_v3_basic(hash1[id1],x0,hash1Mask[id1],xn,y0,i);
        LL id1 = hash_list(y,i)&setN;
        //P("hash = %d, id = %d\n",hash_list(y,i),id1); getchar();
        HN h1  = find_hash_value_multiple_v3_list(hash1[id1],hash1Mask[id1],x,y,i);
        L  n1  = h1?h1->h_num:0;
        /* code below for verifying result */
        //L kk=0; DOJ(vn(x0), if(vI(x0,j)==vI(y0,i)&&vI(x1,j)==vI(y1,i))kk++)
        //if(kk!=n1){
        //    DOJ(vn(x0), if(vI(x0,j)==vI(y0,i)&&vI(x1,j)==vI(y1,i))\
        //            {P("vI(x0,%lld)=%d,vI(x1,%lld)=%d\n",j,vI(x0,j),j,vI(x1,j));})
        //    P("y: i = %lld, kk = %lld, n1 = %lld\n",i,kk,n1);
        //    P("(%d,%d) \n",vI(y0,i),vI(y1,i)); getchar();
        //}
        if(n1>0){
            L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL;
            for(L j=0;j<n1;j++){
                arr0 = appendToHorseArray(arr0, d1[j]);
                arr1 = appendToHorseArray(arr1, i);
            }
            //if(false && vLL(y0,i)==199478){
            //    P("n1 = %lld\n",n1); DOJ(n1, {P("i=%d, d1[j]= %d\n",i,d1[j]);\
            //        P("x0 = %d, y0 = %d\n",vLL(y0,i),vLL(x0,d1[j]));})
            //    P("arr0.size = %lld, arr1.size = %lld\n",arr0->size,arr1->size);
            //    getchar();
            //}
            total += n1;
        }
    }
    P("total = %lld\n", total);
    initV(z0,H_L,total);
    initV(z1,H_L,total);
    copyFromHorseArray(vg(z0),res0);
    copyFromHorseArray(vg(z1),res1);
    freeHorseArray(res0);
    freeHorseArray(res1);
    R 0;
}


#endif
