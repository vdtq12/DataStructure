#include "SymbolTable.h"

void SymbolTable::run(string filename) {
    sub_main(filename);
}

void sub_main(string filename){
    int cur_lv = 0;
    SLL mySll;
    mySll.head = NULL;
    ifstream fs;
    fs.open(filename);
    string str;
    regex reg_insert("^(INSERT )[a-z]+\\w* (string|number)$");
    regex reg_assign_num("^(ASSIGN\\s)[a-z]+\\w*\\s\\d+$");
    regex reg_assign_str("^(ASSIGN\\s)[a-z]+\\w*\\s[']{1}[a-zA-Z0-9\\s]*[']{1}$");
    regex reg_assign_iden("^(ASSIGN\\s)[a-z]+\\w*\\s[a-z]+\\w*$");
    regex reg_look("(LOOKUP\\s)[a-z]+\\w*$");
    regex reg_block("^(BEGIN|END)$");
    regex reg_rprint("^(RPRINT$)");
    regex reg_print("^(PRINT$)");
    smatch m;
    while (!fs.eof()){   
            getline(fs,str);
                if(regex_search(str,m,reg_insert)) {
                    regex reg_split(" ");
                    sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
                    sregex_token_iterator end;
                    string iden = *++i; string type = *++i;
                        if(insert(iden, type, mySll, cur_lv)){
                            cout << "success" << endl;
                        }
                        else{
                            clear(mySll, cur_lv);
                            throw Redeclared(str);
                        }
                }
                else if(regex_search(str,m,reg_assign_num)){
                    regex reg_split("\\s{1}");
                    sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
                    sregex_token_iterator end;
                    string iden = *++i; string type = *++i;
                    int is_okay = assign_number(iden, type, mySll, cur_lv);
                        if(is_okay==8){
                            cout << "success" << endl;
                        }
                        else if(is_okay==1){
                            clear(mySll, cur_lv);
                            throw Undeclared(str);
                        }
                        else if(is_okay==2){
                            clear(mySll, cur_lv);
                            throw TypeMismatch(str);
                        }
                }
                else if(regex_search(str,m,reg_assign_str)){
                    regex reg_split("^ASSIGN\\s|\\s[']|[']");
                    sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
                    sregex_token_iterator end;
                    string iden = *++i; string type = *++i;
                    int is_okay = assign_string(iden, type, mySll, cur_lv); 
                        if(is_okay==8){
                            cout << "success" << endl;
                        }
                        else if(is_okay==1){
                            clear(mySll, cur_lv);
                            throw Undeclared(str);
                        }
                        else if(is_okay==2){
                            clear(mySll, cur_lv);
                            throw TypeMismatch(str);
                        }             
                }
                else if(regex_search(str,m,reg_assign_iden)){
                    regex reg_split("\\s{1}");
                    sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
                    sregex_token_iterator end;
                    string iden = *++i; string type = *++i;
                    int is_okay = assign_identifier(iden, type, mySll, cur_lv);    
                        if(is_okay==8){
                            cout << "success" << endl;
                        }
                        else if(is_okay==1){
                            clear(mySll, cur_lv);
                            throw Undeclared(str);
                        }
                        else if(is_okay==2){
                            clear(mySll, cur_lv);
                            throw TypeMismatch(str);
                        }        
                }
                else if(regex_search(str,m,reg_block)) {
                    if(str=="BEGIN") {
                        begin(mySll, cur_lv);
                    }
                    else if(str=="END") {
                        if(!end(mySll, cur_lv)) {
                            clear(mySll, cur_lv);
                            throw UnknownBlock();
                        }
                    }
                }
                else if(regex_search(str,m,reg_look)) {
                    regex reg_split("\\s{1}");
                    sregex_token_iterator i(str.begin(), str.end(), reg_split, -1);
                    sregex_token_iterator end;
                    string iden = *++i;
                        if(!look_up(iden, mySll, cur_lv)) {
                            clear(mySll, cur_lv);
                            throw Undeclared(str);
                        }
                        else cout << endl;
                }
                else if(regex_search(str,m,reg_print)) {
                    if(mySll.head!=NULL){
                        print(mySll, cur_lv);
                        cout << endl;
                    }
                }
                else if(regex_search(str,m,reg_rprint)) {
                    if(mySll.head!=NULL){
                        rprint(mySll.head, cur_lv);
                        cout << endl;                   
                    }
                }            
                else{
                    clear(mySll, cur_lv);
                    throw InvalidInstruction(str);
                }

            if(cur_lv>0 && fs.eof()) {
                clear(mySll, cur_lv);
                throw UnclosedBlock(cur_lv);
            }

    }

    clear(mySll, cur_lv);
    fs.close();
}

bool insert(string iden, string type, SLL& mySll, int& cur_lv){
    Node* p = mySll.head;
    int dopper_lv = 0;
    bool dopper_appear = 0;
    bool *swi = NULL;
    if(p==NULL){
        //cout << "into NULL" << endl;
        mySll.head = new Node;
        mySll.head->iden = iden; mySll.head->type = type; mySll.head->value = ""; mySll.head->on = 1; mySll.head->lv = cur_lv; mySll.head->next = NULL;
        return 1;
    }
    else {
        while(p->next!=NULL){
            if(p->iden==iden && p->on) {
                dopper_lv = p->lv;
                swi = &p->on;
                dopper_appear = 1;
                }
            p = p->next;
        }
        if(p->iden==iden && p->on) {
                dopper_lv = p->lv;
                swi = &p->on;
                dopper_appear = 1;
                }  
        //không trùng iden
        if(!dopper_appear){
            //cout << "into !iden" << endl;
            p->next = new Node;
            p = p->next;
            p->iden = iden; p->type = type; p->value = ""; p->on = 1; p->lv = cur_lv; p->next = NULL;
            return 1;
        }
        //trùng iden , lv cur > lv iden bị trùng
        else if(dopper_appear && cur_lv>dopper_lv){
            //cout << "into Sameiden !lv" << endl;
            p->next = new Node;
            p = p->next;
            p->iden = iden; p->type = type; p->value = ""; p->on = 1; p->lv = cur_lv; p->next = NULL;
            if(swi!=NULL) *swi = 0;
            return 1;
        }
        //trùng iden, lv cur = lv iden bị trùn
        else if(dopper_appear && dopper_lv==cur_lv){
            //cout << "into Sameiden Samelv" << endl;
            return 0;
        }
    }
    return 0;
}

