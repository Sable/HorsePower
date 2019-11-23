#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/Support/TargetSelect.h"

#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JITSymbol.h"

#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/LambdaResolver.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"

#include "llvmjit.h"

using namespace llvm;
using namespace llvm::orc;


#include "../global.h" // avoid conflicting definitions
#include <stdarg.h>

static LLVMContext theContext;
static std::unique_ptr<Module> theModule;
static IRBuilder<> Builder(theContext);

//static I depth;
static StructType *TyValue;
static StructType *TyValueAnon;
static PointerType *TyValuePtr;
static StructType *TyTimeVal, *TyTimeZone;

//extern List *compiledMethodList;

#define FuncEntryStr "horse_entry"
#define genPrintf(...) llvm_printf(BB, __VA_ARGS__, NULL)

#include "jit.h"  // macros and functions

#define H_LLVM true  // debugging flag

#define setAlign(x)         x->setAlignment(4)
#define newAlloca(typ)      Builder.CreateAlloca(typ,nullptr)
#define newStore(val,x)     Builder.CreateStore(val,x)
#define newLoad(typ,x)      Builder.CreateLoad(typ,x)
#define newGEP(typ,x,pos)   Builder.CreateStructGEP(typ,x,pos)
#define newCast(x,typ)      Builder.CreateBitCast(x,typ)

#define llvm_genValuePtr(x,t) llvm_genValueNodePtr(x,H_##t)
#define llvm_msg(...) if(H_LLVM) WP("LLVM:" __VA_ARGS__)


/* ------------------ Declarations above ------------------------- */

static void initModule(std::string moduleName){
    theModule = llvm::make_unique<Module>(moduleName, theContext);
}

static void initTopDecl(){
    // only for sable-intel now
    theModule->setDataLayout("e-m:e-i64:64-f80:128-n8:16:32:64-S128");
    theModule->setTargetTriple("x86_64-unknown-linux-gnu");
}

static void initTarget(){
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
}

static void initGlobalType(){
    std::vector<Type *> items;
    items.push_back(TyI64); // int64
    items.push_back(TyI8p); // char*
    items.push_back(TyI32); // int32
    TyValueAnon = StructType::create(theContext, {TyI64}, "union.anon");
    items.push_back(TyValueAnon);
    TyValue = StructType::create(theContext, items, "struct.NodeValue");
    TyValuePtr = TyValue->getPointerTo();
    // timeval
    TyTimeVal  = StructType::create(theContext, {TyI64, TyI64}, "struct.timeval");
    TyTimeZone = StructType::create(theContext, {TyI32, TyI32}, "struct.timezone");
}

/*
 * source:
 *   https://stackoverflow.com/a/46337835/4111149
 */
static void llvm_printf(BasicBlock *bb, const char *format, ...){
    Function *funcPrint = theModule->getFunction("printf");
    if(!funcPrint){ // first time
        FunctionType *funcTy = FunctionType::get(TyI32, true);
        funcPrint = Function::Create(funcTy,
                GlobalValue::ExternalLinkage,
                "printf",
                theModule.get());
        funcPrint->setCallingConv(CallingConv::C);
        AttributeList funcPrintAttr;
        funcPrint->setAttributes(funcPrintAttr);
    }
    Builder.SetInsertPoint(bb);
    Value *str = Builder.CreateGlobalStringPtr(format);
    std::vector<Value *> params;
    params.push_back(str);
    va_list ap;
    va_start(ap, format);
    do{
        Value *op = va_arg(ap, Value*);
        if(op)
            params.push_back(op);
        else
            break;
    }while(1);
    va_end(ap);
    CallInst *call = CallInst::Create(funcPrint, params, "call", bb);
}

static void setParamName(Function *func, std::vector<std::string> funcArgs){
    int idx = 0;
    for (auto &arg : func->args()){
        arg.setName(funcArgs[idx++]);
    }
}

