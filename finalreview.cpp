// Skylar Fletcher + Alaysia Miller - Chambliss 12/10/2025

/*Instructions for the Processor 

Create a simple MIPS processor that will perform the following MIPS instructions:
    add, sub, and, or, xor, sub, lw, sw, beq, bne, addi 
1. The program should be able to accept a hex instruction. Input validation: must be 8 digits no less
or more
2. Convert the instruction to binary
3. Parse the binary number into Register format:
    a. op-code- Instruction opcode
    b. rs – The 1st register source operand
    c. rt – The 2nd register source operand
    d. rd – The destination register
    e. shamt – Shift amount
    f. funct – Function code
4. Keep in mind which format of instruction (R or I)
    R-format : 6 bits 5 bits 5 bits 5 bits 5 bits 6 bits
    I-format: 6 bits 5 bits 5 bits 16 bits
5.  Perform tasks based on the opcode
6. Display the content in each register after each instruction
7. Also print the to show the actual instruction: i.e. add $1, $2, $3
8. As stated in class there are 2 sets for the output...Registers 0-15(all other instructions) and
Memory 0-15(for lw and sw) – Display Both with Labels
R[0] - 0000. M[0] - 0000
R[1] - 0001. M[1] - 0001
R[2] – 0002. M[2] - 0002
.. ....
R[15} - 000F. M[15] - 000F


*/

/*Algorithm
1. Create an array of 32 registers and 256 memory locations. Set registers[i] = i and memory[i] = i for simple starting values.

2. Print all registers R[0..15] and memory M[0..15] as 4-digit hex.

3. Ask the user how many instructions they want to run (numInstructions).

4. Create a loop to repeat instructions (k from 0 to numInstructions - 1):
   a) Input
      - Prompt the user for an 8-digit hex instruction string. If the length is not 8, print an error and ask again.

   b) Hex → Binary
      - For each hex digit, convert it to 4-bit binary using hexCharToBinary.  Concatenate into a 32-bit binary string.

   c) Decode Fields
      - Extract opcode, rs, rt, rd, shamt, funct, and imm from the binary string.
      - Convert each field from binary to integer.
      - Sign-extend the 16-bit immediate (imm) to a signed int.

   d) Execute
      - If opcode == 0 (R-type):
        - Use funct to choose: add, sub, and, or, xor.
        -  Perform: R[rd] = R[rs] OP R[rt].
      - Else (I-type):
         • If opcode == 8: addi  R[rt] = R[rs] + imm.
         • If opcode == 35: lw  R[rt] = Memory[R[rs] + imm].
         • If opcode == 43: sw   Memory[R[rs] + imm] = R[rt].
         • If opcode == 4:  beq print whether branch taken (R[rs] == R[rt]).
         • If opcode == 5:  bne print whether branch taken (R[rs] != R[rt]).
         • Otherwise: print "unknown instruction".

   e) Output
      - Print the decoded format (R-Type or I-Type), opcode, rs, rt, rd/shamt/funct or imm,and a human-readable instruction (e.g., "add $1, $2, $3").
      - Call displayState() to show updated registers and memory.
*/ 


// Include the input/output stream library for console output (cout, cin)
#include <iostream>
// Include the string library for working with text strings
#include <string>
// Include the iomanip library for formatting output (hex display, padding with zeros, width)
#include <iomanip>
// Allow us to use cout, cin, string without writing std:: prefix each time
using namespace std;

