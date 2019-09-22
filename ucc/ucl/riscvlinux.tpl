#ifndef TEMPLATE
#error "you must define TEMPLATE macro before include this file"
#endif


TEMPLATE(RISCV_BORI4,     "orl %2, %0")
TEMPLATE(RISCV_BORU4,     "orl %2, %0")
TEMPLATE(RISCV_BORF4,     NULL)
TEMPLATE(RISCV_BORF8,     NULL)

TEMPLATE(RISCV_BXORI4,    "xorl %2, %0")
TEMPLATE(RISCV_BXORU4,    "xorl %2, %0")
TEMPLATE(RISCV_BXORF4,    NULL)
TEMPLATE(RISCV_BXORF8,    NULL)

TEMPLATE(RISCV_BANDI4,    "andl %2, %0")
TEMPLATE(RISCV_BANDU4,    "andl %2, %0")
TEMPLATE(RISCV_BANDF4,    NULL)
TEMPLATE(RISCV_BANDF8,    NULL)

TEMPLATE(RISCV_LSHI4,    "shll %2, %0")
TEMPLATE(RISCV_LSHU4,    "shll %2, %0")
TEMPLATE(RISCV_LSHF4,    NULL)
TEMPLATE(RISCV_LSHF8,    NULL)

TEMPLATE(RISCV_RSHI4,    "sarl %2, %0")
TEMPLATE(RISCV_RSHU4,    "shrl %2, %0")
TEMPLATE(RISCV_RSHF4,    NULL)
TEMPLATE(RISCV_RSHF8,    NULL)

TEMPLATE(RISCV_ADDI4,    "addi %0,%0,%1")
TEMPLATE(RISCV_ADDU4,    "addi %0,%0,%1")
TEMPLATE(RISCV_ADDF4,    "fadds %2")
TEMPLATE(RISCV_ADDF8,    "faddl %2")

TEMPLATE(RISCV_SUBI4,    "subl %2, %0")
TEMPLATE(RISCV_SUBU4,    "subl %2, %0")
TEMPLATE(RISCV_SUBF4,    "fsubs %2")
TEMPLATE(RISCV_SUBF8,    "fsubl %2")

TEMPLATE(RISCV_MULI4,    "imull %2, %0")
TEMPLATE(RISCV_MULU4,    "mull %2")
TEMPLATE(RISCV_MULF4,    "fmuls %2")
TEMPLATE(RISCV_MULF8,    "fmull %2")

TEMPLATE(RISCV_DIVI4,    "cdq;idivl %2")
TEMPLATE(RISCV_DIVU4,    "movl $0, %%edx;divl %2")
TEMPLATE(RISCV_DIVF4,    "fdivs %2")
TEMPLATE(RISCV_DIVF8,    "fdivl %2")

TEMPLATE(RISCV_MODI4,    "cdq;idivl %2")
TEMPLATE(RISCV_MODU4,    "movl $0, %%edx; divl %2")
TEMPLATE(RISCV_MODF4,    NULL)
TEMPLATE(RISCV_MODF8,    NULL)

TEMPLATE(RISCV_NEGI4,    "negl %0")
TEMPLATE(RISCV_NEGU4,    "negl %0")
TEMPLATE(RISCV_NEGF4,    "fchs")
TEMPLATE(RISCV_NEGF8,    "fchs")

TEMPLATE(RISCV_COMPI4,   "notl %0")
TEMPLATE(RISCV_COMPU4,   "notl %0")
TEMPLATE(RISCV_COMPF4,   NULL)
TEMPLATE(RISCV_COMPF8,   NULL)

TEMPLATE(RISCV_JZI4,     "cmpl $0, %1;je %0")
TEMPLATE(RISCV_JZU4,     "cmpl $0, %1;je %0")
TEMPLATE(RISCV_JZF4,     "fldz;fucompp;fnstsw %%ax;test $0x44, %%ah;jnp %0")
TEMPLATE(RISCV_JZF8,     "fldz;fucompp;fnstsw %%ax;test $0x44, %%ah;jnp %0")