int assign_number(string iden, string value, SLL& mySll, int& cur_lv){
    //1: undeclared, 2: typemismatch, 8: success
    Node* p = mySll.head;
    Node* temp = NULL;
    if(p==NULL) return 1;
    else{
        while (p->next!=NULL){
            if(p->iden==iden) temp = p;
            p = p->next;
        }
        if(p->iden==iden) temp = p;
        if(temp==NULL) return 1;
        else{
            if(temp->type=="number"){
                temp->value = value;
                return 8;
            }
            else {return 2;}
        }
    }
    return 0;
}

int assign_string(string iden, string value, SLL& mySll, int& cur_lv){
    Node* p = mySll.head;
    Node* temp = NULL;
    if(p==NULL) return 1;
    else{
        while (p->next!=NULL){
            if(p->iden==iden) temp = p;
            p = p->next;
        }
        if(p->iden==iden) temp = p;
        if(temp==NULL) return 1;
        else{
            if(temp->type=="string"){
                temp->value = value;
                return 8;
            }
            else return 2;
        }
    }
    return 0;
}

int assign_identifier(string iden, string value, SLL& mySll, int& cur_lv){
    Node* p = mySll.head;
    Node* temp1 = NULL;
    Node* temp2 = NULL;
    if(p==NULL) return 1;
    else{
        while (p->next!=NULL){
            if(p->iden==iden) temp1 = p;
            if(p->iden==value) temp2 = p;
            p = p->next;
        }
        if(p->iden==iden) temp1 = p;
        if(p->iden==value) temp2 = p;
        if(temp1==NULL || temp2==NULL) return 1;
        else{
            if(temp1->type==temp2->type){
                temp1->value = temp2->value;
                return 8;
            }
            else return 2;
        }
    }
    return 0;
}

bool begin(SLL& mySll, int& cur_lv){
    cur_lv++;
    return 1;
}

bool end(SLL& mySll, int& cur_lv){
    //làm thêm hàm xóa mySll khi end thành công
    if(cur_lv==0) return 0;
    else{
        removeNode_at_lv(mySll,cur_lv, cur_lv);
        cur_lv--;
        return 1;
    }    
}

bool look_up(string iden, SLL& mySll, int& cur_lv){
    Node* p = mySll.head;
    if(p==NULL) return 0;
    else{
        while(p!=NULL){
            if(p->iden==iden && p->on) {
            cout << p->lv;
            return 1;
            }
            p=p->next;
        }
    }
    return 0;
}

void print(SLL& mySll, int& cur_lv){
    Node* p = mySll.head;
    if(p==NULL) return;
    else{
        while(p!=NULL){
            bool see_out = 0;
            if(p->on){
                cout << p->iden << "//" << p->lv;
                see_out = 1;
            }
            p=p->next;
            if(see_out && p!=NULL) cout << " ";
        }
    }
}

void rprint(Node* head, int& cur_lv){
    if(head->next==NULL){
        cout << head->iden << "//" << head->lv;
        return;
    }
    rprint(head->next, cur_lv);
    if(head->on) cout << " " << head->iden << "//" << head->lv;
}

void removeNode_at_lv(SLL& mySll, int lv, int& cur_lv){
    Node* p = mySll.head;
    if(p==NULL) return;
    else{
        // bật lại công tắc khi xóa đi bất cứ phần tử nào bị trùng (khi mySll nhiểu hơn 1 phần tử)
        while(p->next!=NULL){
            if(p->next->lv==cur_lv) delete_node(mySll,p, cur_lv);
            else{p=p->next;}
        }
        // nếu mySll chỉ có 1 phần tử
        if(p->next==NULL){
            if(p->lv==cur_lv && p->lv!=0){
                delete(mySll.head);
                mySll.head = NULL;
            }
        }
    }
}

void delete_node(SLL& mySll, Node* prev_del, int& cur_lv){
    Node* p = mySll.head;
    bool *swi = NULL;
    //xóa ở giữa SLL
    if(prev_del->next->next!=NULL){
        while(p!=NULL){
            if(p->iden==prev_del->next->iden && !(p->on)) swi = &(p->on);
            p = p->next;
            if(p->lv==prev_del->next->lv) break;
        }
        Node* temp = prev_del->next;
        prev_del->next = prev_del->next->next;
        delete(temp);
        temp = NULL;
        if(swi!=NULL) *swi = 1;
    }
    // xóa ở cuối SLL
    else{
        while(p!=NULL){
            if(p->iden==prev_del->next->iden && !(p->on)) swi = &(p->on);
            p = p->next;
            if(p->lv==prev_del->next->lv) break;
        }
        delete(prev_del->next);
        prev_del->next=NULL;
        if(swi!=NULL) *swi = 1;
    }
}

void clear(SLL& mySll, int& cur_lv){
    while(mySll.head!=NULL){
        Node* temp = mySll.head;
        mySll.head = mySll.head->next;
        delete(temp);
        temp = NULL;
    }
}