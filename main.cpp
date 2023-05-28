#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "Instructions.h"

#define ll long long

// Global Variables

ll initialAddress;
vector<vector<string>> PI; // each vector in this 2D vector is an instruction (ex [add,t0,t1,t2] )
map<string, bool> hasParantheses;
ifstream instructionsInput, memoryInput;

// utility functions

bool check_address(ll);
int find_reg(string);
string regNumToName(int);
string lowercase(string);
void populateParantheses();
string handleParantheses(string);
void assignAddressToInstruction(vector<string>&, int& i);

// file handling

void open_file(string, int);
void remove_spaces(vector<string>&);
vector <string> clean();
void parse(vector<string>&);

// printing functions

void printInstruction();
void printRegisterContents();
void printMemoryContents();

void run_program();
void initialize_memory();


int main()
{
    cout << "Welcome to RV32I Simulator!\n";

    string assembly_file;
    cout << "Enter the name of the assembly file with the extension (ex: assembly.txt): ";
    cin >> assembly_file;
    open_file(assembly_file, 1);

    cout << "Enter the address of the first instruction (non-negative number): ";
    cin >> initialAddress;

    while (!check_address(initialAddress)) {
        cout << "Address not in range. Enter an address between 0 and 2^32 - 1: ";
        cin >> initialAddress;
    }
    PC = (int)initialAddress;

    int option;
    cout << "To load data into the memory press 1, else press 2: ";
    cin >> option;

    

    if (option == 1) {
        string memory_file;
        cout << "Enter the name of the memory file with the extension (ex: memory.txt): ";
        cin >> memory_file;
        open_file(memory_file, 2);
        initialize_memory();
    }



    populateParantheses();
    vector<string> cleaned_instruction = clean();
    parse(cleaned_instruction);

  
    cout << "\n";
    while (1) {
        cout << "Instruction under execution: ";
        printInstruction();
        cout << "Program Counter: " << PC << "\n\n";
        run_program();
        printRegisterContents();
        printMemoryContents();
        cout << "-------------------------------------------------------\n";
    }
}

void printInstruction()
{
    int index = (PC - initialAddress) / 4;
    if (PI[index].size() == 1) cout << PI[index][0];
    else if (hasParantheses[PI[index][0]]) {
        cout << PI[index][0] << " " << PI[index][1] << ", " << PI[index][3] << "(" << PI[index][2] << ")";
    }
    else if (PI[index].size() == 3) {
        cout << PI[index][0] << " " << PI[index][1] << ", " << PI[index][2];

    }
    else if (PI[index].size() == 4) {
        cout << PI[index][0] << " " << PI[index][1] << ", " << PI[index][2] << ", " << PI[index][3];

    }
    cout << "\n\n";
}

bool check_address(ll address)
{
    //return (address < 0 || address >((ll)1 << 32) - 1) ? 0 : 1;
    return (address < 0 || address >= 2 * pow(10, 9)) ? 0 : 1;
}



void open_file(string file_name, int option)
{
    if (option == 1) {
        instructionsInput.open(file_name);
        while (!instructionsInput.is_open()) {
            cout << "The file\"" << file_name << "\"" << "could not be found" << "\n";
            cout << "Enter the name of the assembly file with the extension (ex: assembly.txt): ";
            cin >> file_name;
            instructionsInput.open(file_name);
        }
    }

    else {
        memoryInput.open(file_name);
        while (!memoryInput.is_open()) {
            cout << "The file\"" << file_name << "\"" << "could not be found" << "\n";
            cout << "Enter the name of the memory file with the extension (ex: memory.txt): ";
            cin >> file_name;
            memoryInput.open(file_name);
        }
    }
}

void initialize_memory()
{
    int address, value;
    while (memoryInput >> address) {
        memoryInput >> value;
        memory.insert({ address,value });
    }
}

