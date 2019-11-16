#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
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

#define TyI32 Type::getInt32Ty(theContext)
#define TyF64 Type::getDoubleTy(heContext)

#define TyI32p Type::getInt32PtrTy(theContext)
#define TyF64p Type::getDoublePtrTy(heContext)

#include "../global.h" // avoid conflicting definitions

static LLVMContext theContext;
static std::unique_ptr<Module> theModule;
static IRBuilder<> Builder(theContext);

static void llvm_initModule(std::string moduleName){
    theModule = llvm::make_unique<Module>(moduleName, theContext);
}

static void llvm_genTopDecl(){
    // only for sable-intel now
    theModule->setDataLayout("e-m:e-i64:64-f80:128-n8:16:32:64-S128");
    theModule->setTargetTriple("x86_64-unknown-linux-gnu");
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

static void demo_dumpCode(){
    P("Code dumped\n");
    theModule->print(errs(), nullptr);
}

static void jit_example(){
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    HorseJIT theJIT;
    auto H = theJIT.addModule(std::move(theModule));
    //InitializeModuleAndPassManager();
    int (*foo)(int, int) = (int (*)(int, int)) theJIT.getSymbolAddress("demo");
    P("demo result: %d", foo(4,5));
}

static void demo(){
    Function *func = demo_createFunc();
    if(func){
        demo_createBody(func);
        verifyFunction(*func);
        demo_dumpCode();
        jit_example();
    }
}

static void init(){
    llvm_initModule("ExampleModule");
    llvm_genTopDecl();
}

int HorseCompilerJITNaive(){
    init();
    demo();
    return 0;
}

// maybe move a new file
int HorseCompilerJITOptimized(){
    return 0;
}