// x->i32
static Value *llvm_genValueNodePtr(Value *x, I typ){
    AllocaInst *a_n = newAlloca(TyValuePtr);
    setAlign(a_n);
    StoreInst *s_n = newStore(x, a_n);
    setAlign(s_n);
    LoadInst *l_n = newLoad(TyValuePtr, a_n);
    setAlign(l_n);
    Value *ptr_n  = newGEP(TyValue, l_n, 3);
    Value *rtn = NULL;
    switch(typ){
        caseI rtn = newCast(ptr_n, TyI32p); break;
        caseQ caseZ
        caseL rtn = newCast(ptr_n, TyI64p); break;
        caseF rtn = newCast(ptr_n, TyF32p); break;
        caseE rtn = newCast(ptr_n, TyF64p); break;
        default: EP("Need to support: %s\n", getTypeName(typ));
    }
    return rtn;
}

//-- gen-based functions

static Function *llvm_genMethodHead(
        std::string funcName,
        Type * rtnType){
    llvm_msg("1. Creating function: %s\n", funcName.c_str());
    FunctionType *funcTy = FunctionType::get(rtnType, false);
    Function *func = Function::Create(
            funcTy,
            Function::ExternalLinkage,
            funcName,
            theModule.get());
    return func;
}

static Function *llvm_genMethodHead(
        std::string funcName,
        Type * rtnType,
        std::vector<Type *> paramTypes,
        std::vector<std::string> paramNames = std::vector<std::string>(),
        GlobalValue::LinkageTypes linkType = GlobalValue::ExternalLinkage){
    llvm_msg("2. Creating function: %s\n", funcName.c_str());
    FunctionType *funcTy = FunctionType::get(rtnType, paramTypes, false);
    Function *func = Function::Create(
            funcTy, linkType, funcName, theModule.get());
    if(!paramNames.empty())
        setParamName(func, paramNames);
    return func;
}


static void llvm_genCall_gettimeofday(std::vector<Value *> params){
    Function *func = theModule->getFunction("gettimeofday");
    if(!func){
        FunctionType *funcTy = FunctionType::get(
                TyI32,
                {TyTimeVal->getPointerTo(), TyTimeZone->getPointerTo()},
                false);
        func = Function::Create(funcTy,
                GlobalValue::ExternalLinkage,
                "gettimeofday",
                theModule.get());
        func->setCallingConv(CallingConv::C);
        AttributeList funcAttr;
        func->setAttributes(funcAttr);
    }
    CallInst *call = Builder.CreateCall(func, params);
}

static CallInst *llvm_genCall_calcinterval(std::vector<Value *> params){
    Function *func = theModule->getFunction("calcInterval");
    if(!func){
        llvm_msg("Not found: calcInterval\n");
        FunctionType *funcTy = FunctionType::get(
                TyF64,
                {TyI64,TyI64,TyI64,TyI64},
                false);
        func = Function::Create(funcTy,
                GlobalValue::ExternalLinkage,
                "calcInterval",
                theModule.get());
        func->setCallingConv(CallingConv::C);
        AttributeList funcAttr;
        func->setAttributes(funcAttr);
    }
    return Builder.CreateCall(func, params);
}

