/*
 * LC4.h
 */

typedef struct {
  // PC the current value of the Program Counter register
  unsigned short int PC;

  // PSR : Processor Status Register, bit[0] = P, bit[1] = Z, bit[2] = N, bit[15] = privilege bit
  unsigned short int PSR;

  // Machine registers - all 8
  unsigned short int R[8];

  // Machine memory - all of it
  unsigned short int memory[65536];
} MachineState;


typedef struct {
  // Note that all of the control signals are represented as unsigned 8 bit values although none of them use more than 3 bits
  // You should use the lower bits of the fields to store the mandated control bits. Please refer to the LC4 information sheets
  // on blackboard for an explanation of the control signals and their role in the datapath.

  unsigned char PCMux_CTL;
  unsigned char rsMux_CTL;
  unsigned char rtMux_CTL;
  unsigned char rdMux_CTL;
  unsigned char regFile_WE;
  unsigned char regInputMux_CTL;
  unsigned char Arith_CTL;
  unsigned char ArithMux_CTL;
  unsigned char LOGIC_CTL;
  unsigned char LogicMux_CTL;
  unsigned char SHIFT_CTL;
  unsigned char CONST_CTL;
  unsigned char CMP_CTL;
  unsigned char ALUMux_CTL;
  unsigned char NZP_WE;
  unsigned char DATA_WE;

} ControlSignals;

// Return a pointer to the current machine state struct
MachineState *GetCurrentMachineState ();

// Return a pointer to the current control signals struct
ControlSignals *GetCurrentControlSignals ();

/*
 * You must implement the following functions in LC4.c. You can add additional ones if you like
 */

// Fill in control signals based on the current instruction
int DecodeCurrentInstruction ();

// Update Machine State based on the prevailing control signals
int UpdateMachineState ();

//Update Control Signals based on current instruction
void UpdateControlSignals ();
void branchsignals ();
void arithsignals ();
void cmpsignals ();
void jsrsignals ();
void logicsignals ();
void memsignals ();
void rtisignals ();
void constsignals ();
void shiftsignals ();
void jmprsignals ();
void jmpsignals ();
void hiconstsignals ();
void trapsignals ();


// Reset the machine state as Pennsim would do
void Reset ();

// Clear the machine state as Pennsim would do
void Clear ();
