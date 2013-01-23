/*
 * LC4.c
 */
#include <stdio.h>
#include "LC4.h"

 //quick methods
#define INSN_15_12(I) ((I >> 12) & 0xF)		//bits [15:12]
#define INSN_11(I) ((I >> 11) & 0x1)	  	//bit [11]
#define INSN_11_9(I) ((I >> 9) & 0x7)	 	//bits [11:9]
#define INSN_8(I) ((I >> 8) & 0x1)			//bit [8]
#define INSN_8_7(I) ((I >> 7) & 0x3)		//bits [8:7]
#define INSN_8_6(I) ((I >> 6) & 0x7)		//bits [8:6]
#define INSN_5(I) ((I >> 5) & 0x1)			//bit [5]
#define INSN_5_4(I) ((I >> 3) & 0x3)		//bits [5:4]
#define INSN_5_3(I) ((I >> 3) & 0x7)		//bits [5:3]
#define INSN_2_0(I) (I & 0x7)				//bits [2:0]
#define INSN_15(I) ((I >> 15) & 0x1)		//bit [15]

#define IMM11(I) ((I & 0x7FF) | ((I & 0x400) ? 0xF800 : 0)) 		// get the last 11 bits 
#define IMM9(I) ((I & 0x1FF) | ((I & 0x100) ? 0xFE00 : 0)) 			// get the last 9 bits 
#define IMM8(I) (I & 0xFF) 											// get the last 8 bits of the instruction
#define IMM7(I) ((I & 0x7F) | ((I & 0x40) ? 0xFF00 : 0)) 			// get the last 7 bits 
#define IMM6(I) ((I & 0x3F) | ((I & 0x20) ? 0xFFC0 : 0)) 			// get the last 6 bits 
#define IMM5(I) ((I & 0x1F) | ((I & 0x10) ? 0xFFE0 : 0)) 			// get the last 5 bits

#define SEXT_17(I) (I | ((I & 0x8000) ? 0xFFFF8000 : 0)) 			// sign-extend the instruction to 17 bits for overflow

/*
 *  ######################### GLOBAL VARIABLES THAT MODEL THE MACHINE  #########################
 */

// Global variable defining the current state of the machine
MachineState CurrentMachineState;

// Global variable containing the current state of the control signals
ControlSignals CurrentControlSignals;

/*
 *  ######################### ROUTINES FOR ACCESSING MACHINE STATE  #########################
 */

// Return a pointer to the current machine state struct
MachineState *GetCurrentMachineState () {
  return ( &CurrentMachineState );
}

// Return a pointer to the current control signals struct
ControlSignals *GetCurrentControlSignals () {
  return ( &CurrentControlSignals );
}

int mnemo;		//global int representing instruction

/*
 *  ######################### ROUTINES FOR MANIPULATING MACHINE STATE  #########################
 */

