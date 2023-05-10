#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable
{
public:
    class Node{
    friend class SymbolTable;
    private:
        string iden;
        string type;
        string specify;
        long long lv;
        string* para;
        long long numpara;
        long long key;
        long long probing;
    public:
        Node():iden(""), type(""), specify(""), lv(-1), para(NULL), numpara(-1), key(-1), probing(-1) {};
        void res_node(){
            iden = ""; type = ""; specify = "", lv = -1; numpara = -1; key = -1; probing = -1;
            delete[] para; para = NULL;
        }
    };
private:
    Node* arr;
    string method;
    long long size;
    long long c1;
    long long c2;
public:
    SymbolTable():arr(NULL), method(""), size(-1), c1(-1), c2(-1) {};
    void run(string filename);

    void initialize_method(string str);
    long long hash_probing(string iden, long long cur_lv);
    long long hash_key(string iden, long long cur_lv);
    void delete_at_lv(long long cur_lv);
    void clear();

    long long lookup(string iden, long long cur_lv);
    long long insert_iden(string iden, long long cur_lv);
    long long insert_funct(string iden, string numpara, long long cur_lv);
    long long assign_num(string iden, long long cur_lv);
    long long assign_str(string iden, long long cur_lv);
    long long assign_iden(string iden1, string iden2, long long cur_lv, string& error_ret);
    long long assign_funct(string iden1, string function, long long cur_lv, string& error_ret);
    long long call(string function, long long cur_lv, string& error_ret);
    bool begin(long long& cur_lv);
    bool end(long long& cut_lv);
    bool print();
};
#endif