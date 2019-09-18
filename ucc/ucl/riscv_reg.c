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

Symbol CreateReg(char *name, char *iname, int no) {
	Symbol reg;
	CALLOC(reg);
	reg->kind = SK_Register;
	reg->name = reg->aname = name;
	reg->val.i[0] = no;
	reg->reg = reg;

	if (iname != nul) {
		CALLOC(reg->next);
		reg->next->kind = SK_IRegister;
		reg->next->name = reg->next->aname = iname;	
	}
	return reg;
}

