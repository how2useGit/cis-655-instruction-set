#include "alu.h"

ALU::ALU(std::vector<uint16_t> &mem, int pcInit) : memory(mem), pc(pcInit)
{
    M = {{"M0", 0}, {"M1", 1}, {"M2", 2}, {"M3", 3}, {"M4", 4}, {"M5", 5}, {"M6", 6}, {"M7", 7}, {"M8", 8}, {"M9", 9}, {"M10", 10}, {"M11", 11}, {"M12", 12}, {"M13", 13}, {"M14", 14}, {"M15", 15}};
    OPCODES = {{0b0000, "am"}, {0b0001, "sm"}, {0b0010, "mm"}, {0b0011, "dm"}, {0b0100, "bnem"}, {0b0101, "sltm"}, {0b0110, "ai"}, {0b0111, "si"}, {0b1000, "mi"}, {0b1001, "di"}, {0b1010, "lci"}, {0b1011, "sci"}, {0b1100, "lo"}, {0b1101, "so"}, {0b1110, "jb"}};
    MEMORY_REGISTERS = {{0b0000, "M0"}, {0b0001, "M1"}, {0b0010, "M2"}, {0b0011, "M3"}, {0b0100, "M4"}, {0b0101, "M5"}, {0b0110, "M6"}, {0b0111, "M7"}, {0b1000, "M8"}, {0b1001, "M9"}, {0b1010, "M10"}, {0b1011, "M11"}, {0b1100, "M12"}, {0b1101, "M13"}, {0b1110, "M14"}, {0b1111, "M15"}};

    functions.resize(15);
    functions[0] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return am(op1, op2, op3); };
    functions[1] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return sm(op1, op2, op3); };
    functions[2] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return mm(op1, op2, op3); };
    functions[3] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return dm(op1, op2, op3); };
    functions[4] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return bnem(op1, op2, op3); };
    functions[5] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return sltm(op1, op2, op3); };
    functions[6] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return ai(op1, op2, op3); };
    functions[7] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return si(op1, op2, op3); };
    functions[8] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return mi(op1, op2, op3); };
    functions[9] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return di(op1, op2, op3); };
    functions[10] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return lci(op1, op2, op3); };
    functions[11] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return sci(op1, op2, op3); };
    functions[12] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return lo(op1, op2, op3); };
    functions[13] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return so(op1, op2, op3); };
    functions[14] = [this](uint8_t op1, uint8_t op2, uint8_t op3)
    { return jb(op1, op2, op3); };
}

// Add the function implementations for each ALU operation, printing, and executing here...
int ALU::am(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] + memory[op2];
    return 1;
}

// subtract memory-memory
// opcode 0001
int ALU::sm(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] - memory[op2];
    return 1;
}

// multiply memory-memory
// opcode 0010
int ALU::mm(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] * memory[op2];
    return 1;
}

// divide memory-memory
// opcode 0011
int ALU::dm(uint8_t op1, uint8_t op2, uint8_t op3)
{
    if (memory[op2] == 0)
    {
        std::string op = "dm " + std::to_string(op1) + " " + std::to_string(op2) + " " + std::to_string(op3);
        throw std::invalid_argument("Error: Division by zero.\nOPCODE: " + op);
    }
    memory[op3] = memory[op1] / memory[op2];
    return 1;
}

// branch if not equal memory-memory
// opcode 0100
int ALU::bnem(uint8_t op1, uint8_t op2, uint8_t op3)
{
    if (memory[op1] != memory[op2])
    {
        return int(op3); // Adjust program counter
    }
    else
    {
        return 1;
    }
}

// set less than memory-memory
// opcode 0101
int ALU::sltm(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = (memory[op1] < memory[op2]) ? 1 : 0;
    return 1;
}

// add immediate
// opcode 0110
int ALU::ai(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] + op2;
    return 1;
}

// subtract immediate
// opcode 0111
int ALU::si(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] - op2;
    return 1;
}

// multiply immediate
// opcode 1000
int ALU::mi(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[op1] * op2;
    return 1;
}

// divide immediate
// opcode 1001
int ALU::di(uint8_t op1, uint8_t op2, uint8_t op3)
{
    if (op2 == 0)
    {
        std::string op = "di " + std::to_string(op1) + " " + std::to_string(op2) + " " + std::to_string(op3);
        throw std::invalid_argument("Error: Division by zero.\nOPCODE: " + op);
    }
    memory[op3] = memory[op1] / op2;
    return 1;
}

// load constant immediate
// opcode 1010
int ALU::lci(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = op1 + op2;
    return 1;
}

// subtract constant immediate
// opcode 1011
int ALU::sci(uint8_t op1, uint8_t op2, uint8_t op3)
{
    // is this right?
    memory[op3] = op1 - op2;
    return 1;
}

// load using offset
// opcode 1100
int ALU::lo(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[op3] = memory[memory[op1] + memory[op2]];
    return 1;
}

