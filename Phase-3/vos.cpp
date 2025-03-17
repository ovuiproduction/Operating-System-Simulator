#include <iostream>
#include <fstream> //fstream for file oprations
#include <string>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define secondary_memory_size 100
#define memorySize 800
#define memoryBlock 79
#define programCount 20

#include <time.h> // time.h for random number generation
#include "common.c"
#include "fcfs.c"
#include "sjf.c"

using namespace std;

class opsys
{

private:
    string sec[secondary_memory_size]; // secondary memory
    char MM[memorySize][4];            // main
    char IR[4];                        // instruction register
    char R[40];                        // general purpose register
    int Ic;                            // instruction counter
    int si;                            // system interuppt
    int ti;                            // timer inpeterupt
    int pi;                            // process interuppt
    bool C;                            // toggle
    string buffer;                     // buffer
    int EM;                            // error meassage
    int ptr;                           // page table pointer
    int rd;                            // real Address
    string function;                   // function -- instructions
    int opAdd;                         // virtual address
    int plc;                           // program line count
    int procCount;
    string databuffer[programCount];
    int data_count;
    int pcb_count;
    int reserved_Block;

    class processCB
    { // this is used by os when process executes
    public:
        int pid; // process id
        int ttl; // total time limit
        int tll; // total line limit
        int ttc; // total time counter
        int llc; // line limit counter
    } pcb;

    class pcbArr
    { // set when we load the process used for setting the processCB elements according to process id (pid)
    public:
        int pid; // process id
        int ttl; // total time limit
        int tll; // total line limit
        int ttc; // total time counter
        int llc; // line limit counter
    } pcbarr[programCount];

    // this class for process scheduling all information of required for process scheduling is stored in this class array
    class processExecutionID
    { // set when we load the process , it is used when we schedule the processes;
    public:
        int pid;
        int at;
        int bt;
        int wt;
        int tat;
        int ct;
    } peid[programCount]; // process execution id

public:
    void init();
    void Fetchdata();
    void Load();
    void startexecution(int process);
    void executeuserprogram();
    void MOS();
    void displayMemory();
    void Write(char IR[], char MM[][4]);
    void simulation();
    bool isFree(int fream_no);
    void Allocatefream();
    int addressMap(int vr);
    void Read(string data[], char IR[], char MM[][4]);
    void printPcb();
    void Terminate();
    void executionUnitActive();

    fstream fin;
    fstream fout;
};

void opsys::init()
{ // init function

    for (int i = 0; i < memorySize; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            MM[i][j] = '-';
        }
    }

    si = 0;
    ti = 0;
    pi = 0;
    IR[0] = ' ';
    R[0] = ' ';
    plc = 0;
    procCount = 0;
    data_count = 0;
    pcb_count = 0;
    reserved_Block = 790;
}

void errmsg(int EM)
{ // function for error message print
    ofstream outputFile("output.txt", ios::app);
    string error;
    switch (EM)
    {
    case 0:
        error = "\nNO Error";
        outputFile << error << endl;
        break;
    case 1:
        error = "\nOut Of Data";
        outputFile << error << endl;
        break;
    case 2:
        error = "\nLine Limit Exceeded";
        outputFile << error << endl;
        break;
    case 3:
        error = "\nTime Limit Exceeded";
        outputFile << error << endl;
        break;
    case 4:
        error = "\nOperation Code Error";
        outputFile << error << endl;
        break;
    case 5:
        error = "\nOperand Error";
        outputFile << error << endl;
        break;
    case 6:
        error = "\nInvalid Page Fault";
        outputFile << error << endl;
        break;
    }
}

void opsys::printPcb()
{

    ofstream outputFile("output.txt", ios::app);
    outputFile << "Process ID : " << pcb.pid << "\nTotal Time Limit : " << pcb.ttl << "\tTotal Time Counter : " << pcb.ttc << "\nTotal Line Limit : " << pcb.tll << "\tTotal Line counter : " << pcb.llc << endl;
    outputFile.close();
}

void opsys::Terminate()
{ // function for adding two blank line after execution of one job

    printPcb();
    ofstream outputFile("output.txt", ios::app);
    outputFile << "\n"
               << endl;
    outputFile.close();
}

int find(char ch1, char ch2)
{ // function for return oprand Address

    if (isdigit(ch1) && isdigit(ch2))
    {
        int tempop = ch1 - 48;
        tempop = tempop * 10;
        return tempop;
    }
    return -1;
}

