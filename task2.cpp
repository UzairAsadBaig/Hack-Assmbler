#include <iostream>
#include <string>
#include <fstream>
#include <list>
using namespace std;

struct reg
{
    string name;
    int address;

    reg(string x, int y)
    {
        name = x;
        address = y;
    }
};

list<reg> table;
int varIn = 16;

void createSymbolTable()
{
    for (int i = 0; i < 16; i++)
        table.emplace_back(reg("R" + to_string(i), i));
    table.emplace_back(reg("SCREEN", 16384));
    table.emplace_back(reg("KBD", 24576));
    table.emplace_back(reg("SP", 0));
    table.emplace_back(reg("LCL", 1));
    table.emplace_back(reg("ARG", 2));
    table.emplace_back(reg("THIS", 3));
    table.emplace_back(reg("THAT", 4));
}
void addLable(string line, int add)
{
    reg ins(line, add);
    list<reg>::iterator i;
    for (i = table.begin(); i->address != 16384; i++)
    {
    }
    table.insert(i, ins);

    // for (i = table.begin(); i != table.end(); i++)
    // {
    //     cout << i->address << "\t" << i->name << endl;
    // }
}

void addVariable(string line, int add)
{
    reg ins(line, add);
    list<reg>::iterator i;
    for (i = table.begin(); i->address != 16384; i++)
    {
    }
    table.insert(i, ins);
}

void convBinary(int num, int c, list<int> &binary)
{
    while (num != 0)
    {
        binary.emplace_front(num % 2);

        num = num / 2;
        c--;
    }
    for (int i = c - 1; i >= 0; i--)
    {
        binary.emplace_front(0);
    }
}

string remWhiteSpace(string line)
{
    while (line.find(" ") != string::npos)
        line.replace(line.find(" "), 1, "");

    return line;
}

string remComments(string line)
{
    if (line.find("/") != string ::npos)
    {
        for (int i = line.find("/"); i < line.length(); i++)
            line.replace(i, line.length(), "");
        return line;
    }
    else
        return line;
}

void convSymbol(string line, list<int> &binary)
{
    for (list<reg>::iterator i = table.begin(); i != table.end(); i++)
        if (i->name == line)
            convBinary(i->address, 16, binary);
}

bool checkSymbol(string sym)
{
    bool check = false;
    for (list<reg>::iterator i = table.begin(); i != table.end(); i++)
        if (i->name == sym)
            check = true;
    return check;
}

void setJmp(string jmp, list<int> &binary)
{
    // cout << jmp << endl;
    if (jmp == "JGT")
        convBinary(1, 3, binary);
    if (jmp == "JEQ")
        convBinary(2, 3, binary);
    if (jmp == "JGE")
        convBinary(3, 3, binary);
    if (jmp == "JLT")
        convBinary(4, 3, binary);
    if (jmp == "JNE")
        convBinary(5, 3, binary);
    if (jmp == "JLE")
        convBinary(6, 3, binary);
    if (jmp == "JMP")
        convBinary(7, 3, binary);
}
void setDest(string dest, list<int> &binary)
{
    // cout << dest << endl;
    if (dest == "M")
        convBinary(1, 3, binary);
    if (dest == "D")
        convBinary(2, 3, binary);
    if (dest == "MD")
        convBinary(3, 3, binary);
    if (dest == "A")
        convBinary(4, 3, binary);
    if (dest == "AM")
        convBinary(5, 3, binary);
    if (dest == "AD")
        convBinary(6, 3, binary);
    if (dest == "AMD")
        convBinary(7, 3, binary);
}

