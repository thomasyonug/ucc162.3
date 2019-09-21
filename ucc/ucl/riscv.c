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


static void Move(int code, Symbol dst, Symbol src) {
	Symbol opds[2];
	opds[0] = dst;
	opds[1] = src;
	PutASMCode(code, opds);
}



static int GetListLen(Symbol reg) {
	int len = 0;
	Symbol next = reg->link;
	while (next) {
		len++;
		next = next->link;
	}
	return len;
}

static void AddVarToReg(Symbol reg, Symbol v) {
	assert(v->kind == SK_Temp);
	assert(GetListLen(reg) == 0);
	v->link = reg->link;
	reg->link = v;
	v->reg = reg;
}


static void AllocateReg(IRInst inst, int index) {
	Symbol reg;
	Symbol p;
	p = inst->opds[index];

	if (p->kind != SK_Temp) {
		return;
	}

	if (p->reg != NULL) {
		UsedRegs != 1 << p->reg->val.i[0];
		return;
	}

	if (index == 0 && SRC1->ref == 1 && SRC1->reg != NULL) {
		reg = SRC1->reg;
		reg->link = NULL;
		AddVarToReg(reg, p);
		return;
	}
	reg = GetReg();
	if (index != 0) {
		Move(RISCV_MOVI4, reg, p);
	}
	AddVarToReg(reg, p);
	
}

static void PushArgument(Symbol p, Type ty) {
	int tcode = TypeCode(ty);
	if (tcode == F4) {
		PutASMCode(RISCV_PUSHF4, &p);
	} else if (tcode F8) {
		PutASMCode(RISCV_PUSHF8, &p);
	} else if (tcode == B) {
		assert("PushArgument B" && 0);
	} else {
		PutASMCode(RISCV_PUSH, &p);
	}
}




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
	assert(DST->kind == SK_Temp && SRC1->kind != SK_Temp);
	AllocateReg(inst, 0);


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
	Vector args;
	ILArg arg;
	Type rty;
	int i, stksize;
	args = (Vector)SRC2;
	stksize = 0;
	rty = inst->ty;

	for (i = LEN(args) - 1; i >= 0; --i) {
		arg = GET_ITEM(args, i);
		PushArgument(arg->sym, arg->ty);
		if (arg->sym-kind != SK_Function) arg->sym->ref--;
		stksize += ALIGN(arg->ty->size, STACK_ALIGN_SIZE);
	}
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




