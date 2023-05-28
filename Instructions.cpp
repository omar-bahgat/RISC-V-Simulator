#include <iostream>
#include <iomanip>
using namespace std;

#include "Instructions.h"

#define ll long long

int PC;                          // program counter
int registers[32];               // initial value of all registers is 0
map<int, int> memory;            // {address, value}
map<string, int> labelAddress;

vector<int> SP;

string decimalToHex(int decimal)
{
    stringstream ss;
    ss << hex << decimal;
    string res(ss.str());
    return res;
}

int input_to_decimal(string n)
{
    int result = 0;
    if (n[0] == '0' && n[1] == 'x')
    {
        cout << "hex detected: "<<n<<'\n';
        for (int i = 2; i < n.length(); i++)
        {
            if (n[i] >= '0' && n[i] <= '9')
            {
                result += (n[i] - '0') * pow(16, n.length() - i - 1);
            }
            else if (n[i] >= 'A' && n[i] <= 'F')
            {
                result += (n[i] - 'A' + 10) * pow(16, n.length() - i - 1);
            }
            else if (n[i] >= 'a' && n[i] <= 'f')
            {
                result += (n[i] - 'a' + 10) * pow(16, n.length() - i - 1);
            }
        }
    }
    else
    {
        cout << "decimal detected: " << n << "\n";
        result = stoi(n);
    }
    return result;
}

void twosComplement(string& binary)
{
    bool firstOne = 0;
    for (int i = binary.length() - 1; i >= 0; i--)
    {
        if (firstOne)
            binary[i] = binary[i] == '0' ? '1' : '0';
        if (binary[i] == '1')
            firstOne = 1;
    }
}

string decimalToBinary_Signed(int decimal,int n = 32)
{
    bool isNegative = 0;
    if (decimal < 0)
    {
        decimal = -1 * decimal;
        isNegative = 1;
    }
    string r = "";
    while (decimal > 0)
    {
        r += to_string(decimal % 2);
        decimal /= 2;
    }
    reverse(r.begin(), r.end());
    while (r.length() != n)
        r.insert(r.begin(), '0');
    if (isNegative)
        twosComplement(r);
    return r;
}

int binaryToDecimal(string binary)
{
    int result = 0;
    int len = binary.length();
    for (int i = 0; i < len; i++) {
        if (binary[i] == '1') {
            result += pow(2, len - i - 1);
        }
    }
    return result;
}

bool check_address_data(ll address) {
    return (address < 2 * pow(10, 9) || address >= 6 * pow(10, 9)) ? 0 : 1;
}

// ------------------------------------Instruction Definitions------------------------------------

//R-type

void ADD(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] + registers[rs2];
    PC += 4;
}
void SUB(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] - registers[rs2];
    PC += 4;
}
void AND(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] & registers[rs2];
    PC += 4;
}
void OR(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] | registers[rs2];
    PC += 4;
}
void XOR(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] ^ registers[rs2];
    PC += 4;
}
void SLL(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    int shift_amount = registers[rs2];

    registers[rd] = registers[rs1] << registers[rs2];
    PC += 4;
}
void SRL(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }

    int shift_amount = registers[rs2];

    registers[rd] = (registers[rs1] >> registers[rs2]);
    PC += 4;
}
void SRA(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }

    int shift_amount = registers[rs2];


    registers[rd] = registers[rs1] >> registers[rs2];
    PC += 4;

}
void SLT(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = (registers[rs1] < registers[rs2]) ? 1 : 0;
    PC += 4;

}
void SLTU(int rd, int rs1, int rs2)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    unsigned int x = registers[rs2];
    registers[rd] = registers[rs1] < x;
    PC += 4;

}

// I-type

