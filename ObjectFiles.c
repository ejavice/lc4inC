/*
 * LC4.c
 */
#include <stdio.h>	
#include "ObjectFiles.h"

int ReadObjectFile (char *filename, MachineState *CurrentMachineState){
	FILE *file;
	unsigned short int first_byte;
	unsigned short int second_byte;
	unsigned short int address;
	unsigned short int n;
	short int value;
	int section;

	//open file, but check if it exists
	file = fopen(filename, "rb");
	if (file == NULL) {
		printf("Exception: File not found. \n");
		return 1;
	}

	while(240==240){
		//get the section:
		first_byte = fgetc(file);
		second_byte = fgetc(file);
		section = (first_byte << 8 ) + second_byte;	
		if(section==16842495){			//65535 int as seen when testing
			return 0;
		}
		//switch based 
		switch(section){
			case 0xCADE:	//code
				//get starting address
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: CODE without address \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: CODE without address \n");
					return 1;
				}
				address = (first_byte << 8 ) + second_byte;
				//get number of following instructions (n)
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				n = (first_byte << 8 ) + second_byte;
				//loop over number of instructions
				for(int i=0;i<n;i++){
					first_byte = fgetc(file);
					if(first_byte==65535){
						printf("Exception: Not enough n to store \n");
						return 1;
					}
					second_byte = fgetc(file);
					if(second_byte==65535){
						printf("Exception: Not enough n to store \n");
						return 1;
					}
					value = (first_byte << 8 ) + second_byte;

					if(address<0x2000){ //user code
						CurrentMachineState->memory[address]=value;
					}else if(address<0x8000){	//user data
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}else if(address<0xA000){	//os code
						CurrentMachineState->memory[address]=value;
					}else if(address<0xFFFF){	//os data
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}else{
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}
					address++;
				}
			break;

			case 0xDADA:	//data
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: DATA without address \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: DATA without address \n");
					return 1;
				}
				address = (first_byte << 8 ) + second_byte;
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				n = (first_byte << 8 ) + second_byte;
				for(int i=0;i<n;i++){
					first_byte = fgetc(file);
					if(first_byte==65535){
						printf("Exception: Not enough n to store \n");
						return 1;
					}
					second_byte = fgetc(file);
					if(second_byte==65535){
						printf("Exception: Not enough n to store \n");
						return 1;
					}
					value = (first_byte << 8 ) + second_byte;
					if(address<0x2000){
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}else if(address<0x8000){
						CurrentMachineState->memory[address]=value;
					}else if(address<0xA000){
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}else if(address<0xFFFF){
						CurrentMachineState->memory[address]=value;
					}else{
						printf("Exception: IllegalMemAccessException accessing address x%04X \n", address);
						return 1;
					}
					address++;
				}
			break;

			case 0xC3B7:	//symbol
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: SYMBOL without address \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: SYMBOL without address \n");
					return 1;
				}
				address = (first_byte << 8 ) + second_byte;
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				n = (first_byte << 8 ) + second_byte;
				for(int i=0;i<n;i++){
					first_byte = fgetc(file);
					if(first_byte==65535){
						printf("Exception: Not enough n \n");
						return 1;
					}
				}
			break;

			case 0xF17E:	//file name
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified number of instructions \n");
					return 1;
				}
				n = (first_byte << 8 ) + second_byte;
				for(int i=0;i<n;i++){
					first_byte = fgetc(file);
					if(first_byte==65535){

						printf("Exception: Not enough n to store \n");
						return 1;
					}
				}
			break;

			case 0x715E:	//line number
				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: LINE NUMBER without address. \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: LINE NUMBER without address. \n");
					return 1;
				}

				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified line \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified line \n");
					return 1;
				}

				first_byte = fgetc(file);
				if(first_byte==65535){
					printf("Exception: Unspecified file index \n");
					return 1;
				}
				second_byte = fgetc(file);
				if(second_byte==65535){
					printf("Exception: Unspecified file index \n");
					return 1;
				}

			break;

			default:
				printf("Exception: Reading files format error \n");
				return 1;
		}
		section = 0;
	}
	fclose(file);
	return 0;
}