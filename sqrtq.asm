asm_cmd(LD, 2, 0, 1, 1000); // 0: R2 = MEM[1000] = INPUT
asm_cmd(ADD, 3, 0, 0, 0); // 1: R3 = 0 = SQRT
asm_cmd(JEQ, 0, 0, 0, 5); // 2: JUMP TO MUL START
asm_cmd(JLT, 0, 2, 5, 11); // 3: JUMP TO END IF INPUT < MUL_RES // CHECK MUL_RES
asm_cmd(ADD, 3, 3, 1, 1); // 4: R3++
asm_cmd(ADD, 4, 0, 0, 0); // 5: R4 = 0 = COUNTER // MUL START
asm_cmd(ADD, 5, 0, 0, 0); // 6: R5 = 0 = MUL_RES
asm_cmd(JLE, 0, 3, 4, 3); // 7: IF SQRT <= COUNTER JUMP TO CHECK MUL_RES // COUNTER CONDITION
asm_cmd(ADD, 5, 5, 3, 0); // 8: R5 = R5 + SQRT
asm_cmd(ADD, 4, 4, 1, 1); // 9: R4++
asm_cmd(JEQ, 0, 0, 0, 7); // 10: JUMP TO COUNTER CONDITION // MUL END
asm_cmd(SUB, 3, 3, 1, 1); // 11: R3-- // END
asm_cmd(ST, 0, 3, 1, 1001); // 12: STORE RESULT
asm_cmd(HLT, 0, 0, 0, 0); // 13: HALT