// Fill in control signals based on the current instruction - return a non-zero Exception code if an Exception is detected
int DecodeCurrentInstruction () {
		ControlSignals *signals= GetCurrentControlSignals();
		MachineState *state = GetCurrentMachineState();
		int instruction = state->memory[state->PC];
		unsigned short int opcode = INSN_15_12(instruction);
		unsigned short int subopcode = INSN_11_9(instruction);
		unsigned short int arith = INSN_5_3(instruction);
		unsigned short int comp = INSN_8_7(instruction);
		unsigned short int jesser = INSN_11(instruction);
		unsigned short int logic = INSN_5_3(instruction);
		unsigned short int shift = INSN_5_3(instruction);
		unsigned short int jumper = INSN_11(instruction);
		unsigned short int immcheck = INSN_5(instruction);
		unsigned short int hiconstcheck = INSN_8(instruction);
		switch(opcode){
			//branch opcode
			case 0:
				if(subopcode==0){
					mnemo=0;		//nop
				}
				if(subopcode==4){
					mnemo=1;		//brn	
				}
				if(subopcode==6){
					mnemo=2;		//brnz
				}
				if(subopcode==5){
					mnemo=3;		//brnp
				}
				if(subopcode==2){
					mnemo=4;		//brz
				}
				if(subopcode==3){
					mnemo=5;		//bzp
				}
				if(subopcode==1){
					mnemo=6;		//brp
				}
				if(subopcode==7){
					mnemo=7;		//brnzp
				}
			break;
			//arithmetic opcode
			case 1:
				if(arith==0){
					mnemo=10;		//add
				}else if(arith==1){
					mnemo=11;		//mul
				}else if(arith==2){
					mnemo=12;		//sub
				}else if(arith==3){
					mnemo=13;		//div
				}else{
					if(immcheck==0){
						printf("Exception: Instruction unrecognizable \n ");
						return 1;
					}else{
						mnemo=14;	//addim
					}
				}
			break;
			//cmp opcode
			case 2:
				if(comp==0){
					mnemo=20;		//cmp
				}
				if(comp==1){
					mnemo=21;		//cmpu
				}
				if(comp==2){
					mnemo=22;		//cmpi
				}
				if(comp==3){
					mnemo=23;		//cmpiu
				}
			break;
			//jsr opcode
			case 4:
				if(jesser==1){
					mnemo=30;		//jsr
				}else{
					mnemo=31;		//jsrr
				}
			break;
			//logic opcode
			case 5:
				if(logic==0){
					mnemo=40;		//and
				}else if(logic==1){
					mnemo=41;		//not
				}else if(logic==2){
					mnemo=42;		//or
				}else if(logic==3){
					mnemo=43;		//xor
				}else{
					if(immcheck==0){
						printf("Exception: Instruction unrecognizable \n");
						return 1;
					}else{
						mnemo=44;	//addim
					}
				}
			break;
			//load
			case 6:
				mnemo=50;			//ldr
			break;
			//store
			case 7:
				mnemo=51;			//str
			break;
			//rti
			case 8:
				mnemo=60;			//rti
			break;
			//const
			case 9:
				mnemo=70;			//const
			break;
			//shift
			case 10:
				if(shift==0){
					mnemo=80;		//sll
				}
				if(shift==1){
					mnemo=81;		//sra
				}
				if(shift==2){
					mnemo=82;		//srl
				}
				if(shift==3){
					mnemo=83;		//mod
				}
			break;
			//jmp
			case 12:
				if(jumper==1){
					mnemo=90;		//jmpr
				}else{
					mnemo=100;		//jmp
				}
			break;
			//hiconst
			case 13:
				if(hiconstcheck==0){
					printf("Exception: Instruction unrecognizable \n");
					return 1;
				}else{
					mnemo=110;
				}
			break;
			//trap
			case 15:
				mnemo = 120;		//trap
			break;
			default:
				printf("Exception: Instruction unrecognizable \n");
				return 1;
		}
		UpdateControlSignals();
		return 0;
}

//Updates ControlSignals based on instruction
void UpdateControlSignals(){
	if(mnemo<10){				//branch
		branchsignals();
	}else if(mnemo<20){			//arith
		arithsignals();
	}else if(mnemo<30){			//cmp
		cmpsignals();
	}else if(mnemo<40){			//jsr
		jsrsignals();
	}else if(mnemo<50){			//logic
		logicsignals();
	}else if(mnemo<60){			//mem
		memsignals();
	}else if(mnemo<70){			//rti
		rtisignals();
	}else if(mnemo<80){			//const
		constsignals();
	}else if(mnemo<90){			//shift (and mod)
		shiftsignals();
	}else if(mnemo<100){		//jmpr
		jmprsignals();
	}else if(mnemo<110){		//jmp
		jmpsignals();
	}else if(mnemo<120){		//hiconst
		hiconstsignals();
	}else if(mnemo<130){		//trap
		trapsignals();
	}
}

