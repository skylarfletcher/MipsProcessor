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


#include <iostream>
#include <string>
#include <iomanip> // Used for hex output formatting

using namespace std;

// Utilizing a helper fundtion that converts a single Hex character (0-F) to a 4-bit Binary string
string hexCharToBinary(char hex) {
    // Map hex character to its binary representation
    // Utilize a switch case for clarity and return its binary equivalent
    switch(toupper(hex)) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: return "0000";
    }
}

// Helper Function:  Displays the current state of Registers and Memory
void displayState(int regs[], int mem[]) {
    cout << "\n          --- Current State ---\n";
    cout << endl; 
    cout << "Registers (0-15)\tMemory (0-15)\n";
    cout << endl; 
    
    // Display Registers and Memory side by side
    for (int i = 0; i < 16; ++i) {
    
     // Keep only the lower 16 bits for display
    unsigned int regVal = regs[i] & 0xFFFF;
    unsigned int memVal = mem[i] & 0xFFFF; 
    
    // Print register index in decimal 
    cout << dec << "R[$" << i << "] - " << setfill('0') << setw(4) << hex << uppercase << regVal;
    // Print memory address in decimal 
    cout << "\t\tM[" << dec << i << "] - " << setfill('0') << setw(4) << hex << uppercase << memVal << dec << endl;
    }
    cout << dec; // switch back to decimal for subsequent output
    cout << "           ---------------------\n";
}