void opsys::Write(char IR[], char MM[][4])
{ // write function

    pcb.llc++; // llc increment by printing one line in ouput file
    if (pcb.llc > pcb.tll)
    { // check tll range
        errmsg(2);
        Terminate();
        exit(0);
    }

    int sadd = rd;
    int eadd = sadd + 9;
    ofstream outputFile("output.txt", ios::app);

    for (int i = sadd; i < eadd; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (MM[i][j] != '-')
            {
                outputFile << MM[i][j];
            }
        }
    }
    outputFile.close();
}

int di = 0; // data index if more than one lines want read then to keep track of data lines it used
void opsys::Read(string data[], char IR[], char MM[][4])
{ // read data from input file and store into memory

    // datacard missing condition (out of data)
    if (data->empty())
    {
        errmsg(1);
        Terminate();
        exit(0);
    }
    string dataline = data[di++];

    IR[3] = '0'; // error handling

    int row = rd;
    int col = 0;
    for (int i = 0; i < dataline.length(); i++)
    {
        if (col == 4)
        {
            row++;
            col = 0;
        }
        MM[row][col] = dataline.at(i);
        col++;
    }
}

void opsys::MOS()
{ // Master Mode of Oprating System

    if (ti == 0 && si == 1)
    { // Read opration call
        Read(databuffer, IR, MM);
    }
    else if (ti == 0 && si == 2)
    { // write opration call
        Write(IR, MM);
    }
    else if (ti == 0 && si == 3 && pi == 0)
    { // halt opration call
        errmsg(0);
        Terminate();
    }
    else if (ti == 2 && si == 1)
    { // Time limit exceeded at Read instruction
        errmsg(3);
        Terminate();
        exit(0);
    }
    else if (ti == 2 && si == 2)
    { // Time limit exceeded at write instruction
        Write(IR, MM);
        errmsg(3);
        Terminate();
        exit(0);
    }
    else if (ti == 2 && si == 3 && pi == 0)
    { // Time limit exceeded at Halt instruction
        errmsg(0);
        Terminate();
    }
    else if (ti == 0 && pi == 1)
    { // Opcode Error
        errmsg(4);
        Terminate();
        exit(0);
    }
    else if (ti == 0 && pi == 2)
    { // OPERAND Error
        errmsg(5);
        Terminate();
        exit(0);
    }
    else if (ti == 0 && pi == 3)
    { // Page fault Error
        // page fault eror
        // valid --> allocate memmory and execute user program -->> ic--;
        // invalid --> terminate and EM = 6
        if (function == "GD")
        { // valid page fault
            string progce;
            while (true)
            {

                srand(time(0)); // Allocating fream for GD
                int fream_no = rand() % memoryBlock;

                if (isFree(fream_no))
                {
                    progce = to_string(fream_no);
                    break;
                }
                else
                {
                    continue;
                }
            }

            int k = 3; // Updating page table
            for (int i = progce.length() - 1; i >= 0; i--)
            {
                MM[ptr + (opAdd / 10)][k] = progce.at(i);
                k--;
            }
            for (int i = k; i >= 0; i--)
            {
                MM[ptr + (opAdd / 10)][i] = '0';
            }

            Ic--; // Ic decremented
        }
        else if (function == "PD") // invalid page fault
        {
            errmsg(6);
            Terminate();
            exit(0);
        }
        else
        {
            errmsg(4);
            Terminate();
            exit(0);
        }
    }
    else if (ti == 2 && pi == 1)
    { // Time limit and opration code error
        errmsg(3);
        errmsg(4);
        Terminate();
    }
    else if (ti == 2 && pi == 2)
    { // Time limit and oprand error
        errmsg(3);
        errmsg(5);
        Terminate();
    }
    else if (ti == 2 && pi == 3)
    { // Time limit and Page fault error
        errmsg(3);
        Terminate();
        exit(0);
    }
}
void opsys::simulation()
{ // Function for validating total time counter and total time limit
    if (pcb.ttc > pcb.ttl)
    {
        ti = 2;
    }
}