// Update Machine State based on the prevailing control signals - return a non-zero Exception code if an Exception is detected.
int UpdateMachineState () {
	ControlSignals *signals = GetCurrentControlSignals();
	MachineState *state = GetCurrentMachineState();

	unsigned short int instruction = state->memory[state->PC];
	unsigned short int rs_addr, rt_addr, rd_addr;
	
	short int alu_output;
	short int arith_input, logic_input, comp_input;
	unsigned int shiftby = (instruction & 0xF);

	int compare1, compare2;

	unsigned short int data_address;
	short int data_output;
	short int data_input;

	unsigned short int regInputMuxOutput;
	unsigned int temp;

	short int N, Z, P;

	//set rs, rt, and rd pointers
	switch(signals->rsMux_CTL){
		case 0:
			rs_addr = INSN_8_6(instruction);
		break;
		case 1:
			rs_addr = 0x07;
		break;
		case 2:
			rs_addr = INSN_11_9(instruction);
		break;
		default:
			printf("Exception: rsMux_CTL out of bounds \n");
			return 1;
	}
	switch(signals->rtMux_CTL){
		case 0:
			rt_addr = INSN_2_0(instruction);
		break;
		case 1:
			rt_addr = INSN_11_9(instruction);
		break;
		default:
			printf("Exception: rtMux_CTL out of bounds \n");
			return 1;
	}
	switch(signals->rdMux_CTL){
		case 0:
			rd_addr = INSN_11_9(instruction);
		break;
		case 1:
			rd_addr = 0x07;
		break;
		default:
			printf("Exception: rdMux_CTL out of bounds \n");
			return 1;
	}

	//switch on ALU_Mux
	switch(signals->ALUMux_CTL){
		case 0:
			switch(signals->ALUMux_CTL){
				case 0:
					arith_input = state->R[rt_addr];
				break;
				case 1:
					arith_input = IMM5(instruction);
				break;
				case 2:
					arith_input = IMM6(instruction);
				break;
				default:
					printf("Exception: ALUMux_CTL out of bounds \n");
			}
			switch(signals->Arith_CTL){
				case 0:	//add
					alu_output = state->R[rs_addr] + arith_input;
				break;
				case 1:	//mul
					alu_output = state->R[rs_addr] * (unsigned short int)arith_input;
				break;
				case 2:	//sub
					alu_output = state->R[rs_addr] - arith_input;
				break;
				case 3:	//divide
					if(arith_input==0){
						alu_output = 0;
					}else{
						alu_output = state->R[rs_addr] / (unsigned short int)arith_input;
					}
				break;
				case 4:	//modulus
					if(arith_input==0){	//modulus by zero Exception print corresponding Exception message
						alu_output = 0;
					}else{		//possibly check if a<b??
						alu_output = state->R[rs_addr] % (unsigned short int) arith_input;
					}
				break;
				default:
					printf("Exception: Arith_CTL out of bounds \n");
					return 1;
			}
		break;
		case 1:
			switch(signals->LogicMux_CTL){
				case 0:
					logic_input = state->R[rt_addr];
				break;
				case 1:
					logic_input = IMM5(instruction);
				break;
				default:
					printf("Exception: LogicMux_CTL out of bounds \n");
			}
			switch(signals->LOGIC_CTL){
				case 0:	//and
					alu_output = (state->R[rs_addr] & logic_input);
				break;	
				case 1:	//not
					alu_output = ~(state->R[rs_addr]);
				break;
				case 2: //or
					alu_output = (state->R[rs_addr] | logic_input);
				break;
				case 3:	//xor
					alu_output = (state->R[rs_addr] ^ logic_input);
				break;
				default:
					printf("Exception: LOGIC_CTL out of bounds \n");
					return 1; 
			}
		break;
		case 2:
			switch(signals->SHIFT_CTL){
				case 0:	//sll
					alu_output = state->R[rs_addr] << shiftby;
				break;
				case 1:	//sra
					alu_output = (signed int)state->R[rs_addr] >> shiftby;
				break;
				case 2:	//srl
					alu_output = state->R[rs_addr] >> shiftby ;
				break;
				default:
					printf("Exception: SHIFT_CTL out of bounds \n");
					return 1;
			}
		break;
		case 3:
			switch(signals->CONST_CTL){
				case 0:
					alu_output =  IMM9(instruction);
				break;
				case 1:
					alu_output =  ((int)IMM8(instruction)<<8) | (state->R[rd_addr]);
				break;
				default:
					printf("Exception: CONST_CTL out of bounds \n");
					return 1;
			}
		break;
		case 4:
			switch(signals->CMP_CTL){
				case 0:	//cmp
					compare1=SEXT_17(state->R[rs_addr]);
					compare2=SEXT_17(state->R[rt_addr]);
					if(compare1 > compare2){
						alu_output = 1;
					}else if(compare1 == compare2){
						alu_output = 0;
					}else{
						alu_output = -1;
					}
				break;
				case 1:	//cmpu
					if((unsigned int)state->R[rs_addr] > (unsigned int)state->R[rt_addr]){
						alu_output = 1;
					}else if((unsigned int)state->R[rs_addr] == (unsigned int)state->R[rt_addr]){
						alu_output = 0;
					}else{
						alu_output = -1;
					}
				break;
				case 2:	//cmpi
					compare1=SEXT_17(state->R[rs_addr]);
					compare2=IMM7(instruction);
					if(compare1 > compare2){
						alu_output = 1;
					}else if(compare1 == compare2){
						alu_output = 0;
					}else{
						alu_output = -1;
					}
				break;
				case 3:	//cmpiu
					if((unsigned int)state->R[rs_addr] > (unsigned int)IMM7(instruction)){
						alu_output = 1;
					}else if((unsigned int)state->R[rs_addr] == (unsigned int)IMM7(instruction)){
						alu_output = 0;
					}else{
						alu_output = -1;
					}
				break;
				default:
					printf("Exception: CMP_CTL out of bounds \n");
					return 1;
			}
		break;
		default:
			printf("Exception: ALUMux_CTL out of bounds \n");
			return 1;
	}
	
	//data
	data_address = alu_output;
	data_input = state->R[rt_addr];
	switch(signals->DATA_WE){
		case 0:
			if(data_address<0){
				printf("IllegalMemAccessException accessing address x%04X \n", data_address);
				return 1;
			}else if(data_address< 0x8000){ //user
				//if psr[15]=1 then we can still load something from user code
				
			}else if(data_address<0xFFFF){ //data
				if(INSN_15(state->PSR)==0){
					printf("Exception: IllegalMemAccessException accessing address x%04X with privelege false \n", data_address);
					return 1;
				}
			}else{ //>xFFFF
				printf("Exception: IllegalMemAccessException accessing address x%04X \n", data_address);
				return 1;
			}
			data_output = state->memory[data_address];
		break;
		case 1:
			if(data_address<0){
				printf("Exception: IllegalMemAccessException accessing address x%04X \n", data_address);
				return 1;
			}else if(data_address< 0x2000){
				printf("Exception: IllegalMemAccessException accessing address x%04X \n", data_address);
				return 1;
			}else if(data_address<0x8000){
				if(INSN_15(state->PSR)==1){
					printf("Exception: IllegalMemAccessException accessing address x%04X with privelege true \n", data_address);
					return 1;
				}else{
					state->memory[data_address]=data_input;
				}
			}else if(data_address<0xA000){
				printf("Exception: IllegalMemAccessException accessing address x%04X  \n", data_address);
				return 1;
			}else if(data_address<0xFFFF){
				if(INSN_15(state->PSR)==0){
					printf("Exception: IllegalMemAccessException accessing address x%04X with privelege false \n", data_address);
				}else{
					state->memory[data_address]=data_input;
				}
			}else{
				printf("Exception: IllegalMemAccessException accessing address x%04X \n", data_address);
				return 1;
			}
		break;
		default:
			printf("Exception: ALUMux_CTL out of bounds \n");
			return 1;
	}

	//reg_input_mux
	switch(signals->regInputMux_CTL){
		case 0:
			regInputMuxOutput = alu_output;
		break;
		case 1:
			regInputMuxOutput = data_output;
		break;
		case 2:
			regInputMuxOutput = state->PC +1;
		break;
		default:
			printf("Exception: regInputMux_CTL out of bounds \n");
			return 1;
	}

	//nzp
	switch(signals->NZP_WE){
		case 0:
			//do nothing to nzp because 
		break;
		case 1:
			temp = (unsigned int)INSN_2_0(state->PSR);
			state->PSR= state->PSR-temp;
			if(regInputMuxOutput>0){
				state->PSR = state->PSR + 1;
			}else if(regInputMuxOutput==0){
				state->PSR = state->PSR + 2;
			}else if(regInputMuxOutput<0){
				state->PSR = state->PSR + 4;
			}
		break;
		default:
			printf("Exception: NZP_WE out of bounds \n");
			return 1;
	}

	//regFile.WE
	switch(signals->regFile_WE){
		case 0:
			//no register updates
		break;
		case 1:
			state->R[rd_addr]=regInputMuxOutput;
		break;
		default:
			printf("Exception: ALUMux_CTL out of bounds");
			return 1;
	}

	//PC_Mux.CTL
	switch(signals->PCMux_CTL){
		case 0: //branch
			if((INSN_11_9(instruction) & INSN_2_0(state->PSR)) != 0){
				state->PC = state-> PC + 1 + IMM9(instruction);
			}else{
				state->PC = state->PC +1;
			}
		break;

		case 1:	//pc+1
			state->PC = state->PC + 1;
		break;

		case 2:	//11
			state->PC = state->PC+ 1 + IMM11(instruction);
		break;

		case 3: //rti
			state->PC = state->R[rs_addr];
			state->PSR &= ~(1 << 15);	//PSR[15]=0
		break;

		case 4:	//trap
			state->PC = (0x8000 | IMM8(instruction));
			state->PSR |= 1 << 15;	//PSR[15]=1
		break;

		case 5: //jsr
			state->PC = ((state->PC & 0x0000) | (IMM9(instruction)<<4));
			
		break;

		default:
			printf("Exception: PCMux_CTL out of bounds \n");
			return 1;
	}

	//check if PC value is okay
	if(state->PC<0){
		printf("Exception: PC out of bounds.");
		return 1;
	}else if(state->PC < 0x2000){
		if(INSN_15(state->PSR)==1){
			printf("Exception: IllegalMemAccessException accessing address x%04X with privelege true \n", state->PC);
			return 1;
		}
	}else if(state->PC < 0x8000){
		printf("Exception: IllegalMemAccessException accessing address x%04X ", state->PC);
		return 1;
	}else if(state->PC < 0xA000){
		if( INSN_15(state->PSR)==0){
			printf("Exception: IllegalMemAccessException accessing address x%04X with privelege false \n", state->PC);
			return 1;
		}
	}else if(state->PC < 0xFFFF){
		printf("Exception: IllegalMemAccessException accessing address x%04X ", state->PC);
		return 1;
	}else{
		printf("Exception: IllegalMemAccessException accessing address x%04X ", state->PC);
		return 1;
	}
	
	return 0;
	
}
// Reset the machine state as Pennsim would do
void Reset () {
	MachineState *state = GetCurrentMachineState();
	int i;
	state->PC=0x8200;
	//reset PSR
	state->PSR=0;
	state->PSR |= 1 << 15;	//PSR[15]=1 because we start in OS
	//reset registers
	for(i=0;i<8;i++){
		state->R[i]=0;
	}
	//reset memory
	for(i=0;i<65536;i++){
		state->memory[i]=0;
	}
}

