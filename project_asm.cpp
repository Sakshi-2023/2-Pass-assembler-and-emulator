/*
Submitted by: Sakshi Saxena
Roll no.-2301CS45
*/

#include <bits/stdc++.h>
using namespace std;
int PC=0; // program counter
class instruction{ // a class representing an instruction
    public:
    bool label_present;
    string in;
    string label;
    string operand;
    int type_of_operand,line_num;
    instruction(string label,bool label_present,string in, string operand, int type_of_operand, int line_num){
        this->label_present=label_present;
        this->in=in;
        this->label=label;
        this->type_of_operand=type_of_operand;
        this->operand=operand;
        this->line_num=line_num;
    }
};
class type_of_instruction{ // class for storing inbuilt instructions 
    public:
    string mnemonic;
    string opcode;
    int type_of_operand;
    type_of_instruction(string mnemonic, string opcode, int type_of_operand){
        this->mnemonic=mnemonic;
        this->opcode=opcode;
        this->type_of_operand=type_of_operand;
    }
};
map<string,int> data_seg;
vector<string> data_segment;
vector<pair<instruction,int>> my_data;
vector<string> inst;
vector<pair<string,int>> list_of_errors;
map<string,pair<string,int>> instruction_set;
map<string,int> label_and_address;
set<string> mnemonic_list;
set<string> opcode_list;
vector<string> machine_code;
vector<string> listing_code;
set<int> type_of_operand_list; 
void initialise(){  
    // initialising the mnemonics and their corresponding opcodes
    mnemonic_list.insert("data");mnemonic_list.insert("ldc");mnemonic_list.insert("adc");mnemonic_list.insert("ldl");
    mnemonic_list.insert("stl");mnemonic_list.insert("ldnl");mnemonic_list.insert("stnl");mnemonic_list.insert("add");
    mnemonic_list.insert("sub");mnemonic_list.insert("shl");mnemonic_list.insert("shr");mnemonic_list.insert("adj");
    mnemonic_list.insert("a2sp");mnemonic_list.insert("sp2a");mnemonic_list.insert("call");mnemonic_list.insert("return");
    mnemonic_list.insert("brz");mnemonic_list.insert("brlz");mnemonic_list.insert("HALT");mnemonic_list.insert("SET"); mnemonic_list.insert("br");
    opcode_list.insert("");opcode_list.insert("00");opcode_list.insert("01");opcode_list.insert("02");
    opcode_list.insert("03");opcode_list.insert("04");opcode_list.insert("05");opcode_list.insert("06");
    opcode_list.insert("07");opcode_list.insert("08");opcode_list.insert("09");opcode_list.insert("0A");
    opcode_list.insert("0B");opcode_list.insert("0C");opcode_list.insert("0D");opcode_list.insert("0E");
    opcode_list.insert("0F");opcode_list.insert("10");opcode_list.insert("11");opcode_list.insert("12");
    type_of_operand_list.insert(1);type_of_operand_list.insert(2);type_of_operand_list.insert(0);
    instruction_set["data"]={"",1};
    instruction_set["ldc"]={"00",1};
    instruction_set["adc"]={"01",1};
    instruction_set["ldl"]={"02",2};
    instruction_set["stl"]={"03",2};
    instruction_set["ldnl"]={"04",2};
    instruction_set["stnl"]={"05",2};
    instruction_set["add"]={"06",0};
    instruction_set["sub"]={"07",0};
    instruction_set["shl"]={"08",0};
    instruction_set["shr"]={"09",0};
    instruction_set["adj"]={"0A",1};
    instruction_set["a2sp"]={"0B",0};
    instruction_set["sp2a"]={"0C",0};
    instruction_set["call"]={"0D",2};
    instruction_set["return"]={"0E",0};
    instruction_set["brz"]={"0F",2};
    instruction_set["brlz"]={"10",2};
    instruction_set["br"]={"11",2};
    instruction_set["HALT"]={"12",0};
    instruction_set["SET"]={"",1};
}
bool is_dec(string s){
    // check if a number is decimal or not
    for(int i=0;i<s.size();i++){
        if(s[i]=='+' || s[i]=='-') continue;
        if(s[i]>='0' && s[i]<='9') continue;
        return false;
    }
    return true;
}
bool is_hex(string s){
    // check if a number is hexadecimal or not
    for(int i=0;i<s.size();i++){
        if(s[i]>='0' && s[i]<='9') continue;
        if(s[i]>='A' && s[i]<='F') continue;
        if(s[i]>='a' && s[i]<='f') continue;
        return false;
    }
    return true;
}
bool is_oct(string s){
    // check if a number is octal or not
    for(int i=0;i<s.size();i++){
        if(s[i]>='0' && s[i]<='7') continue;
        return false;
    }
    return true;
}
bool check_valid_label_name(string s){
    for(int i=0;i<s.size();i++){
        if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') || (s[i]>='0' && s[i]<='9') || s[i]=='_') continue;
        return false;
    }
    if(s[0]=='_') return false;
    if(s[0]>='0' && s[0]<='9') return false;
    return true;
}
void label_table(string s, int pc, int line_num){
    // function to create a table of labels and their addresses
    string ans;
    if(s.empty()) return ;
    string label=s;
    bool check_label_name=check_valid_label_name(label);
    if(check_label_name==false){
        string k="bogus label name";
        list_of_errors.push_back({k,line_num});
        return ;
    }
    label_and_address[label]=pc;
}
void make_data_seg(string s, instruction ins){
    // making a data segment
    bool a=false;
    for(int i=0;i<s.size()-3;i++){
        if(s[i]=='d' && s[i+1]=='a' && s[i+2]=='t' && s[i+3]=='a'){
            a=true;
            break;
        }
    }
    if(a==true){
        data_segment.push_back(s);
        if(!ins.label.empty())data_seg[ins.label]=stoi(ins.operand);
    }
    else{
        inst.push_back(s);
    }
}
bool split(string s, int line_num, string &label, bool label_present,string &instruc, string &operand, int pc){
    // splitting a line into label, mnemonic and operand
    int i=0;
    while(i<s.size() && (s[i]==' ' || s[i]=='\t')) i++;
    if(i==s.size()) return true;
    int l=i;
    for(;i<s.size();i++){
        if(s[i]==':'){
            label_present=true;
            for(int j=i-1;j>=l;j--){
                label.push_back(s[j]);
            }
            reverse(label.begin(),label.end());
            i++;
            break;
        }
    }
    if(label.empty()){
        i=0;
        label_present=false;
    }
    if(!label.empty()){
        auto it=label_and_address.find(label);
        if(it!=label_and_address.end()){
            string k="duplicate label definiton";
            list_of_errors.push_back({k,line_num});
            return false;
        }
    }
    while(i<s.size() && (s[i]==' ' || s[i]=='\t')) i++;
    while(i<s.size() && (s[i]!=' ' && s[i]!='\t')){
        instruc.push_back(s[i]);
        i++;
    }
    if(!instruc.empty()){
        auto it1=mnemonic_list.find(instruc);
        if(it1==mnemonic_list.end()){
            string k="bogus mnemonic";
            list_of_errors.push_back({k,line_num});
            return false;
        }
    }
    while(i<s.size() && (s[i]==' ' || s[i]=='\t')) i++;
    while(i<s.size() && (s[i]!=' ' && s[i]!='\t')){
        operand.push_back(s[i]);
        i++;
    }
    if(!instruc.empty()){
        auto it=instruction_set.find(instruc);
        if(it->second.second!=0 && operand.empty()){
            string k="missing operand";
            list_of_errors.push_back({k,line_num});
            return false;
        }
        if(it->second.second==0 && !operand.empty()){
            string k="unexpected operand";
            list_of_errors.push_back({k,line_num});
            return false;
        }
    }
    auto iterator=instruction_set.find(instruc);
    while(i<s.size() && (s[i]==' ' || s[i]=='\t')) i++;
    while(i<s.size() && (s[i]!=' ' && s[i]!='\t')){
        string e="extra on end of line";
        list_of_errors.push_back({e,line_num});
        return false;
    }
    return true;
}
string clean(string s){
    // cleaning the string by removing unnecessary spaces
    int i=0;
    string ans;
    while(i<s.size() && (s[i]==' ' ||  s[i]=='\t')){
        i++;
    }
    for(;i<s.size();i++){
        if(s[i]==';') break;
        else{
            ans.push_back(s[i]);
        }
    }
    reverse(ans.begin(),ans.end());
    i=0;
    while(i<s.size() && (s[i]==' ' ||  s[i]=='\t')){
        i++;
    }
    reverse(ans.begin(),ans.end());
    return ans;
}
bool is_valid_operand(string s){
    // check if an operand is valid(dec, hex, octal) or not
    if(is_dec(s) || is_hex(s) || is_oct(s)) return true;
    auto it=label_and_address.find(s);
    if(it==label_and_address.end()) return false;
    return true;
}
bool cmp(pair<string,int>p1, pair<string,int>p2){
    // comparator function for sorting(in errors)
    if(p1.second>p2.second) return false;
    return true;
}
void see_errors(){
    // making a file of errors
    sort(list_of_errors.begin(),list_of_errors.end(),cmp);
    ofstream fout("error_file.txt");
    if(list_of_errors.empty()){
        cout<<"No errors found"<<endl;
        fout<<"No errors found"<<endl;
        return ;
    }
    fout<<"the errors generated are as follows:"<<endl;
    for(auto val:list_of_errors){
        fout<<"Line "<<val.second<<": "<<val.first<<endl;
    }
    fout.close();
    cout<<"Errors are generated in error_file.txt"<<endl;
    for(auto val:list_of_errors){
        cout<<"Line "<<val.second<<": "<<val.first<<endl;
    }
}
void firstpass(){
    initialise();
    ifstream infile;
    string file;
    cout<<"Enter the name of your file(make sure that your file should be in the same directory as this code):";
    cin>>file;
    infile.open(file);
    if(infile.fail()){
        cout<<"No such file exists"<<endl;
        exit (0);
    }
    string s;
    int line_num=0;
    while(getline(infile,s)){
        string s1=clean(s);
        line_num++;
        if(s1.empty())continue;
        string label,in,operand;
        bool label_present=false;
        bool x=split(s1,line_num,label,label_present,in,operand,PC);
        if(x==false) continue;
        auto it=instruction_set.find(in);
        int type_of_operand=it->second.second;
        instruction i1={label,label_present,in,operand,type_of_operand,line_num};
        my_data.push_back({i1,PC});
        if(!label.empty()){
            label_table(label,PC,line_num);
        }
        make_data_seg(s1,i1);
        if(!s1.empty() && !in.empty() && in!="data"){
            PC++;
        }
    }
    for(auto &val:my_data){
        if(val.first.in=="SET"){
            auto lb=val.first.label;
            auto v=val.first.operand;
            for(auto &val2:my_data){
                if(val2.first.operand==lb) val2.first.operand=v;
            }
        }
    }
    for(auto &val:my_data){
        auto op=val.first.operand;
        if(op.empty()) continue;
        if(is_valid_operand(op)==false){
            string k="Invalid operand";
            list_of_errors.push_back({k,val.first.line_num});
        }
    }
}
string hexstr(int n,bool is24) {
    // to convert a dec to hex
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
    if(is24)return hexStr.substr(2) ;
	return hexStr ;
}
void secondpass(){
    for(auto &val:my_data){
        int line_num=val.second;
        string label=val.first.label;
        bool label_present=val.first.label_present;
        string in=val.first.in;
        string op=val.first.operand;
        int type_of_operand=val.first.type_of_operand;
        bool offset=false;
        bool is_data=false;
        if(type_of_operand==2) offset=true;
        if(!in.empty() && (in=="data" || in=="SET")) is_data=true;
        int operand_value;
        string final;
        if(!op.empty()){
            auto it=label_and_address.find(op);
            if(it==label_and_address.end()){
                operand_value=stoi(op); // operand is an integer
            }
            else{
                operand_value=it->second; // operand is a label(take its address)
            }
            if(offset==true && it!=label_and_address.end()){
                operand_value-=(line_num+1); // op_val is adjusted to adjust the pc
            }
            if(is_data){
				//32bits
				final = hexstr(operand_value , false) ;
			}else {
				//24bits
                //final=value_of_address+opcode
				final = hexstr(operand_value , true) + instruction_set[in].first ;
			}
        }
        else{
            final="000000"+instruction_set[in].first;
        }
        string pc=hexstr(line_num,true);
        string m_code,l_code;
        l_code=pc;
        if(!in.empty()){
            m_code=final;
            l_code+=" ";
            l_code+=final;
        }
        else l_code+="      ";
        if(!label.empty()){
            l_code+=" "+label+":";
        }
        if(!in.empty()){
            l_code+=" "+in;
        }
        if(!op.empty()){
            l_code+=" "+op;
        }
        if(!l_code.empty()) listing_code.push_back(l_code);
        if(!m_code.empty()) machine_code.push_back(m_code);
    }
}
void write_listing_file(){
    // making a listing file
	ofstream fout("listing_file.l");
    for(auto val:listing_code){
        fout<<val<<endl;
    }
    fout.close();
}
void write_machine_code_file(){
    // making a machine file
    string s="machine_file.o";
	ofstream fout;
	fout.open(s,ios::binary | ios::out);
	for(auto & x : machine_code){
		unsigned int y ;
		std::stringstream ss ;
		ss << std::hex << x ;
		ss >> y ;
		static_cast<int>(y);
		fout.write((const char*)&y , sizeof(unsigned )) ;
	}
	fout.close() ;
}
int main(int argc, char const *argv[])
{
    firstpass();
    see_errors();
    if(!list_of_errors.empty()){
        exit(0);
    }
    secondpass();
    write_listing_file();
    write_machine_code_file();
    return 0;
}
