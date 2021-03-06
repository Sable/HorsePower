#include "../global.h"

static void printJSONCore(JSON *x, int dep);

/* -------------- above declarations -------------- */

JSON *initJSON(){
    JSON *x = NEW(JSON);
    return x;
}

static void cleanJSON(JSON *x){
    TODO("Add impl.");
}

JSON *newJSON0(const char *key, const char *val){
    JSON *x = NEW(JSON);
    x->kind  = JSON_VALUE;
    x->size  = 1;
    x->key   = strdup(key);
    x->field = strdup(val);
    return x;
}

JSON *newJSON1(const char *key, int size, const char **val){
    JSON *x = NEW(JSON);
    x->kind  = JSON_VALUES;
    x->size  = size;
    x->key   = strdup(key);
    x->field_list = NEW(char*);
    DOI(size, x->field_list[i] = strdup(val[i]))
    return x;
}

JSON *newJSON2(const char *key, JSON *val){
    JSON *x = NEW(JSON);
    x->kind = JSON_JSON;
    x->size = 1;
    x->key  = strdup(key);
    x->child = val;
    return x;
}

JSON *newJSON3(const char *key, int size, JSON **val){
    JSON *x = NEW(JSON);
    x->kind = JSON_JSONS;
    x->size = size;
    x->key  = strdup(key);
    x->child_list = NEW2(JSON, size);
    DOI(size, x->child_list[i] = val[i])
    return x;
}

JSON *addJSONField(JSON *x, JSON *n){
    n->next = x->next;
    x->next = n;
    return n;
}

static void printIndent(int dep){
    DOI(dep, P("    "))
}

static const char *obtainJsonStr(JsonKind x){
    switch(x){
        case JSON_VALUE : R "Simple value";
        case JSON_VALUES: R "Multiple values";
        case JSON_JSON  : R "Simple JSON value";
        case JSON_JSONS : R "Multiple JSON values";
        default: EP("Unknown JSON kind: %d", x);
    }
}

static void printJSONPairCore(JSON *x, int dep){
    printIndent(dep);
    P("\"%s\":",x->key);
    switch(x->kind){
        case JSON_VALUE :
                P("\"%s\"", x->field); break;
        case JSON_VALUES:
                P("[");
                DOI(x->size, {if(i>0)P(",");P("\"%s\"",x->field_list[i]);})
                P("]"); break;
        case JSON_JSON :
                printJSONCore(x->child, dep); break;
        case JSON_JSONS:
                P("[");
                DOI(x->size, {if(i>0)P(",");printJSONCore(x->child_list[i], dep);})
                P("]");  break;
        default: TODO("Add impl. for %s", obtainJsonStr(x->kind));
    }
}

static void printJSONPair(JSON *x, int dep){
    if(x){
        printJSONPair(x->next,dep);
        if(x->next) P(",\n");
        printJSONPairCore(x,dep);
    }
}

static void printJSONCore(JSON *x, int dep){
    P("\n"); printIndent(dep); P("{\n");
    printJSONPair(x->next, dep+1);
    P("\n"); printIndent(dep); P("}");
}

void printJSON(JSON *x){
    printJSONCore(x, 0); P("\n");
}

// for util/stats.c
void demoJSON(){
    JSON *rt = initJSON();
    const char *vals[] = {"TPC-H1", "TPC-H2"};
    addJSONField(rt, newJSON0("tables1", "TPC-H"));
    addJSONField(rt, newJSON0("tables2", "TPC-H"));
    addJSONField(rt, newJSON1("tables3", 2, vals));
    {
        JSON *level2 = initJSON();
        addJSONField(level2, newJSON0("column0", "v0"));
        addJSONField(level2, newJSON0("column1", "v1"));
        addJSONField(rt, newJSON2("tables4", level2));
    }
    //JSON *t0 = addJSONField(rt, newJSON1("tables3", 2, vals));
    //JSON *t1 = addJSONField(rt, newJSON2("tables4", newJSON0("column0", "v0")));
    //JSON *tvals[] = {t0, t1};
    //addJSONField(rt, newJSON3("tables4", 2, tvals));
    printJSON(rt);
}


