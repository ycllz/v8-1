// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Push all callee-saved registers to get them on the stack for conservative
// stack scanning.
//
// See asm/x64/push_registers_clang.cc for why the function is not generated
// using clang.
//
// Do not depend on V8_TARGET_OS_* defines as some embedders may override the
// GN toolchain (e.g. ChromeOS) and not provide them.

// We maintain 16-byte alignment.
//
// Calling convention source:
// https://en.wikipedia.org/wiki/Calling_convention#ARM_(A64)

asm(
    ".globl PushAllRegistersAndIterateStack             \n"
#ifndef _WIN64
    ".hidden PushAllRegistersAndIterateStack            \n"
#endif
    "PushAllRegistersAndIterateStack:                   \n"
    // x19-x29 are callee-saved.
    "  stp x19, x20, [sp, #-16]!                        \n"
    "  stp x21, x22, [sp, #-16]!                        \n"
    "  stp x23, x24, [sp, #-16]!                        \n"
    "  stp x25, x26, [sp, #-16]!                        \n"
    "  stp x27, x28, [sp, #-16]!                        \n"
    "  stp fp, lr,   [sp, #-16]!                        \n"
    // Maintain frame pointer.
    "  mov fp, sp                                       \n"
    // Pass 1st parameter (x0) unchanged (Stack*).
    // Pass 2nd parameter (x1) unchanged (StackVisitor*).
    // Save 3rd parameter (x2; IterateStackCallback)
    "  mov x7, x2                                       \n"
    // Pass 3rd parameter as sp (stack pointer).
    "  mov x2, sp                                       \n"
    "  blr x7                                           \n"
    // Load return address.
    "  ldr lr, [sp, #8]                                 \n"
    // Restore frame pointer and pop all callee-saved registers.
    "  ldr fp, [sp], #96                                \n"
    "  ret                                              \n");
