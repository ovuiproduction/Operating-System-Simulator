# Virtual_Operating_System
<hr>

## Simulation of Operating System fuctionality such as 

1. Memory Managment
2. Virtual Memory
3. Process Execution
4. Process Scheduling
5. Interuppt Handling

## Phase - 1

### Problem Statement : 
1. CPU / Machine Simulation
2. Supervisor Call through interrupt

### Assumption :
• Jobs entered without error in input file
• No physical separation between jobs
• Job outputs separated in output file by 2 blank lines
• Program loaded in memory starting at location 00
• No multiprogramming, load and run one program at a time
• SI interrupts for service request

NOTATION
M: memory; IR: Instruction Register (4 bytes)
IR [1, 2]: Bytes 1, 2 of IR/Operation Code
IR [3, 4]: Bytes 3,4 of IR/Operand Address
M [&]: Content of memory location &
IC: Instruction Counter Register (2 bytes)
R: General Purpose Register (4 bytes)
C: Toggle (1 byte)
