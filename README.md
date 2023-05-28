# RISC-V-Simulator
  
## Overview
This project was created for the Computer Organization and Assembly Langugage course. The objective of this project is to implement a functional RISC-V simulator capable of tracing the execution of RV32I instructions by showing register and memory contents. The simulator  supports the RV32I base integer instruction set according to the specifications found here: https://riscv.org/technical/specifications. All forty user-level instructions (listed in page 130 of the RISC-V Instruction Set Manual – Volume I: Unprivileged ISA) are implemented as described in the specifications. The project specifics are included in the project report [here](https://github.com/omar-bahgat/RISC-V-Simulator/blob/main/Project%20Report.pdf).

## Running the program

### Simulator Inputs
* <code>Initial Address</code>: The user should specify the program starting address (where the program’s first instruction should be loaded in the memory). <br>
* <code>Assembly Program</code>: The user should input a program to be simulated by specifying a text file that contains the instructions. <br>
* <code>Memory Data</code>: The user should specify any data required by the program to be initially loaded in the memory. For each data item, both its value and memory address should be specified in the follwing format: {address, value}. This information should also be provided through a text file. <br>

### Simulator Outputs
* The simulator outputs the instruction itself, the program counter value, the register contents, and the memory contents after each instruction execution.

## Group Members
* Omar Bahgat: https://github.com/omar-bahgat <br>
* Youssef Mansour: https://github.com/youssef3453 <br>
* Mohamed Abbas: https://github.com/MohamedRagabAbbas <br>