static void genMethodEntry(){
    llvm_msg("step 1: create function head\n");
    Function *func = llvm_genMethodHead(FuncEntryStr, TyF64);
    // std::vector<Type *> params({TyValue, TyValuePtr});
    llvm_msg("step 2: build function body\n");
    BasicBlock *BB = BasicBlock::Create(theContext, "entry", func);
    Builder.SetInsertPoint(BB);
    AllocaInst *a_elapse = newAlloca(TyF64);
    setAlign(a_elapse);
    AllocaInst *a_tv1 = newAlloca(TyTimeVal);
    AllocaInst *a_tv2 = newAlloca(TyTimeVal);
    llvm_genCall_gettimeofday({a_tv1, ConstantPointerNull::get(TyTimeZone->getPointerTo())});
    llvm_msg("(( running code ))\n");
    llvm_genCall_gettimeofday({a_tv2, ConstantPointerNull::get(TyTimeZone->getPointerTo())});
    // invoke params
    llvm_msg("(( casting ))\n");
    Value *v_c1_arg0 = newGEP(TyTimeVal, a_tv1, 0);
    Value *v_c1_arg1 = newGEP(TyTimeVal, a_tv1, 1);
    Value *v_c2_arg0 = newGEP(TyTimeVal, a_tv2, 0);
    Value *v_c2_arg1 = newGEP(TyTimeVal, a_tv2, 1);
    //ArrayType *TyArrayI64 = ArrayType::get(TyI64, 2);
    //PointerType *TyArrayI64p = TyArrayI64->getPointerTo();
    //Value *v_c1 = newCast(a_tv1, TyArrayI64p);
    //Value *v_c1_arg0 = newGEP(TyArrayI64, a_tv1, 0);
    //Value *v_c1_arg1 = newGEP(TyArrayI64, a_tv1, 1);
    //Value *v_c2 = newCast(a_tv2, TyArrayI64p);
    //Value *v_c2_arg0 = newGEP(TyArrayI64, a_tv2, 0);
    //Value *v_c2_arg1 = newGEP(TyArrayI64, a_tv2, 1);
    llvm_msg("(( cast tv2 ))\n");
    std::vector<Value *> vecArgs;
    vecArgs.push_back(newLoad(TyI64, v_c1_arg0));
    vecArgs.push_back(newLoad(TyI64, v_c1_arg1));
    vecArgs.push_back(newLoad(TyI64, v_c2_arg0));
    vecArgs.push_back(newLoad(TyI64, v_c2_arg1));
    llvm_msg("(( invoking ))\n");
    CallInst *rtn = llvm_genCall_calcinterval(vecArgs);
    StoreInst *s_elapse = newStore(rtn, a_elapse);
    setAlign(s_elapse);
    LoadInst   *l_elapse = newLoad(TyF64, a_elapse);
    setAlign(l_elapse);
    genPrintf("The elapsed time (ms): %g\n", l_elapse);
    genPrintf("Output:\n");
    // -- testing NodeValue
    //Function::arg_iterator args = func->arg_begin();
    //Value *arg2 = args+1;
    //Value *castA = llvm_genValuePtr(arg2, I);
    //Builder.CreateStore(ConstantI64(99), castA);
    Builder.CreateRet(l_elapse);
    // step 3: check function
    verifyFunction(*func);
}

static void genMethodExternal(){
    Function *func = llvm_genMethodHead("calcInterval",
            TyF64,
            {TyI64,TyI64,TyI64,TyI64},
            std::vector<std::string>(),
            GlobalValue::InternalLinkage);
    BasicBlock *BB = BasicBlock::Create(theContext, "entry", func);
    Builder.SetInsertPoint(BB);
    Function::arg_iterator args = func->arg_begin();
    Value *arg0 = args;
    Builder.CreateRet(Builder.CreateSIToFP(arg0, TyF64));
}

static void genMethodHead(Node *n){
    // resetCode(); S line = ptr;
    // glueAny("static I horse_%s(", n->val.method.fname);
    // I numRtn   = totalInfoNodeList(nodeMethodMeta(n)->returnTypes);
    // I numParam = totalSymbolNameList(nodeMethodMeta(n)->paramVars);
    // if(numRtn > 0)
    //     glueCode("V *h_rtn");
    // if(numParam > 0){
    //     if(numRtn > 0) glueCode(", ");
    //     genVarList(nodeMethodParameters(n)->val.listS, comma);
    // }
    // glueMethodHead(line, ");");
    // glueCode("){");
    // glueLine();
}


//static void compileMethod(Node *n){
//    genMethodHead(n);
//    //scanNode(n->val.method.block);
//}
//
//
//static void compileCode(List *list){
//    if(list){ compileCode(list->next); compileMethod(list->val); }
//}

static void runCode(){
    if(true){
        WP("Code dumped:\n");
        WP("-------- Begin ---------\n");
        theModule->print(errs(), nullptr);
        WP("--------- End ----------\n");
    }
    // execute code
    HorseJIT theJIT;
    tic();
    auto H = theJIT.addModule(std::move(theModule));
    time_toc("Part 2: JIT load (ms): %g\n", elapsed);
    tic();
    E (*entry)() = (E (*)())theJIT.getSymbolAddress(FuncEntryStr);
    entry();  // P("Program output: %g\n", entry());
    time_toc("Part 3: JIT execution (ms): %g\n", elapsed);
}

static void init(){
    //depth = 0;
    initModule("HorseModule");
    initTopDecl();
    initTarget();
    initGlobalType();
}

int HorseCompilerJITNaive(){
    init();
    tic();
    //compileCode(compiledMethodList->next);
    genMethodExternal();
    genMethodEntry();
    time_toc("Part 1: code gen (ms): %g\n", elapsed);
    runCode();
    return 0;
}

// maybe move a new file
int HorseCompilerJITOptimized(){
    return 0;
}




