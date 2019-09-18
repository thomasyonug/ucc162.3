#ifndef __REG_H_
#define __REG_H_

enum {
	ZERO, RA, SP, GP, TP, T0, T1, T2, FP, S1, A0, A1,
	A2, A3, A4, A5, A6, A7, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11,
	T3, T4, T5, T6
}

#define SK_IRegister (SK_Register + 1)

#define NO_REG -1

void CreateReg(char *name, char *iname, int no);

extern Symbol riscvRegs[];



#endif
