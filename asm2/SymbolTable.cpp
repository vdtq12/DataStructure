#include "SymbolTable.h"


// handle 

// INSERT x (number)->string false
// ASSIGN x x(123,456)

void SymbolTable::run(string filename)
{
    root = NULL;
    int cur_lv = 0;

    ifstream fs;
    fs.open(filename);
    string str;
    smatch m;

    regex reg_insert_strnum("^(INSERT) [a-z]+\\w* (string|number) (true|false)$");
    regex reg_insert_funct("^(INSERT) [a-z]+\\w* (\\((string,|number,)*(string|number)\\)|\\(\\))\\-\\>(string|number) (true|false)$");
    regex reg_assign_num("^(ASSIGN) [a-z]+\\w* \\d+$");
    regex reg_assign_str("^(ASSIGN) [a-z]+\\w* ['][a-zA-Z0-9 ]*[']$");
    regex reg_assign_iden("^(ASSIGN) [a-z]+\\w* [a-z]+\\w*$");
    regex reg_assign_funct("^(ASSIGN) [a-z]+\\w* [a-z]+\\w*(\\(([a-z]+\\w*,|['][a-zA-Z0-9 ]*['],|\\d+,)*([a-z]+\\w*|'[a-zA-Z0-9 ]*'|\\d+)\\)|\\(\\))$");
    regex reg_look("(LOOKUP) [a-z]+\\w*$");
    regex reg_block("^(BEGIN|END)$");
    regex reg_print("^(PRINT$)");
    
    while(!fs.eof()){
        getline(fs,str);
        if(regex_search(str,m,reg_insert_strnum)){
            int num_comp = 0, num_splay = 0;
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string type = *++i; string stat = *++i;
            if(iden==type || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            if(insert_strnum(iden, type, stat, cur_lv, num_comp, num_splay)) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else{
                clear(root);
                root = NULL;
                throw Redeclared(str);
            }
            //cout << "intoNumstr" << endl;
        }
        else if(regex_search(str,m,reg_insert_funct)){
            int num_comp = 0, num_splay = 0;
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string type = *++i; string stat = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int ans = insert_funct(iden, type, stat, cur_lv, num_comp, num_splay);
            if(ans==8) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else if(ans==1){
                clear(root);
                root = NULL;
                throw InvalidDeclaration(str);
            }
            else if(ans==2){
                clear(root);
                root = NULL;
                throw Redeclared(str);
            }
            //cout << "intoFunct" << endl;
        }
        else if(regex_search(str,m,reg_assign_num)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string value = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int num_comp = 0, num_splay = 0;
            int ans = assign_num(iden, value, cur_lv, num_comp, num_splay);
            if(ans==8) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else if(ans==1){
                clear(root);
                root = NULL;
                throw Undeclared(str);
            }
            else if(ans==2){
                clear(root);
                root = NULL;
                throw TypeMismatch(str);
            }
        }
        else if(regex_search(str,m,reg_assign_str)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string value = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int num_comp = 0, num_splay = 0;
            int ans = assign_str(iden, value, cur_lv, num_comp, num_splay);
            if(ans==8) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else if(ans==1){
                clear(root);
                root = NULL;
                throw Undeclared(str);
            }
            else if(ans==2){
                clear(root);
                root = NULL;
                throw TypeMismatch(str);
            }
        }
        else if(regex_search(str,m,reg_assign_iden)){
            regex reg_split(" ");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            string iden = *++i; string value = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int num_comp = 0, num_splay = 0;
            int ans = assign_iden(iden, value, cur_lv, num_comp, num_splay);
            if(ans==8) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else if(ans==1){
                clear(root);
                root = NULL;
                throw Undeclared(str);
            }
            else if(ans==2){
                clear(root);
                root = NULL;
                throw TypeMismatch(str);
            }
        }
        else if(regex_search(str,m,reg_assign_funct)){
            int space = str.find(' ',0);
            string s = str.substr(space+1);
            space = s.find(' ',0);
            string iden = s.substr(0,space);
            string value = s.substr(space+1);
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int num_comp = 0, num_splay = 0;
            int ans = assign_funct(iden, value, cur_lv, num_comp, num_splay);
            if(ans==8) cout /*<< "comp: " */<< num_comp /*<< "\nsplay: "*/<< " " << num_splay << endl;
            else if(ans==1){
                clear(root);
                root = NULL;
                throw Undeclared(str);
            }
            else if(ans==2){
                clear(root);
                root = NULL;
                throw TypeMismatch(str);
            }
        }
        else if(regex_search(str,m,reg_look)) {
            regex reg_split("\\s{1}");
            sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
            sregex_token_iterator end;
            string iden = *++i;
            if(iden=="number" || iden=="string" || iden=="true" || iden=="false"){
                clear(root);
                root = NULL;
                throw InvalidInstruction(str);
            }
            int num_comp = 0;
            node* look = lookup(iden, cur_lv, num_comp);
                if(look == NULL){
                    clear(root);
                    root = NULL;
                    throw Undeclared(str);
                }
                else {
                    cout << look->lv << endl;
                    splay(look);
                }
        }
        else if(regex_search(str,m,reg_block)) {
            if(str=="BEGIN") {
                begin(cur_lv);
            }
            else if(str=="END") {
                if(!end(cur_lv)) {
                    clear(root);
                    root = NULL;
                    throw UnknownBlock();
                }
            }
        }
        else if(regex_search(str,m,reg_print)) {
            if(root!=NULL){
                print(root);
                cout << endl;
            }
        }
        else{
            clear(root);
            root = NULL;
            throw InvalidInstruction(str);
        }
    // cout << "-----------SPLAY----------------------------------------------------\n";
    // printTree(root, 1);
    // cout << endl << endl;
        if(cur_lv>0 && fs.eof()){
            clear(root);
            root = NULL;
            throw UnclosedBlock(cur_lv);
        }
    }
    clear(root);
    root = NULL;
    fs.close();
}


bool SymbolTable::insert_strnum(string iden, string type, string stat, int cur_lv, int& num_comp, int& num_splay){
    if(root==NULL){
        root = new node(iden, type, stat, cur_lv);
        return true;
    }

    node* n = new node(iden, type, stat, cur_lv);
    node* temp = root; node* p = NULL;

    while (temp!=NULL)
    {
        p = temp;
        num_comp++;
        if(compare(n, temp)==-1) temp = temp->left;
        // trên đường duyệt nếu thấy node trùng iden trùng lv thì insert fail
        else if(compare(n, temp)==0) {
            delete(n);
            n = NULL;
            return false;
        }
        else temp = temp->right;
    }

    n->parent = p;
    if(compare(n,p)==-1) p->left = n;
    else p->right = n;
    
    // cout << "----------INSERT-----------------------------------------------------\n";
    // printTree(root, 1);
    if(n!=root) {
        splay(n);
        num_splay++;
    }    
    // cout << "-----------SPLAY----------------------------------------------------\n";
    // printTree(root, 1);

    return true;
}

int SymbolTable::insert_funct(string iden, string type, string stat, int cur_lv, int& num_comp, int& num_splay){
    //value của function là funct
    if(stat=="false" && cur_lv>0) return 1;

    if(root==NULL){
        root = new node(iden, "", "true", 0);
        root->value = "funct";
        split_para(root, type);
        return 8;
    }

    node* n = new node(iden, "", "true", 0);
    n->value = "funct";
    node* temp = root; node* p = NULL;

    while (temp!=NULL)
    {
        p = temp;
        num_comp++;
        if(compare(n, temp)==-1) temp = temp->left;
        // trên đường duyệt nếu thấy node trùng iden trùng lv thì insert fail
        else if(compare(n, temp)==0) {
            delete(n);
            n = NULL;
            return 2;
        }
        else temp = temp->right;
    }

    n->parent = p;
    if(compare(n,p)==-1) p->left = n;
    else p->right = n;
    
    // cout << "----------INSERT-----------------------------------------------------\n";
    // printTree(root, 1);
    if(n!=root) {
        splay(n);
        num_splay++;
    }  
    // cout << "-----------SPLAY----------------------------------------------------\n";
    // printTree(root, 1);

    //tách para
    split_para(n, type);
    return 8;
}

int SymbolTable::assign_num(string iden, string val, int cur_lv, int& num_comp, int& num_splay){
    node* n = lookup(iden, cur_lv, num_comp);
    if(n==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    if(n->type != "number" || n->value == "funct") return 2; // kiểu của val khác kiểu của iden, Type
    n->value = val;
    if(n!=root) {
        splay(n);
        num_splay++;
    }  
    return 8;
}

int SymbolTable::assign_str(string iden, string val, int cur_lv, int& num_comp, int& num_splay){
    node* n = lookup(iden, cur_lv, num_comp);
    if(n==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    if(n->type != "string" || n->value == "funct") return 2; // kiểu của val khác kiểu của iden, TypeMismatch
    n->value = val;
    if(n!=root) {
        splay(n);
        num_splay++;
    }  
    return 8;
}

int SymbolTable::assign_iden(string iden, string val, int cur_lv, int& num_comp, int& num_splay){
    int num_comp_n = 0, num_comp_m = 0;
    // num_splay_n = 0, num_splay_m = 0;
    node* n = lookup(val, cur_lv, num_comp_n);
    if(n==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    if(n!=root) {
        splay(n);
        num_splay++;
    }  
    node* m = lookup(iden, cur_lv, num_comp_m);
    if( m==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    if(n->type!=m->type || n->value=="funct" || m->value=="funct") return 2; // kiểu của val khác kiểu của iden, TypeMismatch
    n->value = m->value;
    if(m!=root) {
        splay(m);
        num_splay++;
    }  
    num_comp = num_comp_m + num_comp_n;
    //num_splay = num_splay_n + num_splay_m;
    return 8;
}

int SymbolTable::assign_funct(string iden, string val, int cur_lv, int& num_comp, int& num_splay){
    int num_comp_n = 0, num_comp_m = 0;
    regex reg_split("\\(|,|\\)");
    sregex_token_iterator i(val.begin(), val.end(), reg_split, -1);
    sregex_token_iterator end;
    string funct_iden = *i; i++;
    node* n = lookup(funct_iden, 0, num_comp_n);
    if(n==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    if(n!=root) {
        splay(n);
        num_splay++;
    }  

    //kiểm tra val
    regex reg1("\\d+"); //number
    regex reg2("['][a-zA-Z0-9 ]*[']"); //string
    regex reg3("[a-z]+\\w*"); //iden (not funct)
    smatch m0;
    Para* p = n->para;
    
    
    // if(i==end){cout << "i reach end\n";}
    // if(p==NULL){cout << "p reach null\n";}
    //int count = 1;
    while(i!=end && p!=NULL){
        int a = 0;
        string str = *i;
        // cout << "string " << count++ << " : " << *i << endl;
        if(p->type=="number"){
            if(regex_match(str,m0,reg1)){}
            else if(regex_match(str,m0,reg3)){
                node* m = lookup(str, cur_lv, a);
                if(m==NULL) return 1; //không tìm thấy iden, fail, Undeclared
                else if(m->type!="number"|| m->value=="funct") return 2; // tham số truyền vào khác kiểu, TypeMismatch
                if(m!=root) {
                    splay(m);
                    num_splay++;
                }  
            }
            else return 2; // tham số truyền vào khác kiểu, TypeMismatch
        }
        if(p->type=="string"){
            if(regex_match(str,m0,reg2)){}
            else if(regex_match(str,m0,reg3)){
                node* m = lookup(str, cur_lv, a);
                if(m==NULL) return 1; //không tìm thấy iden, fail, Undeclared
                else if(m->type!="string" || m->value=="funct") return 2; // tham số truyền vào khác kiểu, TypeMismatch
                if(m!=root) {
                    splay(m);
                    num_splay++;
                }  
            }
            else return 2; // tham số truyền vào khác kiểu, TypeMismatch
        }
        i++;
        p = p->next;
        // count++;
        num_comp_n+=a;
    }
    // cout << "true string: " << nonempty;
    if((i==end || *i=="") && p==NULL){}
    else return 2; // tham số truyền vào khác kiểu, TypeMismatch

    node* m = lookup(iden, cur_lv, num_comp_m);
    if(m==NULL) return 1; //không tìm thấy iden, fail, Undeclared
    else if(n->type!=m->type || m->value=="funct") return 2;
    if(m!=root) {
        splay(m);
        num_splay++;
    }  
    num_comp = num_comp_m+num_comp_n;
    return 8;
}

SymbolTable::node* SymbolTable::lookup(string iden, int cur_lv, int& num_comp){
    if(root==NULL) return nullptr;
    node* n = new node;
    n->iden = iden;
    while(cur_lv >= 0){
        n->lv = cur_lv;
        num_comp = 0;
        node* p = find_at_lv(n, cur_lv, num_comp);
        cur_lv--;
        if(p!=NULL){
            delete_para(n);
            delete(n);
            n = NULL;
            return p;
        }
    }
    delete_para(n);
    delete(n);
    n = NULL;
    return nullptr;
}

bool SymbolTable::begin(int& cur_lv){
    cur_lv++;
    return 1;
}

bool SymbolTable::end(int& cur_lv){
    //làm thêm hàm xóa mySll khi end thành công
    if(cur_lv==0) return 0;
    else if(root==NULL){
        cur_lv--;
        return 1;
    }
    else{
        delete_at_lv(cur_lv);
        cur_lv--;
        return 1;
    }    
}

void SymbolTable::print(node* head){
    if(head==NULL) return;
    if(head!=root) cout << " ";
    cout << head->iden << "//" << head->lv;
    print(head->left);
    print(head->right);
}





// SUPPORT FUNCTION

void SymbolTable::delete_at_lv(int cur_lv){
    node* p = root;
    while(p->lv < cur_lv && p->right!=NULL){
        p = p->right;
    }
    if(p->lv == cur_lv) delete_node(p);
    if(root==NULL) return;
    if(max(root)->lv >= cur_lv) delete_at_lv(cur_lv);
}

void SymbolTable::delete_node(node* n){
    if(n==NULL) return;
    node* l = NULL;
    node* r = NULL;
    split(n, l, r);
    if(l->left!=NULL) l->left->parent = NULL;
    root = join(l->left, r);
    delete_para(l);
    delete(l);
    l = NULL;
};

void SymbolTable::delete_para(node* n){
    if(n->para == NULL) return;
    Para* temp = n->para;
    n->para = n->para->next;
    delete(temp);
    temp = NULL;
    delete_para(n);
}

SymbolTable::node* SymbolTable::join(node* l, node* r){
    if(l==NULL) return r;
    else if(r==NULL) return l;
    node* n = max(l);
    splay(n);
    n->right = r;
    r->parent = n;
    return n;
}

void SymbolTable::split(node* &n, node* &l, node* &r){
    splay(n);
    if(n->right!=NULL){
        r = n->right;
        r->parent = NULL;
    }
    else r=NULL;
    l = n;
    l->right = NULL;
    n = NULL;
}

SymbolTable::node* SymbolTable::max(node* n){
    while(n->right!=NULL) n = n->right;
    return n;
}

void SymbolTable::clear(node* n){
    if(n == NULL) return;
    clear(n->left);
    clear(n->right);
    delete_para(n);
    delete n;
    n = NULL;
}

SymbolTable::node* SymbolTable::find_at_lv(node* n, int finding_lv, int& num_comp){
    if(finding_lv < 0) return nullptr;
    node* p = root;
    while (compare(n,p)!=0){
        num_comp++;
        if(compare(n,p)==-1) p = p->left;
        else if(compare(n,p)==1) p = p->right;
        if(p==NULL) return nullptr;
    }
    num_comp++;
    if(p!=NULL) return p;
    return nullptr;
}

void SymbolTable::split_para(node* n, string type){
    n->para = new Para;
    Para* q = n->para;
    regex r("string|number");  
    smatch sm;
    while (regex_search(type, sm, r)) {
        q->type = sm[0];
        q->next = new Para;
        q = q->next;
        type = sm.suffix();
    }
    q = NULL;
    q = n->para;
    if(q->next->next == NULL){
        n->type = q->type;
        delete(n->para->next);
        n->para->next = NULL;
        delete(n->para);
        n->para = NULL;
    }
    else if(q->next->type == "string" || q->next->type == "number"){
        while(q->next->next->next!=NULL){
            q = q->next;
        }
        n->type = q->next->type;
        delete(q->next->next);
        q->next->next = NULL;
        delete(q->next);
        q->next = NULL;

    }
    // q = n->para;
    // cout << "first: " << n->type << endl;
    // while(p!=NULL){
    //     cout << q->type << endl;
    //     q = q->next;
    // }
}

void SymbolTable::zig(node* n){
    node* m = n->left;
    if(n->parent==NULL) root = m;
    else if(n->parent->right==n) n->parent->right = m;
    else if(n->parent->left==n) n->parent->left = m;
    m->parent = n->parent;
    n->parent = m;
    n->left = m->right;
    if(m->right!=NULL) m->right->parent = n;
    m->right = n;
}

void SymbolTable::zag(node* n){
    node* m = n->right;
    if(n->parent==NULL) root = m;
    else if(n->parent->left==n) n->parent->left = m;
    else if(n->parent->right==n) n->parent->right = m;
    m->parent = n->parent;
    n->parent = m;
    n->right = m->left;
    if(m->left!=NULL) m->left->parent = n;
    m->left = n;
}

void SymbolTable::splay(node* n){
    while(n->parent!=NULL){
        if(n->parent == root){
            if(n==n->parent->left) zig(n->parent);
            else zag(n->parent);
        }
        else{
            node* p = n->parent;
            node* q = n->parent->parent;
            if(n == p->left && p == q->left){
                zig(q); zig(p);
            }
            else if(n == p->right && p == q->right){
                zag(q); zag(p);
            }
            else if(n == p->right && p == q->left){
                zag(p); zig(q);
            }
            else if(n == p->left && p == q->right){
                zig(p); zag(q);
            }
        }
    }
}

void SymbolTable::printTree(node* root, int space)
    {
    if (root == NULL)
         return;
     space += 1;
     printTree(root->right, space);
     for (int i = 1; i < space; i++)
          cout<<"\t";
     cout<<root->iden<<"\n";
      printTree(root->left, space);

    // if (root != nullptr) {
    //     for(int i=0; i<=space; i++) cout << " ";
    //     cout << root->iden << endl;
    //     if (root->left) printTree(root->left, space + 4);
    //     if (root->right) printTree(root->right, space + 4);
    // }
}

int SymbolTable::compare(node* n, node* temp){
    // n < temp return -1 
    if(n->lv < temp->lv) return -1;
    // n > temp return 1;
    else if(n->lv > temp->lv) return 1;

    // n = temp return 0 
    else{
        // trùng iden
        if(n->iden.compare(temp->iden)==0) return 0;
        // iden n > iden temp
        else if(n->iden.compare(temp->iden)>=1) return 1;
        // iden n < iden temp
        else return -1;
    }
}