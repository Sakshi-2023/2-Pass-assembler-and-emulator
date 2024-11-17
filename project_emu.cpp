/*
Submitted by: Sakshi Saxena
Roll no.-2301CS45
*/

#include <bits/stdc++.h>
using namespace std;
int A,B,SP,PC;
const int N=1e4+10;
vector<int> memory(N);  // memory
vector<string> code; // storing instructions
void ldc(int value){
    B=A;
    A=value;
}
void br(int offset){
    PC+=offset;
}
void brlz(int offset){
    if(A<0){
        PC+=offset;
    }
}
void brz(int offset){
    if(A==0) PC+=offset;
}
void rreturn(){
    PC=A;
    A=B;
}
void call(int offset){
    B=A;
    A=PC;
    PC+=offset;
}
void sp2a(){
    B=A;
    A=SP;
}
void a2sp(){
    SP=A;
    A=B;
}
void adj(int value){
    SP+=value;
}
void shr(){
    A=B>>A;
}
void shl(){
    A=B<<A;
}
void sub(){
    A=B-A;
}
void add(){
    A+=B;
}
void stnl(int offset){
    memory[A+offset]=B;
}
void ldnl(int offset){
    A=memory[A+offset];
}
void stl(int offset){
    memory[SP+offset]=A;
    A=B;
}
void ldl(int offset){
    B=A;
    A=memory[SP+offset];
}
void adc(int value){
    A+=value;
}
void choice(){
    cout<<"Welcome to emmulator"<<endl;
    cout<<"Please enter the value corresponding to the function you want to perform"<<endl;
    cout<<"1. Show memory dump"<<endl;
    cout<<"2. Display ISA"<<endl;
    cout<<"3. Show trace"<<endl;
    cout<<"4. Show memory dump before execution"<<endl;
    cout<<"5. Show memory dump after execution"<<endl;
}
int hex_to_dec(string s){
    int decimalValue = std::stoi(s, nullptr, 16);
    return decimalValue;
}
string hexstr(int n) {
    // converting dec to hex
    string hexStr = "00000000";
    int idx = 7;  
    unsigned int num = n;
    
    while (num != 0 && idx >= 0) {
        int remainder = num % 16;
        if (remainder < 10) {
            hexStr[idx] = (char)(remainder + '0');
        } else {
            hexStr[idx] = (char)(remainder - 10 + 'A');
        }
        num /= 16;
        idx--;
    }
	return hexStr ;
}
void enter_machine_code(){
    // taking machine file as an input
    string s;
    cout<<"Enter file name:"<< endl;
    cin>>s;
    ifstream file(s, ios::binary | ios::in);
    unsigned int data;
    int i=0;
    while(file.read((char*)&data,sizeof(int))){
        memory[i]=data;
        code.push_back(hexstr(data));
        i++;
    }
}
void dump(){
    // printing the contents of the memory
    cout<<"The contents of the memory are as follows:"<<endl;
    for(int i=0;i<100;i=i+4){
        cout<<hexstr(i)<<" ";
        if(i<memory.size()) cout<<hexstr(memory[i])<<" ";
        if(i+1<memory.size()) cout<<hexstr(memory[i+1])<<" ";
        if(i+2<memory.size()) cout<<hexstr(memory[i+2])<<" ";
        if(i+3<memory.size()) cout<<hexstr(memory[i+3])<<endl;
    }
    cout<<endl;
}
void isa(){
    // printing the isa
    map<string,pair<string,int>> instruction_set;
    instruction_set["data"]={"",1};
    instruction_set["ldc"]={"0",1};
    instruction_set["adc"]={"1",1};
    instruction_set["ldl"]={"2",2};
    instruction_set["stl"]={"3",2};
    instruction_set["ldnl"]={"4",2};
    instruction_set["stnl"]={"5",2};
    instruction_set["add"]={"6",0};
    instruction_set["sub"]={"7",0};
    instruction_set["shl"]={"8",0};
    instruction_set["shr"]={"9",0};
    instruction_set["adj"]={"10",1};
    instruction_set["a2sp"]={"11",0};
    instruction_set["sp2a"]={"12",0};
    instruction_set["call"]={"13",2};
    instruction_set["return"]={"14",0};
    instruction_set["brz"]={"15",2};
    instruction_set["brlz"]={"16",2};
    instruction_set["br"]={"17",2};
    instruction_set["HALT"]={"18",0};
    instruction_set["SET"]={"",1};
    cout<<"ISA is as follows:"<<endl;
    for(auto val:instruction_set){
        cout<<val.first<<" "<<val.second.first<<" "<<val.second.second<<endl;
    }
}
void print_reg(){
    // printing the registers
    cout<<"The contents of the registers are as follows"<<endl;
    cout<<"A:"<<A<<" "<<"B:"<<" "<<B<<" "<<"SP:"<<SP<<" "<<"PC:"<<PC<<endl;
}
void trace(){
    // tracing the instructions all at once
    while(true){
        string opc,ope;
        string ins=code[PC];
        for(int i=ins.size()-2;i<ins.size();i++){
            opc.push_back(ins[i]);
        }
        if(opc=="12") break;
        int value = stoi(ins.substr(0, 6), 0, 16);
        if (value >= (1 << 23)) {
            value -= (1 << 24); // Handle negative values in two's complement form
        }
        string opcode=opc;
        if(opcode=="00") ldc(value);
        else if(opcode=="01") adc(value);
        else if(opcode=="02") ldl(value);
        else if(opcode=="03") stl(value);
        else if(opcode=="04") ldnl(value);
        else if(opcode=="05") stnl(value);
        else if(opcode=="06") add();
        else if(opcode=="07") sub();
        else if(opcode=="08") shl();
        else if(opcode=="09") shr();
        else if(opcode=="0A") adj(value);
        else if(opcode=="0B") a2sp();
        else if(opcode=="0C") sp2a();
        else if(opcode=="0D") call(value);
        else if(opcode=="0E") rreturn();
        else if(opcode=="0F") brz(value);
        else if(opcode=="10") brlz(value);
        else if(opcode=="11") br(value);
        PC++;
    }
    cout<<"All the instructions have been executed"<<endl;
}
void before(){
    // printing memory and registers before trace
    if(PC<code.size()){
        dump();
        print_reg();
        trace();
    }
}
void after(){
    // printing memory and registers after trace
    if(PC<code.size()){
        trace();
        dump();
        print_reg();
    }
}
void take_instruction_input(){
    int n;
    cin>>n;
    if(n==0){
        cout<<"Thank you!"<<endl;
        exit(0);
    }
    if(n==1){
        dump();
    }
    if(n==2){
        isa();
    }
    if(n==3){
        trace();
    }
    if(n==4){
        before();
    }
    if(n==5){
        after();
    }
}
int main(int argc, char const *argv[])
{
    enter_machine_code();
    choice();
    while(true){
        take_instruction_input();
    }
    return 0;
}
