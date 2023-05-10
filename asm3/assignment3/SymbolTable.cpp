#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
    long long cur_lv = 0;

    ifstream fs;
    fs.open(filename);
    string str;
    smatch m;

    regex reg_method1("^(LINEAR|DOUBLE) \\d+ \\d+$");
    regex reg_method2("^QUADRATIC \\d+ \\d+ \\d+$");
    regex reg_insert_iden("^(INSERT) [a-z]+\\w*$");
    regex reg_insert_funct("^(INSERT) [a-z]+\\w* \\d+$");
    regex reg_assign_num("^(ASSIGN) [a-z]+\\w* \\d+$");
    regex reg_assign_str("^(ASSIGN) [a-z]+\\w* ['][a-zA-Z0-9 ]*[']$");
    regex reg_assign_iden("^(ASSIGN) [a-z]+\\w* [a-z]+\\w*$");
    regex reg_assign_funct("^(ASSIGN) [a-z]+\\w* [a-z]+\\w*(\\(([a-z]+\\w*,|['][a-zA-Z0-9 ]*['],|\\d+,)*([a-z]+\\w*|'[a-zA-Z0-9 ]*'|\\d+)\\)|\\(\\))$");
    regex reg_call("^CALL [a-z]+\\w*(\\(([a-z]+\\w*,|['][a-zA-Z0-9 ]*['],|\\d+,)*([a-z]+\\w*|'[a-zA-Z0-9 ]*'|\\d+)\\)|\\(\\))$");
    regex reg_look("(LOOKUP) [a-z]+\\w*$");
    regex reg_block("^(BEGIN|END)$");
    regex reg_print("^(PRINT$)");

    //first line for method of collision handle
    getline(fs, str);
    if(regex_search(str,m,reg_method1) || regex_search(str,m,reg_method2)) initialize_method(str);
    else throw InvalidInstruction(str);

    while(!fs.eof()){
        getline(fs,str);

        if(regex_search(str,m,reg_insert_iden)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i;
            if(iden=="string" || iden=="number" || iden=="true" || iden=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            long long t = insert_iden(iden, cur_lv);
            if(t==-2) {
                clear();
                throw Redeclared(iden);
            }
            else if(t==-3) {
                clear();
                throw Overflow(iden);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_insert_funct)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string numpara = *++i;
            if(iden=="string" || iden=="number" || iden=="true" || iden=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            if(cur_lv!=0) {
                clear();
                throw InvalidDeclaration(iden);
            }
            long long t = insert_funct(iden, numpara, cur_lv);
            if(t==-2) {
                clear();
                throw Redeclared(iden);
            }
            else if(t==-3) {
                clear();
                throw Overflow(iden);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_assign_num)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string value = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            int t = assign_num(iden, cur_lv);
            if(t==-1){
                clear();
                throw Undeclared(iden);
            }
            else if(t==-2){
                clear();
                throw TypeMismatch(str);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_assign_str)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string value = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            int t = assign_str(iden, cur_lv);
            if(t==-1){
                clear();
                throw Undeclared(iden);
            }
            else if(t==-2){
                clear();
                throw TypeMismatch(str);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_assign_iden)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden1 = *++i; string iden2 = *++i;
            if(iden1=="number" || iden1=="string" || iden1=="true" || iden1=="false"
                || iden2=="number" || iden2=="string" || iden2=="true" || iden2=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            string error_ret ="";
            int t = assign_iden(iden1, iden2, cur_lv, error_ret);
            if(t==-1){
                clear();
                throw Undeclared(error_ret);
            }
            else if(t==-2){
                clear();
                throw TypeMismatch(str);
            }
            else if(t==-3){
                clear();
                throw TypeCannotBeInferred(str);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_assign_funct)){
            int space = str.find(' ',0);
            string s = str.substr(space+1);
            space = s.find(' ',0);
            string iden1 = s.substr(0,space);
            string function = s.substr(space+1);
            if(iden1=="number" || iden1=="string" || iden1=="true" || iden1=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            string error_ret ="";
            int t = assign_funct(iden1, function, cur_lv, error_ret);
            if(t==-1){
                clear();
                throw Undeclared(error_ret);
            }
            else if(t==-2){
                clear();
                throw TypeMismatch(str);
            }
            else if(t==-3){
                clear();
                throw TypeCannotBeInferred(str);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_call)){
            int space = str.find(' ',0);
            string function = str.substr(space+1);
            string error_ret = "";
            int t = call(function, cur_lv, error_ret);
            if(t==-1){
                clear();
                throw Undeclared(error_ret);
            }
            else if(t==-2){
                clear();
                throw TypeMismatch(str);
            }
            else if(t==-3){
                clear();
                throw TypeCannotBeInferred(str);
            }
            else cout << t << endl;
        }
        else if(regex_search(str,m,reg_look)) {
            regex reg_split("\\s{1}");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            sregex_token_iterator end;
            string iden = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear();
                throw InvalidInstruction(str);
            }
            long long look = lookup(iden, cur_lv);
                if(look == -1){
                    clear();
                    throw Undeclared(iden);
                }
                else {
                    cout << look << endl;
                }
        }
        else if(regex_search(str,m,reg_block)) {
            if(str=="BEGIN") {
                begin(cur_lv);
            }
            else if(str=="END") {
                if(!end(cur_lv)) {
                    clear();
                    throw UnknownBlock();
                }
            }
        }
        else if(regex_search(str,m,reg_print)) {
            if(print()) cout << endl;
        }
        else{
            clear();
            throw InvalidInstruction(str);
        }
        if(cur_lv>0 && fs.eof()){
            clear();
            throw UnclosedBlock(cur_lv);
        }
    }

    clear();
    fs.close();
}

void SymbolTable::initialize_method(string str){
    if(str.find("LINEAR")!=string::npos){
        this->method = "line";
        regex reg_split(" ");
        sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
        this->size = stoll(*++i); 
        this->c1 = stoll(*++i);
    }
    else if(str.find("QUADRATIC")!=string::npos){
        this->method = "quad";
        regex reg_split(" ");
        sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
        this->size = stoll(*++i); 
        this->c1 = stoll(*++i);
        this->c2 = stoll(*++i);
    }
    else if(str.find("DOUBLE")!=string::npos){
        this->method = "double";
        regex reg_split(" ");
        sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
        this->size = stoll(*++i); 
        this->c1 = stoll(*++i);
    }
    this->arr = new Node[this->size];
}

long long SymbolTable::hash_key(string iden, long long cur_lv){
    long long key = 0;
    string temp = to_string(cur_lv);
    for(unsigned long long i=0; i<iden.length(); i++){
        temp += to_string(iden[i]-48);
    }
    key = stoll(temp);
    return key;
}

long long SymbolTable::hash_probing(string iden, long long cur_lv){
    return 0;
}

long long SymbolTable::insert_iden(string iden, long long cur_lv){
    // convert iden to key
    long long key = hash_key(iden, cur_lv);

    //key hash and probing
    long long slot = -1;
    if(this->method=="line"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i)%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = cur_lv;
                this->arr[slot].key = key;
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else if(this->method=="quad"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i + this->c2*i*i)%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = cur_lv;
                this->arr[slot].key = key;
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else if(this->method=="double"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i*(1+key%(this->size-2)))%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = cur_lv;
                this->arr[slot].key = key;
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else return -1; //
}

long long SymbolTable::insert_funct(string iden, string numpara, long long cur_lv){
    // convert iden to key
    long long key = hash_key(iden, cur_lv);
    //key hash and probing
    long long slot = -1;
    if(this->method=="line"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i)%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = 0;
                this->arr[slot].numpara = stoll(numpara);
                this->arr[slot].key = key;
                this->arr[slot].specify = "funct";
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else if(this->method=="quad"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i + this->c2*i*i)%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = 0;
                this->arr[slot].numpara = stoll(numpara);
                this->arr[slot].key = key;
                this->arr[slot].specify = "funct";
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else if(this->method=="double"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i*(1+key%(this->size-2)))%this->size;
            if(this->arr[slot].key==key) return -2; // Redeclared
            else if(this->arr[slot].iden == "") {
                this->arr[slot].iden = iden;
                this->arr[slot].lv = 0;
                this->arr[slot].numpara = stoll(numpara);
                this->arr[slot].key = key;
                this->arr[slot].specify = "funct";
                this->arr[slot].probing = i;
                return i;
            }
        }
        return -3; //overflow
    }
    else return -1; //
}

