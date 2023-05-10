#include <iostream>
#include <string.h>
#include <regex>
#include "SymbolTable.h"
using namespace std;

Para* handle(Para* &q){
    q = q->next;
    return q;
}
class N{
    public:
    Para* a;
    int b;

};

int main(){
    string str = "ASSIGN kuVA qi7aH1('o9',kuVA,'abc v')";
    // regex reg_split("ASSIGN |^(?<=')\\w*(?=')");
    // sregex_token_iterator i(val.begin(), val.end(), reg_split, -1);
    // sregex_token_iterator end;
    // while(i!=end){
    //     cout << *i << endl;
    //     i++;
    // }

    int space = str.find(' ',0);
    str = str.substr(space+1);
    space = str.find(' ',0);
    string iden = str.substr(0,space);
    string value = str.substr(space+1);
    cout << iden << endl << value << endl;
}


