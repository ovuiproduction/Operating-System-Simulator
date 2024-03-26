#include <iostream>
#include <fstream>
#include <string>
#include<string.h>
#include <stdlib.h>

using namespace std;

class opsys{

    private:
        char MM[100][4];
        char IR[4];
        char R[40];
        int Ic;
        int si;
        bool C;
        string buffer;
    
    public:
        void init();
        void Load();
        void startexecution(string data[]);
        void executeuserprogram(string data[]);
        void MOS(string data[]);
        void displayMemory();

        fstream fin;
        fstream fout;

};

void opsys::init(){                     //init function

    for(int i =0;i<100;i++){
        for(int j=0;j<4;j++){
            MM[i][j] = '-';
        }
    }
    IR[0] = ' ';
    R[0] = ' ';

}

int find(char ch){      //function for return oprand Address
    
    int tempop = ch - 48;
    tempop = tempop*10;

    return tempop;
}

void Write(char IR[],char MM[][4]){         //write function

    
    int sadd = find(IR[2]);
    int eadd = sadd+9;
    
    ofstream outputFile("output.txt",ios::app); 

    for(int i =sadd;i<eadd;i++){
        for(int j=0;j<4;j++){
            if(MM[i][j] != '-'){
            outputFile<<MM[i][j];
            }
        }
    }
    outputFile.close();
}


int di = 0;
void Read(string data[],char IR[],char MM[][4]){         //read data from input file and store into memory

    string dataline = data[di++];

    IR[3] = '0';  //error handling
    
    int row = find(IR[2]);
  
    int col =0;
    for(int i =0;i<dataline.length();i++){
        if(col == 4){
            row++;
            col = 0;
        }
        MM[row][col] = dataline.at(i);
        col++;
    }
}

void Terminate(){                               //function for adding two blank line after execution of one job

    ofstream outputFile("output.txt",ios::app);
    outputFile<<"\n\n"<<endl;
    outputFile.close();

}

void opsys::MOS(string data[]){
 
    switch(si){

        case 1:
            Read(data,IR,MM);       //read data from inputFile and store it into memory
            break;
        case 2:
            Write(IR,MM);           //read data from memory and put it into outputFile
            break;
        case 3:
            Terminate();            //treminate the Job
            break;
        default:
            return;
            break;
    }
}

void opsys::executeuserprogram(string data[]){
  

    while(true){

    si = 3;                         //default si set to 3

    for (int j = 0; j < 4; j++) {       // load instruction from memory to Instruction Register
            IR[j] = MM[Ic][j];
    }
    
    Ic++;
    
    string function = "";
    function = function + IR[0];
    function = function + IR[1];

    int operandAdd = find(IR[2]);
    
    if(function == "LR"){               //load data from memory to register
        int r_count =0;
        for(int i =operandAdd;i<=(operandAdd+9);i++){
            for(int j = 0;j<4;j++){
                R[r_count++] = MM[i][j];
            }
        }
       si = 0;
    }

    else if(function == "SR"){          //store data into memory from register

        int r_count = 0;
        for(int i = operandAdd;i<(operandAdd+9);i++){
            for(int j=0;j<4;j++){
                MM[i][j] = R[r_count++];
            }
        }  
        si =0;
    }

    else if(function == "CR"){          //compare data of register and perticular memory location
        int r_count = 0;
        int flag = 0;
        for(int i=operandAdd;i<(operandAdd+9);i++){
            for(int j=0;j<4;j++){
                if(MM[i][j] != R[r_count++]){
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 0){
            C = true;
        }
        else{
            C = false;
        }
        si = 0;
    }

    else if(function == "BT"){              //branch on true it is like jump instrction
        if(C == true){
            Ic = operandAdd + (IR[3] - 48);
        }
        else{
            continue;
        }
        si = 0;
    }

    else if(function == "GD"){          //get data
        si = 1;
    }
    else if(function == "PD"){          //put data
        si = 2;
    }
    else if(IR[0] == 'H'){              //halt
        si = 3;
        MOS(data);
        break;
    }
    else{
        printf("Invalid Job");
        return;
    }

    MOS(data);         //  User Mode ---> kernal Mode

    }
}

void opsys::startexecution(string data[]){
    Ic = 0;
    executeuserprogram(data);               //function call for executeuserprogram() function
}


void opsys::Load(){
 
    //Max buffer size 40
    if(buffer.length() > 40){
        printf("index out of bound!");
        return;
    }

    string databuffer[4]; //data buffer for storing data
    int data_count =0;

    
    fin.open("input.txt");  //input file open
 
    while (getline(fin, buffer)) {      //while loop for reading input file line by line

        string opcode = buffer;
        string op = opcode.substr(0,4);
        
        if(op == "$AMJ"){
            init();             //memory reset / memory reallocation
            continue;
        }
        //data card
        else if(op == "$DTA"){
            while(getline(fin, buffer)){
                if(buffer.substr(0,1) == "$"){
                    break;
                }
                if (buffer.substr(0,1) != "$"){
                    databuffer[data_count++] = buffer;
                }
                else{
                   continue;
                }
            }
            //data missing condition
           if(databuffer->empty()){
                cout<<"Data Missing Error"<<endl;
           }
            startexecution(databuffer);         //call for startexecution
        }

        else if(op == "$END"){
           continue;
        }

         // instructions loaded into memory
        else{     
            string inst = buffer;
            int row = 0;
            int col =0;
            for(int i =0;i<inst.length();i++){
                if(col == 4){
                    row++;
                    col = 0;
                }
                MM[row][col] = inst.at(i);
                if(inst.at(i)== 'H'){
                    row++;
                    col = 0;
                }
                else{
                col++;
                }
            }
        }
    }

    fin.close();    //after reading all file closing the file
}

void opsys::displayMemory(){        //function for display memory

    printf("\n Main Memory Simulation\n\n");
    for(int i =0;i<60;i++){
        printf("%2d     ",i);
        for(int j=0;j<4;j++){
            printf("%5c ",MM[i][j]);
        }
        printf("\n");
    }
}

//driver function
int main(){

    opsys vos;                  //contructor for class

    vos.init();                 //init function for initilizing variable and memory
    vos.displayMemory();        //display memory before executing the job card

    vos.Load();                 //calling load function

    cout<<"\n\n\n***After Execution of Job***\n"<<endl;
    vos.displayMemory();        //display memory After executing the job card
   
    return 0;
}