int opsys::addressMap(int vr)
{ // Address Map function for maping virtual address to physical address
    int pte;
    int rd;

    if (vr >= 0 && vr <= 99)
    {
        pte = ptr + vr / 10;
        if (MM[pte][0] == '*')
        {
            pi = 3;
            return -1;
        }
        else
        {
            string sfno = "";
            for (int i = 0; i < 4; i++)
            {
                sfno = sfno + MM[pte][i];
            }
            cout << "SI : " << sfno << endl;
            int fno = stoi(sfno);
            rd = fno * 10 + vr % 10;
            return rd;
        }
    }
    else
    {
        pi = 2;
        string data[] = {};
        MOS();
    }
}

void opsys::executeuserprogram()
{

    while (true)
    {

        si = 3;
        pi = 0;
        ti = 0; // default si set to 3

        int ra = addressMap(Ic);

        if (ra == -1)
        { // Invalid page fault
            Terminate();
            break;
        }

        for (int j = 0; j < 4; j++)
        { // load instruction from memory to Instruction Register
            IR[j] = MM[ra][j];
        }

        Ic++;

        function = "";
        function = function + IR[0];
        function = function + IR[1];
        int operandAdd;

        if (IR[0] != 'H')
        {
            opAdd = find(IR[2], IR[3]);
            operandAdd = addressMap(opAdd);
        }

        if (operandAdd == -1 && IR[0] != 'H')
        {
            pi = 3;
            MOS();
            continue;
        }

        if (function == "LR")
        { // load data from memory to register
            int r_count = 0;
            for (int i = operandAdd; i <= (operandAdd + 9); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    R[r_count++] = MM[i][j];
                }
            }
            si = 0;
        }

        else if (function == "SR")
        { // store data into memory from register

            int r_count = 0;
            for (int i = operandAdd; i < (operandAdd + 9); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    MM[i][j] = R[r_count++];
                }
            }
            si = 0;
        }

        else if (function == "CR")
        { // compare data of register and perticular memory location
            int r_count = 0;
            int flag = 0;
            for (int i = operandAdd; i < (operandAdd + 9); i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (MM[i][j] != R[r_count++])
                    {
                        flag = 1;
                        break;
                    }
                }
            }
            if (flag == 0)
            {
                C = true;
            }
            else
            {
                C = false;
            }
            si = 0;
        }

        else if (function == "BT")
        { // branch on true it is like jump instrction
            if (C == true)
            {
                Ic = operandAdd + (IR[3] - 48);
            }
            else
            {
                continue;
            }
            si = 0;
        }

        else if (function == "GD")
        { // get data
            si = 1;
            rd = operandAdd;
            pcb.ttc = pcb.ttc + 2;
        }
        else if (function == "PD")
        { // put data
            si = 2;
            pcb.ttc++;
            rd = operandAdd;
        }
        else if (IR[0] == 'H')
        { // halt
            si = 3;
            pcb.ttc++;
            simulation();
            MOS();
            break;
        }
        else
        {
            si = 0;
            pi = 1;
        }

        simulation();
        MOS(); //  User Mode ---> kernal Mode
    }
}

void opsys::executionUnitActive()
{
    sjf(process_arr);
    cout << "Total Process : " << pcb_count << endl;
    // fcfs(process_arr);
    for (int i = 0; i < pcb_count; i++)
    {
        proc ele = ganchart[i];
        startexecution(ele.pid);
    }
    // for(int i=1;i<=pcb_count;i++){
    //     startexecution(i);
    // }
}

void opsys::startexecution(int process)
{
    cout << "Start Execution" << endl;
    Ic = 0;
    pcb.pid = pcbarr[process - 1].pid;
    pcb.ttl = pcbarr[process - 1].ttl;
    pcb.tll = pcbarr[process - 1].tll;
    pcb.llc = pcbarr[process - 1].llc;
    pcb.ttc = pcbarr[process - 1].ttc;
    int unit = (int)(MM[reserved_Block + process - 1][3]) - 48;
    int base = (int)(MM[reserved_Block + process - 1][2]) - 48;
    ptr = base * 10 + unit;
    ptr = ptr * 10;
    cout << ptr << endl;
    executeuserprogram(); // function call for executeuserprogram() function
}

