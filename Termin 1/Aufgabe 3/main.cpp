#include "header.h"
#include <iostream>
using namespace std;

int otherVar = 2;

int myGlobal1;
int myGlobal2 = 1;
int myGlobal3 = otherVar;

static int myStaticGlobal1;
static int myStaticGlobal2 = 1;
static int myStaticGlobal3 = otherVar;

// 2 global class objects

//static Class1 *Object_5 = new Class1("global main.c");
//static Class2 *Object_4 = new Class2("global main.c");

Class1 Object_3("global main.c");
Class2 Object_2("global main.c");

Class1 Object_1("global main.c");
Class2 Object0("global main.c");

Class2 *Object4 = new Class2("global main.c");
Class1 *Object3 = new Class1("global main.c");

void myfunc()
{
    static Class1 *Object9 = new Class1("func static local main.c");
    static Class2 *Object10 = new Class2("func static local main.c");
}

int main(void)
{
    int myLocal1;
    int myLocal2 = 1;
    int myLocal3 = otherVar;

    cout << "myLocal1: " << myLocal1 << endl;
    cout << "myLocal2: " << myLocal2 << endl;
    cout << "myLocal3: " << myLocal3 << endl;

    int tmp = myLocal1;
    tmp = myLocal2;
    tmp = myLocal3;

    static int myStatic1;
    static int myStatic2 = 1;
    static int myStatic3 = otherVar;

    Class1 *Object5 = new Class1("local main.c");
    Class2 *Object6 = new Class2("local main.c");

    // static class object in main()
    static Class1 *Object7 = new Class1("static local main.c");
    static Class2 *Object8 = new Class2("static local main.c");

    myfunc();
    myfunc();

    static Class1 Object11("static local main.c not dyn");
    static Class2 Object12("static local main.c not dyn");

    return 0;
}
