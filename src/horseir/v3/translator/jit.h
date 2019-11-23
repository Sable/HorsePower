#ifndef __H_T_JIT__
#define __H_T_JIT__

// type macros

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


#endif
