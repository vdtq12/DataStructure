#include "SymbolTable.h"
#include "SymbolTable.cpp"
using namespace std;

void test(string filename)
{
    SymbolTable *st = new SymbolTable();
    try
    {
        st->run(filename);
    }
    catch (exception &e)
    {
        cout << e.what();
    }
    delete st;
}

int main(int argc, char **argv)
{
    test("D:\\quyen\\HK211\\data_struc_and_alg\\asm1\\assignment1\\testcase\\test1.txt");
    if (argc < 2)
        return 1;
    test(argv[1]);
    return 0;
}