string lowercase(string s)
{
    for (int i = 0; i < (int)s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

void remove_spaces(vector<string>& instructions)
{
    vector<string> modified_instructions;
    string temp = "";
    for (string s : instructions) {
        for (char c : s) {
            if (c != ' ') temp += c;
        }
        modified_instructions.push_back(temp);
        temp = "";
    }
    instructions = modified_instructions;
}

void populateParantheses()
{
    hasParantheses["lb"] = 1;
    hasParantheses["lh"] = 1;
    hasParantheses["lw"] = 1;
    hasParantheses["lbu"] = 1;
    hasParantheses["lhu"] = 1;
    hasParantheses["sb"] = 1;
    hasParantheses["sh"] = 1;
    hasParantheses["sw"] = 1;
    hasParantheses["jalr"] = 1;
}

string handleParantheses(string s) {

    string instruction = "", word = "", rd = "", base = "", offset = "";

    int i = 0;
    while (s[i] != ',') word += s[i++];
    i++;
    while (s[i] != ',') rd += s[i++];
    i++;
    while (s[i] != '(') offset += s[i++];
    i++;
    while (s[i] != ')') base += s[i++];

    instruction += word + ',' + rd + ',' + base + ',' + offset + ',';

    return instruction;
}

vector<string> clean()
{
    vector<string> instructions;
    string single_instruction = "";

    string line, word;

    while (getline(instructionsInput, line)) {
        stringstream str(line);


        getline(str, word, ' ');
        if (word.size() == 0) continue;          // handling blank space
        if (line[line.length() - 1] == ':') {   //handling labels
            instructions.push_back(word+',');
            continue;
        }
        
        word = lowercase(word);
        single_instruction += word + ',';
        while (getline(str, word, ',')) {
            single_instruction += word + ',';
        }
        instructions.push_back(single_instruction);
        single_instruction = "";
    }

    remove_spaces(instructions);

    // check if instruction has parantheses
    for (string& s : instructions) {
        if (s.size() > 2) {
            string word = "";
            int i = 0;
            while (s[i] != ',') word += s[i++];
            if (hasParantheses.count(word)) {
                s = handleParantheses(s);
            }
        }
    }

    instructionsInput.close();

    return instructions;
}

void parse(vector<string>& instruction)
{
    vector<string> singleParsedInstruction;

    string word = "";
    int i = 0;
    for (string s : instruction) {
        for (char c : s) {
            if (c != ',') {
                word += c;
            }
            else if (word.size() != 0) {
                singleParsedInstruction.push_back(word);
                word = "";
            }
        }
        if (singleParsedInstruction.size() == 1 && (singleParsedInstruction[0] != "ecall" && singleParsedInstruction[0] != "ebreak" && singleParsedInstruction[0] != "fence")) {
            singleParsedInstruction[0].pop_back();
            labelAddress[singleParsedInstruction[0]] = PC + (4 * i);
        }
        else {
            PI.push_back(singleParsedInstruction);
            i++;
        }
        singleParsedInstruction.clear();
    }

}

void run_program() {

    int index = (PC - initialAddress) / 4;

    if (PI[index][0] == "add") {
        ADD(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "sub") {
        SUB(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "or") {
        OR(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "and") {
        AND(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "xor") {
        XOR(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "sll") {
        SLL(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "srl") {
        SRL(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "sra") {
        SRL(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "slt") {
        SLT(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "sltu") {
        SLTU(find_reg(PI[index][1]), find_reg(PI[index][2]), find_reg(PI[index][3]));
    }
    else if (PI[index][0] == "addi") {
        ADDI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "andi") {
        ANDI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "ori") {
        ORI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "xori") {
        XORI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "slli") {
        SLLI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "srli") {
        SRLI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "srai") {
        SRAI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "jal") {
        JAL(find_reg(PI[index][1]), PI[index][2]);
    }
    else if (PI[index][0] == "jalr") {
        JALR(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "lw") {
        LW(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "lh") {
        LH(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "lb") {
        LB(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "lhu") {
        LHU(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "lbu") {
        LBU(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "slti") {
        SLTI(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "sltiu") {
        SLTIU(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "sw") {
        SW(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "sh") {
        SH(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "sb") {
        SB(find_reg(PI[index][1]), find_reg(PI[index][2]), input_to_decimal(PI[index][3]));
    }
    else if (PI[index][0] == "beq") {
        BEQ(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "bne") {
        BNE(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "blt") {
        BLT(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "bltu") {
        BLTU(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "bge") {
        BGE(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "bgeu") {
        BGEU(find_reg(PI[index][1]), find_reg(PI[index][2]), PI[index][3]);
    }
    else if (PI[index][0] == "lui") {
        LUI(find_reg(PI[index][1]), input_to_decimal(PI[index][2]));
    }
    else if (PI[index][0] == "auipc") {
        AUIPC(find_reg(PI[index][1]), input_to_decimal(PI[index][2]));
    }

    else if (PI[index][0] == "ecall" || PI[index][0] == "ebreak" || PI[index][0] == "fence") {
        cout << "Terminating Program";
        PC += 4;
        cout << "Program Counter: " << PC << "\n\n";
        printRegisterContents();
        printMemoryContents();
        exit(1);
    }
}

int find_reg(string reg)
{
    if (reg == "zero" || reg == "x0") return 0;
    if (reg == "ra" || reg == "x1") return 1;
    if (reg == "sp" || reg == "x2") return 2;
    if (reg == "gp" || reg == "x3") return 3;
    if (reg == "tp" || reg == "x4") return 4;
    if (reg == "t0" || reg == "x5") return 5;
    if (reg == "t1" || reg == "x6") return 6;
    if (reg == "t2" || reg == "x7") return 7;
    if (reg == "s0" || reg == "x8") return 8;
    if (reg == "s1" || reg == "x9") return 9;
    if (reg == "a0" || reg == "x10") return 10;
    if (reg == "a1" || reg == "x11") return 11;
    if (reg == "a2" || reg == "x12") return 12;
    if (reg == "a3" || reg == "x13") return 13;
    if (reg == "a4" || reg == "x14") return 14;
    if (reg == "a5" || reg == "x15") return 15;
    if (reg == "a6" || reg == "x16") return 16;
    if (reg == "a7" || reg == "x17") return 17;
    if (reg == "s2" || reg == "x18") return 18;
    if (reg == "s3" || reg == "x19") return 19;
    if (reg == "s4" || reg == "x20") return 20;
    if (reg == "s5" || reg == "x21") return 21;
    if (reg == "s6" || reg == "x22") return 22;
    if (reg == "s7" || reg == "x23") return 23;
    if (reg == "s8" || reg == "24") return 24;
    if (reg == "s9" || reg == "25") return 25;
    if (reg == "s10" || reg == "26") return 26;
    if (reg == "s11" || reg == "27") return 27;
    if (reg == "t3" || reg == "28") return 28;
    if (reg == "t4" || reg == "29") return 29;
    if (reg == "t5" || reg == "30") return 30;
    if (reg == "t6" || reg == "31") return 31;

    cout << "Invalid register. Exitting Program\n";
    return 1;
}

string regNumToName(int i)
{
    if (i == 0) return "zero";
    else if (i == 1) return "ra";
    else if (i == 2) return "sp";
    else if (i == 3) return "gp";
    else if (i == 4) return "tp";
    else if (i == 5) return "t0";
    else if (i == 6) return "t1";
    else if (i == 7) return "t2";
    else if (i == 8) return "s0";
    else if (i == 9) return "s1";
    else if (i == 10) return "a0";
    else if (i == 11) return "a1";
    else if (i == 12) return "a2";
    else if (i == 13) return "a3";
    else if (i == 14) return "a4";
    else if (i == 15) return "a5";
    else if (i == 16) return "a6";
    else if (i == 17) return "a7";
    else if (i == 18) return "s2";
    else if (i == 19) return "s3";
    else if (i == 20) return "s4";
    else if (i == 21) return "s5";
    else if (i == 22) return "s6";
    else if (i == 23) return "s7";
    else if (i == 24) return "s8";
    else if (i == 25) return "s9";
    else if (i == 26) return "s10";
    else if (i == 27) return "s11";
    else if (i == 28) return "t3";
    else if (i == 29) return "t4";
    else if (i == 30) return "t5";
    else if (i == 31) return "t6";
    return 0;
}


void printRegisterContents()
{
    cout << "\t\t\t  Registers\n";
    cout << "-----------------------------------------------------------------\n";
    cout << "   Name    |    Number    |    Value (decimal)   | Value (hex)   \n";
    cout << "-----------------------------------------------------------------\n";
    for (int i = 0; i < 32; i++) {
        cout << setw(4) << regNumToName(i) << setw(8) << "|";
        cout << setw(2) << i;
        cout << setw(13) << "|";
        cout << setw(3) << registers[i];
        cout << setw(20) << "|";
        cout << setw(3) << decimalToHex(registers[i]) << "\n";
    }
}


void printMemoryContents()
{
    cout << "\n----------Memory----------:\n\n";
    for (auto u : memory) {
        cout << "Address: " << u.first << "\t\t" << "Memory: " << u.second << "\n";
    }
    cout << "\n";
}