// HELPER FUNCTION: Converts a single Hex character (0-F) to a 4-bit Binary string
// Each hex digit represents exactly 4 bits in binary
string hexCharToBinary(char hex) {
    // Convert the input character to uppercase to handle both 'a' and 'A'
    // Then use a switch statement to map each hex character to its 4-bit binary equivalent
    switch(toupper(hex)) {
        // Hex 0 in binary is 0000
        case '0': return "0000";
        // Hex 1 in binary is 0001
        case '1': return "0001";
        // Hex 2 in binary is 0010
        case '2': return "0010";
        // Hex 3 in binary is 0011
        case '3': return "0011";
        // Hex 4 in binary is 0100
        case '4': return "0100";
        // Hex 5 in binary is 0101
        case '5': return "0101";
        // Hex 6 in binary is 0110
        case '6': return "0110";
        // Hex 7 in binary is 0111
        case '7': return "0111";
        // Hex 8 in binary is 1000
        case '8': return "1000";
        // Hex 9 in binary is 1001
        case '9': return "1001";
        // Hex A (10) in binary is 1010
        case 'A': return "1010";
        // Hex B (11) in binary is 1011
        case 'B': return "1011";
        // Hex C (12) in binary is 1100
        case 'C': return "1100";
        // Hex D (13) in binary is 1101
        case 'D': return "1101";
        // Hex E (14) in binary is 1110
        case 'E': return "1110";
        // Hex F (15) in binary is 1111
        case 'F': return "1111";
        // If invalid character, return 0000 as default
        default: return "0000";
    }
}

// HELPER FUNCTION: Displays the current state of Registers and Memory
// This shows us the values in 16 registers and 16 memory locations after each instruction
void displayState(int regs[], int mem[]) {
    // Print header with title
    cout << "\n          --- Current State ---\n";
    // Print blank line for spacing
    cout << endl; 
    // Print column headers for registers and memory
    cout << "Registers (0-15)\tMemory (0-15)\n";
    // Print another blank line for spacing
    cout << endl;
    
    // Loop through indices 0 to 15 to display registers and memory side by side
    for (int i = 0; i < 16; ++i) {
        // Mask the register value to keep only the lower 16 bits (& 0xFFFF extracts last 4 hex digits)
        unsigned int regVal = regs[i] & 0xFFFF;
        // Mask the memory value to keep only the lower 16 bits
        unsigned int memVal = mem[i] & 0xFFFF; 
        
        // Switch to decimal mode, print register index, then switch to hex with uppercase formatting
        // setfill('0') fills with zeros, setw(4) makes it 4 characters wide
        cout << dec << "R[$" << i << "] - " << setfill('0') << setw(4) << hex << uppercase << regVal;
        // Print tab spacing, then print memory address in decimal with hex values
        cout << "\t\tM[" << dec << i << "] - " << setfill('0') << setw(4) << hex << uppercase << memVal << dec << endl;
    }
    // Switch back to decimal format for all subsequent output
    cout << dec;
    // Print bottom border line
    cout << "           ---------------------\n";
}

