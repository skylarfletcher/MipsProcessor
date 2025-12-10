#include <iostream>
#include <string>
#include <iomanip> // Used for hex output formatting

using namespace std;

// HELPER FUNCTION: Converts a single Hex character (0-F) to a 4-bit Binary string
string hexCharToBinary(char hex) {
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

// HELPER FUNCTION: Displays the current state of Registers and Memory
void displayState(int regs[], int mem[]) {
    cout << "\n--- Current State ---\n";
    cout << "Registers (0-15)\t\tMemory (0-15)\n";
    
    for (int i = 0; i < 16; ++i) {
        cout << "R[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << regs[i];
        cout << "\t\tM[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << mem[i] << endl;
    }
    cout << dec; // Switch back to decimal
    cout << "---------------------\n";
}

int main() {
    // 1. SETUP: Create the imaginary CPU parts
    int registers[32]; 
    int memory[256];

    // Initialize them
    for (int i = 0; i < 32; i++) {
        registers[i] = i; 
    }
    for (int i = 0; i < 256; i++) {
        memory[i] = i; 
    }

    cout << "SIMPLE MIPS SIMULATOR" << endl;
    displayState(registers, memory);

    int numInstructions;
    cout << "How many instructions do you want to run? ";
    cin >> numInstructions;

    // 2. MAIN LOOP
    for (int k = 0; k < numInstructions; k++) {
        string hexInput;
        cout << "\nEnter 8-digit Hex instruction (e.g. 00642820): ";
        cin >> hexInput;

        // Validation
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

        // Convert strings to Integers
        int opcode = stoi(opcodeStr, nullptr, 2);
        int rs     = stoi(rsStr, nullptr, 2);
        int rt     = stoi(rtStr, nullptr, 2);
        int rd     = stoi(rdStr, nullptr, 2);
        int shamt  = stoi(shamtStr, nullptr, 2); // Shift Amount
        int funct  = stoi(functStr, nullptr, 2);
        int imm    = stoi(immStr, nullptr, 2);

        // STEP 3: Parse and Display Components (AS REQUESTED)
        cout << "\nParsed Components:" << endl;
        cout << "a. Opcode: " << opcode << endl;
        cout << "b. Rs:     " << rs << " ($" << rs << ")" << endl;
        cout << "c. Rt:     " << rt << " ($" << rt << ")" << endl;
        cout << "d. Rd:     " << rd << " ($" << rd << ")" << endl;
        cout << "e. Shamt:  " << shamt << endl;
        cout << "f. Funct:  " << funct << endl; 
        cout << "--------------------------------" << endl;

        // STEP 4: Execute logic
        if (opcode == 0) { 
            // R-Format Instructions
            cout << "Format: R-Type" << endl;
            
            if (funct == 32) { // add
                registers[rd] = registers[rs] + registers[rt];
                cout << "Instruction: add $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 34) { // sub
                registers[rd] = registers[rs] - registers[rt];
                cout << "Instruction: sub $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 36) { // and
                registers[rd] = registers[rs] & registers[rt];
                cout << "Instruction: and $" << rd << ", $" << rs << ", $" << rt << endl;
            }
            else if (funct == 37) { // or
                registers[rd] = registers[rs] | registers[rt];
                cout << "Instruction: or $" << rd << ", $" << rs << ", $" << rt << endl;
            }
             else if (funct == 38) { // xor
                registers[rd] = registers[rs] ^ registers[rt];
                cout << "Instruction: xor $" << rd << ", $" << rs << ", $" << rt << endl;
            }
        } 
        else { 
            // I-Format Instructions
            cout << "Format: I-Type" << endl;

            if (opcode == 8) { // addi
                registers[rt] = registers[rs] + imm;
                cout << "Instruction: addi $" << rt << ", $" << rs << ", " << imm << endl;
            }
            else if (opcode == 35) { // lw
                registers[rt] = memory[imm];
                cout << "Instruction: lw $" << rt << ", " << imm << "($" << rs << ")" << endl;
            }
            else if (opcode == 43) { // sw
                memory[imm] = registers[rt];
                cout << "Instruction: sw $" << rt << ", " << imm << "($" << rs << ")" << endl;
            }
            else if (opcode == 4) { // beq
                if (registers[rs] == registers[rt])
                    cout << "Instruction: beq (Branch Taken!)" << endl;
                else
                    cout << "Instruction: beq (Branch Not Taken)" << endl;
            }
            else if (opcode == 5) { // bne
                if (registers[rs] != registers[rt])
                    cout << "Instruction: bne (Branch Taken!)" << endl;
                else
                    cout << "Instruction: bne (Branch Not Taken)" << endl;
            }
        }

        // Final Display
        displayState(registers, memory);
    }

    return 0;
}