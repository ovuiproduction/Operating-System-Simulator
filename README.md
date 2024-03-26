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
1. Jobs entered without error in input file
2. No physical separation between jobs
3. Job outputs separated in output file by 2 blank lines
4. Program loaded in memory starting at location 00
5. No multiprogramming, load and run one program at a time
6. SI interrupts for service request

### NOTATION
<br>
M: memory; IR: Instruction Register (4 bytes)
<br>
IR [1, 2]: Bytes 1, 2 of IR/Operation Code
<br>
IR [3, 4]: Bytes 3,4 of IR/Operand Address
<br>
M [&]: Content of memory location
<br>
IC: Instruction Counter Register (2 bytes)
<br>
R: General Purpose Register (4 bytes)
<br>
C: Toggle (1 byte)
