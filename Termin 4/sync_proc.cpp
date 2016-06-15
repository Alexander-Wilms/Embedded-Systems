// File     sync_proc.h
// Version  1.0
// Author   Jens-Peter Akelbein
// Comment  Softwareentwicklung fuer Embedded Systeme - Exercise 3

#include <iostream>
#include "CNamedSemaphore.h"
#include <stdlib.h>

#include <unistd.h> // für fork()
#include <stdio.h> // für printf()

using namespace std;

// valid states for our two processes, we use the impicit ordering of values
// by an enum starting with the value 1
enum EProc_State {
    STATE_ACTIVE_CHILD = 1,
    STATE_ACTIVE_PARENT,    // = 2
    STATE_TERMINATE         // = 3
};

int pingpongs = 0;
#define NUMBER_OF_LOOPS     10
int i, j;
pid_t pid;
bool forked = 0;

// Erstelle 2 Charrays als Namen für die Semaphoren
const char sem_name1[] = "/semaphore";
const char sem_name2[] = "/state";

// Erstelle eine Variable namens "semaphore" vom Typ CNamedSemaphore mit dem internen Namen "/semaphore"
// create the semaphore
// @name - name of the semaphore being created in /run/shm
// @value - initial value of the semaphore
CNamedSemaphore semaphore(sem_name1, 1);
// Erstelle eine Variable namens "state" vom Typ CNamedSemaphore mit dem internen Namen "/semaphore"
// Speicht den Zustand
CNamedSemaphore state(sem_name2, STATE_ACTIVE_CHILD);

// function being executed as parent or as child process to perform ping pong
// between both processes
void pingpong(bool parent) {
    //cout << "state value: " << state.value() << endl;
    //cout << "trying to ping pong" << parent << endl << std::flush;
    switch(parent) {
    case true:
        semaphore.decrement();
        //sleep(1);
        /* Elternprozess
        * Gibt fork() einen Wert größer 0 zurück, befinden wir uns im Elternprozess
        * in pid steht die ID des Kindprozesses
        * getpid() gibt die eigene PID zurück
        */
        //cout << "\nparent: semaphore value was: " << semaphore.value()+1 << endl << std::flush;
        //cout << "parent: semaphore value is: " << semaphore.value() << endl << std::flush;
        //cout << "parent: pid is: " << getpid() << endl << std::flush;
        //cout << "state value: " << state.value() << endl;

        switch(state.value()) {
        case STATE_ACTIVE_PARENT:
            cout << "parent pong" << endl;
            pingpongs++;
            state.decrement();
            break;
        case STATE_TERMINATE:
            cout << "terminate" << endl;
            exit(0);
        }

        semaphore.increment();
    case false: // child process
        semaphore.decrement();
        //sleep(1);
        //cout << "\nchild: semaphore value was: " << semaphore.value()+1 << endl << std::flush;
        //cout << "child: semaphore value is: " << semaphore.value() << endl << std::flush;
        //cout << "child: pid is: " << getpid() << endl << std::flush;
        //cout << "state value: " << state.value() << endl;
        switch(state.value()) {
        case STATE_ACTIVE_CHILD:
            if(pingpongs == NUMBER_OF_LOOPS) {
                state.increment();
                state.increment();
                exit(0);
            } else {
            cout << "child ping" << endl;
            state.increment();
            }
        }

        semaphore.increment();
    }
}

// main function, here we are just forking into two processes both calling
// pingpong() and indicating with a boolean on who is who
int main() {
    //cout << "running" << endl;

    // test if semaphore works
   /* cout << "semaphore value: " << semaphore.value() << endl;
    cout << "increment semaphore" << endl;
    semaphore.increment();
    cout << "semaphore value: " << semaphore.value() << endl;

    cout << "decrement semaphore" << endl;
    semaphore.decrement();
    cout << "semaphore value: " << semaphore.value() << endl;*/

    /*cout << "decrement semaphore" << endl;
    semaphore.decrement();
    cout << "semaphore value: " << semaphore.value() << endl;

    cout << "decrement semaphore" << endl;
    semaphore.decrement();
    cout << "semaphore value: " << semaphore.value() << endl;*/

    // now we fork...

    if(!forked)
    {
    pid = fork();
    forked = 1;
    }

    while(1)
        pingpong(pid != 0);

    return 0;
}
