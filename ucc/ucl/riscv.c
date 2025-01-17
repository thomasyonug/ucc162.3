#include "ucl.h"
#include "ast.h"
#include "expr.h"
#include "gen.h"
#include "riscv_reg.h"
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


#define PRESERVE_REGS 2
#define SCRATCH_REGS 4
#define STACK_ALIGN_SIZE 4


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





static void ModifyVar(Symbol p) {
	Symbol reg;
	if (p->reg == NULL) {
		return;
	}
	p->needwb = 0;
	reg = p->reg;
	assert(GetListLen(reg) == 1);
	assert(p == reg->link);
	SpillReg(reg);
	AddVarToReg(reg, p);
	p->needwb = 1;
}


static void Move(int code, Symbol dst, Symbol src) {
	Symbol opds[2];
	opds[0] = dst;
	opds[1] = src;
	PutASMCode(code, opds);
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
		assert("PushArgument F4" && 0);
		//PutASMCode(RISCV_PUSHF4, &p);
	} else if (tcode == F8) {
		//PutASMCode(RISCV_PUSHF8, &p);
		assert("PushArgument F8" && 0);
	} else if (tcode == B) {
		assert("PushArgument B" && 0);
	} else {
		//PutASMCode(RISCV_PUSH, &p);
		PutASMCode(RISCV_MOVI4TOARG, &p);
	}
}




static void EmitAssign(IRInst inst) {
	int code;
	int tcode = TypeCode(inst->ty);
	assert(DST->kind == SK_Temp);
	assert(tcode == I4 || tcode == U4);

	code = ASM_CODE(inst->opcode, tcode);
	switch (code) {
		case RISCV_DIVI4:
		case RISCV_DIVU4:
		case RISCV_MODI4:
		case RISCV_MODU4:
		case RISCV_MULU4:
			assert("EmitAssign error" && 0);
		break;

		case RISCV_LSHI4:
		case RISCV_LSHU4:
		case RISCV_RSHI4:
		case RISCV_RSHU4:
			assert("EmitAssign error1" && 0);
		break;

		case RISCV_NEGI4:
		case RISCV_NEGU4:
			assert("EmitAssign error2" && 0);
		break;

		default:
			AllocateReg(inst, 1);
			AllocateReg(inst, 2);

put_code:
			AllocateReg(inst, 0);
			assert(DST->reg != NULL);
			if (DST->reg != SRC1->reg) {
				Move(RISCV_MOVI4, DST, SRC1);
			}
			PutASMCode(code, inst->opds);
			break;
	}
	ModifyVar(DST);
	
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
	PutASMCode(RISCV_ADDR, inst->opds);
	//ModifyVar(DST);
}

static void EmitDeref(IRInst inst) {
	assert("EmitDeref function" && 0);

}
static void EmitCast(IRInst inst) {
	assert("EmitCast function" && 0);

}

static void EmitMoveBlock(IRInst inst) {
	assert("EmitMoveBlock eror" && 0);
}

static void EmitMove(IRInst inst) {
	int tcode = TypeCode(inst->ty);
	Symbol reg;
	if (tcode == B) {
		EmitMoveBlock(inst);
		return;
	}
	switch (tcode) {
		case I1: case U1:
			assert("EmitMove switch1" && 0);
		break;
		case I2: case U2:
			assert("EmitMove switch2" && 0);
		break;
		case I4: case U4:
			if (SRC1->kind == SK_Constant) {
				Move(RISCV_LI, DST, SRC1);
			} else {
				AllocateReg(inst, 1);
				AllocateReg(inst, 0);
				if (SRC1->reg == NULL && DST->reg == NULL) {
					reg = GetReg();
					Move(RISCV_MOVI4, reg, SRC1);
					Move(RISCV_MOVI4, DST, reg);
				} else {
					Move(RISCV_MOVI4, DST, SRC1);
				}
			}
			ModifyVar(DST);
		break;
		default:
			assert(0);
	}
}

static void EmitIndirectMove(IRInst inst) {
	assert("EmitIndirectMove function" && 0);

}
static void EmitCall(IRInst inst) {
	Vector args;
	ILArg arg;
	Type rty;
	int stksize;
	args = (Vector)SRC2;
	stksize = 0;
	rty = inst->ty;
	int argLen = LEN(args);
	
	if (argLen > 1) {
		assert("arglen too long" && 0);
	}

//	SpillReg(riscvRegs[A0]);
//	for (int i = argLen - 1; i >= 0; --i) {
//		arg = GET_ITEM(args, i);
//		PushArgument(arg->sym, arg->ty);
//		if (arg->sym->kind != SK_Function) arg->sym->ref--;
//		stksize += ALIGN(arg->ty->size, STACK_ALIGN_SIZE);
//	}

	if (IsRecordType(rty) && IsNormalRecord(rty)) {
		assert("IsRecordType IsNormalRecord" && 0);
		//Symbol opds[2];
		//opds[0] = GetReg();
		//opds[1] = DST;
		//PutASMCode(RISCV_ADDR, opds);
		//PutASMCode(RISCV_PUSH, opds);
		//stksize += 4;
		//DST = NULL;
	}
	PutASMCode(SRC1->kind == SK_Function ? RISCV_CALL : RISCV_ICALL, inst->opds);
	if (stksize != 0) {
		Symbol p;
		p = IntConstant(stksize);
		//PutASMCode(RISCV_REDUCEF, &p);
	}
	if (DST != NULL) {
		DST->ref--;
	}
	if (SRC1->kind != SK_Function) SRC1->ref--;

	if (DST == NULL) {
		return;
	}
	assert("emit call error" && 0);
}
static void EmitReturn(IRInst inst) {
	Type ty = inst->ty;

	if (IsRecordType(ty) && IsNormalRecord(ty)) {
		//inst->opcode = IMOV;
		//SRC1 = DST;
		//DST = FSYM->params;
		//EmitIndirectMove(inst);
		//return;
	}
	switch (ty->size) {
		case 1:
		case 2:
			assert("EmitReturn error" && 0);
		break;
		case 4:
			if (DST->reg != riscvRegs[A0]) {
				Move(RISCV_LD, riscvRegs[A0], DST);
			}
		break;
		case 8:
			assert("EmitReturn error2" && 0);
		break;
		default:
			assert(0);
	}

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
		PutASMCode(RISCV_EXPANDF, &sym);
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
	Move(RISCV_MOVI4, v, reg);
}