// MAIN FUNCTION - Entry point of the program
int main() {
    // Create an array of 32 integers to represent CPU registers (R0-R31)
    int registers[32]; 
    // Create an array of 256 integers to represent CPU memory (addresses 0-255)
    int memory[256];

    // Initialize all 32 registers with their index values (R[0]=0, R[1]=1, etc.)
    for (int i = 0; i < 32; i++) {
        // Set each register to its index value
        registers[i] = i; 
    }
    // Initialize all 256 memory locations with their address values (M[0]=0, M[1]=1, etc.)
    for (int i = 0; i < 256; i++) {
        // Set each memory location to its index value
        memory[i] = i; 
    }

    // Print welcome message
    cout << "WELCOME TO THE MIPS PROCESSOR!" << endl;
    // Display the initial state of registers and memory
    displayState(registers, memory);

    // Ask the user how many instructions they want to execute
    int numInstructions;
    cout << "How many instructions do you want to run? (Enter a number) ";
    // Read the user's input from keyboard
    cin >> numInstructions;

    // MAIN LOOP: Executes one instruction per iteration
    // This loop repeats numInstructions times to process multiple instructions
    for (int k = 0; k < numInstructions; k++) {
        // Declare string variable to store the hexadecimal instruction input from user
        string hexInput;
        // Prompt the user to enter a valid 8-digit hexadecimal instruction
        cout << "\nEnter 8-digit Hex instruction (e.g. 00642820): ";
        // Read the hexadecimal string from the user
        cin >> hexInput;

        // VALIDATION: Check if the input is exactly 8 characters long (32 bits = 8 hex digits)
        if (hexInput.length() != 8) {
            // If invalid length, print error message
            cout << "Error: Input must be exactly 8 characters." << endl;
            // Decrement k so this invalid attempt doesn't count toward numInstructions
            k--; 
            // Skip to next iteration to ask for input again
            continue;
        }

        // STEP 1: Convert Hex to Binary
        // Create empty string to accumulate the binary representation
        string binaryString = "";
        // Loop through each of the 8 hexadecimal characters
        for (int i = 0; i < 8; i++) {
            // Convert each hex character to 4 bits and concatenate them
            binaryString += hexCharToBinary(hexInput[i]);
        }
        // Display the complete 32-bit binary representation
        cout << "Binary: " << binaryString << endl;

        // STEP 2: Parse the 32-bit binary string into instruction fields
        // Each instruction has specific bit ranges for different fields
        // Extract bits 0-5 (first 6 bits) as the opcode
        string opcodeStr = binaryString.substr(0, 6);
        // Extract bits 6-10 (5 bits) as the rs (first source register)
        string rsStr     = binaryString.substr(6, 5);
        // Extract bits 11-15 (5 bits) as the rt (second source/target register)
        string rtStr     = binaryString.substr(11, 5);
        // Extract bits 16-20 (5 bits) as the rd (destination register)
        string rdStr     = binaryString.substr(16, 5);
        // Extract bits 21-25 (5 bits) as the shamt (shift amount)
        string shamtStr  = binaryString.substr(21, 5);
        // Extract bits 26-31 (last 6 bits) as the funct (function code)
        string functStr  = binaryString.substr(26, 6);
        // Extract bits 16-31 (16 bits) as the immediate value (for I-format instructions)
        string immStr    = binaryString.substr(16, 16);

        // Convert binary strings to integers using stoi with base 2 (binary)
        // stoi(string, position, base) converts string to integer in specified base
        // Opcode determines instruction type (R-type if 0, I-type if not 0)
        int opcode = stoi(opcodeStr, nullptr, 2);
        // First source register number
        int rs     = stoi(rsStr, nullptr, 2);
        // Second source/target register number
        int rt     = stoi(rtStr, nullptr, 2);
        // Destination register number
        int rd     = stoi(rdStr, nullptr, 2);
        // Shift amount (used in shift instructions, not needed here)
        int shamt  = stoi(shamtStr, nullptr, 2);
        // Function code (tells which operation for R-type instructions)
        int funct  = stoi(functStr, nullptr, 2);
        // Immediate value (constant number in I-type instructions)
        int imm    = stoi(immStr, nullptr, 2);
        // Sign-extend the immediate value if it's negative (most significant bit is 1)
        if (immStr[0] == '1') {
            // If sign bit is 1, subtract 2^16 to convert to negative two's complement
            imm -= (1 << 16);
        }

        // STEP 3: Display the instruction format (R-type or I-type)
        // Check opcode to determine instruction type
        if (opcode == 0) {
            // Opcode 0 means R-type (register format)
            cout << "Format: R-Type" << endl;
        } else {
            // Non-zero opcode means I-type (immediate format)
            cout << "Format: I-Type" << endl;
        }

        // Display the parsed instruction components to the user
        // Print opcode value
        cout << "a. Opcode: " << opcode << endl;
        // Print first source register number
        cout << "b. Rs:     " << rs << " ($" << rs << ")" << endl;
        // Print second source register number
        cout << "c. Rt:     " << rt << " ($" << rt << ")" << endl;



        // STEP 4: Execute the instruction
        // The opcode acts as a decoder to determine what operation to perform:
        // - If opcode == 0: R-TYPE instruction (arithmetic operations between registers)
        //   Use funct code to determine: ADD (32), SUB (34), AND (36), OR (37), XOR (38)
        // - If opcode != 0: I-TYPE instruction (operations with immediate values)
        //   Operations based on opcode: ADDI (8), LW (35), SW (43), BEQ (4), BNE (5)

        // R-Type Instructions: opcode is 0, uses rd, shamt, and funct fields
        if (opcode == 0) { 
            // Print destination register number
            cout << "d. Rd:     " << rd << " ($" << rd << ")" << endl;
            // Print shift amount
            cout << "e. Shamt:  " << shamt << endl;
            // Print function code that determines which operation
            cout << "f. Funct:  " << funct << endl;
            
            // Check function code to determine which R-type operation to perform
            if (funct == 32) {
                // ADD instruction: add two registers and store in destination register
                registers[rd] = registers[rs] + registers[rt];
                cout << "Instruction: add $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 34) {
                // SUB instruction: subtract second register from first, store in destination
                registers[rd] = registers[rs] - registers[rt];
                cout << "Instruction: sub $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 36) {
                // AND instruction: bitwise AND two registers, store in destination (result is 1 if only both are 1)
                registers[rd] = registers[rs] & registers[rt];
                cout << "Instruction: and $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 37) {
                // OR instruction: bitwise OR two registers, store in destination (either output is 1 executes 1)
                registers[rd] = registers[rs] | registers[rt];
                cout << "Instruction: or $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 38) {
                // XOR instruction: bitwise XOR two registers (if inputs are different outputs 1), store in destination
                registers[rd] = registers[rs] ^ registers[rt];
                cout << "Instruction: xor $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else {
                // Unknown funct code - print error message
                cout << "Instruction: Unknown R-type (funct = " << funct << ")" << endl;
            }
        } 

        // I-Type Instructions: opcode is not 0, uses immediate value field
        else { 
            // Print the immediate value (constant in the instruction)
            cout << "g. Imm:    " << imm << endl;

            // Check opcode to determine which I-type operation to perform
            if (opcode == 8) {
                // ADDI instruction: add immediate value to register
                // Target register = Source register + Immediate value
                registers[rt] = registers[rs] + imm;
                cout << "Instruction: addi $" << rt << ", $" << rs << ", " << imm << endl;
            }
            else if (opcode == 35) {
                // LW (Load Word) instruction: load value from memory into register
                // Address = value in source register + immediate offset
                int addr = registers[rs] + imm;

                // Check if address is within valid memory range (0-255)
                if (addr < 0 || addr >= 256) {
                    // If address out of range, print error
                    cout << "Error: lw address out of range: " << addr << endl;
                } else {
                    // Load value from memory at calculated address into target register
                    registers[rt] = memory[addr];
                    cout << "Instruction: lw $" << rt << ", " << imm << "($" << rs << ")  -> R[$" << rt << "] = M[" << addr << "]" << endl;
                }
            }    
            else if (opcode == 43) {
                // SW (Store Word) instruction: store register value into memory
                // Address = value in source register + immediate offset
                int addr = registers[rs] + imm;  

                // Check if address is within valid memory range (0-255)
                if (addr < 0 || addr >= 256) {
                    // If address out of range, print error
                    cout << "Error: sw address out of range: " << addr << endl;
                } else {
                    // Store value from target register into memory at calculated address
                    memory[addr] = registers[rt];
                    cout << "Instruction: sw $" << rt << ", " << imm << "($" << rs << ")  -> M[" << addr << "] = R[$" << rt << "]" << endl;
                }
            }
            // Branch Instructions: compare two registers and determine if branch should be taken
            else if (opcode == 4) {
                // BEQ (Branch If Equal) instruction: compare registers rs and rt
                // If they are equal, branch is taken (jump would occur)
                if (registers[rs] == registers[rt])
                    cout << "Instruction: beq $" << rt << ", $" << rs << ", " << imm << "  (Branch Taken)" << endl;
                else
                    cout << "Instruction: beq $" << rt << ", $" << rs << ", " << imm << "  (Branch Not Taken)" << endl;
            }
            else if (opcode == 5) {
                // BNE (Branch If Not Equal) instruction: compare registers rs and rt
                // If they are NOT equal, branch is taken (jump would occur)
                if (registers[rs] != registers[rt])
                    cout << "Instruction: bne $" << rs << ", $" << rt << ", " << imm << "  (Branch Taken)" << endl;
                else
                    cout << "Instruction: bne $" << rs << ", $" << rt << ", " << imm << "  (Branch Not Taken)" << endl;
            }
            else {
                // Unknown opcode - print error message
                cout << "Instruction: Unknown I-type (opcode = " << opcode << ")" << endl;
            }
        }

        // Display the updated state of registers and memory after this instruction executed
        displayState(registers, memory);
    }

    // Program completed successfully - return 0 to indicate normal exit
    return 0;
}