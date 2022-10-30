	asm_cmd(LD, 2, 0, 1, 1000); // 0: R2 = MEM[1000] = A
	asm_cmd(LD, 3, 0, 1, 1001); // 1: R3 = MEM[1001] = B
	asm_cmd(ADD, 4, 0, 1, 1); // 2: R4 = 1
	asm_cmd(ADD, 5, 0, 1, 31); // 3: R5 = 31
	asm_cmd(LSF, 4, 4, 5, 0); // 4: R4 = 1 (0**31);
	asm_cmd(SUB, 5, 4, 1, 1); // 5: R5 = 0 (1**31);
	asm_cmd(AND, 6, 2, 4, 0); // 6: R6 = R4&R2 = A SIGN
	asm_cmd(AND, 4, 3, 4, 0); // 7: R4 = R4&R3 = B SIGN
	asm_cmd(AND, 2, 2, 5, 0); // 8: R2 = R5&R2 = A MAGNITUDE
	asm_cmd(AND, 3, 3, 5, 0); // 9: R3 = R5&R3 = B MAGNITUDE
	asm_cmd(JEQ, 0, 6, 4, 18); // 10: JUMP TO 18 IF R6 = R4 SAME SIGN
	asm_cmd(JLT, 0, 2, 3, 15); // 11: JUMP TO 15 IF |A| < |B|
	asm_cmd(SUB, 5, 2, 3, 0); // 12: R5 = |A| - |B|
	asm_cmd(OR, 5, 5, 6, 0); // 13: R5 = A + B (WITH SIGN)
	asm_cmd(JEQ, 0, 0, 0, 20); // 14: JUMP TO STORE
	asm_cmd(SUB, 5, 3, 2, 0); // 15: R5 = |B| - |A|
	asm_cmd(OR, 5, 5, 4, 0); // 16: R5 = A + B (WITH SIGN)
	asm_cmd(JEQ, 0, 0, 0, 20); // 17: JUMP TO STORE
	asm_cmd(ADD, 5, 2, 3, 0); // 18: R5 = |A| + |B|
	asm_cmd(OR, 5, 5, 6, 0); // 19: R5 = A + B (WITH SIGN)
	asm_cmd(ST, 0, 5, 1, 1002); // 20: STORE RESULT
	asm_cmd(HLT, 0, 0, 0, 0); // 21: HALT