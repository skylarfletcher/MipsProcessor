// Skylar Fletcher + Alaysia 12/9/2025 
/*Instructions

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

1. Create a function to convert hex to binary
2. Create a function to parse binary instruction into its components (opcode, rs, rt, rd, shamt, funct)
3. Create a function to execute the instruction based on the opcode
4. Create a function to display the contents of registers and memory
5. In the main function, accept hex input, validate it, convert to binary, parse, execute, and display results  

*/

#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

// Function to convert hex string to binary string
string hexToBinary(const string& hex) {
    // Initialize an empty binary string
    string binary;

    // Ensure the hex string is 8 characters long
    for (char c : hex) {
        // Convert each hex digit to a 4-bit binary string
        bitset<4> bits(stoi(string(1, c), nullptr, 16));
        // Append the 4-bit binary string to the overall binary string
        binary += bits.to_string();
    }
    return binary;
}


// Function to parse binary instruction into its components
void parseInstruction(const string& binary, string& opcode, string& rs, string& rt, 
                      string& rd, string& shamt, string& funct, string& immediate) {
    opcode = binary.substr(0, 6);      // Extract the opcode (first 6 bits)
    rs = binary.substr(6, 5); // Extract the rs field (next 5 bits)
    rt = binary.substr(11, 5); // Extract the rt field (next 5 bits)
    rd = binary.substr(16, 5); // Extract the rd field (next 5 bits)
    shamt = binary.substr(21, 5); // For R-format instructions
    funct = binary.substr(26, 6); // For R-format instructions
    immediate = binary.substr(16, 16); // For I-format instructions
}

// Function to execute the instruction based on the opcode
void executeInstruction(const string& opcode, const string& rs, const string& rt, 
                        const string& rd, const string& shamt, const string& funct, 
                        const string& immediate, vector<int>& registers, vector<int>& memory) {
    int rsIndex = stoi(rs, nullptr, 2);
    int rtIndex = stoi(rt, nullptr, 2);
    int rdIndex = stoi(rd, nullptr, 2);
    int shamtValue = stoi(shamt, nullptr, 2);
    int immediateValue = stoi(immediate, nullptr, 2);  

    if (opcode == "000000") { // R-format instructions
        if (funct == "100000") { // add
            registers[rdIndex] = registers[rsIndex] + registers[rtIndex];
            cout << "add $" << rdIndex << ", $" << rsIndex << ", $" << rtIndex << endl;
        } else if (funct == "100010") { // sub
            registers[rdIndex] = registers[rsIndex] - registers[rtIndex];
            cout << "sub $" << rdIndex << ", $" << rsIndex << ", $" << rtIndex << endl;
        } else if (funct == "100100") { // and
            registers[rdIndex] = registers[rsIndex] & registers[rtIndex];
            cout << "and $" << rdIndex << ", $" << rsIndex << ", $" << rtIndex << endl;
        } else if (funct == "100101") { // or
            registers[rdIndex] = registers[rsIndex] | registers[rtIndex];
            cout << "or $" << rdIndex << ", $" << rsIndex << ", $" << rtIndex << endl;
        } else if (funct == "100110") { // xor
            registers[rdIndex] = registers[rsIndex] ^ registers[rtIndex];
            cout << "xor $" << rdIndex << ", $" << rsIndex << ", $" << rtIndex << endl;
        }
    } else { // I-format instructions
        if (opcode == "001000") { // addi
            registers[rtIndex] = registers[rsIndex] + immediateValue;
            cout << "addi $" << rtIndex << ", $" << rsIndex << ", " << immediateValue << endl;
        } else if (opcode == "100011") { // lw
            registers[rtIndex] = memory[immediateValue];
            cout << "lw $" << rtIndex << ", " << immediateValue << "($" << rsIndex << ")" << endl;
        } else if (opcode == "101011") { // sw
            memory[immediateValue] = registers[rtIndex];
            cout << "sw $" << rtIndex << ", " << immediateValue << "($" << rsIndex << ")" << endl;
        } else if (opcode == "000100") { // beq
            if (registers[rsIndex] == registers[rtIndex]) {
                cout << "beq $" << rsIndex << ", $" << rtIndex << ", " << immediateValue << endl;
            }
        } else if (opcode == "000101") { // bne
            if (registers[rsIndex] != registers[rtIndex]) {
                cout << "bne $" << rsIndex << ", $" << rtIndex << ", " << immediateValue << endl;
            }
        }
    }
}       

