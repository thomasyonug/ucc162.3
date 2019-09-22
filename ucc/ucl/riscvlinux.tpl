#ifndef TEMPLATE
#error "you must define TEMPLATE macro before include this file"
#endif



TEMPLATE(RISCV_PROLOGUE,   "addi sp,sp,-16\nsd s0,8(sp)\nsd ra,0(sp)\naddi s0,sp,16\n")
TEMPLATE(RISCV_EXPANDF,    "addi sp,sp,%0")
TEMPLATE(RISCV_EPILOGUE,   "ld s0,8(sp)\nld ra,0(sp)\naddi sp,sp,16\njr ra")
TEMPLATE(RISCV_MOVI4,		   "mv %0,%1")
TEMPLATE(RISCV_MOVI4TOARG, "mv a0,%1")
TEMPLATE(RISCV_CALL, "call %1@plt")
TEMPLATE(RISCV_ICALL, "call *%1@plt")
TEMPLATE(RISCV_ADDR, "lla %0, %1")
