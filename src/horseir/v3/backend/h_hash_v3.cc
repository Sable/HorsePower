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
    L  *count      = HASH_AL(L , setT);
    L  *prefix     = HASH_AL(L , setT);
    L  *hashMask   = HASH_AL(L , setT);
    HC**hashCell   = HASH_AL(HC*,setT);
    if(H_DEBUG) WP("v3: //Step 1: scan for basic info\n");
tic();
    DOI(xn, count[vLL(x,i)&setN]++)
    DOIa(setT, prefix[i]=prefix[i-1]+count[i-1])
toc();
    if(H_DEBUG) WP("v3: //Step 2: create partitions (setT=%lld)\n",(L)setT);
tic();
    DOI(setT, { \
        L hashLen=count[i]==0?0:getHashTableSize(count[i]); \
        hashCell[i]=(hashLen>0)?HASH_AL(HC,hashLen):NULL; \
        hashMask[i]=hashLen-1;})
toc();
    //test_hashSize(hashSize);
    if(H_DEBUG) WP("v3 //Step 3: insert items (%lld)\n",xn);
    //P("xn = %lld, setT = %lld\n",xn,setT); getchar();
tic();
    switch(xp){
caseI
    DOI(xn, {LL id=vLL(x,i)&setN; \
        insert_hash_int_v3_I(hashCell[id],hashMask[id],sLL(x),i,id);}) break;
caseE
    DOI(xn, {LL id=vLL(x,i)&setN; \
        insert_hash_int_v3_E(hashCell[id],hashMask[id],sE(x),i,id);}) break;
default: EP("Type not supported: %s", getTypeName(xp));
    }
toc();
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
            DOI(t->c_total, {HN x=t->c_node+i; if(x->h_i==val) R x;})
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
    getInfoVar(x); getInfoVar(y); //getchar();
    if(vp(x)==H_LL && vp(y)==H_LL){
        HN *tempHN = HASH_AL(HN, vn(y));
    tic();
        TB tb = create_hash_multiply_v3(x);
        HC**hashTable = tb.hashCell;
        L  *hashMask  = tb.hashMask;
    time_toc("> Radix hash, build phase\n");
        L c = 0;
    tic();
        DOI(vn(y), { LL id=vLL(y,i)&setN; \
            HN h1=find_hash_value_single_v3(hashTable[id],hashMask[id],vLL(y,i));\
            if(h1) c+=h1->h_num; tempHN[i]=h1;})
    time_toc("> Radix hash, probe phase\n");
        initV(z0, H_L, c);
        initV(z1, H_L, c);
    tic();
        c = 0;
        DOJ(vn(y), {HN h1=tempHN[j]; \
            if(h1){ L n1=h1->h_num; \
            L *d1 = n1==1?(L*)&(h1->h_other):n1>1?(L*)h1->h_other:NULL; \
            /*P("[%lld][%lld] with size %lld\n",d1[0],j,n1); getchar();*/ \
            DOI(n1, {vL(z0,c)=d1[i]; vL(z1,c)=j; c++;})}})
    time_toc("> Radix hash, write phase\n");
        P("Total elements write = %lld\n", c); // expected for Q5 3rd join: 46008
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

I lib_join_index_hash(V z0, V z1, V x, V y, B isEq){
    //lib_join_dummy(x,y); getchar();
    if(isEq) R lib_join_radix_hash(z0,z1,x,y);
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
    //P("total = %lld\n", total); getchar();
    initV(z0,H_L,total);
    initV(z1,H_L,total);
    copyFromHorseArray(vg(z0),res0);
    copyFromHorseArray(vg(z1),res1);
    freeHorseArray(res0);
    freeHorseArray(res1);
    R 0;
}


#endif