int main() {
    //  Create the imaginary CPU registers and memory
    int registers[32]; 
    int memory[256];

    // Initialize the registers and memory with sample values for testing
    for (int i = 0; i < 32; i++) {
        registers[i] = i; 
    }
    for (int i = 0; i < 256; i++) {
        memory[i] = i; 
    }

    cout << "WELCOME TO THE MIPS PROCESSOR!" << endl;
    displayState(registers, memory);

// ask users how many intstructions they want to run
    int numInstructions;
    cout << "How many instructions do you want to run? (Enter a number) ";
    cin >> numInstructions;

    // MAIN LOOP: Get hex instruction from user, Convert to binary, Parse into components, Execute the instruction, Show updated stat
    for (int k = 0; k < numInstructions; k++) {
        // Input Hex Instruction
        string hexInput;
        cout << "\nEnter 8-digit Hex instruction (e.g. 00642820): ";
        cin >> hexInput;

        // Validation,Check length of input
        if (hexInput.length() != 8) {
            cout << "Error: Input must be exactly 8 characters." << endl;
            k--; 
            continue;
        }

        // STEP 1: Convert Hex to Binary
        string binaryString = "";
        for (int i = 0; i < 8; i++) {
            binaryString += hexCharToBinary(hexInput[i]);
        }
        cout << "Binary: " << binaryString << endl;

        // STEP 2: Slice the string (Parsing Strings)
        string opcodeStr = binaryString.substr(0, 6);
        string rsStr     = binaryString.substr(6, 5);
        string rtStr     = binaryString.substr(11, 5);
        string rdStr     = binaryString.substr(16, 5);
        string shamtStr  = binaryString.substr(21, 5);
        string functStr  = binaryString.substr(26, 6);
        string immStr    = binaryString.substr(16, 16); // For I-Format

        // Convert opcode binary strings to Integers (base2 = binary )
        int opcode = stoi(opcodeStr, nullptr, 2);
        int rs     = stoi(rsStr, nullptr, 2);
        int rt     = stoi(rtStr, nullptr, 2);
        int rd     = stoi(rdStr, nullptr, 2);
        int shamt  = stoi(shamtStr, nullptr, 2); // Shift Amount
        int funct  = stoi(functStr, nullptr, 2);
        int imm    = stoi(immStr, nullptr, 2);
        // Sign-extend immediate value if negative
        if (immStr[0] == '1') { // Check sign bit
            imm -= (1 << 16); // Adjust for negative value
        }

        // Display format before parsed components
        if (opcode == 0) {
            cout << "Format: R-Type" << endl;
        } else {
            cout << "Format: I-Type" << endl;
        }

        // STEP 3: Parse and Display Components - NOW CONDITIONAL
        cout << "a. Opcode: " << opcode << endl;
        cout << "b. Rs:     " << rs << " ($" << rs << ")" << endl;
        cout << "c. Rt:     " << rt << " ($" << rt << ")" << endl;



        // STEP 4: Execute logic
        //  - If opcode == 0: This is an R-TYPE instruction
        //    → Operations: ADD (32), SUB (34), AND (36), OR (37), XOR (38)

        //  - If opcode != 0: This is an I-TYPE instruction
        //    Operations: ADDI (8), LW (35), SW (43), BEQ (4), BNE (5)


        // R-Format Instructions
        if (opcode == 0) { 
            cout << "d. Rd:     " << rd << " ($" << rd << ")" << endl;
            cout << "e. Shamt:  " << shamt << endl;
            cout << "f. Funct:  " << funct << endl;
            
            if (funct == 32) { // ADD $rd = $rs +$rt
                registers[rd] = registers[rs] + registers[rt];
                cout << "Instruction: add $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 34) { // SUB $rd = $rs - $rt ;
                registers[rd] = registers[rs] - registers[rt];
                cout << "Instruction: sub $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 36) { // AND $rd = $rs & $rt 
                registers[rd] = registers[rs] & registers[rt];
                cout << "Instruction: and $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 37) { // OR $rd = $rs | $rt 
                registers[rd] = registers[rs] | registers[rt];
                cout << "Instruction: or $" << rd << ", $" << rs << ", $" << rt << endl;
            }
             else if (funct == 38) { // XOR $rd = $rs ^ $rt
                registers[rd] = registers[rs] ^ registers[rt];
                cout << "Instruction: xor $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else {
                cout << "Instruction: Unknown R-type (funct = " << funct << ")" << endl;
            }
        } 

        // I-Format Instructions
        else { 
            // I-Format Instructions
            cout << "g. Imm:    " << imm << endl;

            if (opcode == 8) { // ADDI $rt, $rs, $imm $rt = registers[rs] + imm;
                registers[rt] = registers[rs] + imm;
                cout << "Instruction: addi $" << rt << ", $" << rs << ", " << imm << endl;
            }

            else if (opcode == 35) { // LW registers[rt] = memory[rs + imm]  $rt, imm($rs)  -> R[$rt] = M[rs+imm];
                int addr = registers[rs] + imm;  // $2 + offset

                if (addr < 0 || addr >= 256) {
                     cout << "Error: lw address out of range: " << addr << endl;
                } else {
                    registers[rt] = memory[addr];
                     cout << "Instruction: lw $" << rt << ", " << imm << "($" << rs << ")  -> R[$" << rt << "] = M[" << addr << "]" << endl;
                    }
            }    
            
            
            else if (opcode == 43) { // SW memory[rt] = registers[rt]  $rt, imm($rs)  -> M[rs+imm] = R[$rt];
                int addr = registers[rs] + imm;  

            if (addr < 0 || addr >= 256) {
                cout << "Error: sw address out of range: " << addr << endl;
            } else {
                memory[addr] = registers[rt];
                cout << "Instruction: sw $" << rt << ", " << imm << "($" << rs << ")  -> M[" << addr << "] = R[$" << rt << "]" << endl;
                    }
            }

            // Branch Instructions, checks if $rs = $rt are the same or not
            
            else if (opcode == 4) { // beq registers[rs] == registers[rt]
                if (registers[rs] == registers[rt])
                     cout << "Instruction: beq $" << rt << ", $" << rs << ", " << imm << "  (Branch Taken)" << endl;
                else
                     cout << "Instruction: beq $" << rt << ", $" << rs << ", " << imm << "  (Branch Not Taken)" << endl;
            }
            else if (opcode == 5) { // bne registers[rs] != registers[rt]
                if (registers[rs] != registers[rt])
                    cout << "Instruction: bne $" << rs << ", $" << rt << ", " << imm << "  (Branch Taken)" << endl;
                else
                    cout << "Instruction: bne $" << rs << ", $" << rt << ", " << imm << "  (Branch Not Taken)" << endl;
            }
            else {
                cout << "Instruction: Unknown I-type (opcode = " << opcode << ")" << endl;
            }
        }

        // Final Display
        displayState(registers, memory);
    }

    return 0;
}