// specify = funct && type = "" : function do not know type
// => specify = return && type = string/number : return function <or> specify = void && type = void : void function

long long SymbolTable::assign_num(string iden, long long cur_lv){
    long long slot = lookup(iden, cur_lv);
    if(slot==-1) return -1; //Undeclared
    else if(this->arr[slot].specify!="" || this->arr[slot].type=="string") return -2; // typemismatch
    else if(this->arr[slot].type == ""){
        this->arr[slot].type = "number";
    }
    return this->arr[slot].probing;
}

long long SymbolTable::assign_str(string iden, long long cur_lv){
    long long slot = lookup(iden, cur_lv);
    if(slot==-1) return -1; //Undeclared
    else if(this->arr[slot].specify!="" || this->arr[slot].type=="number") return -2; // typemismatch
    else if(this->arr[slot].type == ""){
        this->arr[slot].type = "string";
    }
    return this->arr[slot].probing;
}

long long SymbolTable::assign_iden(string iden1, string iden2, long long cur_lv, string& error_ret){
    long long slot1 = lookup(iden1, cur_lv);
    long long slot2 = lookup(iden2, cur_lv);
    error_ret = iden2;
    if(slot2==-1) return -1; //Undeclared
    error_ret = iden1;
    if(slot1==-1) return -1; //Undeclared
    else if(this->arr[slot1].specify!="" || this->arr[slot2].specify!="") return -2; // typemismatch
    else if(this->arr[slot1].type=="" && this->arr[slot2].type=="") return -3; //TypeCannotBeInferred
    else if(this->arr[slot2].type == ""){
        this->arr[slot2].type = this->arr[slot1].type;
    }
    else if(this->arr[slot1].type == ""){
        this->arr[slot1].type = this->arr[slot2].type;
    }
    else if(this->arr[slot1].type!=this->arr[slot2].type) return -2; //typemismatch
    return this->arr[slot1].probing+this->arr[slot2].probing;
}