bool opsys::isFree(int fream_no)
{ // function for cheacking avalibility of fream
    int esf = 0;
    for (int i = fream_no * 10; i < fream_no * 10 + 9; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (MM[i][j] != '-')
            {
                esf = 1;
                break;
            }
        }
        if (esf == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void opsys::Allocatefream()
{             // Page table Creation and Allocating fream to page table
    int ptsi; // page table starting index
    while (true)
    {
        srand(time(0));
        int fream_no = rand() % memoryBlock;

        if (isFree(fream_no))
        {
            // cout<<"Frame No. : "<<fream_no;
            int unit = fream_no % 10;
            int d = fream_no / 10;
            MM[reserved_Block + procCount][0] = 'P';
            MM[reserved_Block + procCount][1] = (char)(48 + procCount);
            MM[reserved_Block + procCount][2] = (char)(48 + d);
            MM[reserved_Block + procCount][3] = (char)(48 + unit);
            procCount++;
            ptsi = fream_no * 10;
            for (int i = ptsi; i <= ptsi + 9; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    MM[i][j] = '*';
                }
            }
            break;
        }
        else
        {
            continue;
        }
    }
    ptr = ptsi;
}

void opsys::Load()
{ // Load function
    // Max buffer size 40
    if (buffer.length() > 40)
    {
        printf("index out of bound!");
        return;
    }
    int prc = 0; // program read count

    while (prc < plc)
    {
        buffer = sec[prc++];
        string opcode = buffer;
        string op = opcode.substr(0, 4);

        if (op == "$AMJ")
        { // memory reset / memory reallocation
            Allocatefream();
            string pid = opcode.substr(4, 4);
            string timelimit = opcode.substr(8, 4);
            string linelimit = opcode.substr(12, 4);
            pcb.pid = stoi(pid);
            pcb.ttl = stoi(timelimit);
            pcb.tll = stoi(linelimit);
            pcb.llc = 0;
            pcb.ttc = 0;
            pcbarr[pcb_count].pid = pcb.pid;
            pcbarr[pcb_count].ttl = pcb.ttl;
            pcbarr[pcb_count].tll = pcb.tll;
            pcbarr[pcb_count].llc = pcb.llc;
            pcbarr[pcb_count].ttc = pcb.ttc;
            addprocess(process_arr, pcb.pid, pcb.pid - 1, pcb.ttl);
            pcb_count++;
            continue;
        }
        else if (op == "$DTA")
        {
            // Data card control
            while (prc < plc)
            {
                buffer = sec[prc++];
                if (buffer.substr(0, 1) == "$")
                {
                    break;
                }
                if (buffer.substr(0, 1) != "$")
                {
                    databuffer[data_count++] = buffer;
                }
                else
                {
                    continue;
                }
            }
        }

        else if (op == "$END")
        {
            break;
        }

        // program card control
        else
        {

            string inst = buffer;
            int progei;
            while (true)
            { // finding fream no for instruction

                srand(time(0));
                int fream_no = rand() % memoryBlock;

                if (isFree(fream_no))
                {
                    progei = fream_no;
                    break;
                }
                else
                {
                    continue;
                }
            }

            string progce = to_string(progei); // pagetable update
            int k = 3;
            for (int i = progce.length() - 1; i >= 0; i--)
            {
                MM[ptr][k] = progce.at(i);
                k--;
            }
            for (int i = k; i >= 0; i--)
            {
                MM[ptr][i] = '0';
            }

            int row = progei * 10; // instructions loaded into main memory
            int col = 0;
            for (int i = 0; i < buffer.length(); i++)
            {
                if (col == 4)
                {
                    row++;
                    col = 0;
                }
                MM[row][col] = buffer.at(i);
                if (buffer.at(i) == 'H')
                {
                    row++;
                    col = 0;
                }
                else
                {
                    col++;
                }
            }
        }
    }
    executionUnitActive();
}

void opsys::displayMemory()
{ // function for display memory

    printf("\n Main Memory Simulation\n\n");
    for (int i = 0; i < memorySize; i++)
    {
        // printf("%2d     ",i);
        cout << i << "     ";
        for (int j = 0; j < 4; j++)
        {
            // printf("%5c ",MM[i][j]);
            cout << MM[i][j] << "   ";
        }
        // printf("\n");
        cout << endl;
    }
}

// fetch data to secondary memory
void opsys::Fetchdata()
{
    fin.open("input.txt");
    while (getline(fin, buffer))
    {
        sec[plc] = buffer;
        plc++;
    }
    Load(); // calling load function
    fin.close();
}

// driver function
int main()
{

    opsys vos; // contructor for class

    vos.init(); // init function for initilizing variable and memory
    // vos.displayMemory();        //display memory before executing the job card
    vos.Fetchdata();
    // cout<<"\n\n***After Execution of Job***\n"<<endl;
    // vos.displayMemory();        //display memory After executing the job card
    return 0;
}
