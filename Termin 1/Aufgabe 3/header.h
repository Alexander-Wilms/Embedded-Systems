#include <iostream>
using namespace std;

extern int myGlobal1;
extern int myGlobal2;
extern int myGlobal3;

class Class1
{
public:
    Class1(char * param)
    {
        cout << "Class1 " << param << endl;
    }
};

class Class2
{
public:
    Class2(char * param)
    {
        cout << "Class2 " << param << endl;
    }
};

Class1 *Object1 = new Class1("global header.h");
Class2 *Object2 = new Class2("global header.h");

extern Class1 *Object3;
extern Class2 *Object4;
