int plc = 0;
string sec[10]; 
  
void fetchProgram(){
    fstream fin;
    string buffer;
    fin.open("input.txt");
    while(getline(fin,buffer)){
        sec[plc] = buffer;
        plc++;
    }
    fin.close();
}