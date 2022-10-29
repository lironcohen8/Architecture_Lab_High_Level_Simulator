/*
 * ISS simulator implementation
 * Liron Cohen 207481268
 * Yuval Mor 209011543
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

#define MEM_SIZE_BITS	(16)
#define MEM_SIZE	(1 << MEM_SIZE_BITS)
#define MEM_MASK	(MEM_SIZE - 1)
unsigned int mem[MEM_SIZE];

typedef struct {
    unsigned int opcode;
    unsigned int dst;
    unsigned int src0;
    unsigned int src1;
    signed int imm;
    unsigned int raw_cmd;
} command;

static unsigned int program_length = 0;
static unsigned int inst_cnt = 0;
static bool is_halt = false;
static bool is_jump_taken = false;
static signed int regs[8];
static command *cmd;
static FILE *trace_file;
static FILE *sram_out_file;
static FILE *input_file;
static char *input_file_name;

#define OPCODE_MASK 0x3E000000
#define OPCODE_SHIFT 0x19
#define DST_MASK 0x01C00000
#define DST_SHIFT 0x16
#define SRC0_MASK 0x00380000
#define SRC0_SHIFT 0x13
#define SRC1_MASK 0x00070000
#define SRC1_SHIFT 0x10
#define IMM_MASK 0x0000FFFF
#define SIGN_EXT_MASK 0x00008000

int pc = 0;
int next_pc = 0;

/* opens input_and_output files */
static void open_input_and_output_files() {
	input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        printf("error opening file %s\n", input_file_name);
    }

	trace_file = fopen("trace.txt", "w");
	if (trace_file == NULL) {
        printf("error opening trace file\n");
    }

	sram_out_file = fopen("sram_out.txt", "w");
	if (sram_out_file == NULL) {
        printf("error opening sram out file\n");
    }
}

/* loads memory from input file */
static void load_memory_from_input_file() {
	char line_buffer[8];

	while (fgets(line_buffer, 8 + 2, input_file) != NULL) {
        sscanf_s(line_buffer, "%x", &mem[program_length++]);
    }
}

/* writes the first line of the trace file */
static void trace_first_line() {
	fprintf(trace_file, "program %s loaded, %d lines\n\n", input_file_name, program_length);
}

/* allocates memory for command */
static void allocate_memory_for_command() {
	cmd = (command*)malloc(sizeof(command*));
}

/* sign extention the imm */
static int sign_ext_imm(int imm) {
	// if we need to extend with 1s
	if (SIGN_EXT_MASK & imm) {
        return imm + 0xFFFF0000;
    }
	return imm;
}

/* constructs the command from the input line */
static void parse_command() {
    cmd->opcode = (mem[pc] & OPCODE_MASK) >> OPCODE_SHIFT;
    cmd->dst = (mem[pc] & DST_MASK) >> DST_SHIFT;
    cmd->src0 = (mem[pc] & SRC0_MASK) >> SRC0_SHIFT;
    cmd->src1 = (mem[pc] & SRC1_MASK) >> SRC1_SHIFT;
    cmd->imm = (mem[pc]) & IMM_MASK;
    cmd->raw_cmd = mem[pc];

	regs[1] = sign_ext_imm(cmd->imm);
}

static void run_add_cmd() {
	regs[cmd->dst] = regs[cmd->src0] + regs[cmd->src1];
}

static void run_sub_cmd() {
	regs[cmd->dst] = regs[cmd->src0] - regs[cmd->src1];
}

static void run_lsf_cmd() {
	regs[cmd->dst] = regs[cmd->src0] << abs(regs[cmd->src1]);
}

static void run_rsf_cmd() {
	regs[cmd->dst] = regs[cmd->src0] >> abs(regs[cmd->src1]);
}

static void run_and_cmd() {
	regs[cmd->dst] = regs[cmd->src0] & regs[cmd->src1];
}

static void run_or_cmd() {
	regs[cmd->dst] = regs[cmd->src0] | regs[cmd->src1];
}