void setComp(string comp, list<int> &binary)
{
    // cout << comp << endl;
    if (comp == "0")
        convBinary(42, 6, binary);
    if (comp == "1")
        convBinary(63, 6, binary);
    if (comp == "-1")
        convBinary(58, 6, binary);
    if (comp == "D")
        convBinary(12, 6, binary);
    if (comp == "A" || comp == "M")
        convBinary(48, 6, binary);
    if (comp == "!D")
        convBinary(13, 6, binary);
    if (comp == "!A" || comp == "!M")
        convBinary(49, 6, binary);
    if (comp == "-D")
        convBinary(15, 6, binary);
    if (comp == "-A" || comp == "-M")
        convBinary(51, 6, binary);
    if (comp == "D+1")
        convBinary(31, 6, binary);
    if (comp == "A+1" || comp == "M+1")
        convBinary(55, 6, binary);
    if (comp == "D-1")
        convBinary(14, 6, binary);
    if (comp == "A-1" || comp == "M-1")
        convBinary(50, 6, binary);
    if (comp == "D+A" || comp == "D+M")
        convBinary(2, 6, binary);
    if (comp == "D-A" || comp == "D-M")
        convBinary(19, 6, binary);
    if (comp == "A-D" || comp == "M-D")
        convBinary(7, 6, binary);
    if (comp == "D&A" || comp == "D&M")
        convBinary(0, 6, binary);
    if (comp == "D|A" || comp == "D|M")
        convBinary(21, 6, binary);
}

void convertCInstruction(string line, list<int> &binary)
{

    //Dest = comp
    if (line.find("=") != string::npos)
    {
        for (int i = 0; i < 3; i++)
            binary.emplace_front(0);
        setDest(line.substr(0, line.find("=")), binary);
        setComp(line.substr(line.find("=") + 1), binary);
        if (line.substr(line.find("=") + 1).find("M") != string::npos)
            binary.emplace_front(1);
        else
            binary.emplace_front(0);
    }
    //comp ; jump
    else if (line.find(";") != string::npos)
    {
        setJmp(line.substr(line.find(";") + 1), binary);
        for (int i = 0; i < 3; i++)
            binary.emplace_front(0);
        setComp(line.substr(0, line.find(";")), binary);
        if (line.substr(0, line.find(";")).find("M") != string::npos)
            binary.emplace_front(1);
        else
            binary.emplace_front(0);
    }

    for (int i = 0; i < 3; i++)
        binary.emplace_front(1);
}

void convertAInstruction(string line, list<int> &binary)
{
    try
    {
        int num = stoi(line.substr(1));
        convBinary(num, 16, binary);
    }
    catch (exception &err)
    {

        convSymbol(line.substr(1), binary);
    }
}
void passTwo(string name)
{
    ifstream fin(name);
    string line;
    while (!fin.eof())
    {
        getline(fin, line);
        line = remComments(line);   // Remove comments
        line = remWhiteSpace(line); //Remove white spaces
        if (line[0] == '@' && line[1] > 57)
        {
            if (!checkSymbol(line.substr(1)))
            {
                addVariable(line.substr(1), varIn);
                varIn++;
            }
        }
    }
}
void passOne(string name)
{
    ifstream fin(name);
    string line;
    int lineNo = 0;
    while (!fin.eof())
    {
        getline(fin, line);
        line = remComments(line);   // Remove comments
        line = remWhiteSpace(line); //Remove white spaces
        if (line != "" && line.find("//") == string::npos && line.find("(") == string::npos)
            lineNo++;

        if (line.find("(") != string::npos)
            addLable(line.substr(1, line.length() - 2), lineNo);
    }
}

void convertAssembly(string name)
{
    passOne(name);
    passTwo(name);
    ifstream fin(name);
    string line;
    ofstream fout(name.substr(0, name.find(".")) + ".hack");
    list<int> binary;
    while (!fin.eof())
    {
        binary.clear();
        getline(fin, line);
        line = remComments(line);   // Remove comments
        line = remWhiteSpace(line); //Remove white spaces
        if (line[0] == '@')
            convertAInstruction(line, binary);

        else if (line.find("=") != string::npos || line.find(";") != string::npos)
            convertCInstruction(line, binary);

        else
        {

            continue;
        }

        for (list<int>::iterator i = binary.begin(); i != binary.end(); i++)
        {
            fout << *i;
        }
        fout << "\n";
    }
}

bool check(string name)
{
    ifstream fin(name);
    if (fin)
        return true;
    else
        return false;
}

int main()
{
    cout << "\nEnter the file name with .asm extension\n";
    string name;
    bool entry = 0;
    while (entry == 0)
    {
        cin >> name;
        if (check(name))
        {
            createSymbolTable();
            convertAssembly(name);
            cout << "\n------------------------\nHack File Succesfully Generated\n------------------------\n\n\n";
            entry = 1;
        }
        else
            cout << "\n------------------------\nError, file not found.\n------------------------\nEnter a valid file name\n";
    }
}