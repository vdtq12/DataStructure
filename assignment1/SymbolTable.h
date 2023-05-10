#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

struct Node{
    string iden;
    string type;
    string value;
    bool on;
    int lv;
    Node* next;

    Node():iden(""),type(""),value(""),on(1), lv(0), next(NULL){};
};

struct SLL{
    Node* head;
};

void removeNode_at_lv(SLL& mySll, int lv, int& cur_lv);
bool insert(string iden, string type, SLL& mySll, int& cur_lv);
int assign_number(string iden, string value, SLL& mySll, int& cur_lv);
int assign_string(string iden, string value, SLL& mySll, int& cur_lv);
int assign_identifier(string iden, string value, SLL& mySll, int& cur_lv);
bool look_up(string iden, SLL& mySll, int& cur_lv);
bool begin(SLL& mySll, int& cur_lv);
bool end(SLL& mySll, int& cur_lv);
void print(SLL& mySll, int& cur_lv);
void rprint(Node* head, int& cur_lv);
void clear(SLL& mySll, int& cur_lv);
void delete_node(SLL& mySll, Node* del, int& cur_lv);
void sub_main(string filename);

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
};

#endif