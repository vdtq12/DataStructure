#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct Para{
    //para đầu tiên sẽ chứa return type, các para sau sẽ chứa tham số type
    string type;
    Para* next;
    Para():type(""), next(NULL) {};
};

class SymbolTable
{
public:
    class node{
        friend class SymbolTable;
        private:
            string iden;
            string type;
            string value;
            bool stat;
            int lv;
            node* parent;
            node* left;
            node* right;
            Para* para;
        public:
            node():iden(""), type(""), value(""), stat(0), lv(0), parent(NULL), left(NULL), right(NULL), para(NULL) {};
            node(string iden, string type, string stat, int cur_lv):value(""), parent(NULL), left(NULL), right(NULL), para(NULL){
                this->iden = iden; this->type = type; 
                if(stat == "true"){
                    this->stat = true;
                    this->lv = 0;
                }
                else{
                    this->stat = false;
                    this->lv = cur_lv;
                }
            }
    };
private:
    node* root;
public:
    SymbolTable() {}
    void clear(node* node);
    void splay(node* n);
    int compare(node* n, node* temp);
    void printTree(node *root, int space);
    void zig(node* n);
    void zag(node* n);
    void split_para(node* n, string type);
    void delete_at_lv(int cur_lv);
    node* find_at_lv(node* n, int finding_lv, int& num_comp);
    void delete_node(node* n);
    node* max(node* n);
    node* join(node* l, node* r);
    void split(node* &n, node* &l, node* &r);
    void delete_para(node* n);

    bool insert_strnum(string iden, string type, string stat, int cur_lv, int& num_comp, int& num_splay);
    int insert_funct(string iden, string type, string stat, int cur_lv, int& num_comp, int& num_splay);
    int assign_num(string iden, string val, int cur_lv, int& num_comp, int& num_splay);
    int assign_str(string iden, string val, int cur_lv, int& num_comp, int& num_splay);
    int assign_iden(string iden, string val, int cur_lv, int& num_comp, int& num_splay);
    int assign_funct(string iden, string val, int cur_lv, int& num_comp, int& num_splay);
    node* lookup(string iden, int cur_lv, int& num_comp);
    bool begin(int& cur_lv);
    bool end(int& cut_lv);
    void print(node* head);

    void run(string filename);
};
#endif