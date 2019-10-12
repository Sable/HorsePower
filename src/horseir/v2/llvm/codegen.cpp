#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;

static LLVMContext TheContext;
static std::unique_ptr<Module> TheModule;

int HorseCompilerLLVM(){
    TheModule = llvm::make_unique<Module>("ModuleHorseIR", TheContext);
    // Print out all of the generated code.
    TheModule->print(errs(), nullptr);
    return 0;
}

int main(){
    HorseCompilerLLVM();
}