void ADDI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"imm\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    if (rd == 2 && rs1 == 2)
    {
        if (imm < 0)
        {
            int x = imm / -4;
            for(int i=0;i<x;i++)
             SP.insert(SP.begin(), 0);
        }
        else
        {
            int x = imm / 4;
            for (int i = 0; i < x; i++)
                SP.erase(SP.begin(), SP.begin() + 1);
        }
    }
    registers[rd] = registers[rs1] + imm;
    PC += 4;
}
void ANDI(int rd, int rs1, int imm)
{
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"imm\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    registers[rd] = registers[rs1] & imm;
    PC += 4;
}
void ORI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"imm\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] | imm;
    PC += 4;
}
void XORI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"imm\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] ^ imm;
    PC += 4;
}
void SLLI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    int shift_amount = imm;
    if (shift_amount > 31 || shift_amount < 0) {
        cout << "Left shift amount " << "\"" << shift_amount << "\" is not in range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] << imm;
    PC += 4;
}
void SRLI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    int shift_amount = imm;
    if (shift_amount > 31 || shift_amount < 0) {
        cout << "Right shift amount " << "\"" << shift_amount << "\" is not in range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] >> imm;
    PC += 4;
}
void SRAI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    int shift_amount = imm;
    if (shift_amount > 31 || shift_amount < 0) {
        cout << "Right shift amount " << "\"" << shift_amount << "\" is not in range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] >> imm;
    PC += 4;
}
void JAL(int rd, string label)
{
    if(rd!=0)
        registers[rd] = PC + 4;
    PC = labelAddress[label];
}
void JALR(int rd, int rs1, int imm)
{
    if (rd != 0)
        registers[rd] = PC + 4;
    PC = registers[rs1] + imm; // return address in in register x1
}
void LW(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    if (rs1 == 2)
    {
        if (imm % 4 != 0)
        {
            cout << "the offset is not in the boundry..." << endl;
            PC += 4;
            return;
        }
        registers[rd] = SP[imm / 4];
        PC += 4;
        return;
    }
    registers[rd] = memory[registers[rs1] + imm];
    PC += 4;
}
void LH(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    int content = memory[registers[rs1] + imm];
    string binaryContent = decimalToBinary_Signed(content);
    string sub = binaryContent.substr(16);
    binaryContent = sub[0] == '1' ? "1111111111111111" + sub : "0000000000000000" + sub;
    registers[rd] = binaryToDecimal(binaryContent);
    PC += 4;
}
void LB(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        exit(1);
    }
    int content = memory[registers[rs1] + imm];
    
    string binaryContent = decimalToBinary_Signed(content);
    string sub = binaryContent.substr(24);
    binaryContent = sub[0] == '1' ? "111111111111111111111111" + sub : "000000000000000000000000" + sub;
    registers[rd] = binaryToDecimal(binaryContent);
    PC += 4;
}
void LHU(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    int content = memory[registers[rs1] + imm];
    string binaryContent = decimalToBinary_Signed(content);
    string sub = binaryContent.substr(16);
    binaryContent =  "0000000000000000" + sub;
    registers[rd] = binaryToDecimal(binaryContent);
    PC += 4;
}
void LBU(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    int content = memory[registers[rs1] + imm];
    string binaryContent = decimalToBinary_Signed(content);
    string sub = binaryContent.substr(24);
    binaryContent = "000000000000000000000000" + sub;
    registers[rd] = binaryToDecimal(binaryContent);
    PC += 4;
}
void SLTI(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    registers[rd] = registers[rs1] < imm;
    PC += 4;
}
void SLTIU(int rd, int rs1, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    if (imm > ((1 << 11) - 1) || imm < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    unsigned int imm_unsigned = imm;
    registers[rd] = registers[rs1] < imm_unsigned;
    PC += 4;
}

// S-type

void SW(int rs1, int base, int offset)
{
    if (offset > ((1 << 11) - 1) || offset < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    if (base == 2)
    {
        if (offset % 4 != 0)
        {
            cout << "the offset is not in the boundry..." << endl;
            return;
        }
        SP[offset / 4] = registers[rs1];
        for (int i = 0; i < SP.size(); i++) cout << SP[i] << "   ";
        cout << endl;
        PC += 4;
        return;
    }
    memory[registers[base] + offset] = registers[rs1];
    PC += 4;
}
void SH(int rs1, int base, int offset)
{

    if (offset > ((1 << 11) - 1) || offset < -(1 << 11)) {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    int content = registers[rs1];
    string binaryContent = decimalToBinary_Signed(content);
    memory[registers[base] + offset] = binaryToDecimal(binaryContent.substr(16));
    PC += 4;
}
void SB(int rs1, int base, int offset)
{

    if (offset > ((1 << 11) - 1) || offset < -(1 << 11))
    {
        cout << "\"offset\" not in allowed range\n";
        PC += 4;
        exit(1);
    }
    int content = registers[rs1];
    string binaryContent = decimalToBinary_Signed(content);
    memory[registers[base] + offset] = binaryToDecimal(binaryContent.substr(24));
    PC += 4;
}

// B-type

void BEQ(int rs1, int rs2, string label)
{
    if (registers[rs1] == registers[rs2]) {
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        PC += 4;
    }
}

void BNE(int rs1, int rs2, string label)
{
    cout << "inside BNE\n";
    if (registers[rs1] != registers[rs2]) {
        cout << "i am inside if\n";
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        cout << "i am here\n";
        PC += 4;
        
    }
}
void BLT(int rs1, int rs2, string label)
{
    if (registers[rs1] < registers[rs2]) {
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        PC += 4;
    }
}
void BLTU(int rs1, int rs2, string label)
{

    unsigned int rst_unsigned = registers[rs2];
    if (registers[rs1] < rst_unsigned) {
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        PC += 4;
    }
}
void BGE(int rs1, int rs2, string label)
{
    if (registers[rs1] >= registers[rs2]) {
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        PC += 4;
    }
}
void BGEU(int rs1, int rs2, string label)
{
    unsigned int rst_unsigned = registers[rs2];
    if (registers[rs1] >= rst_unsigned) {
        if (labelAddress.count(label)) {
            PC = labelAddress[label];
        }
        else {
            cout << "Label not found\n";
            PC += 4;
            exit(1);
        }
    }
    else {
        PC += 4;
    }
}

// U-type

void LUI(int rd, int imm)
{
    if (rd == 0)
    {
        PC += 4;
        return;
    }
    string binaryContent = decimalToBinary_Signed(imm, 20);
    if (binaryContent.length() > 20)
        binaryContent = binaryContent.substr(12);
    string binaryContent_rd = decimalToBinary_Signed(registers[rd]);
    string sub_rd = binaryContent_rd.substr(20);
    registers[rd] = binaryToDecimal(binaryContent + sub_rd);
    PC += 4;
}
void AUIPC(int rd, int imm)
{
    if (rd == 0)
    {
        return;
    }
    if (!(imm > (1 << (20 - 1)) - 1 || imm < -(1 << (20 - 1))))
    {
        if (check_address_data(PC + (imm << 12))) {
            registers[rd] = PC + (imm << 12);
        }
        else {
            cout << "address doesn't fall within the available data segment of 4GB";
            exit(1);
        }
        
    }
    else
    {
        cout << "immediate value out of range\n";
        exit(1);
    }
    PC += 4;
}
