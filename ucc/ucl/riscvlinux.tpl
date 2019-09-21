#ifndef TEMPLATE
#error "you must define TEMPLATE macro before include this file"
#endif






TEMPLATE(RISCV_PROLOGUE, "addi sp,sp,-32\nsd s0,24(sp)\naddi s0,sp,32\nmv a5,a0")
TEMPLATE(RISCV_EXPANDF, "addi sp,sp,%0")
TEMPLATE(RISCV_EPILOGUE, "ld s0,24(sp)\naddi sp,sp,32\njr ra")
TEMPLATE(RISCV_MOVI4, "mv %1,%0")
