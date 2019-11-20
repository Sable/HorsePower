# HorseSystem: A System for Processing HorseIR Programs

## Overview

HorseSystem is an important component for handling HorseIR programs.
It supports multiple kinds of back-ends considering
(1) different modes for either compiling or executing input HorseIR programs;
(2) performance with both compilation and interpretation; and
(3) adaptability over various platforms such as CPUs and GPUs.

## Back-ends

- Interpreter: It executes HorseIR directly without any intermediate code generated.
- Compiler-to-C: HorseIR is compiled to C, then C is compiled to binary before generating results.
- Compiler-to-OpenACC: HorseIR is compiled to OpenACC, then C is compiled to binary before generating results.
- Compiler-to-LLVM: It is a JIT system which generates optimized code on the fly in order to reduce compilation time.