TEMPLATE(RISCV_JNZI4,    "cmpl $0, %1;jne %0")
TEMPLATE(RISCV_JNZU4,    "cmpl $0, %1;jne %0")
TEMPLATE(RISCV_JNZF4,    "fldz;fucompp;fnstsw %%ax;test $0x44, %%ah;jp %0")
TEMPLATE(RISCV_JNZF8,    "fldz;fucompp;fnstsw %%ax;test $0x44, %%ah;jp %0")

TEMPLATE(RISCV_JEI4,     "cmpl %2, %1;je %0")
TEMPLATE(RISCV_JEU4,     "cmpl %2, %1;je %0")
TEMPLATE(RISCV_JEF4,     "flds %2;fucompp;fnstsw %%ax;test $0x44, %%ah;jnp %0")
TEMPLATE(RISCV_JEF8,     "fldl %2;fucompp;fnstsw %%ax;test $0x44, %%ah;jnp %0")

TEMPLATE(RISCV_JNEI4,    "cmpl %2, %1;jne %0")
TEMPLATE(RISCV_JNEU4,    "cmpl %2, %1;jne %0")
TEMPLATE(RISCV_JNEF4,    "flds %2;fucompp;fnstsw %%ax;test $0x44, %%ah;jp %0")
TEMPLATE(RISCV_JNEF8,    "fldl %2;fucompp;fnstsw %%ax;test $0x44, %%ah;jp %0")



TEMPLATE(RISCV_JGI4,     "cmpl %2, %1;jg %0")
TEMPLATE(RISCV_JGU4,     "cmpl %2, %1;ja %0")
TEMPLATE(RISCV_JGF4,     "flds %2;fucompp;fnstsw %%ax;test $0x1, %%ah;jne %0")
TEMPLATE(RISCV_JGF8,     "fldl %2;fucompp;fnstsw %%ax;test $0x1, %%ah;jne %0")

TEMPLATE(RISCV_JLI4,     "cmpl %2, %1;jl %0")
TEMPLATE(RISCV_JLU4,     "cmpl %2, %1;jb %0")
TEMPLATE(RISCV_JLF4,     "flds %2;fucompp;fnstsw %%ax;test $0x41, %%ah;jp %0")
TEMPLATE(RISCV_JLF8,     "fldl %2;fucompp;fnstsw %%ax;test $0x41, %%ah;jp %0")

TEMPLATE(RISCV_JGEI4,    "cmpl %2, %1;jge %0")
TEMPLATE(RISCV_JGEU4,    "cmpl %2, %1;jae %0")
TEMPLATE(RISCV_JGEF4,    "flds %2;fucompp;fnstsw %%ax;test $0x41, %%ah;jne %0")
TEMPLATE(RISCV_JGEF8,    "fldl %2;fucompp;fnstsw %%ax;test $0x41, %%ah;jne %0")

TEMPLATE(RISCV_JLEI4,    "cmpl %2, %1;jle %0")
TEMPLATE(RISCV_JLEU4,    "cmpl %2, %1;jbe %0")
TEMPLATE(RISCV_JLEF4,    "flds %2;fucompp;fnstsw %%ax;test $0x5, %%ah;jp %0")
TEMPLATE(RISCV_JLEF8,    "fldl %2;fucompp;fnstsw %%ax;test $0x5, %%ah;jp %0")

TEMPLATE(RISCV_PROLOGUE,   "addi sp,sp,-16\nsd s0,8(sp)\nsd ra,0(sp)\n")
TEMPLATE(RISCV_EXPANDF,    "addi s0,sp,0")
TEMPLATE(RISCV_EPILOGUE,   "ld s0,8(sp)\nld ra,0(sp)\naddi sp,sp,16\njr ra")
TEMPLATE(RISCV_MOVI4,		   "mv %0,%1")
TEMPLATE(RISCV_MOVI4TOARG, "mv a0,%1")
TEMPLATE(RISCV_CALL, "call %1@plt")
TEMPLATE(RISCV_ICALL, "call *%1@plt")
TEMPLATE(RISCV_ADDR, "lla %0,%1")
TEMPLATE(RISCV_LI, "li a5,%1\nsw a5,%0")
TEMPLATE(RISCV_LD, "lw %0,%1")

