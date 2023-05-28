#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <sstream>
#include <cstdlib>
using namespace std;

#define ll long long

extern int PC;                          // program counter
extern int registers[32];               // initial value of all registers is 0
extern map<int, int> memory;            // {address, value}
extern map<string, int> labelAddress;

// Utility functions

void print(map<string, int>& m);
void print(vector<vector<string>> v);
void print(vector<string> v);
string decimalToHex(int decimal);
int input_to_decimal(string n);


 // R-type
void ADD(int rd, int rs1, int rs2);
void SUB(int rd, int rs1, int rs2);
void AND(int rd, int rs1, int rs2);
void OR(int rd, int rs1, int rs2);
void XOR(int rd, int rs1, int rs2);
void SLL(int rd, int rs1, int rs2);
void SRL(int rd, int rs1, int rs2);
void SRA(int rd, int rs1, int rs2);
void SLT(int rd, int rs1, int rs2);
void SLTU(int rd, int rs1, int rs2);

// I-type
void ADDI(int rd, int rs1, int imm);
void ANDI(int rd, int rs1, int imm);
void ORI(int rd, int rs1, int imm);
void XORI(int rd, int rs1, int imm);
void SLLI(int rd, int rs1, int imm);
void SRLI(int rd, int rs1, int imm);
void SRAI(int rd, int rs1, int imm);
void JALR(int rd, int rs1, int imm);
void JAL(int rd, string label);
void LW(int rd, int base, int offset);
void LH(int rd, int base, int offset);
void LB(int rd, int base, int offset);
void LHU(int rd, int base, int offset);
void LBU(int rd, int base, int offset);
void SLTI(int rd, int rs1, int imm);
void SLTIU(int rd, int rs1, int imm);

// S-type
void SW(int rs1, int base, int offset);
void SH(int rs1, int base, int offset);
void SB(int rs1, int base, int offset);

// B-type
void BEQ(int rs1, int rs2, string label);
void BNE(int rs1, int rs2, string label);
void BLT(int rs1, int rs2, string label);
void BLTU(int rs1, int rs2, string label);
void BGE(int rs1, int rs2, string label);
void BGEU(int rs1, int rs2, string label);

// U-type
void LUI(int rd, int imm);
void AUIPC(int rd, int imm);