// Function to display the contents of registers and memory
void displayState(const vector<int>& registers, const vector<int>& memory) {

    cout << "Registers:" << endl;
    for (size_t i = 0; i < 16; ++i) {
        cout << "R[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << registers[i] << endl;
    }
    cout << "Memory:" << endl;
    for (size_t i = 0; i < 16; ++i) {
        cout << "M[" << i << "] - " << setw(4) << setfill('0') << hex << uppercase << memory[i] << endl;
    }
}

int main() {

    cout << "MIPS Processor Simulation" << endl << endl;

    vector<int> registers(32, 0); // Initialize 32 registers to 0
    vector<int> memory(256, 0);   // Initialize memory with 256 words to 0

    // Initialize first 16 registers and memory locations to their index values
    // (This matches the output format shown in the assignment: $0: 0000, $1: 0001, ... $15: 0015)
    for (int idx = 0; idx < 16; ++idx) {
        registers[idx] = idx;
        memory[idx] = idx;
    }

    // STEP 1: Display initial register and memory state
    cout << "STEP 1: Display the 16 registers and the 16 memory locations" << endl;
    cout << setfill('0');
    for (int i = 0; i < 16; ++i) {
        cout << "$" << i << ":\t" << setw(4) << hex << uppercase << registers[i];
        cout << "\t\t\tMemory[$" << i << "]: " << setw(4) << hex << uppercase << memory[i] << endl;
    }
    cout << dec << endl;

    // STEP 2: Ask how many instructions to execute
    int numInstructions = 0;
    cout << "STEP 2: How many instructions do you want to execute? (optional): ";
    cin >> numInstructions;
    cout << endl;

    // STEP 3 & 4: Loop to accept and execute instructions
    for (int i = 0; i < numInstructions; ++i) {
        string hexInstruction;
        cout << "STEP 3: Please enter a hexadecimal instruction (8 bits): ";
        cin >> hexInstruction;

        // Input validation
        if (hexInstruction.length() != 8 || !all_of(hexInstruction.begin(), hexInstruction.end(), ::isxdigit)) {
            cerr << "Invalid input. Please enter exactly 8 hexadecimal digits." << endl;
            i--; // Retry this iteration
            continue;
        }

        // Convert hex to binary
        string binaryInstruction = hexToBinary(hexInstruction);
        
        cout << endl << "STEP 4:" << endl;
        cout << "Binary value: " << binaryInstruction << endl;

        // Parse the binary instruction
        string opcode, rs, rt, rd, shamt, funct, immediate;
        parseInstruction(binaryInstruction, opcode, rs, rt, rd, shamt, funct, immediate);

        // Determine format and display
        if (opcode == "000000") {
            cout << "Format: R-format" << endl;
        } else {
            cout << "Format: I-format" << endl;
        }

        cout << "Op code = " << stoi(opcode, nullptr, 2) << endl;
        cout << "Rs = $" << stoi(rs, nullptr, 2) << endl;
        cout << "Rt = $" << stoi(rt, nullptr, 2) << endl;
        cout << "Rd = $" << stoi(rd, nullptr, 2) << endl;
        if (opcode == "000000") {
            cout << "Funct = " << stoi(funct, nullptr, 2) << endl;
        } else {
            cout << "Immediate = " << stoi(immediate, nullptr, 2) << endl;
        }

        // Execute the instruction
        executeInstruction(opcode, rs, rt, rd, shamt, funct, immediate, registers, memory);

        // Display the state of registers and memory
        cout << endl << "Display the results in the appropriate registers" << endl;
        cout << "Data in Registers is Hexadecimal\tHexadecimal" << endl;
        cout << setfill('0');
        for (int j = 0; j < 16; ++j) {
            cout << "$" << j << ":\t" << setw(4) << hex << uppercase << registers[j];
            cout << "\t\t\tMemory[$" << j << "]: " << setw(4) << hex << uppercase << memory[j] << endl;
        }
        cout << dec << endl;
    }

    return 0;
}



