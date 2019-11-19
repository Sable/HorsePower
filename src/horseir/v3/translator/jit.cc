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

#include "jit.h"

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

//extern List *compiledMethodList;

#define TyI8  Type::getInt8Ty(theContext)
#define TyI32 Type::getInt32Ty(theContext)
#define TyI64 Type::getInt64Ty(theContext)
#define TyF32 Type::getDoubleTy(theContext)
#define TyF64 Type::getDoubleTy(theContext)

#define TyI8p  Type::getInt8PtrTy(theContext)
#define TyI32p Type::getInt32PtrTy(theContext)
#define TyI64p Type::getInt64PtrTy(theContext)
#define TyF32p Type::getDoublePtrTy(theContext)
#define TyF64p Type::getDoublePtrTy(theContext)

#define ConstantI32(x) ConstantInt::get(TyI32,x)
#define ConstantI64(x) ConstantInt::get(TyI64,x)
#define ConstantF32(x) ConstantFP::get(TyF32,x)
#define ConstantF64(x) ConstantFP::get(TyF64,x)

#define genPrintf(...) llvm_printf(BB, __VA_ARGS__, NULL)

#define FuncEntryStr "horse_entry"

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
}

static Function* demo_createFunc(){
    std::vector<Type *> paramTy({TyI32, TyI32});
    FunctionType *funcTy = FunctionType::get(TyI32, paramTy, false);
    std::string funcName = "demo";
    std::vector<std::string> funcArgs({"x", "y"});
    Function *func = Function::Create(funcTy,
                                      Function::ExternalLinkage,
                                      funcName,
                                      theModule.get());
    int idx = 0;
    for (auto &arg : func->args()){
        arg.setName(funcArgs[idx++]);
    }
    return func;
}

#define isDemoNaive false
static void demo_createBody(Function *func){
    if(isDemoNaive){
        // follow: "Getting Started with LLVM Core Libraries" p119
        Function::arg_iterator args = func->arg_begin();
        Value* arg1 = args++;  // arg1.setName("a");
        Value* arg2 = args++;
        BasicBlock *BB = BasicBlock::Create(theContext, "entry", func);
        Builder.SetInsertPoint(BB); // insert content into BB
        // init two variables
        AllocaInst* localA = Builder.CreateAlloca(TyI32, nullptr, "a");
        localA->setAlignment(4);
        AllocaInst* localB = Builder.CreateAlloca(TyI32, nullptr, "b");
        localB->setAlignment(4);
        // save to local vars
        StoreInst* instA = Builder.CreateStore(arg1, localA);
        instA->setAlignment(4);
        StoreInst* instB = Builder.CreateStore(arg2, localB);
        instB->setAlignment(4);
        // load from local vars
        LoadInst* valA = Builder.CreateLoad(TyI32, localA);
        valA->setAlignment(4);
        LoadInst* valB = Builder.CreateLoad(TyI32, localB);
        valB->setAlignment(4);
        // operation: add
        Value* res = Builder.CreateAdd(valA, valB);
        Builder.CreateRet(res);
    }
    else {
        // simplified version (remove intermediate variables)
        BasicBlock *BB = BasicBlock::Create(theContext, "entry", func);
        Builder.SetInsertPoint(BB); // insert content into BB
        Function::arg_iterator args = func->arg_begin();
        Value *arg1 = args++;
        Value *arg2 = args++;
        Value *res = Builder.CreateAdd(arg1, arg2, "rtn");
        Builder.CreateRet(res);
    }
}

/*
 * source:
 *   https://stackoverflow.com/a/46337835/4111149
 */
void llvm_printf(BasicBlock *bb, const char *format, ...){
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

static void genMethodEntry(){
    std::vector<Type *> params({TyValue, TyValuePtr});
    // step 1: create function head
    FunctionType *funcTy = FunctionType::get(TyF64, params, false);
    std::string funcName = FuncEntryStr;
    Function *func = Function::Create(funcTy,
            Function::ExternalLinkage,
            funcName,
            theModule.get());
    setParamName(func, {"xy", "y"});
    // step 2: build function body
    BasicBlock *BB = BasicBlock::Create(theContext, "entry", func);
    Builder.SetInsertPoint(BB);
    AllocaInst *new_elapse = Builder.CreateAlloca(TyF64, nullptr, "elapsed");
    new_elapse->setAlignment(4);
    StoreInst *instElapse = Builder.CreateStore(ConstantF64(3.14), new_elapse);
    instElapse->setAlignment(4);
    LoadInst *valElapse = Builder.CreateLoad(TyF64, new_elapse);
    genPrintf("The elapsed time (ms): %g\n", valElapse);
    genPrintf("Output:\n");
    // -- testing NodeValue
    Function::arg_iterator args = func->arg_begin();
    Value *arg2 = args+1;
    AllocaInst *localA = Builder.CreateAlloca(TyValuePtr, nullptr);
    localA->setAlignment(4);
    StoreInst *instA = Builder.CreateStore(arg2, localA);
    instA->setAlignment(4);
    LoadInst *valA = Builder.CreateLoad(TyValuePtr, localA);
    valA->setAlignment(4);
    Value *ptrA = Builder.CreateStructGEP(TyValue, valA, 3);
    Value *castA = Builder.CreateBitCast(ptrA, TyI64p);
    Builder.CreateStore(ConstantI64(99), castA);
    Builder.CreateRet(valElapse);
    // step 3: check function
    verifyFunction(*func);
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
    initModule("ExampleModule");
    initTopDecl();
    initTarget();
    initGlobalType();
}

int HorseCompilerJITNaive(){
    init();
    tic();
    //demo();
    //compileCode(compiledMethodList->next);
    genMethodEntry();
    //demo_createFunc();
    time_toc("Part 1: code gen (ms): %g\n", elapsed);
    runCode();
    return 0;
}

// maybe move a new file
int HorseCompilerJITOptimized(){
    return 0;
}




