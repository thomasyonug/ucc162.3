#include "ucl.h"
#include "ast.h"
#include "expr.h"
#include "gen.h"
#include "reg.h"
#include "target.h"
#include "output.h"

#include <assert.h>


enum ASMCode {
#define TEMPLATE(code, str) code,
#include "riscvlinux.tpl"
#undef TEMPLATE
};





#define ASM_CODE(opcode, tcode) ((opcode << 2) + tcode - I4)
#define DST inst->opds[0]
#define SRC1 inst->opds[1]
#define SRC2 inst->opds[2]
#define IsNormalRecord(rty) (rty->size != 1 && rty->size != 2 && rty->size != 4 && rty->size != 8)


#define PRESERVE_REGS 4
#define SCRATCH_REGS 4
#define STACK_ALIGN_SIZE 4








static void EmitAssign(IRInst inst) {
	assert("EmitAssign function" && 0);
}
static void EmitBranch(IRInst inst) {
	assert("EmitBranch function" && 0);
}
static void EmitJump(IRInst inst) {
	assert("EmitJump function" && 0);
}
static void EmitIndirectJump(IRInst inst) {
	assert("EmitIndirectJump function" && 0);
}
static void EmitInc(IRInst inst) {
	assert("EmitInc function" && 0);
}
static void EmitDec(IRInst inst) {
	assert("EmitDec function" && 0);

}
static void EmitAddress(IRInst inst) {
	assert("EmitAddress function" && 0);

}
static void EmitDeref(IRInst inst) {
	assert("EmitDeref function" && 0);

}
static void EmitCast(IRInst inst) {
	assert("EmitCast function" && 0);

}
static void EmitMove(IRInst inst) {
	assert("EmitMove function" && 0);

}
static void EmitIndirectMove(IRInst inst) {
	assert("EmitIndirectMove function" && 0);

}
static void EmitCall(IRInst inst) {
	assert("EmitCall function" && 0);

}
static void EmitReturn(IRInst inst) {
	assert("EmitReturn function" && 0);

}
static void EmitClear(IRInst inst) {
	assert("EmitClear function" && 0);

}
static void EmitNOP(IRInst inst) {
	assert("EmitNop function" && 0);
}




static void (*Emitter[])(IRInst inst) = {
#define OPCODE(code, name, func) Emit##func,
#include "opcode.h"
#undef OPCODE
};





static void EmitIRInst(IRInst inst) {
	struct irinst instc = *inst;
	(* Emitter[inst->opcode])(&instc);
	return;
}


static void EmitPrologue(int  stksize) {
	PutASMCode(RISCV_PROLOGUE, NULL);
	if (stksize != 0) {
		Symbol sym = IntConstant(stksize);
		PutASMCode(RISCV_PROLOGUE, &sym);
	}
}

static void EmitEpilogue(int stksize) {
	PutASMCode(RISCV_EPILOGUE, NULL);
}
static void EmitBBlock(BBlock bb) {
	IRInst inst = bb->insth.next;
	while (inst != &bb->insth) {
		UsedRegs = 0;
//		if ( (inst->opcode >= JZ && inst->opcode <= IJMP) || (inst->opcde == CALL) ) {
//			SaveX87Top();
//		}
		EmitIRInst(inst);

		inst = inst->next;
		ClearRegs();

	}
}

static int LayoutFrame(FunctionSymbol fsym, int fstParamPos) {
	Symbol p;
	int offset;

	offset = fstParamPos * STACK_ALIGN_SIZE;
	p = fsym->params;
	while (p) {
		AsVar(p)->offset = offset;
		if (p->ty->size == 0) {
			offset += ALIGN(EMPTY_OBJECT_SIZE, STACK_ALIGN_SIZE);
		} else {
			offset += ALIGN(p->ty->size, STACK_ALIGN_SIZE);
		}
		p = p->next;
	}
	offset = 0;
	p = fsym->locals;
	while (p) {
		if (p->ref == 0) 
			goto next;
		if (p->ty->size == 0) {
			offset += ALIGN(EMPTY_OBJECT_SIZE, STACK_ALIGN_SIZE);
		} else {
			offset += ALIGN(p->ty->size, STACK_ALIGN_SIZE);
		}
		AsVar(p)->offset = -offset;
next:
		p = p->next;
	}
	return offset;
}

void EmitFunction(FunctionSymbol fsym) {
	BBlock bb;
	Type rty;
	int stksize;
	FSYM = fsym;
	if (fsym->sclass != TK_STATIC) {
		Export((Symbol)fsym);
	}
	DefineLabel((Symbol)fsym);
	
	rty = fsym->ty->bty;

	if (IsRecordType(rty) && IsNormalRecord(rty)) {
		VariableSymbol p;

		CALLOC(p);

		p->kind = SK_Variable;
		p->name = "recvaddr";
		p->ty = T(POINTER);
		p->level = 1;
		p->sclass = TK_AUTO;
	
		p->next = fsym->params;
		fsym->params = (Symbol)p;
	}
	stksize = LayoutFrame(fsym, PRESERVE_REGS + 1);

	EmitPrologue(stksize);

	bb = fsym->entryBB;
	while (bb != NULL) {
		DefineLabel(bb->sym);
		EmitBBlock(bb);
		bb = bb->next;
	}

	EmitEpilogue(stksize);
	PutString("\n");
}

void StoreVar(Symbol reg, Symbol v) {
	assert("StoreVar" && 0);
}




