// Include the input/output stream library for printing to console
#include <iostream>
// Include the string library for working with text strings
#include <string>
// Include the iomanip library for formatting output (like hex display, padding with zeros)
#include <iomanip>

// Allow us to use cout, cin, string without writing std:: prefix
using namespace std;

/*
================================================================================
                        MIPS PROCESSOR SIMULATOR
================================================================================

This program simulates how a real CPU executes MIPS instructions. It mimics 
the fetch-decode-execute cycle that real processors use.

THE BIG PICTURE:
- We have a simulated CPU with 32 REGISTERS (fast storage) and 256 MEMORY 
  locations (slower storage)
- User provides MIPS instructions in HEXADECIMAL format (8 hex digits = 32 bits)
- The program DECODES the binary instruction to extract its components
- Then it EXECUTES the appropriate operation based on the instruction type
- Finally, it DISPLAYS the updated register and memory state

INSTRUCTION TYPES:
1. R-TYPE (Register format): opcode=0, uses registers rs, rt, rd and funct code
   - Arithmetic: ADD, SUB, AND, OR, XOR
   
2. I-TYPE (Immediate format): opcode≠0, uses registers rs, rt and immediate value
   - Arithmetic: ADDI (add with immediate value)
   - Memory: LW (load word), SW (store word)
   - Branching: BEQ (branch if equal), BNE (branch if not equal)

DATA FLOW FOR EACH INSTRUCTION:
  User Input (Hex) 
       ↓
  [STEP 1] Convert Hex → Binary (00642820 becomes 00000000011001000010100000100000)
       ↓
  [STEP 2] Parse Binary → Extract Fields (opcode, rs, rt, rd, funct, imm)
       ↓
  [STEP 3] Display Parsed Components (show what was extracted)
       ↓
  [STEP 4] Execute Instruction (modify registers or memory based on operation)
       ↓
  Display Updated State (show new values)

================================================================================
*/

// HELPER FUNCTION: Converts a single Hex character (0-F) to a 4-bit Binary string
string hexCharToBinary(char hex) {
    // Use a switch statement to check the hex character and return its binary equivalent
    switch(toupper(hex)) {
        // If hex char is '0', return binary "0000"
        case '0': return "0000";
        // If hex char is '1', return binary "0001"
        case '1': return "0001";
        // If hex char is '2', return binary "0010"
        case '2': return "0010";
        // If hex char is '3', return binary "0011"
        case '3': return "0011";
        // If hex char is '4', return binary "0100"
        case '4': return "0100";
        // If hex char is '5', return binary "0101"
        case '5': return "0101";
        // If hex char is '6', return binary "0110"
        case '6': return "0110";
        // If hex char is '7', return binary "0111"
        case '7': return "0111";
        // If hex char is '8', return binary "1000"
        case '8': return "1000";
        // If hex char is '9', return binary "1001"
        case '9': return "1001";
        // If hex char is 'A', return binary "1010"
        case 'A': return "1010";
        // If hex char is 'B', return binary "1011"
        case 'B': return "1011";
        // If hex char is 'C', return binary "1100"
        case 'C': return "1100";
        // If hex char is 'D', return binary "1101"
        case 'D': return "1101";
        // If hex char is 'E', return binary "1110"
        case 'E': return "1110";
        // If hex char is 'F', return binary "1111"
        case 'F': return "1111";
        // If the character is invalid, default return "0000"
        default: return "0000";
    }
}

// HELPER FUNCTION: Displays the current state of Registers and Memory
void displayState(int regs[], int mem[]) {
    /* 
    This function shows us the current values in the CPU's registers and memory
    after each instruction executes. This lets us see what changed.
    
    We display:
    - Registers R[0] through R[15] in HEXADECIMAL format with leading zeros
    - Memory M[0] through M[15] in HEXADECIMAL format with leading zeros
    
    This is called TWICE:
    1. After initialization (to show starting state)
    2. After every instruction execution (to show what changed)
    */
    // Print header line for the state display
    cout << "\n--- Current State ---\n";
    // Print column headers for registers and memory
    cout << "Registers (0-15)\t\tMemory (0-15)\n";
    
    // Loop through indices 0 to 15 to display registers and memory side by side
    for (int i = 0; i < 16; ++i) {
        // Print register label and its value in hexadecimal format with padding
        cout << "R[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << regs[i];
        // Print memory label and its value in hexadecimal format with padding
        cout << "\t\tM[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << mem[i] << endl;
    }
    // Switch output format back to decimal (default)
    cout << dec;
    // Print closing line for the state display
    cout << "---------------------\n";
}

