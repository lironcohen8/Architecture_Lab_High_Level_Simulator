/*
 * ISS simulator implementation
 * Liron Cohen 207481268
 * Yuval Mor 209011543
 */

#include <stdio.h>
#include <stdlib.h>

#define ADD 0
#define SUB 1
#define LSF 2
#define RSF 3
#define AND 4
#define OR  5
#define XOR 6
#define LHI 7
#define LD 8
#define ST 9
#define JLT 16
#define JLE 17
#define JEQ 18
#define JNE 19
#define JIN 20
#define HLT 24

#define NUM_OF_REGS = (8)
#define MEM_SIZE_BITS	(16)
#define MEM_SIZE	(1 << MEM_SIZE_BITS)
#define MEM_MASK	(MEM_SIZE - 1)
unsigned int mem[MEM_SIZE];
unsigned int num_of_cmds = 0;
unsigned int regs[NUM_OF_REGS];

#define OPCODE_MASK 0x3E000000
#define OPCODE_SHIFT 0x19
#define DST_MASK 0x01C00000
#define DST_SHIFT 0x16
#define SRC0_MASK 0x00380000
#define SRC0_SHIFT 0x13
#define SRC1_MASK 0x00070000
#define SRC1_SHIFT 0x10
#define IMM_MASK 0x0000FFFF

typedef struct {
    int8_t opcode;
    int4_t dst;
    int4_t src0;
    int4_t src1;
    int16_t imm;
    char* raw_cmd;
} command;

int pc = 0;

/* constructs the command from the input line */
static void parse_command(char* line, command* cmd) {
    cmd->opcode = line & 0xFFF;
    cmd->dst = (line >> OPCODE_SHIFT) & OPCODE_MASK;
    cmd->src0 = (line >> SRC0_SHIFT) & SRC0_MASK;
    cmd->src1 = (line >> SRC1_SHIFT) & SRC1_MASK;
    cmd->imm = (line) & IMM_MASK;
    cmd->raw_cmd = line;
}

/* executes the relevant command by opcode */
static void exec_command(command* cmd) {
	if (cmd->opcode == ADD) run_add_cmd(cmd);
	else if (cmd->opcode == SUB) run_sub_cmd(cmd);
	else if (cmd->opcode == LSF) run_lsf_cmd(cmd);
	else if (cmd->opcode == RSF) run_rsf_cmd(cmd);
	else if (cmd->opcode == AND) run_and_cmd(cmd);
	else if (cmd->opcode == OR) run_or_cmd(cmd);
	else if (cmd->opcode == XOR) run_xor_cmd(cmd);
	else if (cmd->opcode == LHI) run_lhi_cmd(cmd);
	else if (cmd->opcode == LD) run_ld_cmd(cmd);
	else if (cmd->opcode == ST) run_st_cmd(cmd);
	else if (cmd->opcode == JLT) run_jlt_cmd(cmd);
	else if (cmd->opcode == JLE) run_jle_cmd(cmd);
	else if (cmd->opcode == JEQ) run_jeq_cmd(cmd);
	else if (cmd->opcode == JNE) run_jne_cmd(cmd);
	else if (cmd->opcode == JIN) run_jin_cmd(cmd);
	else return; // halt
}

static void run_add_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_sub_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] - regs[cmd->src1];
}

static void run_lsf_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_rsf_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_and_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_or_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_xor_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_lhi_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_ld_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_st_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_jlt_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_jle_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_jeq_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_jne_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_jin_cmd(command* cmd) {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}


int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: asm program_name\n");
		return -1;
	} else {
		open_files();
		load_commands_from_file();
		for (int i = 0; i < num_of_cmds; i++) {
			parse_command();
			trace_dump();
			exec_command();
			trace_command();
		}
		close_files();
		free_memory();
		return 0;
	}
}
