#include "ucl.h"
#include "ast.h"
#include "expr.h"
#include "gen.h"
#include "reg.h"
#include "target.h"
#include "output.h"



static const char * asmCodeName[] = {
#define TEMPLATE(code, str) #code,
#undef TEMPLATE
}




#define ASM_CODE(opcode, tcode) ((opcode << 2) + tcode - I4)
#define DST inst->opds[0]
#define SRC1 inst->opds[1]
#define SRC2 inst->opds[2]
#define IsNormalRecord(rty) (rty->size != 1 && rty->size != 2 && rty->size != 4 && rty->size != 8)


#define PRESERVE_REGS 4
#define SCRATCH_REGS 4
#define STACK_ALIGN_SIZE 4

};







static void (*Emitter[])(IRInst inst) = {
#define OPCODE(code, name, func) Emit##func,
#include "opcode.h"
#undef OPCODE
}




static void EmitIRInst(IRInst inst) {
	struct irinst instc = *inst;
	(* Emitter[inst->opcode])(&instc);
	return;
}


static void EmitPrologue(int  stksize) {
	PutASMCode(RISCV_PROLOGUE, NULL);
}