long long SymbolTable::assign_funct(string iden1, string function, long long cur_lv, string& error_ret){
    //tách function
    regex reg_split("\\(|,|\\)");
    sregex_token_iterator i(function.begin(), function.end(), reg_split, -1);
    sregex_token_iterator end;
    string funct_iden = *i; i++;
    error_ret = funct_iden;
    long long slot2 = lookup(funct_iden, cur_lv);
    if(slot2==-1) return -1; //Undeclared
    long long ret_val = this->arr[slot2].probing;

    //kiểm tra val
    regex reg1("\\d+"); //number
    regex reg2("['][a-zA-Z0-9 ]*[']"); //string
    regex reg3("[a-z]+\\w*"); //iden (not funct)
    smatch m0; 

    long long npara = 0;
    if(this->arr[slot2].numpara==-1) return -2;
    if(this->arr[slot2].para==NULL){
        this->arr[slot2].para = new string[this->arr[slot2].numpara];
        while(i!=end && npara!=this->arr[slot2].numpara){
            string str = *i;
            error_ret = str;
            if(regex_match(str,m0,reg1)){
                this->arr[slot2].para[npara] = "number";
            }
            else if(regex_match(str,m0,reg2)){
                this->arr[slot2].para[npara] = "string";
            }
            else if(regex_match(str,m0,reg3)){
                long long slot_temp = lookup(str, cur_lv);
                if(slot_temp==-1) return -1; //không tìm thấy iden, fail, Undeclared
                else if(this->arr[slot_temp].specify!="") return -2; // truyền tên hàm vào tham số TypeMismatch
                else if(this->arr[slot_temp].type=="") return -3; //TypeCannotBeInferred
                this->arr[slot2].para[npara] = this->arr[slot_temp].type;
                ret_val+=this->arr[slot_temp].probing;
            }
            else{
                return -4; // InvalidInvalidInstruction
            }
            i++;
            npara++;
        }
    }
    else{
        long long npara = 0;
        while(i!=end && npara!=this->arr[slot2].numpara){
            string str = *i;
            error_ret = str;
            if(regex_match(str,m0,reg1)){
                if(this->arr[slot2].para[npara] != "number") return -2; // truyền sai loại tham số, TypeMismatch
            }
            else if(regex_match(str,m0,reg2)){
                if(this->arr[slot2].para[npara] != "string") return -2; // truyền sai loại tham số, TypeMismatch
            }
            else if(regex_match(str,m0,reg3)){
                long long slot_temp = lookup(str, cur_lv);
                if(slot_temp==-1) return -1; //không tìm thấy iden, fail, Undeclared
                else if(this->arr[slot_temp].specify!="") return -2; // truyền tên hàm vào tham số TypeMismatch
                else if(this->arr[slot_temp].type=="") return -3; //TypeCannotBeInferred
                else if(this->arr[slot2].para[npara] != this->arr[slot_temp].type) return -2; // truyền sai loại tham số, TypeMismatch
                ret_val+=this->arr[slot_temp].probing;
            }
            else{
                return -4; //InvalidInstruction
            }
            i++;
            npara++;
        }
    }
    if((i==end || *i=="") && npara==this->arr[slot2].numpara){}
    else return -2; // dư/thiếu tham số truyền vào, TypeMismatch

    if(this->arr[slot2].specify=="" || this->arr[slot2].specify=="void") return -2; //TypeMismatch

    long long slot1 = lookup(iden1, cur_lv);
    error_ret = iden1;
    if(slot1==-1) return -1; //không tìm thấy iden, fail, Undeclared
    else if(this->arr[slot1].specify!="") return -2; //TypeMismatch
    else if(this->arr[slot1].type=="" && this->arr[slot2].type=="") return -3; // TypeCannotBeInferred
    else if(this->arr[slot1].type==""){
        this->arr[slot1].type = this->arr[slot2].type;
    }
    else if(this->arr[slot2].type==""){
        this->arr[slot2].type = this->arr[slot1].type;
    }
    else if(this->arr[slot1].type != this->arr[slot2].type) return -2; //TypeMismatch
    ret_val+=this->arr[slot1].probing;
    return ret_val;
}