// Clear the machine state as Pennsim would do
void Clear () {
	//do nothing
}

/** START UPDATES OF CONSTROL SIGNALS BASED ON SPECIFIC INSTRUCTION */
//branch signals
void branchsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 0;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 0;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 0;
	signals->DATA_WE = 0;
}
//arithmetic signals
void arithsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 0;
	if(mnemo==10){			//add
		signals->Arith_CTL = 0;
	}else if(mnemo==11){	//mul
		signals->Arith_CTL = 1;
	}else if(mnemo==12){	//sub
		signals->Arith_CTL = 2;
	}else if(mnemo==13){	//div
		signals->Arith_CTL = 3;
	}else if(mnemo==14){	//addim
		signals->Arith_CTL = 0;
	}
	if(mnemo==14){			//addim
		signals->ArithMux_CTL = 1;
	}else{					//all non immediate arith
		signals->ArithMux_CTL = 0;
	}
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//cmp signals
void cmpsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 2;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 0;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	if(mnemo==20){		//cmp
		signals->CMP_CTL = 0;
	}else if(mnemo==21){	//cmpu
		signals->CMP_CTL = 1;
	}else if(mnemo==22){	//cmpi
		signals->CMP_CTL = 2;
	}else if(mnemo==23){	//cmpiu
		signals->CMP_CTL = 3;
	}	
	signals->ALUMux_CTL = 4;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//jsr signals
void jsrsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	if(mnemo==30){	//jsr
		signals->PCMux_CTL = 5;
	}else if(mnemo==31){	//jsrr
		signals->PCMux_CTL = 3;
	}
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 1;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 2;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//logic signals
void logicsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	if(mnemo==40){			//and
		signals->LOGIC_CTL = 0;
	}else if(mnemo==41){	//not
		signals->LOGIC_CTL = 1;
	}else if(mnemo==42){	//or
		signals->LOGIC_CTL = 2;
	}else if(mnemo==43){	//xor
		signals->LOGIC_CTL = 3;
	}else if(mnemo==44){	//andim
		signals->LOGIC_CTL = 0;
	}
	if(mnemo==44){			//andim
		signals->LogicMux_CTL = 1;
	}else{					//all non immediate logic
		signals->LogicMux_CTL = 0;
	}
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 1;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//mem signals
void memsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 0;
	if(mnemo==51){			//str
		signals->rtMux_CTL = 1;
	}else{					//ldr
		signals->rtMux_CTL = 0;
	}
	signals->rdMux_CTL = 0;
	if(mnemo==50){			//ldr
		signals->regFile_WE = 1;
		signals->regInputMux_CTL = 1;
	}else {					//str
		signals->regFile_WE = 0;
		signals->regInputMux_CTL = 0;
	}
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 2;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	if(mnemo==50){			//ldr
		signals->NZP_WE = 1;
		signals->DATA_WE = 0;
	}else {					//str
		signals->NZP_WE = 0;
		signals->DATA_WE = 1;
	}
}
//rti signals
void rtisignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 3;
	signals->rsMux_CTL = 1;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 0;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 0;
	signals->DATA_WE = 0;
}
//const signals
void constsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 3;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//shift signals and mod
void shiftsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 0;
	if(mnemo==83){			//mod
		signals->ArithMux_CTL = 4;
	}else{					//shifts
		signals->ArithMux_CTL = 0;
	}
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	if(mnemo==80){			//sll
		signals->SHIFT_CTL = 0;
	}else if(mnemo==81){	//sra
		signals->SHIFT_CTL = 1;
	}else if(mnemo==82){	//srl
		signals->SHIFT_CTL = 2;
	}else if(mnemo==83){
		signals->SHIFT_CTL = 0;
	}
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	if(mnemo==83){			//mod
		signals->ALUMux_CTL = 0;
	}else{					//shifts
		signals->ALUMux_CTL = 2;
	}
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//jmpr signals
void jmprsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 3;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 0;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 0;
	signals->DATA_WE = 0;
}
//jmp signals
void jmpsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 2;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 0;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 0;
	signals->DATA_WE = 0;
}
//hiconstsignals
void hiconstsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 1;
	signals->rsMux_CTL = 2;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 0;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 0;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 1;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 3;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
//trapsignals
void trapsignals(){
	ControlSignals *signals = GetCurrentControlSignals();
	signals->PCMux_CTL = 4;
	signals->rsMux_CTL = 0;
	signals->rtMux_CTL = 0;
	signals->rdMux_CTL = 1;
	signals->regFile_WE = 1;
	signals->regInputMux_CTL = 2;
	signals->Arith_CTL = 0;
	signals->ArithMux_CTL = 0;
	signals->LOGIC_CTL = 0;
	signals->LogicMux_CTL = 0;
	signals->SHIFT_CTL = 0;
	signals->CONST_CTL = 0;
	signals->CMP_CTL = 0;
	signals->ALUMux_CTL = 0;
	signals->NZP_WE = 1;
	signals->DATA_WE = 0;
}
/*END OF UPDATE CONTROL SIGNALS */

