#include "ucl.h"
#include "gen.h"
#include "riscv_reg.h"
#include "target.h"
#include "output.h"

#include "ast.h"
#include "expr.h"
#include "input.h"

Symbol riscvRegs[T6 + 1];
Symbol riscvByteRegs[T6 + 1];
Symbol riscvWordRegs[T6 + 1];
int UsedRegs;

static int FindEmptyReg(int endr) {
	int i;
	for (i = A0; i <= endr; i++) {
		if (riscvRegs[i] != NULL && riscvRegs[i]->link == NULL && ! (1 << i & UsedRegs)) {
			return i;
		}
	}
	return NO_REG;
}

static int SelectSpillReg(int endr) {
	Symbol p;
	int i;
	int reg = NO_REG;
	int ref, mref = INT_MAX;
	for (i = ZERO; i <= T6; i++) {
		if (riscvRegs[i] == NULL || (1 << i & UsedRegs)) {
			continue;
		}
		p = riscvRegs[i]->link;
		ref = 0;
		while (p) {
			if (p->needwb && p->ref > 0) {
				ref += p->ref;
			}
			p = p->link;
		}
		if (ref < mref) {
			mref = ref;
			reg = i;
		}
	}
	assert(reg != NO_REG);
	return reg;
}

static Symbol GetRegInternal(int width) {
	int i, endr;
	Symbol *regs;
	switch (width) {
		case 1:
			assert("GetRegInternal switch 1" && 0);
		break;
		case 2:
			assert("GetRegInternal switch 2" && 0);
			break;
		case 4:
			endr = A5;
			regs = riscvRegs;
			break;
	}
	i = FindEmptyReg(endr);
	if (i == NO_REG) {
		i = SelectSpillReg(endr);
		SpillReg(riscvRegs[i]);
	}
	UsedRegs |= 1 << i;
	return regs[i];
}

Symbol GetReg(void) {
	return GetRegInternal(4);
}


Symbol CreateReg(char *name, char *iname, int no) {
	Symbol reg;
	CALLOC(reg);
	reg->kind = SK_Register;
	reg->name = reg->aname = name;
	reg->val.i[0] = no;
	reg->reg = reg;

	if (iname != NULL) {
		CALLOC(reg->next);
		reg->next->kind = SK_IRegister;
		reg->next->name = reg->next->aname = iname;	
	}
	return reg;
}


void ClearRegs() {
	int i;
	for (i = ZERO; i <= T6; i++) {
		if (riscvRegs[i]) {
			SpillReg(riscvRegs[i]);
		}
	}
}

void SpillReg(Symbol reg) {
	Symbol p;
	p = reg->link;
	while (p) {
		p->reg = NULL;
		if (p->needwb && p->ref > 0) {
			p->needwb = 0;
			StoreVar(reg, p);
		}
		p = p->link;
	}
	reg->link = NULL;
}
