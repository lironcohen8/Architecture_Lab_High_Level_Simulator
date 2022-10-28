asm_cmd(LD, 2, 0, 1000, 0) // R2 = MEM[1000] = A
asm_cmd(LD, 3, 0, 1001, 0) // R3 = MEM[1001] = B
asm_cmd(ADD, 4, 0, 0, 1) // R4 = 1
asm_cmd(ADD, 5, 0, 0, 31) // R5 = 31
asm_cmd(LSF, 4, 4, 5, 0) // R4 = 1(0^31)
asm_cmd(SUB, 5, 4, 0, 1) // R5 = 0(1^31)
asm_cmd(AND, 6, 2, 4, 0) // R6 = R4&R2 = A SIGN
asm_cmd(AND, 4, 3, 4, 0) // R4 = R4&R3 = B SIGN
asm_cmd(AND, 2, 2, 5, 0) // R2 = R5&R2 = A MAGNITUDE
asm_cmd(AND, 3, 3, 5, 0) // R3 = R5&R3 = B MAGNITUDE
asm_cmd(JEQ, 0, 6, 4, 18) // JUMP TO ADD IF R6 = R4 SAME SIGN
asm_cmd(SUB, 5, 2, 3, 0) // R5 = |A| - |B|
asm_cmd(JLT, 0, 2, 3, 16) // JUMP IF  |A| < |B|
asm_cmd(OR, 5, 5, 6, 0) // R5 = A + B (WITH SIGN)
asm_cmd(JEQ, 0, 0, 0, 20) // JUMP TO FINISH
asm_cmd(OR, 5, 5, 4, 0) // R5 = A + B (WITH SIGN)
asm_cmd(JEQ, 0, 0, 0, 20) // JUMP TO FINISH
asm_cmd(ADD, 5, 2, 3, 0) // R5 = |A| + |B|
asm_cmd(OR, 5, 5, 6, 0) // R5 = A + B (WITH SIGN)
asm_cmd(ST, 0, 5, 1002, 0) // STORE RESULT
asm_cmd(HLT, 0, 0, 0, 0) // HALT
