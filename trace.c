#include <stdio.h>
#include "ObjectFiles.h"

int main(int argc, char *argv[]){
	MachineState *state;
	int i;
	int return_value;
	//check if proper amount of arguments
	if(argc<2){
		printf("Exception: Not enough arguments \n");
		return 1;
	}
	if(argc==2){
		printf("No files to load into memory \n");
		return 0;
	}
	//output_file open and Exception check
	FILE *file;
	file = fopen(argv[1], "wb");

	//pointer to current machine state, and reset machine
	state = GetCurrentMachineState();
	Reset();

	//load into memory based on object files
	for(i=2; i < argc; i=1+i){
		return_value = ReadObjectFile(argv[i], state);
		if(return_value==1){
			return 1;
		}
	}

	//loop through instructions
	while(state->PC != 0x80FF){
		fwrite(&(state->PC),2, 1, file);
		fwrite(&(state->memory[state->PC]),2, 1, file);

		//decode instruction 
		// printf("psr: %04X \n", state->PSR);
		return_value = DecodeCurrentInstruction();
		if(return_value!=0){
			return 1;
		}

		//update machine state
		return_value = UpdateMachineState();
		if(return_value!=0){
			return 1;
		}
		
	}
	//close file
	fclose(file);
	return 0;
}