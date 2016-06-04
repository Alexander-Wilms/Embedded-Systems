// File     CNamedSemaphore.cpp
// Version  1.0
// Author   Jens-Peter Akelbein
// Comment  Softwareentwicklung fuer Embedded Systeme - Exercise 3

#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "CNamedSemaphore.h"

using namespace std;

// Vorgegeben waren die folgenden Funktionen ohne Körper:

// construct a new named semaphore
// @name - name of the semaphore being created in /run/shm
// @value - initial value of the semaphore
CNamedSemaphore::CNamedSemaphore(const char *name, int value) {
    // Konstruktor
    cout << "CNamedSemaphore constructor" << endl;
    cout << "name: " << name << endl;
    cout << "value: " << value << endl;
    remember_my_name = name;
    remember_my_creator = getppid();

    cout << "creator: " << remember_my_creator << endl;
    static sem_t mysemaphore;
    semaphore = &mysemaphore;
    // The pshared argument indicates whether the semaphore is
    // local to the current process (pshared is zero) or is to
    // be shared between several processes (pshared is not zero).
    int retval;
    try {
        semaphore = sem_open(name, (int) O_CREAT, 0777, (unsigned int) value);
        cout << "after sem_init" << endl;
    } catch (int e) {
        exitproc("sem_init failed \n", e);
    }
}

// deconstruct the semaphore
// question:
// How do we delete the semaphore by the last process only?
CNamedSemaphore::~CNamedSemaphore() {
    // Destruktor
    cout << "CNamedSemaphore destructor" << endl;

    // if(last_process)
    try {
        sem_unlink(remember_my_name);
    } catch (int e) {
        exitproc("sem_destroy failed \n", e);
    }
}


void CNamedSemaphore::increment(void) {
    // gives semaphores
    int retval;
    try {
        retval = sem_post(semaphore);
        //if(retval == 0) cout << "semaphore succesfully incremented" << endl << std::flush; else cout << "failed to increment semaphore" << endl << std::flush;

    } catch (int e) {
        exitproc("sem_post failed \n", e);
    }
}


void CNamedSemaphore::decrement(void) {
    // takes semaphore
    int retval;
    try {
        retval = sem_wait (semaphore);
        //cout << "sem_wait returns " << retval << endl;
        //if(retval == 0) cout << "semaphore succesfully decremented" << endl << std::flush; else cout << "failed to decrement semaphore" << endl << std::flush;
    } catch (int e) {
        exitproc("sem_wait failed \n", e);
    }
}


int CNamedSemaphore::value(void) {
    int tmp;
    try {
        sem_getvalue (semaphore, &tmp);
    } catch (int e) {
        exitproc("sem_getvalue failed \n", e);
    }
    return tmp;
}


// helper function to display errors and terminate our
// process as a very simple error handling mechanism
void CNamedSemaphore::exitproc(const char *text, int err) {

    cout << text;
    switch (err) {
        case EACCES:
            cerr << "EACCES";
            break;
        case EEXIST:
            cerr << "EEXIST";
            break;
        case EINVAL:
            cerr << "EINVAL";
            break;
        case EMFILE:
            cerr << "EMFILE";
            break;
        case ENAMETOOLONG:
            cerr << "TOOLNG";
            break;
        case ENOENT:
            cerr << "ENOENT";
            break;
        case ENOMEM:
            cerr << "ENOMEM";
            break;
        case EOVERFLOW:
            cerr << "EOVERFLOW";
            break;
        default:
            cerr << "UNKNWN";
    }
    cout << endl;
    exit(1);
}