// store using offset
// opcode 1101
int ALU::so(uint8_t op1, uint8_t op2, uint8_t op3)
{
    memory[memory[op1] + memory[op2]] = memory[op3];
    return 1;
}

// jump back
// opcode 1110
int ALU::jb(uint8_t op1, uint8_t op2, uint8_t op3)
{
    return -(int(op1 + op2 + op3));
}

void ALU::printMemory()
{
    int c = 0;
    for (auto const &x : memory)
    {
        std::cout << c << " : " << x << std::endl;
        c++;
    }
}

void ALU::printInColumnsBinary(const std::vector<uint16_t> &list, int columns)
{
    int rows = (list.size() + columns - 1) / columns; // Calculate number of rows needed

    // Print the elements in multiple columns
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            int index = i + j * rows;
            if (index < list.size())
            {

                // print the 16 bit number in binary
                std::cout << std::setw(3) << index << " : " << std::setw(6) << std::bitset<16>(list[index]) << " | ";
            }
        }
        std::cout << std::endl;
    }
}

void ALU::printInColumnsDecimal(const std::vector<uint16_t> &list, int columns)
{
    int rows = (list.size() + columns - 1) / columns; // Calculate number of rows needed

    // Print the elements in multiple columns
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            int index = i + j * rows;
            if (index < list.size())
            {
                // print the 16 bit number in binary
                std::cout << std::setw(3) << index << " : " << std::setw(6) << list[index] << " | ";
            }
        }
        std::cout << std::endl;
    }
}

void ALU::printPC()
{
    std::cout << "PC: " << pc << std::endl;
}

void ALU::printInstruction(uint16_t instruction)
{
    uint8_t opcode = (instruction >> 12) & 0x0F; // Extract bits [15:12]
    uint8_t op1 = (instruction >> 8) & 0x0F;     // Extract bits [11:8]
    uint8_t op2 = (instruction >> 4) & 0x0F;     // Extract bits [7:4]
    uint8_t op3 = instruction & 0x0F;

    std::cout << "OPCODE: " << std::bitset<4>(opcode) << " " << std::bitset<4>(op1) << " " << std::bitset<4>(op2) << " " << std::bitset<4>(op3) << std::endl;
}

void ALU::printExecuting(uint16_t instruction)
{
    uint8_t opcode = (instruction >> 12) & 0x0F; // Extract bits [15:12]
    uint8_t op1 = (instruction >> 8) & 0x0F;     // Extract bits [11:8]
    uint8_t op2 = (instruction >> 4) & 0x0F;     // Extract bits [7:4]
    uint8_t op3 = instruction & 0x0F;

    if (opcode == 0b1110)
    {
        std::cout << "EXECUTING: " << OPCODES[opcode] << " " << std::to_string(op1) << " " << std::to_string(op2) << " " << std::to_string(op3) << std::endl;
        return;
    }
    else if (opcode == 0b1010 || opcode == 0b1011)
    {
        std::cout << "EXECUTING: " << OPCODES[opcode] << " " << std::to_string(op1) << " " << std::to_string(op2) << " " << MEMORY_REGISTERS[op3] << std::endl;
        return;
    }
    else if (opcode == 0b0110 || opcode == 0b0111 || opcode == 0b1000 || opcode == 0b1001)
    {
        std::cout << "EXECUTING: " << OPCODES[opcode] << " " << MEMORY_REGISTERS[op1] << " " << std::to_string(op2) << " " << MEMORY_REGISTERS[op3] << std::endl;
        return;
    }
    else if (opcode == 0b0100)
    {
        std::cout << "EXECUTING: " << OPCODES[opcode] << " " << MEMORY_REGISTERS[op1] << " " << MEMORY_REGISTERS[op2] << " " << std::to_string(op3) << std::endl;
        return;
    }

    std::cout << "EXECUTING: " << OPCODES[opcode] << " " << MEMORY_REGISTERS[op1] << " " << MEMORY_REGISTERS[op2] << " " << MEMORY_REGISTERS[op3] << std::endl;
}

// execute(0000 0000 0000 0000)
int ALU::execute(uint16_t instruction)
{
    uint8_t opcode = (instruction >> 12) & 0x0F; // Extract bits [15:12]
    uint8_t op1 = (instruction >> 8) & 0x0F;     // Extract bits [11:8]
    uint8_t op2 = (instruction >> 4) & 0x0F;     // Extract bits [7:4]
    uint8_t op3 = instruction & 0x0F;

    if (opcode >= 0 && opcode < functions.size())
    {
        pc += functions[opcode](op1, op2, op3);
    }
    else
    {
        throw std::invalid_argument("Error: Invalid opcode." + std::to_string(opcode));
    }

    return pc;
}

