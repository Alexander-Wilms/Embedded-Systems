#include "header.h"
#include <iostream>
using namespace std;

int otherVar = 2;

int myGlobal1;
int myGlobal2 = 1;
int myGlobal3 = otherVar;

Class1 *Object3 = new Class1("global main.c");
Class2 *Object4 = new Class2("global main.c");

void myfunc()
{
    static Class1 *Object9 = new Class1("static local main.c");
    static Class2 *Object10 = new Class2("static local main.c");
}

int main(void)
{
    static int myStatic1;
    static int myStatic2 = 1;
    static int myStatic3 = otherVar;

    Class1 *Object5 = new Class1("local main.c");
    Class2 *Object6 = new Class2("local main.c");

    static Class1 *Object7 = new Class1("static local main.c");
    static Class2 *Object8 = new Class2("static local main.c");

    myfunc();
    myfunc();

    return 0;
}
