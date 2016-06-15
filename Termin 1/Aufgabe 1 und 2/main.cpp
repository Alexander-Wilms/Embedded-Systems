#include <iostream>

using namespace std;

class CBeispiel
{
    virtual int funktion(void)
    {
        return 0;
    }

    virtual int funktion2(void)
    {
        return 0;
    }
};

class CBeispiel2
{
    virtual int funktion3(void)
    {
        return 0;
    }

    virtual int funktion4(void)
    {
        return 0;
    }
};

class CAbgeleitet : CBeispiel, CBeispiel2
{
    int funktion(void)
    {
        return 1;
    }

    int funktion2(void)
    {
        return 1;
    }

    int funktion3(void)
    {
        return 1;
    }

    int funktion4(void)
    {
        return 1;
    }
};

struct SData {
    char    a; // Offset 0x0
    int     b; // Offset 0x4 Alignment auf 32 Bit-Grenze
    char    c; // Offset 0x8
    short   d; // Offset 0xA Alignment auf 16 Bit-Grenze
    int     e; // Offset 0xC
};

struct SData2 {
    char    a; // Offset 0x0
    char    c; // Offset 0x1
    short   d; // Offset 0x2
    int     b; // Offset 0x4
    int     e; // Offset 0x8
};

union UData {
    char    a; // Offset 0x0
    int     b; // Offset 0x4 Alignment auf 32 Bit-Grenze
    char    c; // Offset 0x8
    short   d; // Offset 0xA Alignment auf 16 Bit-Grenze
    int     e; // Offset 0xC
};

union UData2 {
    char    a; // Offset 0x0
    char    c; // Offset 0x1
    short   d; // Offset 0x2
    int     b; // Offset 0x4
    int     e; // Offset 0x8
};

int main()
{
    long no_use;
    cout << "Architecture = " << sizeof(no_use)*8 << " Bit" << endl;

    CBeispiel *beispiel = new CBeispiel;
    cout << "Size CBeispiel = " << sizeof(beispiel) << " byte" << endl;

    CBeispiel2 *beispiel2 = new CBeispiel2;
    cout << "Size CBeispiel2 = " << sizeof(beispiel2) << " byte" << endl;

    CAbgeleitet *beispiel3= new CAbgeleitet;
    cout << "Size CAbgeleitet = " << sizeof(beispiel3) << " byte" << endl;

    cout << "Size SData = " << sizeof(SData) << " byte" << endl; // Sollte 16 Byte groß sein

    cout << "Size SData2 = " << sizeof(SData2) << " byte" << endl; // Sollte 12 Byte groß sein

    cout << "Size SData = " << sizeof(UData) << " byte" << endl; // Unions sollten gleich groß sein

    cout << "Size SData2 = " << sizeof(UData2) << " byte" << endl;

    return 0;
}

/*

nm -C -S ./Termin1 > Aufgabe_1.1_1_virt_Methode

*/