static void run_xor_cmd() {
	regs[cmd->dst] = regs[cmd->src0] ^ regs[cmd->src1];
}

static void run_lhi_cmd() {
	regs[cmd->dst] = (cmd->imm) << 16;
}

static void run_ld_cmd() {
	regs[cmd->dst] = mem[regs[cmd->src1]];
}

static void run_st_cmd() {
	mem[regs[cmd->src1]] = regs[cmd->src0];
}

static void run_jlt_cmd() {
	if (regs[cmd->src0] < regs[cmd->src1]) {
		next_pc = cmd->imm;
		is_jump_taken = true;
	}
	else {
		next_pc = pc + 1;
	}
}

static void run_jle_cmd() {
	if (regs[cmd->src0] <= regs[cmd->src1]) {
		next_pc = cmd->imm;
		is_jump_taken = true;
	}
	else {
		next_pc = pc + 1;
	}
}

static void run_jeq_cmd() {
	if (regs[cmd->src0] == regs[cmd->src1]) {
		next_pc = cmd->imm;
		is_jump_taken = true;
	}
	else {
		next_pc = pc + 1;
	}
}

static void run_jne_cmd() {
	if (regs[cmd->src0] != regs[cmd->src1]) {
		next_pc = cmd->imm;
		is_jump_taken = true;
	}
	else {
		next_pc = pc + 1;
	}
}

static void run_jin_cmd() {
	next_pc = regs[cmd->src0];
}

/* executes the relevant command by opcode */
static void exec_command() {
	if (cmd->opcode == ADD) run_add_cmd();
	else if (cmd->opcode == SUB) run_sub_cmd();
	else if (cmd->opcode == LSF) run_lsf_cmd();
	else if (cmd->opcode == RSF) run_rsf_cmd();
	else if (cmd->opcode == AND) run_and_cmd();
	else if (cmd->opcode == OR) run_or_cmd();
	else if (cmd->opcode == XOR) run_xor_cmd();
	else if (cmd->opcode == LHI) run_lhi_cmd();
	else if (cmd->opcode == LD) run_ld_cmd();
	else if (cmd->opcode == ST) run_st_cmd();
	else if (cmd->opcode == JLT) run_jlt_cmd();
	else if (cmd->opcode == JLE) run_jle_cmd();
	else if (cmd->opcode == JEQ) run_jeq_cmd();
	else if (cmd->opcode == JNE) run_jne_cmd();
	else if (cmd->opcode == JIN) run_jin_cmd();
	else is_halt = true; // halt
}

/* returns true if a command is a jump or halt command and false otherwise */
static bool is_jump_or_halt_cmd() {
	return (cmd->opcode == JLT) |  
		   (cmd->opcode == JLE) | 
		   (cmd->opcode == JEQ) | 
		   (cmd->opcode == JNE) | 
		   (cmd->opcode == JIN) |
		   (cmd->opcode == HLT);
}

/* update reg 7 to next pc if branch is taken */
static void update_reg_7() {
	regs[7] = pc;
}

/* returns string representation of current command's opcode */
static char* get_curr_opcode_str() {
	int opcode = cmd->opcode;
	if      (opcode == ADD) return "ADD";
    else if (opcode == SUB) return "SUB";
    else if (opcode == LSF) return "LSF";
    else if (opcode == RSF) return "RSF";
    else if (opcode == AND) return "AND";
    else if (opcode == OR)  return "OR";
    else if (opcode == XOR) return "XOR";
    else if (opcode == LHI) return "LHI";
    else if (opcode == LD)  return "LD";
    else if (opcode == ST)  return "ST";
    else if (opcode == JLT) return "JLT";
    else if (opcode == JLE) return "JLE";
    else if (opcode == JEQ) return "JEQ";
    else if (opcode == JNE) return "JNE";
    else if (opcode == JIN) return "JIN";
    else if (opcode == HLT) return "HLT";
    else                    return "";
}

