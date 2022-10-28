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
unsigned int program_length = 0;
unsigned int inst_cnt = 0;
bool is_halt = false;
signed int regs[NUM_OF_REGS];
command *cmd;

#define TRACE_FILE_NAME = "trace.txt"
#define SRAM_OUT_FILE_NAME = "sram_out.txt"
File *trace_file;
File *sram_out_file;
File *input_file;
char *input_file_name;

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

typedef struct {
    int8_t opcode;
    int4_t dst;
    int4_t src0;
    int4_t src1;
    int16_t imm;
    char* raw_cmd;
} command;

int pc = 0;

/* opens input_and_output files */
static void open_input_and_output_files() {
	fopen_s(&input_file, input_file_name, "r");
    if (input_file == NULL) {
        printf("error opening file %s\n", file_name);
    }

	fopen_s(&trace_file, TRACE_FILE_NAME, "w");
	if (trace_file == NULL) {
        printf("error opening trace file\n");
    }

	fopen_s(&sram_out_file, SRAM_OUT_FILE_NAME, "w");
	if (sram_file == NULL) {
        printf("error opening sram out file\n");
    }
}

/* loads memory from input file */
static void load_memory_from_input_file() {
	char line_buffer[8];

	while ((read = getline(&line_buffer, 8, file)) != -1) {
    	mem[program_length++] = line_buffer;
	}
}

/* writes the first lineof the trace file */
static void trace_first_line() {
	fprintf(trace_file, "program %s loaded, %d lines\n\n", input_file_name, program_length);
}

/* constructs the command from the input line */
static void parse_command() {
	char *line[9];
	sprintf(line, "%d", mem[pc]);

    cmd->opcode = line & 0xFFF;
    cmd->dst = (line >> OPCODE_SHIFT) & OPCODE_MASK;
    cmd->src0 = (line >> SRC0_SHIFT) & SRC0_MASK;
    cmd->src1 = (line >> SRC1_SHIFT) & SRC1_MASK;
    cmd->imm = (line) & IMM_MASK;
    cmd->raw_cmd = line;

	regs[1] = sign_ext_imm(imm);
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
		regs[7] = pc;
		pc = cmd->imm;
	}
}

static void run_jle_cmd() {
	if (regs[cmd->src0] <= regs[cmd->src1]) {
	regs[7] = pc;
	pc = cmd->imm;
	}
}

static void run_jeq_cmd() {
	if (regs[cmd->src0] == regs[cmd->src1]) {
		regs[7] = pc;
		pc = cmd->imm;
	}
}

static void run_jne_cmd() {
	if (regs[cmd->src0] != regs[cmd->src1]) {
		regs[7] = pc;
		pc = cmd->imm;
	}
}

static void run_jin_cmd() {
	regs[7] = pc;
	pc = cmd->regs[cmd->src0];
}

/* returns true if a command is a jump command and false otherwise */
static bool is_jump_cmd() {
	return cmd->opcode == JLT |  
		   cmd->opcode == JLE | 
		   cmd->opcode == JEQ | 
		   cmd->opcode == JNE | 
		   cmd->opcode == JIN
}

/* sign extention the imm */
static int sign_ext_imm(int imm) {
	// if we need to extend with 1s
	if (SIGN_EXT_MASK & imm) {
        return imm + 0xFFFF0000;
    }
	return imm;
}

/* traces command to trace file */
static void trace_command() {
	fprintf(trace_file, "--- instruction %i (%04x) @ PC %d (%04x) -----------------------------------------------------------\n", inst_cnt, inst_cnt, pc, pc);
    fprintf(trace_file, "pc = %04d, inst = %08x, opcode = %d (%s), ", pc, cmd->raw_cmd, cmd->opcode, get_opcode_str());
    fprintf(trace_file, "dst = %d, src0 = %d, src1 = %d, immediate = %08x\n", cmd->dst, cmd->src0, cmd->src1, cmd->imm);
    fprintf(trace_file, "r[0] = %08x r[1] = %08x r[2] = %08x r[3] = %08x \nr[4] = %08x r[5] = %08x r[6] = %08x r[7] = %08x \n\n", regs[0], regs[1], regs[2], regs[3], regs[4], regs[5], regs[6], regs[7]);

    if ((opcode == ADD) || (opcode == SUB) || (opcode == LSF) || (opcode == RSF) || (opcode == AND) || (opcode == OR) || (opcode == XOR) || (opcode == LHI)) {
        fprintf(trace_file, ">>>> EXEC: R[%d] = %d %s %d <<<<\n\n", cmd->dst, regs[src0], get_curr_opcode_str(), regs[src1]);
    }
    else if (opcode == LD) {
        fprintf(trace_file, ">>>> EXEC: R[%d] = MEM[%d] = %08x <<<<\n\n", cmd->dst, regs[cmd->src1], output_arr[regs[src1]]);
    }
    else if (opcode == ST) {
        fprintf(trace_file, ">>>> EXEC: MEM[%d] = R[%d] = %08x <<<<\n\n", regs[cmd->src1], cmd->drc0, regs[cmd->src0]);
    }
    else if (opcode == HLT) {
        fprintf(trace_file, ">>>> EXEC: HALT at PC %04x<<<<\n", pc);
    }
}

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

/* updates PC for non-jump commands */
static void update_pc() {
	if (is_jump_cmd() == 0) {
				pc += 1;
	}
	if (pc > 0xffff) {
		pc = 0;
	}
}

/* writes memory into sram_out file */
static void dump_sram_to_file() {
    for (int i = 0; i < MEM_SIZE; i++) {
        fprintf(sram_file,"%08X\n",mem[i]);
    }
}

/* closes the input and output files */
static void close_files() {
	fclose(input_file);
	fclose(trace_file);
	fclose(sram_out_file);
}

// TODO fill if needed
static void free_memory() {
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("number of args should be one\n");
		return -1;
	}
	else {
		open_input_and_output_files(args[1]);
		load_memory_from_input_file();
		trace_first_line();
		while (!is_halt) {
			parse_command();
			trace_command();
			exec_command();
			update_pc();
		}
		dump_sram_to_file();
		close_files();
		free_memory();
		return 0;
	}
}