long long SymbolTable::call(string function, long long cur_lv, string& error_ret){
    regex reg_split("\\(|,|\\)");
    sregex_token_iterator i(function.begin(), function.end(), reg_split, -1);
    sregex_token_iterator end;
    string funct_iden = *i; i++;
    error_ret = funct_iden;
    long long slot = lookup(funct_iden, cur_lv);
    if(slot==-1) return -1; //Undeclared
    long long ret_val = this->arr[slot].probing;
    
    if(this->arr[slot].specify=="funct"){
        this->arr[slot].specify="void";
        this->arr[slot].type="void";
    }

    regex reg1("\\d+"); //number
    regex reg2("['][a-zA-Z0-9 ]*[']"); //string
    regex reg3("[a-z]+\\w*"); //iden (not funct)
    smatch m0; 

    long long npara = 0;
    if(this->arr[slot].numpara==-1) return -2;
    if(this->arr[slot].para==NULL){
        this->arr[slot].para = new string[this->arr[slot].numpara];
        while(i!=end && npara!=this->arr[slot].numpara){
            string str = *i;
            error_ret = str;
            if(regex_match(str,m0,reg1)){
                this->arr[slot].para[npara] = "number";
            }
            else if(regex_match(str,m0,reg2)){
                this->arr[slot].para[npara] = "string";
            }
            else if(regex_match(str,m0,reg3)){
                long long slot_temp = lookup(str, cur_lv);
                if(slot_temp==-1) return -1; //không tìm thấy iden, fail, Undeclared
                else if(this->arr[slot_temp].specify!="") return -2; // truyền tên hàm vào tham số TypeMismatch
                else if(this->arr[slot_temp].type=="") return -3; //TypeCannotBeInferred
                this->arr[slot].para[npara] = this->arr[slot_temp].type;
                ret_val+=this->arr[slot_temp].probing;
            }
            else{
                return -4; // InvalidInstruction
            }
            i++;
            npara++;
        }
    }
    else{
        long long npara = 0;
        while(i!=end && npara!=this->arr[slot].numpara){
            string str = *i;
            error_ret = str;
            if(regex_match(str,m0,reg1)){
                if(this->arr[slot].para[npara] != "number") return -2; // truyền sai loại tham số, TypeMismatch
            }
            else if(regex_match(str,m0,reg1)){
                if(this->arr[slot].para[npara] != "string") return -2; // truyền sai loại tham số, TypeMismatch
            }
            else if(regex_match(str,m0,reg3)){
                long long slot_temp = lookup(str, cur_lv);
                if(slot_temp==-1) return -1; //không tìm thấy iden, fail, Undeclared
                else if(this->arr[slot_temp].specify!="") return -2; // truyền tên hàm vào tham số TypeMismatch
                else if(this->arr[slot_temp].type=="") return -3; //TypeCannotBeInferred
                else if(this->arr[slot].para[npara] != this->arr[slot_temp].type) return -2; // truyền sai loại tham số, TypeMismatch
                ret_val+=this->arr[slot_temp].probing;
            }
            else {
                return -4; //InvalidInstruction
            }
            i++;
            npara++;
        }
    }
    if(this->arr[slot].specify=="" || this->arr[slot].specify=="return") return -2; //TypeMismatch
    return ret_val;
}