/* traces command to trace file */
static void trace_command() {
	fprintf(trace_file, "--- instruction %i (%04x) @ PC %d (%04x) -----------------------------------------------------------\n", inst_cnt, inst_cnt, pc, pc);
    fprintf(trace_file, "pc = %04d, inst = %08x, opcode = %d (%s), ", pc, cmd->raw_cmd, cmd->opcode, get_curr_opcode_str());
    fprintf(trace_file, "dst = %d, src0 = %d, src1 = %d, immediate = %08x\n", cmd->dst, cmd->src0, cmd->src1, cmd->imm);
    fprintf(trace_file, "r[0] = %08x r[1] = %08x r[2] = %08x r[3] = %08x \nr[4] = %08x r[5] = %08x r[6] = %08x r[7] = %08x \n\n", regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6], regs[7]);

    if ((cmd->opcode == ADD) || (cmd->opcode == SUB) || (cmd->opcode == LSF) || (cmd->opcode == RSF) || (cmd->opcode == AND) || (cmd->opcode == OR) || (cmd->opcode == XOR) || (cmd->opcode == LHI)) {
        fprintf(trace_file, ">>>> EXEC: R[%d] = %d %s %d <<<<\n\n", cmd->dst, regs[cmd->src0], get_curr_opcode_str(), regs[cmd->src1]);
    }
    else if (cmd->opcode == LD) {
        fprintf(trace_file, ">>>> EXEC: R[%d] = MEM[%d] = %08x <<<<\n\n", cmd->dst, regs[cmd->src1], mem[regs[cmd->src1]]);
    }
    else if (cmd->opcode == ST) {
        fprintf(trace_file, ">>>> EXEC: MEM[%d] = R[%d] = %08x <<<<\n\n", regs[cmd->src1], cmd->src0, regs[cmd->src0]);
    }
	else if ((cmd->opcode == JLT) || (cmd->opcode == JLE) || (cmd->opcode == JEQ) || (cmd->opcode == JNE) || (cmd->opcode == JIN)) {
		fprintf(trace_file, ">>>> EXEC: %s %d, %d, %d <<<<\n\n", get_curr_opcode_str(), regs[cmd->src0], regs[cmd->src1],  next_pc);
	}
    else if (cmd->opcode == HLT) {
        fprintf(trace_file, ">>>> EXEC: HALT at PC %04x<<<<\n", pc);
    }
}

/* updates PC  and increments instructions count */
static void update_pc_and_inst_cnt() {
	if (!is_jump_or_halt_cmd()) {
		pc++;
	}
	else {
		pc = next_pc;
	}
	if (pc > 0xffff) {
		pc = 0;
	}
	inst_cnt++;
}

/* writes the last line of the trace file */
static void trace_last_line() {
	fprintf(trace_file, "sim finished at pc %d, %d instructions", pc, inst_cnt);
}

/* writes memory into sram_out file */
static void dump_sram_to_file() {
    for (int i = 0; i < MEM_SIZE; i++) {
        fprintf(sram_out_file,"%08x\n",mem[i]);
    }
}

/* closes the input and output files */
static void close_files() {
	fclose(input_file);
	fclose(trace_file);
	fclose(sram_out_file);
}

/* free memory */
static void free_memory() {
	free(cmd);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("number of args should be one\n");
		return -1;
	}
	else {
		input_file_name = argv[1];
		open_input_and_output_files();
		load_memory_from_input_file();
		trace_first_line();
		allocate_memory_for_command();
		while (!is_halt) {
			parse_command();
			if (is_jump_or_halt_cmd()) {
				exec_command();
				trace_command();
				if (is_jump_taken) {
					update_reg_7();
					is_jump_taken = false;
				}
			}
			else {
				trace_command();
				exec_command();
			}
			update_pc_and_inst_cnt();
		}
		trace_last_line();
		dump_sram_to_file();
		close_files();
		free_memory();
		return 0;
	}
}
