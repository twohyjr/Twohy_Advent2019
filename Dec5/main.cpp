#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum InputModes {
    POSITION, // read by index
    IMMEDIATE // read by value
};

struct OpcodeProgram {
    int opcode;
    InputModes param1InputMode;
    InputModes param2InputMode;
    InputModes param3InputMode;
};

std::vector<int> readComputerProgramOpcodes(std::string filepath) {
    std::vector<int> massValues;
    std::ifstream inputFile;
    std::string line;
    try {
        inputFile.open(filepath);
        while(getline(inputFile,line)) {
            std::stringstream   linestream(line);
            std::string opcodeString;
            int opcode;
            while(getline(linestream, opcodeString, ',')) {
                std::stringstream opcodeStream(opcodeString);
                opcodeStream >> opcode;
                massValues.push_back(opcode);
            }
        }
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::INPUT_FILE::FILE_NOT_SUCCESFULLY_READ::" << e.what() << std::endl;
    }
    
    return massValues;
}

void padWithZeros(std::string &value, int desiredLength) {
    while(value.size() < desiredLength) {
        value.insert(0, "0"); // add zero to beginning
    }
}

OpcodeProgram getOpcodeProgram(int programInstruction) {
    // Convert program instruction to string then pad with zeros
    std::string programInstructionString = std::to_string(programInstruction);
    padWithZeros(programInstructionString, 5);
    
    // Get opcode from leftmost 2 digits
    std::string opcodeString = programInstructionString.substr(3, 5);
    int opcode;
    std::stringstream opcodeStringStream(opcodeString);
    opcodeStringStream >> opcode;

    // Get input mode of first parameter
    char inputMode1Char = programInstructionString[2];
    InputModes inputMode1 = inputMode1Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;
    
    // Get input mode of second parameter
    char inputMode2Char = programInstructionString[1];
    InputModes inputMode2 = inputMode2Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;
    
    // Get input mode of third parameter
    char inputMode3Char = programInstructionString[0];
    InputModes inputMode3 = inputMode3Char == '0' ? InputModes::POSITION : InputModes::IMMEDIATE;

    return OpcodeProgram{
        opcode,
        inputMode1,
        inputMode2,
        inputMode3
    };
}

//Adds together numbers read from two positions and stores the result in a
//third position. The three integers immediately after the opcode tell you these three
//positions - the first two indicate the positions from which you should read the input
//values, and the third indicates the position at which the output should be stored.
void processOpcode1(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    
    opcodes[param3] = value1 + value2;
}

//Works exactly like opcode 1, except it multiplies the two inputs instead of
//adding them. Again, the three integers after the opcode indicate where the inputs and
//outputs are, not their values.
void processOpcode2(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    
    opcodes[param3] = value1 * value2;
}

//Takes a single integer as input and saves it to the position given by its
//only parameter. For example, the instruction 3,50 would take an input value and
//store it at address 50.
void processOpcode3(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int id) {
    opcodes[param1] = id;
}

//Outputs the value of its only parameter. For example, the instruction 4,50
//would output the value at address 50.
int processOpcode4(std::vector<int> const &opcodes,
                   int address) {
    return opcodes[address];
}

//jump-if-true: if the first parameter is non-zero, it sets the instruction
//pointer to the value from the second parameter. Otherwise, it does nothing.
void processOpcode5(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int &instructionPointer) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    
    if(value1 != 0) {
        instructionPointer = value2;
    } else {
        instructionPointer += 3;
    }
}

//jump-if-false: if the first parameter is zero, it sets the instruction pointer
//to the value from the second parameter. Otherwise, it does nothing.
void processOpcode6(std::vector<int> &opcodes,
                    OpcodeProgram program,
                    int param1,
                    int param2,
                    int &instructionPointer) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    
    if(value1 == 0) {
        instructionPointer = value2;
    } else {
        instructionPointer += 3;
    }
}

