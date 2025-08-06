# Operating System Simulator
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
    M: memory; 
    <br>
    IR: Instruction Register (4 bytes)
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

<hr>

## Phase - 2

### Problem Statement : 
1. CPU / Machine Simulation
2. Supervisor Call through interrupt
3. Multiple-Program execution
4. Error handling in jobs
5. Handling Program interrupt,Timing interrupt and system interrupt

### ASSUMPTIONS:
1. Jobs may have program errors
2. PI interrupt for program errors introduced
3. No physical separation between jobs
4. Job outputs separated in output file by 2 blank lines
5. Paging introduced, page table stored in real memory
6. Program pages allocated one of 30 memory block using random number generator
7. Load and run one program at a time
8. Time limit, line limit, out-of-data errors introduced
9. TI interrupt for time-out error introduced
10. 2-line messages printed at termination


### NOTATION
    <br>
    M: memory
    <br>
    IR: Instruction Register (4 bytes)
    <br>
    IR [1,2]: Bytes 1,2 of IR/Operation Code
    <br>
    IR [3, 4]: Bytes 3, 4 of IR/Operand Address
    <br>
    M[&]: Content of memory location 
    <br>
    IC: Instruction Counter Register (2 bytes)
    <br>
    R: General Purpose Register (4 bytes)
    <br>
    C: Toggle (1 byte)
    <br>
    PTR: Page Table Register (4 bytes)
    <br>
    PCB: Process Control Block (data structure)
    <br>
    VA: Virtual Address
    <br>
    RA: Real Address
    <br>
    TTC: Total Time Counter
    <br>
    LLC: Line Limit Counter
    <br>
    TTL: Total Time Limit
    <br>
    TLL: Total Line Limit
    <br>
    EM: Error Message
    <br>

## License
This project is licensed under the [MIT License](https://github.com/ovuiproduction/Operating-System-Simulator/blob/main/LICENSE)