long long SymbolTable::lookup(string iden, long long cur_lv){
    if(cur_lv < 0) return -1; //Undeclared
    // convert iden to key
    long long key = hash_key(iden, cur_lv);

    //key hash and probing
    long long slot = -1;
    if(this->method=="line"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i)%this->size;
            if(this->arr[slot].key==key) return slot; // Redeclared
            else if(this->arr[slot].iden == "") {
                return lookup(iden, cur_lv-1);
            }
        }
    }
    else if(this->method=="quad"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i + this->c2*i*i)%this->size;
            if(this->arr[slot].key==key) return slot; // Redeclared
            else if(this->arr[slot].iden == "") {
                return lookup(iden, cur_lv-1);
            }
        }
    }
    else if(this->method=="double"){
        for(long long i=0; i<this->size; i++){
            slot = (key%this->size + this->c1*i*(1+key%(this->size-2)))%this->size;
            if(this->arr[slot].key==key) return slot; // Redeclared
            else if(this->arr[slot].iden == "") {
                return lookup(iden, cur_lv-1);
            }
        }
    }
    return lookup(iden, cur_lv-1);
}

bool SymbolTable::begin(long long& cur_lv){
    cur_lv++;
    return true;
}

bool SymbolTable::end(long long& cur_lv){
    if(cur_lv==0) return 0; //unknown block
    delete_at_lv(cur_lv);
    cur_lv--;
    return 1;
}

void SymbolTable::delete_at_lv(long long cur_lv){
    for(long long i=0; i<this->size; i++){
        if(this->arr[i].lv == cur_lv) this->arr[i].res_node();
        else continue;
    }
}

bool SymbolTable::print(){
    bool first = true;
    for(long long i=0; i<this->size; i++){
        if(first && this->arr[i].iden!=""){
            cout << i << " " << this->arr[i].iden << "//" << this->arr[i].lv;
            first = false;
        }
        else if(!first && this->arr[i].iden!=""){
            cout << ";" << i << " " << this->arr[i].iden << "//" << this->arr[i].lv;
        }
        else continue;
    }
    if(first==true) return false; //not print anything
    return true;
}

void SymbolTable::clear(){
    for(int i=0; i<this->size; i++){
        if(this->arr[i].para!=NULL){
            delete[] this->arr[i].para;
            this->arr[i].para = NULL;
        }
    }
    delete[] this->arr;
    this->arr = NULL;
}