std::vector<int> ALU::getAffectedRegisters(uint16_t instruction) 
{
    std::vector<int> affectedRegisters;
    uint8_t opcode = (instruction >> 12) & 0x0F;        // Extract opcode

    switch (opcode) 
    {
        case 0b1010: // lci instruction
        case 0b1011: // sci (Subtract Constant Immediate)
        {
            uint8_t dest = instruction & 0x0F;          // Destination register
            affectedRegisters.push_back(dest);
            break;
        }
        case 0b0000: // am (Add Memory)
        case 0b0001: // sm (Subtract Memory)
        case 0b0010: // mm (Multiply Memory)
        case 0b0011: // dm (Divide Memory)
        case 0b0101: // sltm (Set Less Than Memory)
        case 0b0110: // ai (Add Immediate)
        case 0b0111: // si (Subtract Immediate)
        case 0b1000: // mi (Multiply Immediate)
        case 0b1001: // di (Divide Immediate)
        case 0b1100: // lo (Load using Offset)
        case 0b1101: // so (Store using Offset)
        {
            uint8_t op1 = (instruction >> 8) & 0x0F;    // First operand
            uint8_t op2 = (instruction >> 4) & 0x0F;    // Second operand
            uint8_t dest = instruction & 0x0F;          // Destination register
            affectedRegisters.push_back(op1);           // Source 1
            affectedRegisters.push_back(op2);           // Source 2
            affectedRegisters.push_back(dest);          // Destination
            break;
        }
        default:
            break; // No registers affected
    }
    
    return affectedRegisters;
}

void ALU::printAligned(const std::string& label, const std::string& value) 
{
    const int regWidth = 20; 
    const int valWidth = 0;

    // Using fixed width for alignment
    std::cout << std::left << std::setw(regWidth) << label 
              << std::setw(valWidth) << "| " << value << std::endl;
}

void ALU::printAffectedRegisterBefore(uint16_t instruction) 
{
    std::vector<int> affectedRegisters = getAffectedRegisters(instruction);
    uint8_t opcode = (instruction >> 12) & 0x0F;        // Extract opcode

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "BEFORE EXECUTION:" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "MEMORY REGISTERS    | CONTENTS" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    if (opcode == 0b0100 || opcode == 0b1110)
    { 
        printAligned("NO MEM REG AFFECTED", "NO MEM REG AFFECTED");
    } else if (affectedRegisters.empty()) 
    {
        printAligned("CURR --> NULL", "NULL");
    } else 
    {
        for (size_t i = 0; i < affectedRegisters.size(); ++i) 
        {
            int reg = affectedRegisters[i];
            // Print previous register if applicable
            if (reg > 0) 
            {
                printAligned("PREV --> M" + std::to_string(reg - 1), std::to_string(memory[reg - 1]));
            } else 
            {
                printAligned("PREV --> NULL", "NULL");
            }

            // Print current register
            printAligned("CURR --> M" + std::to_string(reg), std::to_string(memory[reg]));

            // Print next register if applicable
            if (reg < memory.size() - 1) 
            {
                printAligned("NEXT --> M" + std::to_string(reg + 1), std::to_string(memory[reg + 1]));
            } else 
            {
                printAligned("NEXT --> NULL", "NULL");
            }

            // Add a line break only if there are more registers to print
            if (i < affectedRegisters.size() - 1) {
                std::cout << std::endl; // Add line break between entries
            }
        }
    }
        std::cout << "---------------------------------------------" << std::endl;
}

void ALU::printAffectedRegisterAfter(uint16_t instruction) 
{
    std::vector<int> affectedRegisters = getAffectedRegisters(instruction);
    uint8_t opcode = (instruction >> 12) & 0x0F;        // Extract opcode

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "AFTER EXECUTION:" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "MEMORY REGISTERS    | CONTENTS" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;

    if (opcode == 0b0100 || opcode == 0b1110)
    {
        printAligned("NO MEM REG AFFECTED", "NO MEM REG AFFECTED");
    } else if (affectedRegisters.empty()) 
    {
        printAligned("CURR --> NULL", "NULL");
    } else 
    {
        for (size_t i = 0; i < affectedRegisters.size(); ++i) 
        {
            int reg = affectedRegisters[i];
            // Print previous register if applicable
            if (reg > 0) 
            {
                printAligned("PREV --> M" + std::to_string(reg - 1), std::to_string(memory[reg - 1]));
            } else 
            {
                printAligned("PREV --> NULL", "NULL");
            }

            // Print current register
            printAligned("CURR --> M" + std::to_string(reg), std::to_string(memory[reg]));

            // Print next register if applicable
            if (reg < memory.size() - 1) 
            {
                printAligned("NEXT --> M" + std::to_string(reg + 1), std::to_string(memory[reg + 1]));
            } else 
            {
                printAligned("NEXT --> NULL", "NULL");
            }

            // Add a line break only if there are more registers to print
            if (i < affectedRegisters.size() - 1) 
            {
                std::cout << std::endl;
            }
        }
    }
    std::cout << "---------------------------------------------" << std::endl;
}

void ALU::setPC(int newPC)
{
    pc = newPC;
}

int ALU::getPC()
{
    return pc;
}
