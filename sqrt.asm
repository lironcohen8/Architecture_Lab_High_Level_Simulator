asm_cmd(LD, 2, 0, 1000, 0) // R2 = MEM[1000] = INPUT
asm_cmd(ADD, 3, 0, 0, 0) // R3 = 0 = SQRT
asm_cmd(JEQ, 0, 0, 0, 7) // JUMP TO MUL START
asm_cmd(JLT, 0, 2, 5, 13) // JUMP IF  INPUT < MUL_RES //CHECK MUL_RES
asm_cmd(ADD, 3, 3, 0, 1) // R3 = R3 + 1
asm_cmd(JEQ, 0, 0, 0, 7) // JUMP TO MUL START
asm_cmd(ADD, 4, 0, 0, 0) // R4 = 0 = COUNTER //MUL START
asm_cmd(ADD, 5, 0, 0, 0) // R5 = 0 = MUL_RES
asm_cmd(JLE, 0, 3, 4, 4) // IF SQRT <= COUNTER  JUMP TO CHECK MUL_RES //COUNTER CONDITION
asm_cmd(ADD, 5, 5, 3, 0) // R5 = R5 + SQRT
asm_cmd(ADD, 4, 4, 0, 1) // R4 ++
asm_cmd(JEQ, 0, 0, 0, 9) // JUMP TO COUNTER CONDITION
asm_cmd(SUB, 3, 3, 0, 1) // R3-- //END
asm_cmd(ST, 0, 3, 1001, 0) // STORE RESULT
asm_cmd(HLT, 0, 0, 0, 0) // HALT
