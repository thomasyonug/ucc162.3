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