//less than: if the first parameter is less than the second parameter, it
//stores 1 in the position given by the third parameter. Otherwise, it stores 0.
void procesOpcode7(std::vector<int> &opcodes,
                   OpcodeProgram program,
                   int param1,
                   int param2,
                   int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    int value3 = param3;

    if(value1 < value2) {
        opcodes[value3] = 1;
    } else {
        opcodes[value3] = 0;
    }
}

//equals: if the first parameter is equal to the second parameter, it
//stores 1 in the position given by the third parameter. Otherwise, it stores 0.
void procesOpcode8(std::vector<int> &opcodes,
                   OpcodeProgram program,
                   int param1,
                   int param2,
                   int param3) {
    int value1 = program.param1InputMode == InputModes::POSITION ? opcodes[param1] : param1;
    int value2 = program.param2InputMode == InputModes::POSITION ? opcodes[param2] : param2;
    int value3 = param3;

    if(value1 == value2) {
        opcodes[value3] = 1;
    } else {
        opcodes[value3] = 0;
    }
}

// Executes the opcodes program based on the given id
int runProgram(std::vector<int> &opcodes, int id) {
    std::vector<int> outputs;

    int instructionPointer = 0;
    bool programIsTerminated = false;
    while(!programIsTerminated) {
        if(instructionPointer == opcodes.size() - 1) {
            programIsTerminated = true;
        }

        OpcodeProgram program = getOpcodeProgram(opcodes[instructionPointer]);
        int opcode = program.opcode;
        
        switch (opcode) {
            case 1:
                processOpcode1(opcodes,
                               program,
                               opcodes[instructionPointer + 1],
                               opcodes[instructionPointer + 2],
                               opcodes[instructionPointer + 3]);
                instructionPointer += 4;
                break;
            case 2:
                processOpcode2(opcodes,
                               program,
                               opcodes[instructionPointer + 1],
                               opcodes[instructionPointer + 2],
                               opcodes[instructionPointer + 3]);
                instructionPointer += 4;
                break;
            case 3:
                processOpcode3(opcodes,
                               program,
                               opcodes[instructionPointer + 1],
                               id);
                instructionPointer += 2;
                break;
            case 4:
                outputs.push_back(processOpcode4(opcodes, opcodes[instructionPointer + 1]));
                instructionPointer += 2;
                break;
            case 5:
                processOpcode5(opcodes,
                               program,
                               opcodes[instructionPointer + 1],
                               opcodes[instructionPointer + 2],
                               instructionPointer);
                break;
            case 6:
                processOpcode6(opcodes,
                               program,
                               opcodes[instructionPointer + 1],
                               opcodes[instructionPointer + 2],
                               instructionPointer);
                break;
            case 7:
                procesOpcode7(opcodes,
                              program,
                              opcodes[instructionPointer + 1],
                              opcodes[instructionPointer + 2],
                              opcodes[instructionPointer + 3]);
                instructionPointer += 4;
                break;
            case 8:
                procesOpcode8(opcodes,
                              program,
                              opcodes[instructionPointer + 1],
                              opcodes[instructionPointer + 2],
                              opcodes[instructionPointer + 3]);
                instructionPointer += 4;
                break;
            case 99:
                programIsTerminated = true;
                break;
            default:
                programIsTerminated = true;
                std::cout << "Something horrible has happened" << std::endl;
                break;
        }
    }

    return id == 1 ? outputs[outputs.size() - 1] : outputs[0];
}

int main(int argc, const char * argv[]) {
    
    // Part 1
    auto opcodes1 = readComputerProgramOpcodes("Dec5_input");
    int programOutput1 = runProgram(opcodes1, 1);
    std::cout << "Part 1: The final program output is: " << programOutput1 << std::endl;
    // Answer -- 15386262
    
    // Part 2
    auto opcodes2 = readComputerProgramOpcodes("Dec5_input");
    int programOutput2 = runProgram(opcodes2, 5);
    std::cout << "Part 2: The final program output is: " << programOutput2 << std::endl;
    // Answer -- 10376124
    
    return 0;
}