int main() {
    /*
    MAIN FUNCTION - The entry point where the program starts
    
    PHASE 1: INITIALIZATION
    - Create the simulated CPU components (registers and memory)
    - Initialize them with starting values
    
    PHASE 2: USER INTERACTION
    - Ask user how many instructions they want to run
    - Loop that many times, each time:
      * Get hex instruction from user
      * Convert to binary
      * Parse into components
      * Execute the instruction
      * Show updated state
    */
    // Create an array of 32 integers to represent the CPU registers (R0-R31)
    int registers[32]; 
    // Create an array of 256 integers to represent CPU memory addresses (0-255)
    int memory[256];

    /*
    INITIALIZATION LOOP 1: Initialize all 32 registers
    Each register R[i] is set to its index value as a starting point
    Examples: R[0]=0, R[1]=1, R[2]=2, ... R[31]=31
    */
    // Loop through all 32 registers to initialize them
    for (int i = 0; i < 32; i++) {
        // Set each register to its index value
        registers[i] = i; 
    }
    
    /*
    INITIALIZATION LOOP 2: Initialize all 256 memory locations
    Each memory location M[i] is set to its index value as a starting point
    Examples: M[0]=0, M[1]=1, M[2]=2, ... M[255]=255
    */
    // Loop through all 256 memory locations to initialize them
    for (int i = 0; i < 256; i++) {
        // Set each memory location to its address value
        memory[i] = i; 
    }

    // Print the title of the simulator program
    cout << "SIMPLE MIPS SIMULATOR" << endl;
    // Display the current state of all registers and memory
    displayState(registers, memory);

    /*
    USER INPUT PHASE: Ask how many instructions to run
    This determines how many times the main loop will execute.
    Each iteration processes one MIPS instruction.
    */
    // Declare a variable to store how many instructions the user wants to run
    int numInstructions;
    // Ask the user to input the number of instructions
    cout << "How many instructions do you want to run? ";
    // Read the user's input from the keyboard
    cin >> numInstructions;

    // Main execution loop - runs for each instruction the user wants to execute
    for (int k = 0; k < numInstructions; k++) {
        /*
        ====================================================================
        START OF INSTRUCTION PROCESSING CYCLE
        ====================================================================
        Each iteration processes ONE complete MIPS instruction through:
        STEP 1: Hex to Binary conversion
        STEP 2: Parse binary into instruction fields
        STEP 3: Display what was extracted
        STEP 4: Execute the instruction
        STEP 5: Show updated state
        ====================================================================
        */
        // Declare a string variable to store the hex instruction input
        string hexInput;
        // Prompt the user to enter an 8-digit hexadecimal instruction
        cout << "\nEnter 8-digit Hex instruction (e.g. 00642820): ";
        // Read the hexadecimal instruction from the user
        cin >> hexInput;

        /*
        INPUT VALIDATION:
        The instruction must be EXACTLY 8 hexadecimal digits (4 bytes = 32 bits)
        If not, we reject it and ask again WITHOUT counting this attempt
        */
        // Check if the input is not exactly 8 characters long (validation)
        if (hexInput.length() != 8) {
            // If input length is invalid, print an error message
            cout << "Error: Input must be exactly 8 characters." << endl;
            // Decrement loop counter to re-ask for input without counting this attempt
            k--; 
            // Skip to the next iteration of the loop to ask again
            continue;
        }

        // STEP 1: Convert Hex instruction to Binary representation
        // Declare an empty string to store the binary conversion
        string binaryString = "";
        // Loop through each of the 8 hexadecimal characters
        for (int i = 0; i < 8; i++) {
            // Convert each hex character to 4-bit binary and append to the binary string
            binaryString += hexCharToBinary(hexInput[i]);
        }
        // Print the binary representation of the instruction
        cout << "Binary: " << binaryString << endl;

        /*
        STEP 2: INSTRUCTION PARSING
        
        Now we extract the different fields from the 32-bit instruction.
        The exact layout depends on instruction type, but we extract all possible fields:
        
        For R-TYPE (opcode=0):
        [Opcode(6)] [Rs(5)] [Rt(5)] [Rd(5)] [Shamt(5)] [Funct(6)]
           0-5        6-10   11-15   16-20   21-25      26-31
        
        For I-TYPE (opcode≠0):
        [Opcode(6)] [Rs(5)] [Rt(5)] [Immediate(16)]
           0-5        6-10   11-15      16-31
        
        We extract ALL fields and convert them to integers so we can use them
        */
        // Extract bits 0-5 (the first 6 bits) as the opcode
        string opcodeStr = binaryString.substr(0, 6);
        // Extract bits 6-10 (5 bits) as the rs (source register) field
        string rsStr     = binaryString.substr(6, 5);
        // Extract bits 11-15 (5 bits) as the rt (target register) field
        string rtStr     = binaryString.substr(11, 5);
        // Extract bits 16-20 (5 bits) as the rd (destination register) field
        string rdStr     = binaryString.substr(16, 5);
        // Extract bits 21-25 (5 bits) as the shamt (shift amount) field
        string shamtStr  = binaryString.substr(21, 5);
        // Extract bits 26-31 (the last 6 bits) as the funct (function) field
        string functStr  = binaryString.substr(26, 6);
        // Extract bits 16-31 (16 bits) as the immediate value field (for I-format instructions)
        string immStr    = binaryString.substr(16, 16);

        // Convert the opcode binary string to an integer (base 2 = binary)
        int opcode = stoi(opcodeStr, nullptr, 2);
        // Convert the rs binary string to an integer
        int rs     = stoi(rsStr, nullptr, 2);
        // Convert the rt binary string to an integer
        int rt     = stoi(rtStr, nullptr, 2);
        // Convert the rd binary string to an integer
        int rd     = stoi(rdStr, nullptr, 2);
        // Convert the shamt binary string to an integer (represents how many positions to shift)
        int shamt  = stoi(shamtStr, nullptr, 2);
        // Convert the funct binary string to an integer (specific function within R-type instructions)
        int funct  = stoi(functStr, nullptr, 2);
        // Convert the immediate binary string to an integer
        int imm    = stoi(immStr, nullptr, 2);

        // STEP 3: Display the parsed instruction components to the user
        // Print a header for the component section
        cout << "\nParsed Components:" << endl;
        // Display the opcode value
        cout << "a. Opcode: " << opcode << endl;
        // Display the rs register number and its corresponding register name
        cout << "b. Rs:     " << rs << " ($" << rs << ")" << endl;
        // Display the rt register number and its corresponding register name
        cout << "c. Rt:     " << rt << " ($" << rt << ")" << endl;
        // Display the rd register number and its corresponding register name
        cout << "d. Rd:     " << rd << " ($" << rd << ")" << endl;
        // Display the shift amount value
        cout << "e. Shamt:  " << shamt << endl;
        // Display the function code
        cout << "f. Funct:  " << funct << endl; 
        // Print a separator line for clarity
        cout << "--------------------------------" << endl;

        /*
        STEP 4: INSTRUCTION EXECUTION
        
        Now we DECODE and EXECUTE based on the opcode:
        
        The OPCODE acts as the primary decoder:
        - If opcode == 0: This is an R-TYPE instruction
          → The FUNCT field tells us which specific operation
          → Operations: ADD (32), SUB (34), AND (36), OR (37), XOR (38)
        
        - If opcode != 0: This is an I-TYPE instruction
          → The opcode directly tells us the operation
          → Operations: ADDI (8), LW (35), SW (43), BEQ (4), BNE (5)
        
        For R-TYPE: registers[rd] = registers[rs] OP registers[rt]
        For I-TYPE: registers[rt] = registers[rs] OP immediate_value
        
        Memory operations (LW/SW) directly access the memory array
        Branch operations (BEQ/BNE) only print result, they don't modify state
        */
        // Check if opcode is 0, which indicates an R-type instruction (register format)
        if (opcode == 0) { 
            // Print that this is an R-type instruction format
            cout << "Format: R-Type" << endl;
            
            /*
            R-TYPE INSTRUCTIONS: All arithmetic operations between registers
            Format: operation registers[rd] = registers[rs] OP registers[rt]
            The FUNCT code determines which operation to perform
            */
            
            // Check if funct code is 32, which means ADD instruction
            if (funct == 32) {
                // Add the values in registers rs and rt, store result in register rd
                registers[rd] = registers[rs] + registers[rt];
                // Display which registers were used and which one got the result
                cout << "Instruction: add $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            // Check if funct code is 34, which means SUBTRACT instruction
            else if (funct == 34) {
                // Subtract rt from rs, store result in register rd
                registers[rd] = registers[rs] - registers[rt];
                // Display which registers were used and which one got the result
                cout << "Instruction: sub $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            // Check if funct code is 36, which means BITWISE AND instruction
            else if (funct == 36) {
                // Perform bitwise AND between rs and rt, store result in register rd
                registers[rd] = registers[rs] & registers[rt];
                // Display which registers were used and which one got the result
                cout << "Instruction: and $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            // Check if funct code is 37, which means BITWISE OR instruction
            else if (funct == 37) {
                // Perform bitwise OR between rs and rt, store result in register rd
                registers[rd] = registers[rs] | registers[rt];
                // Display which registers were used and which one got the result
                cout << "Instruction: or $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            // Check if funct code is 38, which means BITWISE XOR instruction
            else if (funct == 38) {
                // Perform bitwise XOR between rs and rt, store result in register rd
                registers[rd] = registers[rs] ^ registers[rt];
                // Display which registers were used and which one got the result
                cout << "Instruction: xor $" << rd << ", $" << rs << ", $" << rt << endl;
            }
        } 
        // If opcode is not 0, this is an I-type instruction (immediate format)
        else { 
            // Print that this is an I-type instruction format
            cout << "Format: I-Type" << endl;

            /*
            I-TYPE INSTRUCTIONS: Instructions with an immediate (constant) value
            Includes: arithmetic with immediate, memory operations, and branches
            */

            // Check if opcode is 8, which means ADD IMMEDIATE instruction
            if (opcode == 8) {
                // Add the immediate value to register rs, store result in register rt
                registers[rt] = registers[rs] + imm;
                // Display which register was used, the immediate value, and which one got the result
                cout << "Instruction: addi $" << rt << ", $" << rs << ", " << imm << endl;
            }
            // Check if opcode is 35, which means LOAD WORD instruction
            else if (opcode == 35) {
                /*
                LW (Load Word): Reads a VALUE from memory and puts it in a register
                - imm is the memory ADDRESS to read from
                - The value at memory[imm] is copied into registers[rt]
                This is how we get data from the slow memory into fast registers
                */
                // Load a value from memory at address imm, store it in register rt
                registers[rt] = memory[imm];
                // Display the memory address loaded from and which register got the value
                cout << "Instruction: lw $" << rt << ", " << imm << "($" << rs << ")" << endl;
            }
            // Check if opcode is 43, which means STORE WORD instruction
            else if (opcode == 43) {
                /*
                SW (Store Word): Writes a VALUE from a register into memory
                - The value in registers[rt] is copied to memory[imm]
                - imm is the memory ADDRESS to write to
                This is how we save data from fast registers to slow memory
                */
                // Store the value from register rt into memory at address imm
                memory[imm] = registers[rt];
                // Display which register was stored and where in memory it was stored
                cout << "Instruction: sw $" << rt << ", " << imm << "($" << rs << ")" << endl;
            }
            // Check if opcode is 4, which means BRANCH IF EQUAL instruction
            else if (opcode == 4) {
                /*
                BEQ (Branch If Equal): Compares two registers
                - If registers[rs] == registers[rt], the branch is TAKEN (jump)
                - Otherwise, execution continues normally
                In a full processor, this would change the Program Counter (PC)
                Here, we just print the result
                */
                // Compare if the values in registers rs and rt are equal
                if (registers[rs] == registers[rt])
                    // If they're equal, print that the branch was taken
                    cout << "Instruction: beq (Branch Taken!)" << endl;
                else
                    // If they're not equal, print that the branch was not taken
                    cout << "Instruction: beq (Branch Not Taken)" << endl;
            }
            // Check if opcode is 5, which means BRANCH IF NOT EQUAL instruction
            else if (opcode == 5) {
                /*
                BNE (Branch If Not Equal): Compares two registers
                - If registers[rs] != registers[rt], the branch is TAKEN (jump)
                - Otherwise, execution continues normally
                In a full processor, this would change the Program Counter (PC)
                Here, we just print the result
                */
                // Compare if the values in registers rs and rt are NOT equal
                if (registers[rs] != registers[rt])
                    // If they're not equal, print that the branch was taken
                    cout << "Instruction: bne (Branch Taken!)" << endl;
                else
                    // If they're equal, print that the branch was not taken
                    cout << "Instruction: bne (Branch Not Taken)" << endl;
            }
        }

        /*
        END OF INSTRUCTION EXECUTION
        Now we show the user what changed after the instruction ran
        */
        // Display the updated state of all registers and memory after execution
        displayState(registers, memory);
    }

    // Return 0 to indicate successful program completion
    return 0;
}