#include "../global.h"

static I qsort_order_default (const void * a, const void * b) {
    return ( *(L*)a - *(L*)b );
}

static void displayPartition(L par, L tot, L *val, L *cnt, L all){
    P("Total groups: %lld out of total %lld items (avg %.2lf)\n\n", tot,all,percent(all,tot)/100);
    if(all == tot){
        P("  ** Note: all items are distinct**\n");
    }
    else if(par >= tot){ // display all
        DOI(tot, P("val[%lld] = %lld with total %lld occurences (%.2lf%%)\n",i,val[i],cnt[i],percent(cnt[i],all)))
    }
    else { // display only 10 partitions
        L seg = tot/par, seg2 = seg>2?2:seg;
        DOI(par, {L k=seg*i; L sub=0; \
           DOJ3(k, (i==par-1)?tot:(k+seg),sub+=cnt[j]) \
           P("partition[%lld]: ",i); \
           DOJ(seg2, P("%lld ",val[k+j])) if(seg!=seg2)P("..."); \
           P(", total %lld (%.2lf%%)\n",sub,percent(sub,all));})
    }
}

// profile duplicated data
static I profileDupData(L *x, L n){
    qsort(x, n, sizeof(L), qsort_order_default);
    //DOI(n, P("x[%lld] = %lld\n",i,x[i]))
    L tot = 1;
    DOIa(n, if(x[i]!=x[i-1]){tot++;})
    L k = 0;
    L *x_val = NEWL(L, tot);
    L *x_cnt = NEWL(L, tot);
    x_val[0] = x[0];
    x_cnt[0] = 1;
    DOIa(n, if(x[i]!=x[i-1]){k++; x_val[k]=x[i]; x_cnt[k]=1;}else{x_cnt[k]++;})
    //DOI(tot, P("cnt[%lld] = %lld\n",i,x_cnt[i]))
    displayPartition(25,tot,x_val,x_cnt,n);
    free(x_val);
    free(x_cnt);
    R 0;
}

// can delete
I testFunc1(){ 
    L values[] = { 2, 25, 2, 88, 56, 100, 2, 25 };
    profileDupData(values, 8);
    R 0;
}

static void profileHashTableSingle(HN x, L size){
    L tot=0; DOI(size, {HN t=hV(x,i); if(hN(t))tot++;})
    P("slots used %.2lf %% (%lld/%lld)\n",percent(tot,size),tot,size);
}


static I profileBoolData(V x){
    L tot = 0; DOI(xn, if(xB(i))tot++)
    P(">> boolean vector ratio = %.2lf %% (%lld/%lld)\n",percent(tot,xn),tot,xn);
    R 0;
}

I profileV(V x){
    getInfoVar(x);
    switch(xp){
        caseB profileBoolData(x); break;
        caseL profileDupData(sL(x), xn);  break;
        default: EP("type is not supported: %s\n", getTypeName(xp));
    }
    R 0;
}


